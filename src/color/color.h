#ifndef COLOR_H
#define COLOR_H
#include <stdint.h>
struct color_s{
  uint8_t r,g,b;
};
struct color_s color_s_HexCreate(const char*hex_code);
static inline struct color_s color_s_RGBCreate(uint8_t r,uint8_t g,uint8_t b){
  return (struct color_s){.r=r,.g=g,.b=b};
}
#define color_s_Expand(_col) _col.r,_col.g,_col.b
#endif//COLOR_H
