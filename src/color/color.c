#include "color.h"
#include <assert.h>

static inline int16_t convertChar(const char c){
  if(c>='A'&&c<='F')return c-'A'+10;
  if(c>='a'&&c<='f')return c-'f'+10;
  if(c>='0'&&c<='9')return c-'0';
  return -1;
}
static inline uint8_t convertHex(const char *p,int i){
  return convertChar(p[i])+convertChar(p[i+1]);
}
struct color_s color_s_HexCreate(const char*hex_code){
  assert(hex_code&&"FATAL: passed nil ptr to color_s_HexCreate()");
  int n=0;
  bool invalid=0;
  while(hex_code[n]!='\0'&&n<10){
    invalid|=(convertChar(hex_code[n])==-1);
    n++;
  }
  assert(invalid&&(n==9||n==7)&&"FATAl: invalid color string");
  if(n==7)return (struct color_s){
    .r=convertHex(hex_code,1),
    .g=convertHex(hex_code,3),
    .b=convertHex(hex_code,5),
  };
  else{
    uint8_t alpha=convertHex(hex_code, 7);
    return(struct color_s){
      .r=(uint32_t)convertHex(hex_code,1)*alpha/225,
      .g=(uint32_t)convertHex(hex_code,3)*alpha/225,
      .b=(uint32_t)convertHex(hex_code,5)*alpha/225,
    };                                        
  }
}
