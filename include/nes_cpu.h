#include <cstdlib>

#ifndef NES_CPU_H
#define NES_CPU_H

namespace fc
{
  class nes_cpu
  {
  public:
    // 四种中断向量
    static const uint16_t NMI_VECTOR    = 0xfffa;
    static const uint16_t RESET_VECTOR  = 0xfffc;
    static const uint16_t IRQBRK_VECTOR = 0xfffe;
  };
}

#endif