#include <cstdlib>

#ifndef NES_6502_H
#define NES_6502_H

namespace fc
{
  // 暂存指令数据
  typedef union {
    // 用32位保存数据
    uint32_t data;
    // 4个8位数据
    struct {
        // 操作码
        uint8_t op;
        // 地址码1
        uint8_t a1;
        // 地址码2
        uint8_t a2;
        // 显示控制
        uint8_t ctrl;
    };
  } nes_code;

  // 6502 的寻址模式
  enum nes_6502_addressing_mode {
    SFC_AM_UNK = 0,     // 未知寻址
    SFC_AM_ACC,         // 操累加器A: Op Accumulator
    SFC_AM_IMP,         // 隐含 寻址: Implied    Addressing
    SFC_AM_IMM,         // 立即 寻址: Immediate  Addressing
    SFC_AM_ABS,         // 直接 寻址: Absolute   Addressing
    SFC_AM_ABX,         // 直接X变址: Absolute X Addressing
    SFC_AM_ABY,         // 直接Y变址: Absolute Y Addressing
    SFC_AM_ZPG,         // 零页 寻址: Zero-Page  Addressing
    SFC_AM_ZPX,         // 零页X变址: Zero-PageX Addressing
    SFC_AM_ZPY,         // 零页Y变址: Zero-PageY Addressing
    SFC_AM_INX,         // 间接X变址: Pre-indexed Indirect Addressing
    SFC_AM_INY,         // 间接Y变址: Post-indexed Indirect Addressing
    SFC_AM_IND,         // 间接 寻址: Indirect   Addressing
    SFC_AM_REL,         // 相对 寻址: Relative   Addressing
  };


  // 根据 code 参数来把对应的助记符写入到 buf 中，同时返回该指令的长度
  uint8_t disassemble(nes_code code, char buf[]);
}

#endif