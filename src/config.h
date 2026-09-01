#ifndef CONFIG_H
#define CONFIG_H
#include "global_var.h"
#include <lua.h>

int initializeLua(app_state_t*app);
int initializeApp(app_state_t*app);
// assigning does allocate memory in the app's memory
#endif
