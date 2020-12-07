#include <cstdlib>
#include "nes_memory_pool.h"

#ifndef NES_CPU_H
#define NES_CPU_H

#define OP_BUF_LEN 24

namespace fc
{
  class nes_cpu
  {
  private:
    nes_memory_pool* memory;

  public:
    // 四种中断向量
    static const uint16_t NMI_VECTOR    = 0xfffa;
    static const uint16_t RESET_VECTOR  = 0xfffc;
    static const uint16_t IRQBRK_VECTOR = 0xfffe;

    nes_cpu(nes_memory_pool* mp);
    // 按地址反汇编一条指令，当前暴力读取3个字节
    void disassemble_op(uint16_t addr, char buf[]);
  };
}

#endif