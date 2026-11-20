#define TB_IMPL
#include "termbox2.h"
#include "global_var.h"

#include <stdio.h>
#include <alloca.h>
#include <stdlib.h>
int initializeUI(app_state_t*app){
  tb_init();
  return 1;
}

int putImage(char *path){
  if(!path){
    fprintf(stderr,"ERROR: passing null to putImage\n");
    return 0;
  }
  uint64_t len=0;
  char*payload=(char*)charBase64(path,&len);
  if(!payload||!len){
    fprintf(stderr,"ERROR: failed converting %s image to base64\n",path);
    return 0;
  }
  uint32_t size=4096;
  char*cur=payload;
  if(size>len)size=len;
  printf("\033_Ga=T,f=100,m=1;%.*s\033\\",size,cur);
  if(len>=size){
    len-=size;
    cur+=size;
    while(len){
      if(size>len)size=len;
      printf("\033_Gm=1;%.*s\033\\",size,cur);
      cur+=size;
      len-=size;
    }
  }
  printf("\033_Gm=0;\033\\");
  fflush(stdout);
  free(payload);
  return 1;
}


// static int updateWidget(widget_t*widget,int event){

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

int drawBorder(uint32_t sx,uint32_t sy,uint32_t w,uint32_t h,struct border_s*bord){
  if(!bord)return 0;
  bool drawn=0;

  return drawn;
}
// int drawWidget(widget_t*node){
    
void freeUI(){
  tb_shutdown();
}
void waitKey(){
  tb_present();
  struct tb_event e;
  tb_poll_event(&e);
}


// int handleEvent(app_state_t *app, int event){

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
