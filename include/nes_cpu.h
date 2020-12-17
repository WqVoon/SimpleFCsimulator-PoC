#include <cstdlib>
#include "nes_memory_pool.h"

#ifndef NES_CPU_H
#define NES_CPU_H

#define OP_BUF_LEN 24

namespace fc
{

  // 状态寄存器标记
  enum sfc_status_index {
      SFC_INDEX_C = 0,
      SFC_INDEX_Z = 1,
      SFC_INDEX_I = 2,
      SFC_INDEX_D = 3,
      SFC_INDEX_B = 4,
      SFC_INDEX_R = 5,
      SFC_INDEX_V = 6,
      SFC_INDEX_S = 7,
      SFC_INDEX_N = SFC_INDEX_S,
  };

  // 状态寄存器标记
  enum sfc_status_flag {
      SFC_FLAG_C = 1 << 0,    // 进位标记(Carry flag)
      SFC_FLAG_Z = 1 << 1,    // 零标记 (Zero flag)
      SFC_FLAG_I = 1 << 2,    // 禁止中断(Irq disabled flag)
      SFC_FLAG_D = 1 << 3,    // 十进制模式(Decimal mode flag)
      SFC_FLAG_B = 1 << 4,    // 软件中断(BRK flag)
      SFC_FLAG_R = 1 << 5,    // 保留标记(Reserved), 一直为1
      SFC_FLAG_V = 1 << 6,    // 溢出标记(Overflow  flag)
      SFC_FLAG_S = 1 << 7,    // 符号标记(Sign flag)
      SFC_FLAG_N = SFC_FLAG_S,// 又叫(Negative Flag)
  };

  class nes_cpu
  {
  private:
    nes_memory_pool* memory;

    struct {
      // 指令计数器 PC
      uint16_t program_counter;
      // 状态寄存器
      uint8_t status;
      // 累加寄存器
      uint8_t accumulator;
      // X 变址寄存器
      uint8_t x_index;
      // Y 变址寄存器
      uint8_t y_index;
      // 栈指针
      uint8_t stack_pointer;
      // 对齐用
      uint8_t unused;
    } registers;

    // 根据操作数来判断如何为 ZF 和 SF 置位
    void check_zf_and_sf(uint8_t);
    // 将一个 8 位数据压栈
    void stack_push(uint8_t);
    // 将栈顶元素出栈
    uint8_t stack_pop();

    // 未知寻址模式
    uint16_t address_unk();
    // TODO: 累加器寻址(是否需要实现？)
    uint16_t address_acc();
    // TODO: 隐含寻址模式(是否需要实现？)
    uint16_t address_imp();
    // 立即数寻址模式
    uint16_t address_imm();
    // 绝对寻址
    uint16_t address_abs();
    // 绝对X变址
    uint16_t address_abx();
    // 绝对Y变址
    uint16_t address_aby();
    // 零页寻址
    uint16_t address_zpg();
    // 零页X变址
    uint16_t address_zpx();
    // 零页Y变址
    uint16_t address_zpy();
    // 间接X变址
    uint16_t address_inx();
    // 间接Y变址
    uint16_t address_iny();
    // 间接寻址
    uint16_t address_ind();
    // 相对寻址
    uint16_t address_rel();


    // JMP 指令（修改 pc 为地址）
    void operate_jmp(uint16_t);
    // LDX 指令（加载地址中的值到 x 寄存器中，影响Z/SF）
    void operate_ldx(uint16_t);
    // STX 指令（将 x 寄存器中的值保存到地址中）
    void operate_stx(uint16_t);
    // JSR 指令（将当前 pc 压栈并跳转到指定地址）
    void operate_jsr(uint16_t);
    // NOP 指令
    void operate_nop(uint16_t);
    // SEC 指令（设置 CF）
    void operate_sec(uint16_t);
    // BCS 指令（如果 CF 为 1 那么跳转到地址）
    void operate_bcs(uint16_t);
    // CLC 指令（清空 CF）
    void operate_clc(uint16_t);
    // BCC 指令（如果 CF 为 0 那么跳转到地址）
    void operate_bcc(uint16_t);
    // LDA 指令（加载地址中的值到 A 寄存器中，影响Z/SF）
    void operate_lda(uint16_t);
    // BEQ 指令（如果 ZF 为 1 那么跳转到地址）
    void operate_beq(uint16_t);
    // BNE 指令（如果 ZF 为 0 那么跳转到地址）
    void operate_bne(uint16_t);
    // STA 指令（将 A 寄存器中的值保存到地址中）
    void operate_sta(uint16_t);
    // BIT 指令（根据 A 寄存器和内存中的值影响 V/S/ZF）
    void operate_bit(uint16_t);
    // BVS 指令（如果 VF 为 1 那么跳转到地址）
    void operate_bvs(uint16_t);
    // BVC 指令（如果 VF 为 0 那么跳转到地址）
    void operate_bvc(uint16_t);
    // BPL 指令（如果 SF 为 0 那么跳转到地址）
    void operate_bpl(uint16_t);
    // RTS 指令（弹出栈中地址给 pc ，并自增 pc）
    void operate_rts(uint16_t);
    // SEI 指令（设置 IF）
    void operate_sei(uint16_t);
    // SED 指令（设置 DF）
    void operate_sed(uint16_t);
    // PHP 指令（将状态寄存器压栈）
    void operate_php(uint16_t);
    // PLA 指令（把栈顶元素送入 A 寄存器，影响 Z/SF）
    void operate_pla(uint16_t);
    // AND 指令（把地址中的值与 A 寄存器做与运算，影响 Z/SF）
    void operate_and(uint16_t);
    // CMP 指令（比较地址中的值与 A 寄存器，影响 C/S/ZF）
    void operate_cmp(uint16_t);
    // CLD 指令（清空 DF）
    void operate_cld(uint16_t);
    // PHA 指令（将 A 寄存器压栈）
    void operate_pha(uint16_t);
    // PLP 指令（弹出状态寄存器，并置 BF 为 0）
    void operate_plp(uint16_t);
    // BMI 指令（如果 SF 为 1 那么跳转到地址）
    void operate_bmi(uint16_t);
    // ORA 指令（把地址中的值与 A 寄存器做或运算，影响 Z/SF）
    void operate_ora(uint16_t);
    // CLV 指令（清空 VF）
    void operate_clv(uint16_t);
    // EOR 指令（将地址中的值与 A 寄存器做异或运算，影响 Z/SF）
    void operate_eor(uint16_t);
    // ADC 指令（将地址中的值与 A 寄存器做带进位加法，影响 C/V/Z/SF）
    void operate_adc(uint16_t);
    // LDY 指令（加载地址中的值到 Y 寄存器中，影响 Z/SF）
    void operate_ldy(uint16_t);
    // CPY 指令（比较地址中的值与 Y 寄存器，影响 C/S/ZF）
    void operate_cpy(uint16_t);
    // CPX 指令（比较地址中的值与 X 寄存器，影响 C/S/ZF）
    void operate_cpx(uint16_t);
    // SBC 指令（用 A 寄存器与地址中的值做带进位减法，影响 C/V/Z/SF）
    void operate_sbc(uint16_t);
    // INY 指令（自增 Y 寄存器，影响 Z/SF）
    void operate_iny(uint16_t);
    // INX 指令（自增 X 寄存器，影响 Z/SF）
    void operate_inx(uint16_t);
    // DEY 指令（自减 Y 寄存器，影响 Z/SF）
    void operate_dey(uint16_t);
    // DEX 指令（自减 X 寄存器，影响 Z/SF）
    void operate_dex(uint16_t);
    // TAY 指令（将 A 寄存器中的值送入 Y 寄存器，影响 Z/SF）
    void operate_tay(uint16_t);
    // TAX 指令（将 A 寄存器中的值送入 X 寄存器，影响 Z/SF）
    void operate_tax(uint16_t);
    // TYA 指令（将 Y 寄存器中的值送入 A 寄存器，影响 Z/SF）
    void operate_tya(uint16_t);
    // TXA 指令（将 X 寄存器中的值送入 A 寄存器，影响 Z/SF）
    void operate_txa(uint16_t);
    // TSX 指令（将 SP 寄存器中的值送入 X 寄存器，影响 Z/SF）
    void operate_tsx(uint16_t);
    // TXS 指令（将 X 寄存器中的值送入 SP 寄存器）
    void operate_txs(uint16_t);
    // RTI 指令（从中断返回，影响 P/R/BF）
    void operate_rti(uint16_t);
    // LSRA 指令（对 A 寄存器进行逻辑右移，影响 S/Z/CF）
    void operate_lsra(uint16_t);
    // TODO：(将它移动到该出现的地方) LSR 指令（对地址中的值进行逻辑右移，影响 S/Z/CF）
    void operate_lsr(uint16_t);
    // ASLA 指令（对 A 寄存器进行算数左移，影响 S/Z/CF）
    void operate_asla(uint16_t);
    // RORA 指令（对 A 寄存器进行循环右移，影响 S/Z/CF）
    void operate_rora(uint16_t);
    // ROLA 指令（对 A 寄存器进行循环左移，影响 S/Z/CF）
    void operate_rola(uint16_t);
    // STY 指令（将 Y 寄存器中的值保存到地址中）
    void operate_sty(uint16_t);
    // ASL 指令（对地址中的值进行算数左移，影响 S/F/CF）
    void operate_asl(uint16_t);
    // ROR 指令（对地址中的值进行循环右移，影响 S/Z/CF）
    void operate_ror(uint16_t);
    // ROL 指令（对地址中的值进行循环左移，影响 S/Z/CF）
    void operate_rol(uint16_t);
    // INC 指令（将地址中的值自增，影响 S/ZF）
    void operate_inc(uint16_t);
    // DEC 指令（将地址中的值自减，影响 S/ZF）
    void operate_dec(uint16_t);


  public:
    // 四种中断向量
    static const uint16_t NMI_VECTOR    = 0xfffa;
    static const uint16_t RESET_VECTOR  = 0xfffc;
    static const uint16_t IRQBRK_VECTOR = 0xfffe;

    void init(nes_memory_pool* mp);
    // 执行当前 PC 指向的指令
    void execute();
    // 按地址反汇编一条指令，内部调用 output_registers_and_flags 并输出读取的字节
    void disassemble_op(uint16_t addr, char buf[]);
    // 输出当前寄存器的值和状态寄存器的标记
    void output_registers_and_flags();
    // 获取当前 PC 寄存器中的值
    uint16_t get_pc() { return registers.program_counter; }
  };
}

#endif