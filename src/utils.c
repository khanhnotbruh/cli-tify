#include "global_var.h"
#include "helper.h"
#include "ui.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    uint64_t size=s->len;
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
// does copy text buffer,locating after string_t struct
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
    mem=allocateMore(mem,size);
    if(!mem)return 0;
    app->mem[CURR]=mem;
  }
  void*ans=((uint8_t*)mem->buf+mem->cnt);
  mem->cnt+=size;
  memset(ans,0,size);
  return ans;
}
void*pushComponent(app_state_t*app,void*com,uint64_t size){
  if(!app)return 0;
  memory_t*mem=app->mem[CURR];
  if(!mem||!com)return 0;
  if(mem->cnt+size>mem->size){
    mem=allocateMore(mem,size);
    if(!mem)return 0;
  }
  void*des=(uint8_t*)mem->buf+mem->cnt;
  memcpy(des,com,size);
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
  string_t*new=createComponent(app,size+sizeof(string_t));
  if(!new)return 0;
  new->buf=(char*)new+sizeof(string_t);
  if(!stringTCpy(new,s,size)){
    fprintf(stderr,"ERROR: failed to copy string\n");
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
        if(!pushStringT(app,border->edges[i][j]))return 0;
      }
    }
    if(border->corners[i]){
      if(!pushStringT(app,border->corners[i]))return 0;
    }
  }
  return new;
}
struct config_s*pushConfig(app_state_t*app,struct config_s*config){
  if(!app)return 0;
  uint64_t size=sizeof(struct config_s);
  struct config_s*new=pushComponent(app,config,size);
  if(!new)return 0;
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
  free(app->screen);
  freeLua(app->L);
  freeUI();
}

void freeStringT(string_t*s){
  if(!s)return;
  if(s->type==FIXED)return;
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

int initializeApp(app_state_t *app){
  if(!app)app=malloc(sizeof(app_state_t));
  if(!app){
    fprintf(stderr,"error failed allocating memory\n");
    return 0;
  }
  memset(app,0,sizeof(app_state_t));
  memory_t*mem=malloc(sizeof(memory_t));
  if(!mem){
    fprintf(stderr,"error failed allocating memory\n");
    return 0;
  }
  mem->cnt=0;mem->size=1024;
  mem->buf=malloc(mem->size);
  if(!mem->buf){
    fprintf(stderr,"error failed allocating memory\n");
    return 0;
  }
  mem->next=0;
  app->mem[ROOT]=mem;
  app->mem[CURR]=mem;
  widget_t*screen=malloc(sizeof(widget_t));
  if(!screen){
    fprintf(stderr,"error failed allocating memory\n");
    return 0;
  }
  memset(screen,0,sizeof(widget_t));
  app->screen=screen;
  return 1;
}

uint8_t*charBase64(char*path){
  if(!path)return 0;
  FILE*inp=fopen(path,"rb");
  if(!inp)return 0;
  fseek(inp,0,SEEK_END);
  uint64_t len=ftell(inp);
  fseek(inp,0,SEEK_SET);
  uint8_t*new=malloc((len/3+1)*4);
  if(!new){
    fprintf(stderr,"ERROR: failed to allocate memory while converting string_t to base64\n");
    return 0;
  }
  uint8_t buf[1<<12]={0};
  uint8_t*wrt=new,bit=0;
  uint32_t tmp=0,read=0;
  while((read=fread(buf,1,1<<12,inp))>0){
    for(int j=0;j<read;j++){
      uint8_t c=buf[j];
      if(c>='A'&&c<='Z')c-='A';
      else if(c>='0'&&c<='9')c=c-'0'+52;
      else if(c>='a'&&c<='z')c=c-'a'+26;
      else if(c=='+')c=62;
      else if(c=='/')c=63;
      else continue;
      tmp=(tmp<<6)|c;
      bit+=6;
      if(bit>=8){
        bit-=8;
        *(wrt++)=(tmp>>bit)&0xFF;
      }
    }
  }
  *wrt='\0';
  fclose(inp);
  return new;
}
