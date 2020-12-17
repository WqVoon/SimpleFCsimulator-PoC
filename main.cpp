#include <cstdio>
#include <cstdlib>
#include <cassert>
#include "include/nes_utils.h"
#include "include/nes_cpu.h"
#include "include/simulator.h"

int main(int argc, char const *argv[])
{
  fc::simulator fc;
  
  if (argc == 2) {
    fc.load_rom(argv[1]);

    for (
      uint32_t idx=1;
      // idx<=1563
      ; idx++) {
      char buf[OP_BUF_LEN] = {0};
      printf("\nInst-%d\n", idx);

      fc.get_cpu().disassemble_op(fc.get_cpu().get_pc(), buf);
      printf("STR : %s\n", buf+6);

      fc.get_cpu().execute();
      // getchar();
    }

  } else {
    assert(!"请提供参数");
  }

  fc.free_rom();
  return 0;
}
