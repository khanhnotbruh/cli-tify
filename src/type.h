#ifndef TYPE_H
#define TYPE_H
#include <stdint.h>
#if !defined(USE_8BIT)||!USE_8BIT
struct i8vec2_s{  int8_t x,y; };
struct u8vec2_s{ uint8_t x,y; };
struct i8vec3_s{  int8_t x,y,z; };
struct u8vec3_s{ uint8_t x,y,z; };
typedef struct i8vec2_s i8vec2_t;
typedef struct u8vec2_s u8vec2_t;
typedef struct i8vec3_s i8vec3_t;
typedef struct u8vec3_s u8vec3_t;
#endif
#if !defined(USE_16BIT)||!USE_16BIT
struct i16vec2_s{  int16_t x,y; };
struct u16vec2_s{ uint16_t x,y; };
struct i16vec3_s{  int16_t x,y,z; };
struct u16vec3_s{ uint16_t x,y,z; };
typedef struct i16vec2_s i16vec2_t;
typedef struct u16vec2_s u16vec2_t;
typedef struct i16vec3_s i16vec3_t;
typedef struct u16vec3_s u16vec3_t;
#endif
#if !defined(USE_32BIT)||!USE_32BIT
struct i32vec2_s{  int32_t x,y; };
struct u32vec2_s{ uint32_t x,y; };
struct i32vec3_s{  int32_t x,y,z; };
struct u32vec3_s{ uint32_t x,y,z; };
typedef struct i32vec2_s i32vec2_t;
typedef struct u32vec2_s u32vec2_t;
typedef struct i32vec3_s i32vec3_t;
typedef struct u32vec3_s u32vec3_t;
#endif
#if !defined(USE_64BIT)||!USE_64BIT
struct i64vec2_s{  int64_t x,y; };
struct u64vec2_s{ uint64_t x,y; };
struct i64vec3_s{  int64_t x,y,z; };
struct u64vec3_s{ uint64_t x,y,z; };
typedef struct i64vec2_s i64vec2_t;
typedef struct u64vec2_s u64vec2_t;
typedef struct i64vec3_s i64vec3_t;
typedef struct u64vec3_s u64vec3_t;
#endif
#endif//TYPE_H
#if defined(TYPE_IMPLEMENTATION)&&defined(OVERLOAD_IMPLEMETATION)
#define vec2_s_Expose(_a) (_a).x,(_a).y
#define vec3_s_Expose(_a) (_a).x,(_a).y,(_a).z
//                 func,type suffix,macro
#if !defined(USE_8BIT)||!USE_8BIT
#define vec8_preset(_mac,_suffix,...)\
  _mac( int8_t ,i8##_suffix  __VA_OPT__(,)__VA_ARGS__)\
  _mac(uint8_t ,u8##_suffix  __VA_OPT__(,)__VA_ARGS__)
#else
#define vec8_preset(_mac,_suffix,...)
#endif
#if !defined(USE_16BIT)||!USE_16BIT
#define vec16_preset(_mac,_suffix,...)\
  _mac( int16_t,i16##_suffix __VA_OPT__(,)__VA_ARGS__)\
  _mac(uint16_t,u16##_suffix __VA_OPT__(,)__VA_ARGS__)
#else
#define vec16_preset(_mac,_suffix,...)
#endif
#if !defined(USE_32BIT)||!USE_32BIT
#define vec32_preset(_mac,_suffix,...)\
  _mac( int32_t,i32##_suffix __VA_OPT__(,)__VA_ARGS__)\
  _mac(uint32_t,u32##_suffix __VA_OPT__(,)__VA_ARGS__)
#else
#define vec32_preset(_mac,_suffix,...)
#endif
#if !defined(USE_64BIT)||!USE_64BIT
#define vec64preset(_mac,_suffix,...)\
  _mac( int64_t,i64##_suffix __VA_OPT__(,)__VA_ARGS__)\
  _mac(uint64_t,u64##_suffix __VA_OPT__(,)__VA_ARGS__)
#else
#define vec64_preset(_mac,_suffix,...)
#endif
#define vec_preset(_mac,_suffix,...)\
  vec8_preset(_mac,_suffix,__VA_OPT__(,)__VA_ARGS__)\
  vec16_preset(_mac,_suffix,__VA_OPT__(,)__VA_ARGS__)\
  vec32_preset(_mac,_suffix,__VA_OPT__(,)__VA_ARGS__)\
  vec64_preset(_mac,_suffix,__VA_OPT__(,)__VA_ARGS__)

#define ops_preset(_mac,...)\
  _mac(Add,+ __VA_OPT__(,)__VA_ARGS__)\
  _mac(Sub,- __VA_OPT__(,)__VA_ARGS__)\
  _mac(Mul,* __VA_OPT__(,)__VA_ARGS__)\
  _mac(Div,/ __VA_OPT__(,)__VA_ARGS__)
#define opsvec_preset_step(_itype,_vtype,_mac,...)\
  ops_preset(_mac,_itype,_vtype)
#define opsvec_preset(_mac,_suffix,...) \
  vec_preset(opsvec_preset_step,_suffix,_mac __VA_OPT__(,)__VA_ARGS__)
#define generate_createfunctionvec2(_itype,_vtype)\
  static inline _vtype _vtype##_Create\
    (_itype a,_itype b){return (_vtype){.x=a,.y=b};}
#define generate_createfunctionvec3(_itype,_vtype)\
  static inline _vtype _vtype##_Create\
    (_itype a,_itype b,_itype c){return (_vtype){.x=a,.y=b,.z=c};}\

#define generate_opsfunctionvec2(_opsname,_opssym,_itype,_vtype)\
  static inline _vtype _vtype##_##_opsname##Scalar(_vtype a,_itype b){return (_vtype){.x=a.x _opssym b  ,.y=a.y _opssym b};}\
  static inline _vtype _vtype##_##_opsname##Vector(_vtype a,_vtype b){return (_vtype){.x=a.x _opssym b.x,.y=a.y _opssym b.y};}
#define generate_opsfunctionvec3(_opsname,_opssym,_itype,_vtype)\
  static inline _vtype _vtype##_##_opsname##Scalar(_vtype a,_itype b){return (_vtype){.x=a.x _opssym b  ,.y=a.y _opssym b  ,.z=a.z _opssym b};}\
  static inline _vtype _vtype##_##_opsname##Vector(_vtype a,_vtype b){return (_vtype){.x=a.x _opssym b.x,.y=a.y _opssym b.y,.z=a.z _opssym b.z};}

opsvec_preset(generate_opsfunctionvec2,vec2_t)
opsvec_preset(generate_opsfunctionvec3,vec3_t)
#undef generatefunctionvec2
#undef generatefunctionvec3

#define scatype_withoutcast(_,_itype,...)     _itype:__VA_ARGS__,
#define vectype_withoutcast(_vtype,_,...)     _vtype:__VA_ARGS__,
#define scatype_withcast(_vtype,_itype,...)   _itype:(_vtype){__VA_ARGS__},
#define vectype_withcast(_vtype,_,...)        _vtype:(_vtype){__VA_ARGS__},

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
