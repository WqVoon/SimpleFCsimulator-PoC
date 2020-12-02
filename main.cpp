#include <cstdio>
#include <cstdlib>
#include <cassert>
#include "include/nes_cpu.h"
#include "include/simulator.h"

int main(int argc, char const *argv[])
{
  fc::simulator fc;
  
  if (argc == 2) {
    fc.load_rom(argv[1]);

    uint16_t nmi    = fc.get_memory_pool().read(fc::nes_cpu::NMI_VECTOR) |
                     (fc.get_memory_pool().read(fc::nes_cpu::NMI_VECTOR + 1) << 8);
    uint16_t reset  = fc.get_memory_pool().read(fc::nes_cpu::RESET_VECTOR) |
                     (fc.get_memory_pool().read(fc::nes_cpu::RESET_VECTOR + 1) << 8);
    uint16_t irqbrk = fc.get_memory_pool().read(fc::nes_cpu::IRQBRK_VECTOR) |
                     (fc.get_memory_pool().read(fc::nes_cpu::IRQBRK_VECTOR + 1) << 8);

    printf(
      "各中断向量: NMI: $%04x  RESET: $%04x  IRQ/BRK: $%04x\n",
      (int)nmi, (int)reset, (int)irqbrk
    );
  } else {
    assert(!"请提供参数");
  }

  fc.free_rom();
  return 0;
}
