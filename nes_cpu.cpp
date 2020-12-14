#include "cstring"
#include "include/nes_cpu.h"
#include "include/nes_6502.h"
#include "include/nes_utils.h"
#include "include/nes_memory_pool.h"

namespace fc
{
  void nes_cpu::init(nes_memory_pool* mp) {
    this->memory = mp;
    const uint8_t pcl = memory->read(RESET_VECTOR);
    const uint8_t pch = memory->read(RESET_VECTOR + 1);
    registers.program_counter = (uint16_t)pcl | ((uint16_t)pch << 8);
    registers.accumulator = 0;
    registers.x_index = 0;
    registers.y_index = 0;
    registers.stack_pointer = 0xfd;
    registers.status = 0x34;

    // 测试用
    registers.program_counter = 0xc000;
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
    printf(
      "Hex: 0x%02X(%3d) 0x%02X(%3d) 0x%02X(%3d)\n",
      code.op, code.op,
      code.a1, code.a1,
      code.a2, code.a2
    );

    disassemble(code, buf+6);
  }
}