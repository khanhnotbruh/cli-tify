#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global_var.h"
#include "helper.h"
#include "config.h"
static void logError(lua_State*L,const char*fmt,...){
  lua_Debug ar;
  if(lua_getstack(L,1,&ar)){
    lua_getinfo(L,"lS",&ar);
    fprintf(stderr,"ERROR %s:%d: ",ar.short_src,ar.currentline);
  }else{
    fprintf(stderr,"ERROR[Lua]: ");
  }
  va_list args;
  va_start(args,fmt);
  vfprintf(stderr,fmt,args);
  va_end(args);
  fprintf(stderr,"\n");
}
static int getRegVal(lua_State*L,int ref,const char*inp[],uint64_t size,int*out){
  if(!out)return 0;
  if(ref==LUA_NOREF)return 0;
  for(uint64_t i=0;i<size;i++)out[i]=LUA_NOREF;
  lua_rawgeti(L,LUA_REGISTRYINDEX,ref);
  if(!lua_istable(L,-1)){
    lua_pop(L,1);
    return 0;
  }
  int idx=lua_gettop(L);
  lua_pushnil(L);
  while(lua_next(L,idx)!=0){
    if(lua_type(L,-2)!=LUA_TSTRING){
      logError(L,"unknown key type in config table(expected string)");
      lua_settop(L,idx-1);
      return 0;
    }
    const char*key=lua_tostring(L,-2);
    for(uint64_t i=0;i<size;i++){
      if(out[i]!=LUA_NOREF||!inp[i])continue;
      if(strcmp(inp[i],key)==0){
        lua_pushvalue(L,-1);
        out[i]=luaL_ref(L,LUA_REGISTRYINDEX);
        goto next_cycle;
      }
    }
    logError(L,"unexpected key in config table (a value set twice or unknow key)");
    lua_settop(L,idx-1);
    return 0;
next_cycle:
    lua_pop(L,1);
  }
  lua_settop(L,idx-1);
  return 1;
}

static int l_hello(lua_State*L){
  const char*name=luaL_optstring(L,1,"World");
  printf("Hello %s\n",name);
  return 1;
}
// &L
#define GET_RAWI(id,ref,ltype,cleanup,error) do{\
  lua_rawgeti(L,LUA_REGISTRYINDEX,ref[id]);\
  if(!lua_is##ltype(L,-1)){\
    fprintf(stderr,"ERROR: unable to recognise "#id"'s type\n");\
    lua_pop(L,1);\
    error=0;\
    goto cleanup;\
  }\
}while(0)
#define CLAMP_NEGATIVE(val)_Generic((val),\
  int:((val)<0?0:(val)),\
  long:((val)<0?0:(val)),\
  long long:((val)<0?0:(val)),\
  double:((val)< 0?0:(val)),\
  default:(val)\
)
// &L
#define ASSIGN_FIELD(id,ref,des,ltype,ctype,cleanup,error) do{\
  if(ref[id]!=LUA_NOREF){\
    GET_RAWI(id,ref,ltype,cleanup,error);\
    des=(ctype)CLAMP_NEGATIVE(lua_to##ltype(L,-1));\
    lua_pop(L,1);\
  }\
}while(0)
// &L,&app
#define ASSIGN_STRINGT(id,ref,des,cleanup,error) do{\
  if(ref[id]!=LUA_NOREF){\
    GET_RAWI(id,ref,string,cleanup,error);\
    string_t tmp={\
       .type=DYNAMIC,\
       .buf=(char*)lua_tolstring(L,-1,&tmp.len),\
       .end=tmp.buf+tmp.len,\
    };\
    tmp.last=tmp.end;\
    des=pushStringT(app,&tmp);\
    lua_pop(L,1);\
    if(!des){\
      fprintf(stderr,"ERROR: failed pushing string into memory\n");\
      error=0;\
      goto cleanup;\
    }\
  }\
}while(0)
// &L
#define ASSIGN_HASH(id,ref,des,cleanup,error) do{\
  if(ref[id]!=LUA_NOREF){\
    GET_RAWI(id,ref,string,cleanup,error);\
    des=hash_id64(lua_tostring(L,-1));\
    lua_pop(L,1);\
  }\
}while(0)

int assignConfig(app_state_t*app,widget_t*widget,int ref_idx){
  lua_State*L=app->L;
  bool error=1;
  if(!widget->config)widget->config=createComponent(app, sizeof(struct config_s));
  if(!widget->config){
    fprintf(stderr, "ERROR: failed allocating space for widget's config\n");
    return 0;
  }
  struct config_s*cfg=widget->config;
  const char* key[CONFIG_COUNT]={
    [ID]="id",[W]="w",[H]="h",[SX]="sx",[SY]="sy",[ASCII]="ascii",
    [EMPTY]="empty",[FOCUS]="focus",[MOUSE]="mouse",[FALLTHROUGH]="fallthrough",
    [DRAG]="drag",[PRESERVE_LAYERS]="preserve_layers",[FILL]="fillFunction",
    [TEXT]="text",[ANCHORS]="anchors",[ON_EVENT]="onEvent"
  };
  int ref[CONFIG_COUNT];
  if(!getRegVal(L,ref_idx,key,CONFIG_COUNT,ref)){
    fprintf(stderr,"ERROR: failed to get values from lua's register\n");
    error=1;
    goto cleanup;
  }
  ASSIGN_FIELD(W,ref,cfg->w,number,uint32_t,cleanup,error);
  ASSIGN_FIELD(H,ref,cfg->h,number,uint32_t,cleanup,error);
  ASSIGN_FIELD(SX,ref,cfg->sx,number,uint32_t,cleanup,error);
  ASSIGN_FIELD(SY,ref,cfg->sy,number,uint32_t,cleanup,error);
  ASSIGN_FIELD(ASCII,ref,cfg->ascii,boolean,bool,cleanup,error);     
  ASSIGN_FIELD(EMPTY,ref,cfg->empty,boolean,bool,cleanup,error);   
  ASSIGN_FIELD(FOCUS,ref,cfg->focus,boolean,bool,cleanup,error);      
  ASSIGN_FIELD(MOUSE,ref,cfg->mouse,boolean,bool,cleanup,error);      
  ASSIGN_FIELD(FALLTHROUGH,ref,cfg->fallthrough,boolean,bool,cleanup,error);
  ASSIGN_FIELD(DRAG,ref,cfg->drag,boolean,bool,cleanup,error);       
  ASSIGN_FIELD(PRESERVE_LAYERS,ref,cfg->preserve_layers,boolean,bool,cleanup,error);
  ASSIGN_STRINGT(TEXT,ref,cfg->text,cleanup,error);
  ASSIGN_HASH(ID,ref,cfg->id,cleanup,error);
  if(ref[ANCHORS]!=LUA_NOREF){
    const char*anchor_key[SIDE_COUNT]={
      [TOP]="top",[BOTTOM]="bottom",
      [LEFT]="left",[RIGHT]="right"
    };
    int anchor_ref[SIDE_COUNT]={0};
    uint64_t*anchors=cfg->anchors;
    if(!getRegVal(L,ref[ANCHORS],anchor_key,SIDE_COUNT,anchor_ref)){
      fprintf(stderr,"ERROR: failed to get values from lua's register\n");
      error=0;
      goto anchor_cleanup;
    }
    for(int i=0;i<SIDE_COUNT;i++)
      ASSIGN_HASH(i,anchor_ref,anchors[i],anchor_cleanup,error);
anchor_cleanup:
    for(int i=0;i<SIDE_COUNT;i++){
      if(anchor_ref[i]!=LUA_NOREF){
        luaL_unref(L,LUA_REGISTRYINDEX,anchor_ref[i]);
        anchor_ref[i]=LUA_NOREF;
      }
    }
    luaL_unref(L,LUA_REGISTRYINDEX,ref[ANCHORS]);
    ref[ANCHORS]=LUA_NOREF;
    if(!error)goto cleanup;
  }
cleanup:
  for(int i=0;i<CONFIG_COUNT;i++){
    if(ref[i]!=LUA_NOREF){
      luaL_unref(L,LUA_REGISTRYINDEX,ref[i]);
      ref[i]=LUA_NOREF;
    }
  }
  return error;
}
int assignBorder(app_state_t*app,widget_t*widget,int ref_idx){
  lua_State*L=app->L;bool error=1;
  if(!widget->borders)widget->borders=createComponent(app, sizeof(struct border_s));
  if(!widget->borders){
    fprintf(stderr,"ERROR: failed allocating space for widget's borders");
    return 0;
  }
  struct border_s*bd=widget->borders;
  const char*key[BORDER_COUNT]={
    [EDGES]="edges",[CORNERS]="corners",
    [PATTERN]="pattern"
  };
  int ref[BORDER_COUNT];
  if(!getRegVal(L,ref_idx,key,BORDER_COUNT,ref)){
    fprintf(stderr,"ERROR: failed to get values from lua's register\n");
    error=0;goto cleanup;
  }
  ASSIGN_STRINGT(PATTERN,ref,bd->pattern,cleanup,error);
  if(ref[CORNERS]!=LUA_NOREF){
    const char*corner_key[CORNER_COUNT]={
      [TOP_LEFT]="top_left",
      [TOP_RIGHT]="top_right",
      [BOTTOM_LEFT]="bottom_left",
      [BOTTOM_RIGHT]="bottom_right",
    };
    int corner_ref[CORNER_COUNT]={0};
    if(!getRegVal(L,ref[CORNERS],corner_key,CORNER_COUNT,corner_ref)){
      fprintf(stderr,"ERROR: failed to get values from lua's register\n");
      return 0;
    }
    string_t**corner=bd->corners;
    for(int i=0;i<CORNER_COUNT;i++)
      ASSIGN_STRINGT(i,corner_ref,corner[i],corner_cleanup,error);
corner_cleanup:
    for(int i=0;i<CORNER_COUNT;i++){
      if(corner_ref[i]!=LUA_NOREF){
        luaL_unref(L,LUA_REGISTRYINDEX,corner_ref[i]);
        corner_ref[i]=LUA_NOREF;
      }
    }
    luaL_unref(L,LUA_REGISTRYINDEX,ref[CORNERS]);
    ref[CORNERS]=LUA_NOREF;
    if(!error)goto cleanup;
  }
  if(ref[EDGES]!=LUA_NOREF){
    const char*side_key[SIDE_COUNT]={
      [TOP]="top",[BOTTOM]="bottom",
      [LEFT]="left",[RIGHT]="right"
    };
    int side_ref[SIDE_COUNT]={0};
    if(!getRegVal(L,ref[EDGES],side_key,SIDE_COUNT,side_ref)){
      fprintf(stderr,"ERROR: failed to get values from lua's register\n");
      error=0;
      goto side_cleanup;
    }
    string_t*(*edge)[6]=bd->edges;
    const char*edge_key[EDGE_COUNT]={
      [EDGE_PATTERN]="pattern",
      [EDGE_MIDDLE]="middle",
      [EDGE_UPPER]="upper",
      [EDGE_LOWER]="lower",
      [EDGE_RIGHT]="right",
      [EDGE_LEFT]="left",
    };
    int edge_ref[EDGE_COUNT]={0};
    for(int i=0;i<SIDE_COUNT;i++){
      if(!getRegVal(L,side_ref[i],edge_key,EDGE_COUNT,edge_ref)){
        fprintf(stderr,"ERROR: failed to get values from lua's register\n");
        error=1;
        goto side_cleanup;
      }
      for(int j=0;j<EDGE_COUNT;j++)
        ASSIGN_STRINGT(j,edge_ref,edge[i][j],edge_cleanup,error);
edge_cleanup:
      for(int i=0;i<EDGE_COUNT;i++){
        if(edge_ref[i]!=LUA_NOREF){
          luaL_unref(L,LUA_REGISTRYINDEX,edge_ref[i]);
        }
      }
      if(!error)goto side_cleanup;
    }
side_cleanup:
    for(int i=0;i<SIDE_COUNT;i++){
      if(side_ref[i]!=LUA_NOREF){
        luaL_unref(L,LUA_REGISTRYINDEX,side_ref[i]);
      }
    }
    luaL_unref(L,LUA_REGISTRYINDEX,ref[EDGES]);
    ref[EDGES]=LUA_NOREF;
    if(!error)goto cleanup;
  }
cleanup:
  for(int i=0;i<BORDER_COUNT;i++){
    if(ref[i]!=LUA_NOREF){
      luaL_unref(L,LUA_REGISTRYINDEX,ref[i]);
      ref[i]=LUA_NOREF;
    }
  }
  return error;
}
// idx is the table idx
int assignWidget(app_state_t*app,widget_t*parent,int idx){
  lua_State*L=app->L;
  idx=lua_absindex(L,idx);
  lua_pushnil(L);
  while(lua_next(L,idx)!=0){
    if(lua_type(L,-2)==LUA_TNUMBER){
      lua_pop(L,1);
      continue;
    }
    if(lua_type(L,-2)!=LUA_TSTRING){
      logError(L,"unknown key type in config table(expected string)");
      lua_settop(L,idx);
      return 0;
    }
    const char *key=lua_tostring(L,-2);
    uint64_t size=strlen(key);
    if(lua_istable(L,-1)){
      lua_pushvalue(L,-1);
      int conf_ref=luaL_ref(L,LUA_REGISTRYINDEX);
      if(size==6&&!strcmp(key,"config")){
        if(!assignConfig(app,parent,conf_ref)){
          fprintf(stderr,"ERROR: failed assigning value to widget's config\n");
          luaL_unref(L,LUA_REGISTRYINDEX,conf_ref);
          lua_settop(L,idx);
          return 0;
        }
      }else if(size==7&&!strcmp(key,"borders")){
        if(!assignBorder(app,parent,conf_ref)){
          fprintf(stderr,"ERROR: failed assigning value to widget's borders\n");
          luaL_unref(L,LUA_REGISTRYINDEX,conf_ref);
          lua_settop(L,idx);
          return 0;
        }
      }else{
        logError(L, "unknown property table '%s'",key);
        luaL_unref(L,LUA_REGISTRYINDEX,conf_ref);
        lua_settop(L,idx);
        return 0;
      }
      luaL_unref(L,LUA_REGISTRYINDEX,conf_ref);
    }else{
      logError(L, "expected table for property '%s',got %s",key,lua_typename(L,-1));
      lua_settop(L,idx);
      return 0;
    }
    lua_pop(L,1);
  }
  lua_pushlightuserdata(L,parent);
  return 1;
}
static int l_widget(lua_State*L){
  if(!lua_istable(L,1)){
    return luaL_error(L,"expected table for 'Widget', got %s",luaL_typename(L,1));
  }
  lua_pushstring(L,"app_state");
  lua_gettable(L,LUA_REGISTRYINDEX);
  app_state_t*app=lua_touserdata(L,-1);
  lua_pop(L,1);
  widget_t*parent=createComponent(app,sizeof(widget_t));
  if(!parent){
    fprintf(stderr,"ERROR: failed creating widget\n");
    return 0;
  }
  uint64_t child_cnt=lua_rawlen(L,1);
  widget_t*prev=0;
  for(uint64_t i=1;i<=child_cnt;i++){
    lua_rawgeti(L,1,i);
    if(lua_islightuserdata(L,-1)){
      widget_t*child=(widget_t*)lua_touserdata(L,-1);
      child->parent=parent;
      if(!prev){
        prev=child;
        parent->child=child;
      }else{
        prev->sibling=child;
        prev=child;
      }
    }
    lua_pop(L,1);
  }
  if(!app->screen->child)app->screen->child=parent;
  else if(app->screen->child==parent->child)app->screen->child=parent;
  int argc=assignWidget(app,parent,1);
  if(!argc) fprintf(stderr, "ERROR: unable to apply config to widget\n");
  return argc;
}

#define DUMP(name,frm,level,max,...)do{\
  int _len=(int)sizeof(#name)- 1;\
  int _pad=((max)> _len)?((max)- _len): 0;\
  fprintf(stdout,"%*s%s%*s: " frm "\n",\
      (level),"",#name,_pad,"",__VA_ARGS__);\
}while(0)
#define DUMP_STR(name,level,max,arg) do{\
  char*_tmp=readStringT(arg);\
  DUMP(name,"%s",level,max,_tmp?_tmp:"(null)");\
  free(_tmp);\
}while(0)

int dumpConfig(struct config_s *conf,int level){
  if(!conf)return 0;
  int M=15;
  DUMP(id,"%llu",level,M,(unsigned long long)conf->id);
  DUMP(anchors,"%s",level,M,"");
  DUMP(left,"%llu",level+2,M,(unsigned long long)conf->anchors[LEFT]);
  DUMP(right,"%llu",level+2,M,(unsigned long long)conf->anchors[RIGHT]);
  DUMP(top,"%llu",level+2,M,(unsigned long long)conf->anchors[TOP]);
  DUMP(bottom,"%llu",level+2,M,(unsigned long long)conf->anchors[BOTTOM]);

  DUMP(w,"%u",level,M,conf->w);
  DUMP(h,"%u",level,M,conf->h);
  DUMP(sx,"%u",level,M,conf->sx);
  DUMP(sy,"%u",level,M,conf->sy);

  DUMP(raw,"%08b",level,M,conf->raw);
  DUMP(fillFunction,"%d",level,M,conf->fillFunction);
  DUMP(onEvent,"%d",level,M,conf->onEvent);
  DUMP_STR(text,level,M,conf->text);
  return 1;
}
int dumpBorder(struct border_s*bord,int level){
  if(!bord)return 0;
  int M=15;
#define DUMP_EDGES(name,arg)do{\
  DUMP(name,"%s",level,M,"");\
  level+=2;\
  DUMP_STR(pattern,level,M,arg[EDGE_PATTERN]);\
  DUMP_STR(middle,level,M,arg[EDGE_MIDDLE]);\
  DUMP_STR(upper,level,M,arg[EDGE_UPPER]);\
  DUMP_STR(lower,level,M,arg[EDGE_LOWER]);\
  DUMP_STR(right,level,M,arg[EDGE_RIGHT]);\
  DUMP_STR(left,level,M,arg[EDGE_LEFT]);\
  level-=2;\
}while(0)
  DUMP(edges,"%s",level,M,"");
  level+=2;
  DUMP_EDGES(top,bord->edges[TOP]);
  DUMP_EDGES(left,bord->edges[LEFT]);
  DUMP_EDGES(right,bord->edges[RIGHT]);
  DUMP_EDGES(bottom,bord->edges[BOTTOM]);
  level-=2;
#undef DUMP_EDGES
  DUMP(corners,"%s",level,M,"");
  level+=2;
  DUMP_STR(top_left,level,M,bord->corners[TOP_LEFT]);
  DUMP_STR(top_right,level,M,bord->corners[TOP_RIGHT]);
  DUMP_STR(bottom_left,level,M,bord->corners[BOTTOM_LEFT]);
  DUMP_STR(bottom_right,level,M,bord->corners[BOTTOM_RIGHT]);
  level-=2;
  DUMP_STR(pattern,level,M,bord->pattern);
  return 1;
  }
int dumpWidget(widget_t *root,int level){
  if(!root)return 0;
  widget_t *cur=root;
  struct config_s *conf=cur->config;
  struct border_s *bord=cur->borders;
  DUMP(widget,"%s",level,10,"");
  level+=2;
  if(!dumpConfig(conf,level))return 0;
  if(!dumpBorder(bord,level))return 0;
  return 1;
}

int initializeLua(app_state_t*app){
  lua_State*L=luaL_newstate();
  app->L=L;
  if(!L){
    fprintf(stderr,"ERROR: unable to create new lua state\n");
    return 0;
  }
  luaL_openlibs(L);
  lua_newtable(L);
  //throw app state
  lua_pushstring(L,"app_state");
  lua_pushlightuserdata(L,app);
  lua_settable(L,LUA_REGISTRYINDEX);
  //throw functions
  lua_pushcfunction(L,l_hello);
  lua_setfield(L, -2, "hello");
  lua_pushcfunction(L,l_widget);
  lua_setfield(L, -2, "widget");
  lua_setglobal(L,"musicli");
  char*config_path=readStringT(app->config_path);
  if(!config_path){
    fprintf(stderr,"ERROR: unable to recognise config path\n");
    return 0;
  }
  free(config_path);
  return 1;
}
