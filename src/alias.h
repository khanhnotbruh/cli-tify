#ifndef ALIAS_H
#define ALIAS_H
#include "global.h"
typedef struct point_s point_t;
typedef struct rectangle_s rectangle_t;
static inline struct point_s pCreate(int32_t x,int32_t y){
  return point_s_Create(x, y);
}
#define pAdd(a,b) point_s_Add(a,b);
#endif//ALIAS_H
