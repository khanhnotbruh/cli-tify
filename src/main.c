#define TB_OPT_ATTR_W 32
#define TB_IMPL
#include "global.h"
#include "termbox2.h"
#include <stdio.h>
int main(){
  // struct tb_event ev;
  // tb_init();
  // if (tb_has_truecolor()) {
  //   tb_set_output_mode(TB_OUTPUT_TRUECOLOR);
  // }
  // tb_print(2, 2, 0xFF5733 | TB_BOLD, 0x1A1A2E, "Hello in 24-bit Truecolor!");
  for(int i=0;i<(1<<10);i++)printf("%010b\n",i);
  // tb_present();
  // tb_poll_event(&ev);
  // tb_shutdown();
  return 0;
}
