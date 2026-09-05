#include "config.h"
#include "global_var.h"

#include <stdio.h>
#include <signal.h>
void handle_signal(int sig) {
    fprintf(stderr, "\nCaught signal %d.\n Exiting...\n", sig);
    signal(sig, SIG_DFL);
    raise(sig);
}
char config_path[]="./config.lua";
char img_path[]="./test.jpg";
int main(){
  app_state_t app={0};
  if(!initializeApp(&app)){
    fprintf(stderr,"FATAL: failed to initialize app\n");
    return 1;
  }
  app.config_path=makeStringT(&app,config_path,sizeof(config_path)-1);
  createComponent(&app,sizeof(config_path)+sizeof(string_t));
  if(!initializeLua(&app)){
    fprintf(stderr,"FATAL: failed to initialize Lua\n");
    return 1;
  }

  if(luaL_dofile(app.L,config_path)!=LUA_OK){
    fprintf(stderr,"%s\n",lua_tostring(app.L,-1));
    return 0;
  }
  // widget_t*cur=app.screen->child;
  // while(cur){
  //   dumpWidget(cur,1);
  //   cur=cur->child;
  // }


  freeApp(&app);
  signal(SIGINT,  handle_signal); // Ctrl+C
  signal(SIGTERM, handle_signal); // Kill command
  signal(SIGSEGV, handle_signal); // Segfault
  return 0;
}
