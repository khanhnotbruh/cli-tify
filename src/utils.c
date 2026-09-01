#include "global_var.h"
#include "helper.h"
#include <lua.h>
#include <ncurses.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// i dont freaking know anything abt hashing, and this code is llm-generated
uint64_t hash_id64(const char *str){
  if(!str)return 0;
  uint64_t hash=14695981039346656037ULL; // Offset basis
  uint64_t fnv_prime=1099511628211ULL;   // FNV prime
  while(*str){
    hash ^=(uint8_t)(*str); // XOR the bottom byte with current char
    hash *=fnv_prime;       // Multiply to scramble bits across 64-bits
    str++;
  }
  return hash;
}

//remember to free memory
char*readStringT(string_t*s){
  if(!s||!s->buf)return 0;
  uint64_t len=s->len;
  char*new=malloc(len+1);
  if(!new){
    fprintf(stderr,"ERROR: failed allocating memory for reading\n");
    return 0;
  }
  char*c=new;
  while(s&&len){
    uint64_t size=s->last-s->buf;
    if(size>len)size=len;
    memcpy(c,s->buf,size);
    c+=size;
    if(len>=size)len-=size;
    else break;
    s=s->next;
  }
  *c='\0';
  return new;
}
// does copy text buffer, locating after string_t struct
int stringTCpy(string_t*destination,string_t*source,uint64_t size){
  uint64_t len=source->len;
  string_t*s=source;
  string_t*d=destination;
  if(!d->buf){
    fprintf(stderr,"ERROR: destination buffer hasnt been initialized yet\n");
    return 0;
  }
  d->len=len;
  d->next=0;
  char *w=d->buf;
  while(s&&len){
    uint64_t size=s->last-s->buf;
    if(size>len)size=len;
    memcpy(w,s->buf,size);
    w+=size;
    if(len>=size)len-=size;
    else break;
    s=s->next;
  }
  return 1;
}
static memory_t*allocateMore(memory_t*mem,uint64_t size){
  if(!mem)return 0;
  memory_t*new=malloc(sizeof(memory_t));
  if(!new){
    fprintf(stderr,"ERROR: failed allocating space\n");
    return 0;
  }
  uint64_t new_size=2*mem->size;
  while(new_size<size)new_size*=2;
  new->size=new_size;new->cnt=0;
  new->buf=malloc(new_size);
  if(!new->buf){
    fprintf(stderr,"ERROR: failed allocating space\n");
    free(new);
    return 0;
  }
  new->next=0;mem->next=new;
  return new;
}
void*createComponent(app_state_t*app,uint64_t size){
  if(!app)return 0;
  memory_t*mem=app->mem[CURR];
  if(mem->cnt+size>mem->size){
    mem=allocateMore(mem, size);
    if(!mem) return 0;
    app->mem[CURR]=mem;
  }
  void*ans=((uint8_t*)mem->buf+mem->cnt);
  memset(ans,0,size);
  mem->cnt+=size;
  return ans;
}
void*pushComponent(app_state_t*app,void*com,uint64_t size){
  if(!app)return 0;
  memory_t*mem=app->mem[CURR];
  if(!mem||!com)return 0;
  if(mem->cnt+size>mem->size){
    mem=allocateMore(mem, size);
    if(!mem) return 0;
  }
  void*des=(uint8_t*)mem->buf+mem->cnt;
  memcpy(des, com, size);
  mem->cnt+=size;
  app->mem[CURR]=mem;
  return des;
}
string_t*makeStringT(app_state_t*app,char*s,uint64_t size){
  if(!s)return 0;
  string_t*new=createComponent(app,sizeof(string_t));
  if(!new)return 0;
  new->buf=pushComponent(app,s,size+1);
  if(!new->buf)return 0;
  new->end=new->buf+size;
  new->last=new->end;
  *new->last='\0';
  new->last++;
  new->len=size;
  new->next=0;
  new->type=FIXED;
  return new;
}
string_t*pushStringT(app_state_t*app,string_t*s){
  if(!app)return 0;
  memory_t*mem=app->mem[CURR];
  if(!mem||!s)return 0;
  if(s->type==FIXED)return s;
  uint64_t size=s->len;
  string_t*new=createComponent(app, size+sizeof(string_t));
  if(!new)return 0;
  new->buf=(char*)new+sizeof(string_t);
  if(!stringTCpy(new,s,size)){
    fprintf(stderr, "ERROR: failed to copy string\n");
    return 0;
  }
  new->type=FIXED;
  return new;
}
struct border_s*pushBorder(app_state_t*app,struct border_s*border){
  if(!app)return 0;
  uint64_t size=sizeof(struct border_s);
  struct border_s*new=pushComponent(app,border,size);
  if(!new)return 0;
  for(int i=0;i<4;i++){
    for(int j=0;j<6;j++){
      if(border->edges[i][j]){
        if(!pushStringT(app, border->edges[i][j]))return 0;
      }
    }
    if(border->corners[i]){
        if(!pushStringT(app, border->corners[i]))return 0;
    }
  }
  return new;
}
struct config_s*pushConfig(app_state_t*app,struct config_s*config){
  if(!app)return 0;
  uint64_t size=sizeof(struct config_s);
  struct config_s*new=pushComponent(app,config,size);
  if(!new)return 0;
  if(config->text){
    if(!pushStringT(app,config->text))return 0;
  }
  return new;
}
widget_t*pushWidget(app_state_t*app,widget_t*widget){
  if(!app||!widget)return 0;
  widget_t*new=pushComponent(app,widget,sizeof(widget_t));
  if(!new){
    fprintf(stderr,"ERROR: failed pushing border' property\n");
    return 0;
  }
  app->wid_cnt++;
  if(widget->state){
    struct state_s*tmp=pushComponent(app,widget->state,sizeof(struct state_s));
    if(!tmp){
      fprintf(stderr,"ERROR: failed pushing state\n");
      return 0;
    }
    new->state=tmp;
  }
  if(widget->config){
    struct config_s*tmp=pushConfig(app,widget->config);
    if(!tmp){
      fprintf(stderr,"ERROR: failed pushing config's property\n");
      return 0;
    }
    new->config=tmp;
  }
  if(widget->borders){
    struct border_s*tmp=pushBorder(app,widget->borders);
    if(!tmp){
      fprintf(stderr,"ERROR: failed pushing borders' property\n");
      return 0;
    }
    new->borders=tmp;
  }
  return new;
}

void freeLua(lua_State*L){
  if(L)lua_close(L);
}
void freeNcurse(){
  curs_set(1);
  clear();
  refresh();
  endwin();
}
void freeMemory(memory_t*mem){
  memory_t*c=mem;
  while(c){
    memory_t*n=c->next;
    free(c->buf); free(c);
    c=n;
  }
}
void freeApp(app_state_t*app){
  if(!app)return;
  freeMemory(app->mem[ROOT]);
  app->mem[ROOT]=0;
  app->mem[CURR]=0;
  freeLua(app->L);
  freeNcurse();
}

void freeStringT(string_t*s){
  if(!s)return;
  while(s){
    string_t*n=s->next;
    if(s->type==DYNAMIC){
      free(s->buf);
      free(s);
    }
    s=n;
  }
}
uint32_t decodeUTF(char*s,uint64_t*size){
  if(!s){
    if(size)*size=0;
    return 0;
  }
  uint8_t c=(uint8_t)s[0];
  if(!(c&(1<<7))){
    if(size)*size=1;
    return c;
  }
  for(int i=3;i>0;i--){
    uint8_t mask=0xFF<<(7-i);
    if((c&mask)==mask){
      if(size)*size=i+1;
      uint32_t tmp;memcpy(&tmp,s,i+1);
      return tmp;
    }
  }
  return 0;
}

