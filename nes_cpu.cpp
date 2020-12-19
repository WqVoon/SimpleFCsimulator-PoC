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
      OP(03, inx, slo)
      OP(04, zpg, nop)
      OP(05, zpg, ora)
      OP(06, zpg, asl)
      OP(07, zpg, slo)
      OP(08, imp, php)
      OP(09, imm, ora)
      OP(0A, acc, asla)
      OP(0C, abs, nop)
      OP(0D, abs, ora)
      OP(0E, abs, asl)
      OP(0F, abs, slo)
      OP(10, rel, bpl)
      OP(11, iny, ora)
      OP(13, iny, slo)
      OP(14, zpx, nop)
      OP(15, zpx, ora)
      OP(16, zpx, asl)
      OP(17, zpx, slo)
      OP(18, imp, clc)
      OP(19, aby, ora)
      OP(1A, imp, nop)
      OP(1B, aby, slo)
      OP(1C, abx, nop)
      OP(1D, abx, ora)
      OP(1E, abx, asl)
      OP(1F, abx, slo)
      OP(20, abs, jsr)
      OP(21, inx, and)
      OP(23, inx, rla)
      OP(24, zpg, bit)
      OP(25, zpg, and)
      OP(26, zpg, rol)
      OP(27, zpg, rla)
      OP(28, imp, plp)
      OP(29, imm, and)
      OP(2A, acc, rola)
      OP(2C, abs, bit)
      OP(2D, abs, and)
      OP(2E, abs, rol)
      OP(2F, abs, rla)
      OP(30, rel, bmi)
      OP(31, iny, and)
      OP(33, iny, rla)
      OP(34, zpx, nop)
      OP(35, zpx, and)
      OP(36, zpx, rol)
      OP(37, zpx, rla)
      OP(38, imp, sec)
      OP(39, aby, and)
      OP(3A, imp, nop)
      OP(3B, aby, rla)
      OP(3C, abx, nop)
      OP(3D, abx, and)
      OP(3E, abx, rol)
      OP(3F, abx, rla)
      OP(40, imp, rti)
      OP(41, inx, eor)
      OP(43, inx, sre)
      OP(44, zpg, nop)
      OP(45, zpg, eor)
      OP(46, zpg, lsr)
      OP(47, zpg, sre)
      OP(48, imp, pha)
      OP(49, imm, eor)
      OP(4A, acc, lsra)
      OP(4C, abs, jmp)
      OP(4D, abs, eor)
      OP(4E, abs, lsr)
      OP(4F, abs, sre)
      OP(50, rel, bvc)
      OP(51, iny, eor)
      OP(53, iny, sre)
      OP(54, zpx, nop)
      OP(55, zpx, eor)
      OP(56, zpx, lsr)
      OP(57, zpx, sre)
      OP(59, aby, eor)
      OP(5A, imp, nop)
      OP(5B, aby, sre)
      OP(5C, abx, nop)
      OP(5D, abx, eor)
      OP(5E, abx, lsr)
      OP(5F, abx, sre)
      OP(60, imp, rts)
      OP(61, inx, adc)
      OP(63, inx, rra)
      OP(64, zpg, nop)
      OP(65, zpg, adc)
      OP(66, zpg, ror)
      OP(67, zpg, rra)
      OP(68, imp, pla)
      OP(69, imm, adc)
      OP(6A, acc, rora)
      OP(6C, ind, jmp)
      OP(6D, abs, adc)
      OP(6E, abs, ror)
      OP(6F, abs, rra)
      OP(70, rel, bvs)
      OP(71, iny, adc)
      OP(73, iny, rra)
      OP(74, zpx, nop)
      OP(75, zpx, adc)
      OP(76, zpx, ror)
      OP(77, zpx, rra)
      OP(78, imp, sei)
      OP(79, aby, adc)
      OP(7A, imp, nop)
      OP(7B, aby, rra)
      OP(7C, abx, nop)
      OP(7D, abx, adc)
      OP(7E, abx, ror)
      OP(7F, abx, rra)
      OP(80, imm, nop)
      OP(81, inx, sta)
      OP(83, inx, sax)
      OP(84, zpg, sty)
      OP(85, zpg, sta)
      OP(86, zpg, stx)
      OP(87, zpg, sax)
      OP(88, imp, dey)
      OP(8A, imp, txa)
      OP(8C, abs, sty)
      OP(8D, abs, sta)
      OP(8E, abs, stx)
      OP(8F, abs, sax)
      OP(90, rel, bcc)
      OP(91, iny, sta)
      OP(94, zpx, sty)
      OP(95, zpx, sta)
      OP(96, zpy, stx)
      OP(97, zpy, sax)
      OP(98, imp, tya)
      OP(99, aby, sta)
      OP(9A, imp, txs)
      OP(9D, abx, sta)
      OP(A0, imm, ldy)
      OP(A1, inx, lda)
      OP(A2, imm, ldx)
      OP(A3, inx, lax)
      OP(A4, zpg, ldy)
      OP(A5, zpg, lda)
      OP(A6, zpg, ldx)
      OP(A7, zpg, lax)
      OP(A8, imp, tay)
      OP(A9, imm, lda)
      OP(AA, imp, tax)
      OP(AC, abs, ldy)
      OP(AD, abs, lda)
      OP(AE, abs, ldx)
      OP(AF, abs, lax)
      OP(B0, rel, bcs)
      OP(B1, iny, lda)
      OP(B3, iny, lax)
      OP(B4, zpx, ldy)
      OP(B5, zpx ,lda)
      OP(B6, zpy, ldx)
      OP(B7, zpy, lax)
      OP(B8, imp, clv)
      OP(B9, aby, lda)
      OP(BA, imp, tsx)
      OP(BC, abx, ldy)
      OP(BD, abx, lda)
      OP(BE, aby, ldx)
      OP(BF, aby, lax)
      OP(C0, imm, cpy)
      OP(C1, inx, cmp)
      OP(C3, inx, dcp)
      OP(C4, zpg, cpy)
      OP(C5, zpg, cmp)
      OP(C6, zpg, dec)
      OP(C7, zpg, dcp)
      OP(C8, imp, iny)
      OP(C9, imm, cmp)
      OP(CA, imp, dex)
      OP(CC, abs, cpy)
      OP(CD, abs, cmp)
      OP(CE, abs, dec)
      OP(CF, abs, dcp)
      OP(D0, rel, bne)
      OP(D1, iny, cmp)
      OP(D3, iny, dcp)
      OP(D4, zpx, nop)
      OP(D5, zpx, cmp)
      OP(D6, zpx, dec)
      OP(D7, zpx, dcp)
      OP(D8, imp, cld)
      OP(D9, aby, cmp)
      OP(DA, imp, nop)
      OP(DB, aby, dcp)
      OP(DC, abx, nop)
      OP(DD, abx, cmp)
      OP(DE, abx, dec)
      OP(DF, abx, dcp)
      OP(E0, imm, cpx)
      OP(E1, inx, sbc)
      OP(E3, inx, isb)
      OP(E4, zpg, cpx)
      OP(E5, zpg, sbc)
      OP(E6, zpg, inc)
      OP(E7, zpg, isb)
      OP(E8, imp, inx)
      OP(E9, imm, sbc)
      OP(EA, imp, nop)
      OP(EB, imm, sbc)
      OP(EC, abs, cpx)
      OP(ED, abs, sbc)
      OP(EE, abs, inc)
      OP(EF, abs, isb)
      OP(F0, rel, beq)
      OP(F1, iny, sbc)
      OP(F3, iny, isb)
      OP(F4, zpx, nop)
      OP(F5, zpx, sbc)
      OP(F6, zpx, inc)
      OP(F7, zpx, isb)
      OP(F8, imp, sed)
      OP(F9, aby, sbc)
      OP(FA, imp, nop)
      OP(FB, aby, isb)
      OP(FC, abx, nop)
      OP(FD, abx, sbc)
      OP(FE, abx, inc)
      OP(FF, abx, isb)
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
    printf("Get data %X from %X\n", registers.accumulator, address);
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

  void nes_cpu::operate_lax(uint16_t address) {
    const uint8_t data = memory->read(address);
    registers.accumulator = data;
    registers.x_index = data;
    check_zf_and_sf(data);
  }

  void nes_cpu::operate_sax(uint16_t address) {
    const uint16_t data
      = registers.accumulator
      & registers.x_index;
    memory->write(address, data);
  }

  void nes_cpu::operate_dcp(uint16_t address) {
    uint8_t data = memory->read(address);
    --data;
    memory->write(address, data);

    // CMP
    const uint16_t result16
      = (uint16_t)registers.accumulator
      - (uint16_t)data;
    if (! (result16 & (uint16_t)0x8000)) {
      registers.status |= SFC_FLAG_C;
    } else {
      registers.status &= ~SFC_FLAG_C;
    }
    check_zf_and_sf((uint8_t)result16);
  }

  void nes_cpu::operate_isb(uint16_t address) {
    uint8_t data = memory->read(address);
    ++data;
    memory->write(address, data);

    const uint16_t result16
      = (uint16_t)registers.accumulator
      - (uint16_t)data
      - (registers.status & SFC_FLAG_C? 0: 1);
    if (! (result16 >> 8)) {
      registers.status |= SFC_FLAG_C;
    } else {
      registers.status &= ~SFC_FLAG_C;
    }
    const uint8_t result8 = (uint8_t)result16;
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

  void nes_cpu::operate_slo(uint16_t address) {
    uint8_t data = memory->read(address);
    if (data & (uint8_t)0x80) {
      registers.status |= SFC_FLAG_C;
    } else {
      registers.status &= ~SFC_FLAG_C;
    }
    data <<= 1;
    memory->write(address, data);

    registers.accumulator |= data;
    check_zf_and_sf(registers.accumulator);
  }

  void nes_cpu::operate_rla(uint16_t address) {
    puts("Wahahahaha");
    uint16_t result16 = memory->read(address);
    result16 <<= 1;
    result16 |= registers.status & SFC_FLAG_C;
    if (result16 & (uint16_t)0x100) {
      registers.status |= SFC_FLAG_C;
    } else {
      registers.status &= ~SFC_FLAG_C;
    }
    const uint8_t result8 = (uint8_t)result16;
    memory->write(address, result8);

    registers.accumulator &= result8;
    check_zf_and_sf(registers.accumulator);
  }

  void nes_cpu::operate_sre(uint16_t address) {
    uint8_t data = memory->read(address);
    if (data & 1) {
      registers.status |= SFC_FLAG_C;
    } else {
      registers.status &= ~SFC_FLAG_C;
    }
    data >>= 1;
    memory->write(address, data);

    registers.accumulator ^= data;
    check_zf_and_sf(registers.accumulator);
  }

  void nes_cpu::operate_rra(uint16_t address) {
    uint16_t result16 = memory->read(address);
    result16 |= (registers.status & SFC_FLAG_C) << 8;
    uint8_t tmp_cp = result16 & 1;
    result16 >>= 1;
    uint8_t result8 = (uint8_t)result16;
    memory->write(address, result8);

    const uint8_t src = result8;
    result16
      = (uint16_t)registers.accumulator
      + (uint16_t)src
      + tmp_cp;
    if (result16 >> 8) {
      registers.status |= SFC_FLAG_C;
    } else {
      registers.status &= ~SFC_FLAG_C;
    }
    result8 = (uint8_t)result16;
    if (
      !((registers.accumulator ^ src) & 0x80)
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

}