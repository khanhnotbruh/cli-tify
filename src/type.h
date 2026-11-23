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
#if defined(TYPE_IMPLEMENTATION)&&defined(OVERLOAD_IMPLEMETATION)
#define vec2_s_Expose(_a) (_a).x,(_a).y
#define vec3_s_Expose(_a) (_a).x,(_a).y,(_a).z
//                 func,type suffix,macro
#define vectortypehelper(_mac,_suffix,...)\
  _mac(i8##_suffix, int8_t   __VA_OPT__(,)__VA_ARGS__)\
  _mac(u8##_suffix,uint8_t   __VA_OPT__(,)__VA_ARGS__)\
  _mac(i16##_suffix, int16_t __VA_OPT__(,)__VA_ARGS__)\
  _mac(u16##_suffix,uint16_t __VA_OPT__(,)__VA_ARGS__)\
  _mac(i32##_suffix, int32_t __VA_OPT__(,)__VA_ARGS__)\
  _mac(u32##_suffix,uint32_t __VA_OPT__(,)__VA_ARGS__)


#define generateaddingfunctionvec2(_vtype,_itype)\
  static inline _vtype _vtype##_AddScalar(_vtype a,_itype b){return (_vtype){.x=a.x+b,.y=a.y+b};}\
  static inline _vtype _vtype##_AddVector(_vtype a,_vtype b){return (_vtype){.x=a.x+b.x,.y=a.y+b.y};}
#define generateaddingfunctionvec3(_vtype,_itype)\
  static inline _vtype _vtype##_AddScalar(_vtype a,_itype b){return (_vtype){.x=a.x+b,.y=a.y+b,.z=a.z+b};}\
  static inline _vtype _vtype##_AddVector(_vtype a,_vtype b){return (_vtype){.x=a.x+b.x,.y=a.y+b.y,.z=a.z+b.z};}

vectortypehelper(generateaddingfunctionvec2,vec2_t);
vectortypehelper(generateaddingfunctionvec3,vec3_t);


#define idkwhattoputinside(_,_itype,...)     _itype:__VA_ARGS__,
#define idkwheretoputthis(_vtype,_,...)      _vtype:__VA_ARGS__,
#define idkwhattonamethis(_vtype,_itype,...) _itype:(_vtype){__VA_ARGS__},
#define idkhowtonamethis(_vtype,_,...)       _vtype:(_vtype){__VA_ARGS__},
#define coolvec2_t(_x,_y)    .x=(_x),.y=(_y)
#define coolvec3_t(_x,_y,_z) .x=(_x),.y=(_y),.z=(_z)
#define coolvvec2_t(_a,_b) coolvec2_t((_a).x+(_b).x,(_a).y+(_b).y)
#define coolsvec2_t(_a,_b) coolvec2_t((_a).x+(_b),(_a).y+(_b))
#define coolvvec3_t(_a,_b) coolvec3_t((_a).x+(_b).x,(_a).y+(_b).y,(_a).z+(_b).z)
#define coolsvec3_t(_a,_b) coolvec3_t((_a).x+(_b),(_a).y+(_b),(_a).z+(_b))

#define coolhelperfornumber(_a,_suffix,...)_Generic((_a),\
  vectypehelper(idkwhattonamethis,_suffix,__VA_ARGS__)\
)
#define vec2_Create(_x,_y)    coolhelperfornumber((_x),vec2_t,coolvec2_t(_x,_y)   )
#define vec3_Create(_x,_y,_z) coolhelperfornumber((_x),vec3_t,coolvec3_t(_x,_y,_z))


#define cooleradd(_suffix,_a,_b) _Generic((_b),\
  vectypehelper(idkhowtonamethis ,_suffix,coolv##_suffix(_a,_b))\
  vectypehelper(idkwhattonamethis,_suffix,cools##_suffix(_a,_b))\
  )
#define coolhelperforvector(_suffix,_a,_b)_Generic((_a),\
  vectypehelper(idkwheretoputthis,_suffix,cooleradd(_suffix,_a,_b)\
)

#define vec2_Add(_a,_b) coolhelperforvector(vec2_t,(_a),(_b))
#define vec3_Add(_a,_b) coolhelperforvector(vec3_t,(_a),(_b))

#endif//TYPE_IMPLEMENTATION
