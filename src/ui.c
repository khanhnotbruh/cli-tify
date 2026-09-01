#include <alloca.h>
#include <ncurses.h>
#include <stdio.h>
#include "global_var.h"
int initializeUI(app_state_t app){
  initscr();             // Start ncurses mod
  cbreak();              // Line buffering disabled,pass key events instantly
  noecho();              // Don't print user keystrokes to screen
  keypad(stdscr,TRUE);  // Enable arrow keys,F1-F12,Home,End
  nodelay(stdscr,TRUE); // Non-blocking input(wgetch returns ERR if no key)
  curs_set(0);           // Hide cursor(0=invisible,1=normal,2=high visibility)
  mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
  if(!stdscr){
    fprintf(stderr,"ERROR: failed to initialize window\n");
    return 1;
  }
  app.scr=stdscr;
  return 0;
}

// Basically, app is keeping a root widget,
// we have to loop through all of them to draw the whole thing..
// well i will have to do the widget creation part

static int updateWidget(widget_t*widget,int event){
  if(!widget||!widget->state) return 1;
  widget->state->key_pressed=0;
  if(event==ERR)return 1;
  widget->state->key_pressed=event;
  widget->state->is_clicked=event==KEY_MOUSE;
  widget->state->is_pressed=1;
  return 1;
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
