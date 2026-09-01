#ifndef GLOBAL_H
#define GLOBAL_H
#include <luajit-2.1/lua.h>
#include <stdbool.h>
#include <stdint.h>
#include <ncurses.h>

#if LUA_VERSION_NUM < 502
#define lua_rawlen(L, i) lua_objlen(L, (i))
#endif
#ifndef lua_absindex
#define lua_absindex(L, i) \
    (((i) > 0 || (i) <= LUA_REGISTRYINDEX) ? (i) : lua_gettop(L) + (i) + 1)
#endif

//------------------- STRING -------------------//
enum string_type_e{
  DYNAMIC=0,// idk where is it?
  FIXED=1,//already inside app's mem
};
typedef struct string_t{
  struct string_t*next;
  enum string_type_e type;
  uint64_t len;//does count '\0'
  char*buf,*last,*end;
  // last point to the next available slot
}string_t;
//------------------- WIDGET -------------------//
struct border_s{
  string_t*edges[4][6];
  string_t*corners[4];
  string_t*pattern;
};
struct state_s{
  uint32_t cx,cy;
  uint16_t key_pressed;
  union{
    uint8_t raw;
    struct {
      uint8_t is_hovered:1;   
      uint8_t is_empty:1;
      uint8_t is_focus:1;
      uint8_t is_visible:1; 
      uint8_t is_clicked:1;   
      uint8_t is_pressed:1;
      uint8_t _:2;
    };
  };
}; 
struct config_s{
  uint64_t id; //encoded from string to idx
  uint64_t anchors[4];//id of parent
  uint32_t w,h,sx,sy;
  union{
    uint8_t raw;
    struct{
      uint8_t ascii       : 1;
      uint8_t empty       : 1;
      uint8_t focus       : 1;
      uint8_t mouse       : 1;
      uint8_t fallthrough : 1;
      uint8_t drag        : 1;
      uint8_t preserve_layers : 1;
      uint8_t _:1;
    };
  };
  //char (*fillFunction)(int x,int y);
  int fillFunction;
  int onEvent;
  string_t*text;
};
typedef struct widget_t{
  struct config_s*config;
  struct border_s*borders;
  struct state_s *state;

  struct widget_t*parent;
  struct widget_t*child;
  struct widget_t*sibling;
}widget_t;

//------------------- MEMORY -------------------//

typedef struct memory_t{
  uint8_t*buf;
  struct memory_t*next;
  uint64_t cnt,size;
}memory_t;
//------------------- APP -------------------//
typedef struct{
  WINDOW*scr;
  lua_State*L;
  memory_t*mem[2];
  string_t*config_path;
  string_t*music_path;
  widget_t*screen;
  widget_t*focusing;
  uint32_t wid_cnt;
  uint32_t key_event;
}app_state_t;
//------------------- FUNCTIONS -------------------//
uint64_t hash_id64(const char*str);
char*readStringT(string_t*s);
int stringTCpy(string_t*destination,string_t*source,uint64_t size);
//the only few functions that can change mem[CURR]
void*createComponent(app_state_t*app,uint64_t size);
void*pushComponent(app_state_t*app,void*com,uint64_t size);
// possibly allocate memory (involve createComponent and pushComponent)
char*pushString(app_state_t*app,char*s);
widget_t*pushWidget(app_state_t*app,widget_t*widget);
string_t*pushStringT(app_state_t*app,string_t*s);
string_t*makeStringT(app_state_t*app,char*s,uint64_t size);
struct border_s*pushBorder(app_state_t*app,struct border_s*border);
struct config_s*pushConfig(app_state_t*app,struct config_s*config);
// harmless function
void handleEvent(app_state_t*app,int event);
void freeLua(lua_State*L);
void freeNcurse();
void freeApp(app_state_t*app);
void freeStringT(string_t*s);
int dumpConfig(struct config_s *conf,int level);
int dumpWidget(widget_t *root,int level);

uint32_t decodeUTF(char*s,uint64_t*size);
#endif
