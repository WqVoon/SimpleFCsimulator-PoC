#include "cstring"
#include "include/nes_cpu.h"
#include "include/nes_6502.h"
#include "include/nes_utils.h"
#include "include/nes_memory_pool.h"

namespace fc
{
  nes_cpu::nes_cpu(nes_memory_pool* mp) {
    this->memory = mp;
  }

  void nes_cpu::disassemble_op(uint16_t addr, char buf[]) {
    memset(buf, ' ', OP_BUF_LEN);
    buf[OP_BUF_LEN - 2] = ';';
    buf[OP_BUF_LEN - 1] = 0;

    buf[0] = '$';
    btoh(buf+1, (uint8_t)(addr >> 8));
    btoh(buf+3, (uint8_t)(addr & (uint8_t)0xFF));

    nes_code code;
    code.op = memory->read(addr);
    code.a1 = memory->read(addr + 1);
    code.a2 = memory->read(addr + 2);

    disassemble(code, buf+6);
  }
}