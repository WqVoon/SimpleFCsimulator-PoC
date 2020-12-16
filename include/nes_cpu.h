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

    // 根据操作数来判断是否需要为 ZF 和 SF 置位
    void check_zf_and_sf(uint8_t);

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
    // JMP 指令
    void operate_jmp(uint16_t);
    // LDX 指令
    void operate_ldx(uint16_t);

  public:
    // 四种中断向量
    static const uint16_t NMI_VECTOR    = 0xfffa;
    static const uint16_t RESET_VECTOR  = 0xfffc;
    static const uint16_t IRQBRK_VECTOR = 0xfffe;

    void init(nes_memory_pool* mp);
    // 执行当前 PC 指向的指令
    void execute();
    // 按地址反汇编一条指令，当前暴力读取3个字节
    void disassemble_op(uint16_t addr, char buf[]);
    // 输出当前寄存器的值和状态寄存器的标记
    void output_registers_and_flags();
    // 获取当前 PC 寄存器中的值
    uint16_t get_pc() { return registers.program_counter; }
  };
}

#endif