// #define TB_OPT_ATTR_W 32
// #define TB_IMPL
// #include "termbox2.h"
#include "global.h"
#include <stdio.h>
#include <assert.h>
#define TYPE_IMPLEMENTATION 1
#define TEST_VEC2(T,S) do {                  \
    vec2_Create((S)2,(S)3);            \
    vec2_Create((S)4,(S)5);            \
    T c = vec2_Add(a,b);                     \
    T d = vec2_Add(a,(S)10);                 \
                                               \
    assert(c.x == 6 && c.y == 8);             \
    assert(d.x == 12 && d.y == 13);           \
} while(0)

#define TEST_VEC3(T,S) do {                  \
    vec3_Create((S)2,(S)3,(S)4);        \
    vec3_Create((S)4,(S)5,(S)6);        \
    T c = vec3_Add(a,b);                     \
    T d = vec3_Add(a,(S)10);                 \
                                               \
    assert(c.x == 6 && c.y == 8 && c.z == 10);\
    assert(d.x == 12 && d.y == 13 && d.z == 14);\
} while(0)

int testtype(void) {
    TEST_VEC2(i8vec2_t, int8_t);
    TEST_VEC2(u8vec2_t, uint8_t);
    TEST_VEC2(i16vec2_t, int16_t);
    TEST_VEC2(u16vec2_t, uint16_t);
    TEST_VEC2(i32vec2_t, int32_t);
    TEST_VEC2(u32vec2_t, uint32_t);

    TEST_VEC3(i8vec3_t, int8_t);
    TEST_VEC3(u8vec3_t, uint8_t);
    TEST_VEC3(i16vec3_t, int16_t);
    TEST_VEC3(u16vec3_t, uint16_t);
    TEST_VEC3(i32vec3_t, int32_t);
    TEST_VEC3(u32vec3_t, uint32_t);

    puts("All vector tests passed!");
    return 0;
}
int main(){
  // struct tb_event ev;
  // tb_init();
  // if (tb_has_truecolor()) {
  //   tb_set_output_mode(TB_OUTPUT_TRUECOLOR);
  // }
  // tb_print(2, 2, 0xFF5733 | TB_BOLD, 0x1A1A2E, "Hello in 24-bit Truecolor!");
  testtype();
  // tb_present();
  // tb_poll_event(&ev);
  // tb_shutdown();
  return 0;
}
