#ifndef POINT_H
#define POINT_H
#include <stdint.h>
struct point_s{ int32_t x,y; };
static inline struct point_s point_s_Create(int32_t x,int32_t y){
  return (struct point_s){.x=x,.y=y,};
}
static inline struct point_s point_s_AddPoint(struct point_s a,struct point_s b){
  return (struct point_s){.x=a.x+b.x,.y=a.y+b.y};
}
static inline struct point_s point_s_AddScalar(struct point_s a,int32_t b){
  return (struct point_s){.x=a.x+b,.y=a.y+b};
}
static inline bool point_s_Validate(struct point_s a){
  // wait till screen size is available
  return 1;
}
#define       point_s_Add(a,b) _Generic((a),\
  point_s:_Generic((b),\
    point_s:point_s_AddPoint(a,b),\
    int32_t:point_s_AddScalar(a,b)\
  ),\
)(a,b);
// struct size_s{ uint32_t h,w; };
// inline struct size_s size_s_Create(int32_t h,int32_t w);
// inline struct size_s size_s_AddSize  (struct size_s a,struct size_s b);
// inline struct size_s size_s_AddScalar(struct size_s a,int32_t b);
// #define       size_s_Add(a,b) _Generic((a),\
//   size_s:_Generic((b),\
//     size_s:size_s_AddSize(a,b),\
//     int32_t: size_s_AddScalar(a,b)\
//     uint32_t:size_s_AddScalar(a,b)\
//   ),\
// )(a,b);

#endif//POINT_H
