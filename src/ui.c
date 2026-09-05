#include <stdio.h>
#define _XOPEN_SOURCE_EXTENDED 1
#include <alloca.h>
#include <stdlib.h>
#include "global_var.h"
#include "termbox2.h"
int initializeUI(app_state_t*app){
  tb_init();
  return 0;
}
char img_path[]="./test.jpeg";
int putImage(char *path){
  if(!path)return 0;
  char*payload=(char*)charBase64(path);
  if(!payload)return 0;
  printf("\033_Ga=T;%s\033\\",payload);
  fflush(stdout);
  free(payload);
  return 1;
}

// Basically, app is keeping a root widget,
// we have to loop through all of them to draw the whole thing..

// static int updateWidget(widget_t*widget,int event){
//   if(!widget||!widget->state) return 1;
//   widget->state->key_pressed=0;
//   if(event==ERR)return 1;
//   widget->state->key_pressed=event;
//   widget->state->is_clicked=event==KEY_MOUSE;
//   widget->state->is_pressed=1;
//   return 1;
// }
static int sortByZ(const void*a,const void*b){
  const widget_t*x=a;
  const widget_t*y=b;
  uint64_t za=x->config->z;
  uint64_t zb=y->config->z;
  if(za!=zb)return za-zb;
  uint64_t ca=x->creation_idx;
  uint64_t cb=y->creation_idx;
  return ca-cb;
}
int createZtable(app_state_t*app,widget_t**zt){
  if(!zt)zt=createComponent(app, app->wid_cnt*sizeof(widget_t*));
  if(!zt){
    fprintf(stderr,"ERROR: failed to allocate memory for z table\n");
    return 0;
  }
  //fill table
  uint64_t head=0,tail=0;
  zt[tail++]=app->screen->child;
  while(head<tail&&tail<app->wid_cnt){
    widget_t*p=zt[head++];
    widget_t*u=p->child;
    widget_t*v=p->sibling;
    bool pushed=(u||v);
    if(u)zt[tail++]=u;
    if(v)zt[tail++]=v;
  }
  qsort(zt,app->wid_cnt,sizeof(widget_t*),sortByZ);
  return 1;
}

// 1 mean had drawn / 0 mean hadnt drawn anything
// int drawContent(uint32_t sx,uint32_t sy,uint32_t w,uint32_t h,struct content_s*cont){
//   uint32_t cy=sy;
//   if(!cont)return 0;
//   string_t*text=cont->text;
//   char*cur=text->buf;
//   uint64_t len=text->last-cur;
//   bool drawn=0;
//   while(len&&cy<sy+h){
//     mvaddnstr(cy,sx,cur,w);
//     if(len>=w)len-=w;
//     else break;
//     cur+=w;
//     cy++;
//     drawn=1;
//   }
//   return drawn;
// }
int drawBorder(uint32_t sx,uint32_t sy,uint32_t w,uint32_t h,struct border_s*bord){
  if(!bord)return 0;
  bool drawn=0;
  return drawn;
}
// int drawWidget(widget_t*node){
//   if(!node)return 0;
//   struct config_s*conf=node->config;
//   if(!conf)return 0;
//   uint32_t sx=conf->sx,sy=conf->sy;
//   uint32_t w=conf->w,    h=conf->h;
//   return drawContent(sx,sy,w,h,node->content)||
//     drawBorder(sx,sy,w,h,node->borders);
// }
void freeUI(){
  tb_shutdown();
}


// int handleEvent(app_state_t *app, int event){
//   if(!app||!app->screen)return 1;
//   if(event==ERR)return 1;
//   app->key_event=event;
//   if(event==KEY_MOUSE){
//     MEVENT mevent;
//     if(getmouse(&mevent)!=OK){
//       fprintf(stderr, "ERROR: failed to get mouse\n");
//       return 0;
//     }
//     uint32_t mx=mevent.x,my=mevent.y;
//     widget_t*node[app->wid_cnt];
//     uint16_t c=0;
//     node[c++]=app->screen;
//     while(c){
//       widget_t*cur=node[--c];
//       if(cur->sibling)node[c++]=cur->sibling;
//       if(!cur->config)continue;
//       struct config_s*config=cur->config;
//       if(!config->mouse||config->fallthrough)continue;
//
//     }
//   }else{
//     if(!app->focusing)return 1;
//     app->focusing->state->is_pressed=1;
//     app->focusing->state->key_pressed=event;
//   }
//   return 1;
// }

/*
   ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
   ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
   ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡏⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
   ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠁⠈⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
   ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣤⡀⠀♥VN⠀⠀⢀⣤⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿
   ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡖⠀⠀⠀⠀⢲⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
   ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠁⣀⣴⣦⣀⠈⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
   ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣾⣿⣿⣿⣿⣷⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
   ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
   ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
   */
