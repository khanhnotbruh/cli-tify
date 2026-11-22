#ifndef GLOBAL_H
#define GLOBAL_H
#if LUA_VERSION_NUM < 502
#define lua_rawlen(L, i) lua_objlen(L, (i))
#endif
#ifndef lua_absindex
#define lua_absindex(L, i) \
    (((i) > 0 || (i) <= LUA_REGISTRYINDEX) ? (i) : lua_gettop(L) + (i) + 1)
#endif
#include "point.h"
#include "rectangle.h"
#include "color/color.h"
#endif//GLOBAL_H
