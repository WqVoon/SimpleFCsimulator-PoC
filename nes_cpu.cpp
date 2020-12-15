#include <cassert>
#include <cstring>
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
      "Hex : 0x%02X(%3d) 0x%02X(%3d) 0x%02X(%3d)\nREGS: ",
      code.op, code.op,
      code.a1, code.a1,
      code.a2, code.a2
    );
    output_registers_and_flags();

    disassemble(code, buf+6);
  }

  uint16_t nes_cpu::address_unk() {
    assert(! "未知的寻址模式");
    return 0;
  }

  uint16_t nes_cpu::address_acc() {
    return 0;
  }

  uint16_t nes_cpu::address_imp() {
    return 0;
  }

  uint16_t nes_cpu::address_imm() {
    const uint16_t address = registers.program_counter;
    registers.program_counter++;
    return address;
  }

  uint16_t nes_cpu::address_abs() {
    const uint8_t address0 = memory->read(registers.program_counter++);
    const uint8_t address1 = memory->read(registers.program_counter++);
    return (uint16_t)address0 | (uint16_t)address1 << 8;
  }

  uint16_t nes_cpu::address_abx() {
    const uint16_t base = address_abs();
    return base + registers.x_index;
  }

  uint16_t nes_cpu::address_aby() {
    const uint16_t base = address_abs();
    return base + registers.y_index;
  }

  uint16_t nes_cpu::address_zpg() {
    return (uint16_t) memory->read(registers.program_counter++);
  }

  uint16_t nes_cpu::address_zpx() {
    const uint16_t base = address_zpg();
    return (base + registers.x_index) & (uint16_t)0x00ff;
  }

  uint16_t nes_cpu::address_zpy() {
    const uint16_t base = address_zpg();
    return (base + registers.y_index) & (uint16_t)0x00ff;
  }

  uint16_t nes_cpu::address_inx() {
    const uint8_t base
      = memory->read(registers.program_counter++)
      + registers.x_index;
    const uint8_t address0 = memory->read(base);
    const uint8_t address1 = memory->read(base + 1);
    return (uint16_t)address0 | (uint16_t)address1 << 8;
  }

  uint16_t nes_cpu::address_iny() {
    const uint8_t base = memory->read(registers.program_counter++);
    const uint8_t address0 = memory->read(base);
    const uint8_t address1 = memory->read(base + 1);
    const uint16_t address = (uint16_t)address0 | (uint16_t)address1<<8;
    return address + registers.y_index;
  }

  uint16_t nes_cpu::address_ind() {
    const uint16_t base1 = address_abs();
    // 6502 的BUG，在 base1 的低位为FF时，base2 的低位为00
    const uint16_t base2
      = (base1 & (uint16_t)0xff00)
      | ((base1 + 1) & (uint16_t)0x00ff);
    const uint16_t address
      = memory->read(base1)
      | memory->read(base2) << 8;
    return address;
  }

  uint16_t nes_cpu::address_rel() {
    const uint8_t offset = memory->read(registers.program_counter++);
    const uint16_t address = registers.program_counter + (int8_t)offset;
    return address;
  }

  void nes_cpu::output_registers_and_flags() {
    printf(
      "PC:%04X ACC:%02X X:%02X Y:%02X SP:%02X "
      "(CF:%d ZF:%d IF:%d BF:%d OF:%d SF:%d)\n",
      registers.program_counter,
      registers.accumulator,
      registers.x_index,
      registers.y_index,
      registers.stack_pointer,
      (registers.status & SFC_INDEX_C) == SFC_FLAG_C,
      (registers.status & SFC_INDEX_Z) == SFC_FLAG_Z,
      (registers.status & SFC_INDEX_I) == SFC_FLAG_I,
      (registers.status & SFC_INDEX_B) == SFC_FLAG_B,
      (registers.status & SFC_INDEX_V) == SFC_FLAG_V,
      (registers.status & SFC_INDEX_S) == SFC_FLAG_S
    );
  }

}