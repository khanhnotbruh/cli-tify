#ifndef RECTANGLE_H
#define RECTANGLE_H
#include <stdint.h>
#include "point.h"
struct rectangle_s{
  struct point_s upper_left;
  struct point_s bottom_right;
};
static inline struct rectangle_s rectangle_s_2PointsCreate  (struct point_s upper_left,struct point_s bottom_right){
  return (struct rectangle_s){.upper_left=upper_left,.bottom_right=bottom_right};
}
static inline struct rectangle_s rectangle_s_4ValuesCreate  (int32_t ulx,int32_t uly,int32_t brx,int32_t bry){
  return rectangle_s_2PointsCreate(point_s_Create(ulx,uly),point_s_Create(brx,bry));
}

#endif//RECTANGLE_H
