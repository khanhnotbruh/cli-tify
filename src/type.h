#ifndef TYPE_H
#define TYPE_H
#include <stdint.h>
struct i8vec2_s{  int8_t x,y; };
struct u8vec2_s{ uint8_t x,y; };
struct i8vec3_s{  int8_t x,y,z; };
struct u8vec3_s{ uint8_t x,y,z; };

struct i16vec2_s{  int16_t x,y; };
struct u16vec2_s{ uint16_t x,y; };
struct i16vec3_s{  int16_t x,y,z; };
struct u16vec3_s{ uint16_t x,y,z; };

struct i32vec2_s{  int32_t x,y; };
struct u32vec2_s{ uint32_t x,y; };
struct i32vec3_s{  int32_t x,y,z; };
struct u32vec3_s{ uint32_t x,y,z; };

typedef struct i8vec2_s i8vec2_t;
typedef struct u8vec2_s u8vec2_t;
typedef struct i8vec3_s i8vec3_t;
typedef struct u8vec3_s u8vec3_t;

typedef struct i16vec2_s i16vec2_t;
typedef struct u16vec2_s u16vec2_t;
typedef struct i16vec3_s i16vec3_t;
typedef struct u16vec3_s u16vec3_t;

typedef struct i32vec2_s i32vec2_t;
typedef struct u32vec2_s u32vec2_t;
typedef struct i32vec3_s i32vec3_t;
typedef struct u32vec3_s u32vec3_t;


#endif//TYPE_H
#ifdef  TYPE_IMPLEMENTATION
#define vectypehelper(_x,_suffix,...)\
  _x(i8##_suffix, int8_t __VA_OPT__(,)__VA_ARGS__)\
  _x(u8##_suffix,uint8_t __VA_OPT__(,)__VA_ARGS__)\
  _x(i16##_suffix, int16_t __VA_OPT__(,)__VA_ARGS__)\
  _x(u16##_suffix,uint16_t __VA_OPT__(,)__VA_ARGS__)\
  _x(i32##_suffix, int32_t __VA_OPT__(,)__VA_ARGS__)\
  _x(u32##_suffix,uint32_t __VA_OPT__(,)__VA_ARGS__)
#define idkwhattonamethis(_cast,_type,_mac)\
  _type:(_cast)_mac,
#define idkhowtonamethis(_cast,_,_mac)\
  _cast:(cast)_mac,

#define coolhelperfornumber(_a,_mac,_suffix)_Generic((_a),\
  vectypehelper(idkwhattonamethis,_suffix,_mac)\
)
#define vec2_Create(_x,_y)    coolhelperfornumber((_x),{.x=(_x),.y=(_y)},vec2_t)
#define vec3_Create(_x,_y,_z) coolhelperfornumber((_x),{.x=(_x),.y=(_y),.z=(_z)},vec3_t)
#define anothercoolhelper(_b,_vectormac,_suffix)_Generic((_b)),\
  vectypehelper(idkhowtonamethis,_suffix,_vectormac)\
)
#define vec2_Add(_a,_b)_Generic((_a),\
  )
#define vec3_Add(_a,_b)_Generic((_a),\
  anothercoolhelper((_a),(_b),\
    {.x=(_a).x+(_b).x,.y=(_a).y+(_b).y,.z=(_a).z+(_b).z},\
    {.x=(_a).x+(_b),.y=(_a).y+(_b),.z=(_a).z+(_b)},vec3_t)\
  )
#define vec2_s_Expose(_a) (_a).x,(_a).y
#define vec3_s_Expose(_a) (_a).x,(_a).y,(_a).z
#endif//TYPE_IMPLEMENTATION
