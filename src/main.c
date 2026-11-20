#include "global_var.h"
#include <stdio.h>
#include <signal.h>
void handle_signal(int sig) {
    fprintf(stderr, "\nCaught signal %d.\n Exiting...\n", sig);
    signal(sig, SIG_DFL);
    raise(sig);
}
char config_path[]="./config.lua";
char img_path[]="./test.png";
int main(){
  signal(SIGINT,  handle_signal); // Ctrl+C
  signal(SIGTERM, handle_signal); // Kill command
#ifndef __SANITIZE_ADDRESS__
  signal(SIGSEGV, handle_signal);
#endif
  app_state_t app={0};
  if(!initializeApp(&app,config_path)){
    fprintf(stderr,"FATAL: failed to initialize app\n");
    freeApp(&app);
    return 1;
  }
  freeApp(&app);
  return 0;
}
