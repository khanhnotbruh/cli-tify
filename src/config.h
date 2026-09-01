#ifndef CONFIG_H
#define CONFIG_H
#include "global_var.h"

int dumpBorder(struct border_s*bord,int level);
int dumpWidget(widget_t *root,int level);
int dumpConfig(struct config_s *conf,int level);
int initializeLua(app_state_t*app);
// assigning does allocate memory in the app's memory
#endif
