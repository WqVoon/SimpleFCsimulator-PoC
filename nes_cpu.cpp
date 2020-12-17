#include <cassert>
#include <cstring>
#include "include/nes_cpu.h"
#include "include/nes_6502.h"
#include "include/nes_utils.h"
#include "include/nes_memory_pool.h"

// 用来简化 case 的排列
#define OP(n, a, o)\
case 0x##n:\
{\
  operate_##o(address_##a());\
  break;\
}

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

  void nes_cpu::execute() {
    const uint8_t opcode = memory->read(registers.program_counter++);
    switch (opcode) {
      OP(01, inx, ora)
      OP(05, zpg, ora)
      OP(06, zpg, asl)
      OP(08, imp, php)
      OP(09, imm, ora)
      OP(0A, acc, asla)
      OP(0D, abs, ora)
      OP(0E, abs, asl)
      OP(10, rel, bpl)
      OP(18, imp, clc)
      OP(20, abs, jsr)
      OP(21, inx, and)
      OP(24, zpg, bit)
      OP(25, zpg, and)
      OP(26, zpg, rol)
      OP(28, imp, plp)
      OP(29, imm, and)
      OP(2A, acc, rola)
      OP(2C, abs, bit)
      OP(2D, abs, and)
      OP(2E, abs, rol)
      OP(30, rel, bmi)
      OP(38, imp, sec)
      OP(40, imp, rti)
      OP(41, inx, eor)
      OP(45, zpg, eor)
      OP(46, zpg, lsr)
      OP(48, imp, pha)
      OP(49, imm, eor)
      OP(4A, acc, lsra)
      OP(4C, abs, jmp)
      OP(4D, abs, eor)
      OP(4E, abs, lsr)
      OP(50, rel, bvc)
      OP(60, imp, rts)
      OP(61, inx, adc)
      OP(65, zpg, adc)
      OP(66, zpg, ror)
      OP(68, imp, pla)
      OP(69, imm, adc)
      OP(6A, acc, rora)
      OP(6D, abs, adc)
      OP(6E, abs, ror)
      OP(70, rel, bvs)
      OP(78, imp, sei)
      OP(81, inx, sta)
      OP(84, zpg, sty)
      OP(85, zpg, sta)
      OP(86, zpg, stx)
      OP(88, imp, dey)
      OP(8A, imp, txa)
      OP(8D, abs, sta)
      OP(8C, abs, sty)
      OP(8E, abs, stx)
      OP(90, rel, bcc)
      OP(98, imp, tya)
      OP(9A, imp, txs)
      OP(A0, imm, ldy)
      OP(A1, inx, lda)
      OP(A2, imm, ldx)
      OP(A4, zpg, ldy)
      OP(A5, zpg, lda)
      OP(A6, zpg, ldx)
      OP(A8, imp, tay)
      OP(A9, imm, lda)
      OP(AA, imp, tax)
      OP(AC, abs, ldy)
      OP(AD, abs, lda)
      OP(AE, abs, ldx)
      OP(B0, rel, bcs)
      OP(B8, imp, clv)
      OP(BA, imp, tsx)
      OP(C0, imm, cpy)
      OP(C1, inx, cmp)
      OP(C4, zpg, cpy)
      OP(C5, zpg, cmp)
      OP(C6, zpg, dec)
      OP(C8, imp, iny)
      OP(C9, imm, cmp)
      OP(CA, imp, dex)
      OP(CC, abs, cpy)
      OP(CD, abs, cmp)
      OP(CE, abs, dec)
      OP(D0, rel, bne)
      OP(D8, imp, cld)
      OP(E0, imm, cpx)
      OP(E1, inx, sbc)
      OP(E4, zpg, cpx)
      OP(E5, zpg, sbc)
      OP(E6, zpg, inc)
      OP(E8, imp, inx)
      OP(E9, imm, sbc)
      OP(EA, imp, nop)
      OP(EC, abs, cpx)
      OP(ED, abs, sbc)
      OP(EE, abs, inc)
      OP(F0, rel, beq)
      OP(F8, imp, sed)
      default: assert(! "尚未实现的指令");
    }
  }

  void nes_cpu::check_zf_and_sf(uint8_t data) {
    if (! data) {
      registers.status |= SFC_FLAG_Z;
    } else {
      registers.status &= ~SFC_FLAG_Z;
    }

    if (data & (uint8_t)0x80) {
      registers.status |= SFC_FLAG_S;
    } else {
      registers.status &= ~SFC_FLAG_S;
    }
  }

  void nes_cpu::stack_push(uint8_t data) {
    (memory->main_memory + 0x100)[registers.stack_pointer--] = data;
  }

  uint8_t nes_cpu::stack_pop() {
    return (memory->main_memory + 0x100)[++registers.stack_pointer];
  }

  void nes_cpu::disassemble_op(uint16_t addr, char buf[]) {
    memset(buf, ' ', OP_BUF_LEN);
    buf[OP_BUF_LEN - 2] = ';';
    buf[OP_BUF_LEN - 1] = 0;

    buf[0] = '$';
    btoh(buf+1, (uint8_t)(addr >> 8));
    btoh(buf+3, (uint8_t)(addr & (uint8_t)0xFF));

    nes_code code;
    uint8_t bytes[3];
    code.op = bytes[0] = memory->read(addr);
    code.a1 = bytes[1] = memory->read(addr + 1);
    code.a2 = bytes[2] = memory->read(addr + 2);
    const uint8_t length = disassemble(code, buf+6);

    // 输出内部寄存器的值
    output_registers_and_flags();
    // 输出读取的几个字节的十六进制及十进制表示
    printf("Hex :");
    for (uint8_t i=0; i < length; i++) {
      printf(" %02X(%3d)", bytes[i], bytes[i]);
    }
    printf("\n");
  }

  void nes_cpu::output_registers_and_flags() {
    printf(
      "REGS: "
      "PC:%04X ACC:%02X X:%02X Y:%02X SP:%02X "
      "(CF:%d ZF:%d IF:%d DF:%d BF:%d VF:%d SF:%d)\n",
      registers.program_counter,
      registers.accumulator,
      registers.x_index,
      registers.y_index,
      registers.stack_pointer,
      (registers.status & SFC_FLAG_C) == SFC_FLAG_C,
      (registers.status & SFC_FLAG_Z) == SFC_FLAG_Z,
      (registers.status & SFC_FLAG_I) == SFC_FLAG_I,
      (registers.status & SFC_FLAG_D) == SFC_FLAG_D,
      (registers.status & SFC_FLAG_B) == SFC_FLAG_B,
      (registers.status & SFC_FLAG_V) == SFC_FLAG_V,
      (registers.status & SFC_FLAG_S) == SFC_FLAG_S
    );
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
    uint8_t base
      = memory->read(registers.program_counter++)
      + registers.x_index;
    const uint8_t address0 = memory->read(base);
    // 注意这里的自增，不能直接加一，因为 0xff 的下一位取了 0x00（6502 的 bug）
    const uint8_t address1 = memory->read(++base);
    return (uint16_t)address0 | uint16_t(address1) << 8;
  }

  uint16_t nes_cpu::address_iny() {
    uint8_t base = memory->read(registers.program_counter++);
    const uint8_t address0 = memory->read(base);
    // 自增同上
    const uint8_t address1 = memory->read(++base);
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

  void nes_cpu::operate_jmp(uint16_t address) {
    registers.program_counter = address;
  }

  void nes_cpu::operate_ldx(uint16_t address) {
    registers.x_index = memory->read(address);
    check_zf_and_sf(registers.x_index);
  }

  void nes_cpu::operate_stx(uint16_t address) {
    memory->write(address, registers.x_index);
  }

  void nes_cpu::operate_jsr(uint16_t address) {
    const uint16_t pc = registers.program_counter - 1;
    stack_push(uint8_t(pc >> 8));
    stack_push(uint8_t(pc));
    registers.program_counter = address;
  }

  void nes_cpu::operate_nop(uint16_t) {}

  void nes_cpu::operate_sec(uint16_t) {
    registers.status |= SFC_FLAG_C;
  }

  void nes_cpu::operate_bcs(uint16_t address) {
    if (registers.status & SFC_FLAG_C) {
      registers.program_counter = address;
    }
  }

  void nes_cpu::operate_clc(uint16_t) {
    registers.status &= ~SFC_FLAG_C;
  }

  void nes_cpu::operate_bcc(uint16_t address) {
    if (! (registers.status & SFC_FLAG_C)) {
      registers.program_counter = address;
    }
  }

  void nes_cpu::operate_lda(uint16_t address) {
    registers.accumulator = memory->read(address);
    check_zf_and_sf(registers.accumulator);
  }

  void nes_cpu::operate_beq(uint16_t address) {
    if (registers.status & SFC_FLAG_Z) {
      registers.program_counter = address;
    }
  }

  void nes_cpu::operate_bne(uint16_t address) {
    if (! (registers.status & SFC_FLAG_Z)) {
      registers.program_counter = address;
    }
  }

  void nes_cpu::operate_sta(uint16_t address) {
    memory->write(address, registers.accumulator);
  }

  void nes_cpu::operate_bit(uint16_t address) {
    const uint8_t data = memory->read(address);

    // VF = (data >> 6) & 1
    if (data & (uint8_t)0x40) {
      registers.status |= SFC_FLAG_V;
    } else {
      registers.status &= ~SFC_FLAG_V;
    }
    // SF = (data >> 7) & 1
    if (data & (uint8_t)0x80) {
      registers.status |= SFC_FLAG_S;
    } else {
      registers.status &= ~SFC_FLAG_S;
    }
    // ZF = A & tmp? 0: 1
    if (registers.accumulator & data) {
      registers.status &= ~SFC_FLAG_Z;
    } else {
      registers.status |= SFC_FLAG_Z;
    }
  }

  void nes_cpu::operate_bvs(uint16_t address) {
    if (registers.status & SFC_FLAG_V) {
      registers.program_counter = address;
    }
  }

  void nes_cpu::operate_bvc(uint16_t address) {
    if (! (registers.status & SFC_FLAG_V)) {
      registers.program_counter = address;
    }
  }

  void nes_cpu::operate_bpl(uint16_t address) {
    if (! (registers.status & SFC_FLAG_S)) {
      registers.program_counter = address;
    }
  }

  void nes_cpu::operate_rts(uint16_t address) {
    registers.program_counter
      = uint16_t(stack_pop())
      | (uint16_t(stack_pop()) << 8);
    ++registers.program_counter;
  }

  void nes_cpu::operate_sei(uint16_t) {
    registers.status |= SFC_FLAG_I;
  }

  void nes_cpu::operate_sed(uint16_t) {
    registers.status |= SFC_FLAG_D;
  }

  void nes_cpu::operate_php(uint16_t) {
    stack_push(registers.status | SFC_FLAG_B | SFC_FLAG_R);
  }

  void nes_cpu::operate_pla(uint16_t) {
    registers.accumulator = stack_pop();
    check_zf_and_sf(registers.accumulator);
  }

  void nes_cpu::operate_and(uint16_t address) {
    const uint8_t data = memory->read(address);
    registers.accumulator &= data;
    check_zf_and_sf(registers.accumulator);
  }

  void nes_cpu::operate_cmp(uint16_t address) {
    uint16_t result
      = (uint16_t)registers.accumulator
      - (uint16_t)memory->read(address);

    if (result < 0x100) {
      registers.status |= SFC_FLAG_C;
    } else {
      registers.status &= ~SFC_FLAG_C;
    }

    check_zf_and_sf((uint8_t)result);
  }

  void nes_cpu::operate_cld(uint16_t) {
    registers.status &= ~SFC_FLAG_D;
  }

  void nes_cpu::operate_pha(uint16_t) {
    stack_push(registers.accumulator);
  }

  void nes_cpu::operate_plp(uint16_t) {
    registers.status = stack_pop();
    registers.status &= ~SFC_FLAG_B;
  }

  void nes_cpu::operate_bmi(uint16_t address) {
    if (registers.status & SFC_FLAG_S) {
      registers.program_counter = address;
    }
  }

  void nes_cpu::operate_ora(uint16_t address) {
    const uint8_t data = memory->read(address);
    registers.accumulator |= data;
    check_zf_and_sf(registers.accumulator);
  }

  void nes_cpu::operate_clv(uint16_t) {
    registers.status &= ~SFC_FLAG_V;
  }

  void nes_cpu::operate_eor(uint16_t address) {
    const uint8_t data = memory->read(address);
    registers.accumulator ^= data;
    check_zf_and_sf(registers.accumulator);
  }

  void nes_cpu::operate_adc(uint16_t address) {
    const uint8_t data = memory->read(address);
    const uint16_t result16
      = (uint16_t)registers.accumulator
      + (uint16_t)data
      + (registers.status & SFC_FLAG_C? 1: 0);
    const uint8_t result8 = (uint8_t)result16;

    // 如果有进位那么置 CF，否则清空
    if (result16 >> 8) {
      registers.status |= SFC_FLAG_C;
    } else {
      registers.status &= ~SFC_FLAG_C;
    }
    // 如果两个操作数同号而结果与之异号，那么置 VF，否则清空
    if (
      !((registers.accumulator ^ data) & 0x80)
      &&
      ((registers.accumulator ^ result8) & 0x80)
    ) {
      registers.status |= SFC_FLAG_V;
    } else {
      registers.status &= ~SFC_FLAG_V;
    }
    registers.accumulator = result8;
    check_zf_and_sf(result8);
  }

  void nes_cpu::operate_ldy(uint16_t address) {
    const uint8_t data = memory->read(address);
    registers.y_index = data;
    check_zf_and_sf(data);
  }

  void nes_cpu::operate_cpy(uint16_t address) {
    const uint16_t result
      = (uint16_t)registers.y_index
      - (uint16_t)memory->read(address);

    if (result < 0x100) {
      registers.status |= SFC_FLAG_C;
    } else {
      registers.status &= ~SFC_FLAG_C;
    }

    check_zf_and_sf((uint8_t)result);
  }

  void nes_cpu::operate_cpx(uint16_t address) {
    const uint16_t result
      = (uint16_t)registers.x_index
      - (uint16_t)memory->read(address);

    if (result < 0x100) {
      registers.status |= SFC_FLAG_C;
    } else {
      registers.status &= ~SFC_FLAG_C;
    }

    check_zf_and_sf((uint8_t)result);
  }

  void nes_cpu::operate_sbc(uint16_t address) {
    const uint8_t data = memory->read(address);
    const uint16_t result16
      = (uint16_t)registers.accumulator
      - (uint16_t)data
      - (registers.status & SFC_FLAG_C? 0: 1);
    const uint8_t result8 = (uint8_t)result16;

    // 如果有进位那么置 CF，否则清空
    if (! (result16 >> 8)) {
      registers.status |= SFC_FLAG_C;
    } else {
      registers.status &= ~SFC_FLAG_C;
    }
    // 如果两个操作数异号且结果与被减数异号，那么置 VF，否则清空
    if (
      ((registers.accumulator ^ data) & 0x80)
      &&
      ((registers.accumulator ^ result8) & 0x80)
    ) {
      registers.status |= SFC_FLAG_V;
    } else {
      registers.status &= ~SFC_FLAG_V;
    }
    registers.accumulator = result8;
    check_zf_and_sf(result8);
  }

  void nes_cpu::operate_iny(uint16_t) {
    ++registers.y_index;
    check_zf_and_sf(registers.y_index);
  }

  void nes_cpu::operate_inx(uint16_t) {
    ++registers.x_index;
    check_zf_and_sf(registers.x_index);
  }

  void nes_cpu::operate_dey(uint16_t) {
    --registers.y_index;
    check_zf_and_sf(registers.y_index);
  }

  void nes_cpu::operate_dex(uint16_t) {
    --registers.x_index;
    check_zf_and_sf(registers.x_index);
  }

  void nes_cpu::operate_tay(uint16_t) {
    registers.y_index = registers.accumulator;
    check_zf_and_sf(registers.y_index);
  }

  void nes_cpu::operate_tax(uint16_t) {
    registers.x_index = registers.accumulator;
    check_zf_and_sf(registers.x_index);
  }

  void nes_cpu::operate_tya(uint16_t) {
    registers.accumulator = registers.y_index;
    check_zf_and_sf(registers.accumulator);
  }

  void nes_cpu::operate_txa(uint16_t) {
    registers.accumulator = registers.x_index;
    check_zf_and_sf(registers.accumulator);
  }

  void nes_cpu::operate_tsx(uint16_t) {
    registers.x_index = registers.stack_pointer;
    check_zf_and_sf(registers.x_index);
  }

  void nes_cpu::operate_txs(uint16_t) {
    registers.stack_pointer = registers.x_index;
  }

  void nes_cpu::operate_rti(uint16_t) {
    registers.status = stack_pop();
    registers.status |= SFC_FLAG_R;
    registers.status &= ~SFC_FLAG_B;

    const uint8_t pcl = stack_pop();
    const uint8_t pch = stack_pop();
    registers.program_counter
      = (uint16_t)pcl
      | (uint16_t)pch << 8;
  }

  void nes_cpu::operate_lsra(uint16_t) {
    uint8_t data = registers.accumulator;

    // 把最低位送入到 CF 中
    if (data & 1) {
      registers.status |= SFC_FLAG_C;
    } else {
      registers.status &= ~SFC_FLAG_C;
    }

    data >>= 1;
    registers.accumulator = data;
    check_zf_and_sf(data);
  }

  void nes_cpu::operate_lsr(uint16_t address) {
    uint8_t data = memory->read(address);

    // 把最低位送入到 CF 中
    if (data & 1) {
      registers.status |= SFC_FLAG_C;
    } else {
      registers.status &= ~SFC_FLAG_C;
    }

    data >>= 1;
    memory->write(address, data);
    check_zf_and_sf(data);
  }

  void nes_cpu::operate_asla(uint16_t) {
    uint8_t data = registers.accumulator;

    if (data & 0x80) {
      registers.status |= SFC_FLAG_C;
    } else {
      registers.status &= ~SFC_FLAG_C;
    }

    data <<= 1;
    registers.accumulator = data;
    check_zf_and_sf(data);
  }

  void nes_cpu::operate_rora(uint16_t) {
    uint16_t data = registers.accumulator;
    data |= uint16_t(registers.status & SFC_FLAG_C) << 8;
    if (data & 1) {
      registers.status |= SFC_FLAG_C;
    } else {
      registers.status &= ~SFC_FLAG_C;
    }
    data >>= 1;
    registers.accumulator = (uint8_t)data;
    check_zf_and_sf(registers.accumulator);
  }

  void nes_cpu::operate_rola(uint16_t) {
    uint16_t data = registers.accumulator;
    data <<= 1;
    data |= (registers.status & SFC_FLAG_C);
    if (data & (uint16_t)0x0100) {
      registers.status |= SFC_FLAG_C;
    } else {
      registers.status &= ~SFC_FLAG_C;
    }
    registers.accumulator = (uint8_t)data;
    check_zf_and_sf(registers.accumulator);
  }

  void nes_cpu::operate_sty(uint16_t address) {
    memory->write(address, registers.y_index);
  }

  void nes_cpu::operate_asl(uint16_t address) {
    uint8_t data = memory->read(address);

    if (data & 0x80) {
      registers.status |= SFC_FLAG_C;
    } else {
      registers.status &= ~SFC_FLAG_C;
    }

    data <<= 1;
    memory->write(address, data);
    check_zf_and_sf(data);
  }

  void nes_cpu::operate_ror(uint16_t address) {
    uint16_t data = memory->read(address);
    data |= uint16_t(registers.status & SFC_FLAG_C) << 8;
    if (data & 1) {
      registers.status |= SFC_FLAG_C;
    } else {
      registers.status &= ~SFC_FLAG_C;
    }
    data >>= 1;
    memory->write(address, (uint8_t)data);
    check_zf_and_sf((uint8_t)data);
  }

  void nes_cpu::operate_rol(uint16_t address) {
    uint16_t data = memory->read(address);
    data <<= 1;
    data |= (registers.status & SFC_FLAG_C);
    if (data & (uint16_t)0x0100) {
      registers.status |= SFC_FLAG_C;
    } else {
      registers.status &= ~SFC_FLAG_C;
    }
    registers.accumulator = (uint8_t)data;
    memory->write(address, (uint8_t)data);
    check_zf_and_sf((uint8_t)data);
  }

  void nes_cpu::operate_inc(uint16_t address) {
    uint8_t data = memory->read(address);
    ++data;
    memory->write(address, data);
    check_zf_and_sf(data);
  }

  void nes_cpu::operate_dec(uint16_t address) {
    uint8_t data = memory->read(address);
    --data;
    memory->write(address, data);
    check_zf_and_sf(data);
  }

}