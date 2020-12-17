#include <cstdio>
#include "include/nes_6502.h"
#include "include/nes_utils.h"

namespace fc
{
  // 用来描述一条指令
  struct nes_opname {
    // 3字名称
    char        name[3];
    // 寻址模式
    uint8_t     mode;
  };

  // 6502 的所有指令，opcode 为指令的下标
  static const struct nes_opname nes_opname_data[256] = {
    { 'B', 'R', 'K', SFC_AM_IMP },
    { 'O', 'R', 'A', SFC_AM_INX },
    { 'S', 'T', 'P', SFC_AM_UNK },
    { 'S', 'L', 'O', SFC_AM_INX },
    { 'N', 'O', 'P', SFC_AM_ZPG },
    { 'O', 'R', 'A', SFC_AM_ZPG },
    { 'A', 'S', 'L', SFC_AM_ZPG },
    { 'S', 'L', 'O', SFC_AM_ZPG },
    { 'P', 'H', 'P', SFC_AM_IMP },
    { 'O', 'R', 'A', SFC_AM_IMM },
    { 'A', 'S', 'L', SFC_AM_ACC },
    { 'A', 'N', 'C', SFC_AM_IMM },
    { 'N', 'O', 'P', SFC_AM_ABS },
    { 'O', 'R', 'A', SFC_AM_ABS },
    { 'A', 'S', 'L', SFC_AM_ABS },
    { 'S', 'L', 'O', SFC_AM_ABS },

    { 'B', 'P', 'L', SFC_AM_REL },
    { 'O', 'R', 'A', SFC_AM_INY },
    { 'S', 'T', 'P', SFC_AM_UNK },
    { 'S', 'L', 'O', SFC_AM_INY },
    { 'N', 'O', 'P', SFC_AM_ZPX },
    { 'O', 'R', 'A', SFC_AM_ZPX },
    { 'A', 'S', 'L', SFC_AM_ZPX },
    { 'S', 'L', 'O', SFC_AM_ZPX },
    { 'C', 'L', 'C', SFC_AM_IMP },
    { 'O', 'R', 'A', SFC_AM_ABY },
    { 'N', 'O', 'P', SFC_AM_IMP },
    { 'S', 'L', 'O', SFC_AM_ABY },
    { 'N', 'O', 'P', SFC_AM_ABX },
    { 'O', 'R', 'A', SFC_AM_ABX },
    { 'A', 'S', 'L', SFC_AM_ABX },
    { 'S', 'L', 'O', SFC_AM_ABX },

    { 'J', 'S', 'R', SFC_AM_ABS },
    { 'A', 'N', 'D', SFC_AM_INX },
    { 'S', 'T', 'P', SFC_AM_UNK },
    { 'R', 'L', 'A', SFC_AM_INX },
    { 'B', 'I', 'T', SFC_AM_ZPG },
    { 'A', 'N', 'D', SFC_AM_ZPG },
    { 'R', 'O', 'L', SFC_AM_ZPG },
    { 'R', 'L', 'A', SFC_AM_ZPG },
    { 'P', 'L', 'P', SFC_AM_IMP },
    { 'A', 'N', 'D', SFC_AM_IMM },
    { 'R', 'O', 'L', SFC_AM_ACC },
    { 'A', 'N', 'C', SFC_AM_IMM },
    { 'B', 'I', 'T', SFC_AM_ABS },
    { 'A', 'N', 'D', SFC_AM_ABS },
    { 'R', 'O', 'L', SFC_AM_ABS },
    { 'R', 'L', 'A', SFC_AM_ABS },

    { 'B', 'M', 'I', SFC_AM_REL },
    { 'A', 'N', 'D', SFC_AM_INY },
    { 'S', 'T', 'P', SFC_AM_UNK },
    { 'R', 'L', 'A', SFC_AM_INY },
    { 'N', 'O', 'P', SFC_AM_ZPX },
    { 'A', 'N', 'D', SFC_AM_ZPX },
    { 'R', 'O', 'L', SFC_AM_ZPX },
    { 'R', 'L', 'A', SFC_AM_ZPX },
    { 'S', 'E', 'C', SFC_AM_IMP },
    { 'A', 'N', 'D', SFC_AM_ABY },
    { 'N', 'O', 'P', SFC_AM_IMP },
    { 'R', 'L', 'A', SFC_AM_ABY },
    { 'N', 'O', 'P', SFC_AM_ABX },
    { 'A', 'N', 'D', SFC_AM_ABX },
    { 'R', 'O', 'L', SFC_AM_ABX },
    { 'R', 'L', 'A', SFC_AM_ABX },

    { 'R', 'T', 'I', SFC_AM_IMP },
    { 'E', 'O', 'R', SFC_AM_INX },
    { 'S', 'T', 'P', SFC_AM_UNK },
    { 'S', 'R', 'E', SFC_AM_INX },
    { 'N', 'O', 'P', SFC_AM_ZPG },
    { 'E', 'O', 'R', SFC_AM_ZPG },
    { 'L', 'S', 'R', SFC_AM_ZPG },
    { 'S', 'R', 'E', SFC_AM_ZPG },
    { 'P', 'H', 'A', SFC_AM_IMP },
    { 'E', 'O', 'R', SFC_AM_IMM },
    { 'L', 'S', 'R', SFC_AM_ACC },
    { 'A', 'S', 'R', SFC_AM_IMM },
    { 'J', 'M', 'P', SFC_AM_ABS },
    { 'E', 'O', 'R', SFC_AM_ABS },
    { 'L', 'S', 'R', SFC_AM_ABS },
    { 'S', 'R', 'E', SFC_AM_ABS },

    { 'B', 'V', 'C', SFC_AM_REL },
    { 'E', 'O', 'R', SFC_AM_INY },
    { 'S', 'T', 'P', SFC_AM_UNK },
    { 'S', 'R', 'E', SFC_AM_INY },
    { 'N', 'O', 'P', SFC_AM_ZPX },
    { 'E', 'O', 'R', SFC_AM_ZPX },
    { 'L', 'S', 'R', SFC_AM_ZPX },
    { 'S', 'R', 'E', SFC_AM_ZPX },
    { 'C', 'L', 'I', SFC_AM_IMP },
    { 'E', 'O', 'R', SFC_AM_ABY },
    { 'N', 'O', 'P', SFC_AM_IMP },
    { 'S', 'R', 'E', SFC_AM_ABY },
    { 'N', 'O', 'P', SFC_AM_ABX },
    { 'E', 'O', 'R', SFC_AM_ABX },
    { 'L', 'S', 'R', SFC_AM_ABX },
    { 'S', 'R', 'E', SFC_AM_ABX },

    { 'R', 'T', 'S', SFC_AM_IMP },
    { 'A', 'D', 'C', SFC_AM_INX },
    { 'S', 'T', 'P', SFC_AM_UNK },
    { 'R', 'R', 'A', SFC_AM_INX },
    { 'N', 'O', 'P', SFC_AM_ZPG },
    { 'A', 'D', 'C', SFC_AM_ZPG },
    { 'R', 'O', 'R', SFC_AM_ZPG },
    { 'R', 'R', 'A', SFC_AM_ZPG },
    { 'P', 'L', 'A', SFC_AM_IMP },
    { 'A', 'D', 'C', SFC_AM_IMM },
    { 'R', 'O', 'R', SFC_AM_ACC },
    { 'A', 'R', 'R', SFC_AM_IMM },
    { 'J', 'M', 'P', SFC_AM_IND },
    { 'A', 'D', 'C', SFC_AM_ABS },
    { 'R', 'O', 'R', SFC_AM_ABS },
    { 'R', 'R', 'A', SFC_AM_ABS },

    { 'B', 'V', 'S', SFC_AM_REL },
    { 'A', 'D', 'C', SFC_AM_INY },
    { 'S', 'T', 'P', SFC_AM_UNK },
    { 'R', 'R', 'A', SFC_AM_INY },
    { 'N', 'O', 'P', SFC_AM_ZPX },
    { 'A', 'D', 'C', SFC_AM_ZPX },
    { 'R', 'O', 'R', SFC_AM_ZPX },
    { 'R', 'R', 'A', SFC_AM_ZPX },
    { 'S', 'E', 'I', SFC_AM_IMP },
    { 'A', 'D', 'C', SFC_AM_ABY },
    { 'N', 'O', 'P', SFC_AM_IMP },
    { 'R', 'R', 'A', SFC_AM_ABY },
    { 'N', 'O', 'P', SFC_AM_ABX },
    { 'A', 'D', 'C', SFC_AM_ABX },
    { 'R', 'O', 'R', SFC_AM_ABX },
    { 'R', 'R', 'A', SFC_AM_ABX },

    { 'N', 'O', 'P', SFC_AM_IMM },
    { 'S', 'T', 'A', SFC_AM_INX },
    { 'N', 'O', 'P', SFC_AM_IMM },
    { 'S', 'A', 'X', SFC_AM_INX },
    { 'S', 'T', 'Y', SFC_AM_ZPG },
    { 'S', 'T', 'A', SFC_AM_ZPG },
    { 'S', 'T', 'X', SFC_AM_ZPG },
    { 'S', 'A', 'X', SFC_AM_ZPG },
    { 'D', 'E', 'Y', SFC_AM_IMP },
    { 'N', 'O', 'P', SFC_AM_IMM },
    { 'T', 'X', 'A', SFC_AM_IMP },
    { 'X', 'X', 'A', SFC_AM_IMM },
    { 'S', 'T', 'Y', SFC_AM_ABS },
    { 'S', 'T', 'A', SFC_AM_ABS },
    { 'S', 'T', 'X', SFC_AM_ABS },
    { 'S', 'A', 'X', SFC_AM_ABS },

    { 'B', 'C', 'C', SFC_AM_REL },
    { 'S', 'T', 'A', SFC_AM_INY },
    { 'S', 'T', 'P', SFC_AM_UNK },
    { 'A', 'H', 'X', SFC_AM_INY },
    { 'S', 'T', 'Y', SFC_AM_ZPX },
    { 'S', 'T', 'A', SFC_AM_ZPX },
    { 'S', 'T', 'X', SFC_AM_ZPY },
    { 'S', 'A', 'X', SFC_AM_ZPY },
    { 'T', 'Y', 'A', SFC_AM_IMP },
    { 'S', 'T', 'A', SFC_AM_ABY },
    { 'T', 'X', 'S', SFC_AM_IMP },
    { 'T', 'A', 'S', SFC_AM_ABY },
    { 'S', 'H', 'Y', SFC_AM_ABX },
    { 'S', 'T', 'A', SFC_AM_ABX },
    { 'S', 'H', 'X', SFC_AM_ABY },
    { 'A', 'H', 'X', SFC_AM_ABY },

    { 'L', 'D', 'Y', SFC_AM_IMM },
    { 'L', 'D', 'A', SFC_AM_INX },
    { 'L', 'D', 'X', SFC_AM_IMM },
    { 'L', 'A', 'X', SFC_AM_INX },
    { 'L', 'D', 'Y', SFC_AM_ZPG },
    { 'L', 'D', 'A', SFC_AM_ZPG },
    { 'L', 'D', 'X', SFC_AM_ZPG },
    { 'L', 'A', 'X', SFC_AM_ZPG },
    { 'T', 'A', 'Y', SFC_AM_IMP },
    { 'L', 'D', 'A', SFC_AM_IMM },
    { 'T', 'A', 'X', SFC_AM_IMP },
    { 'L', 'A', 'X', SFC_AM_IMM },
    { 'L', 'D', 'Y', SFC_AM_ABS },
    { 'L', 'D', 'A', SFC_AM_ABS },
    { 'L', 'D', 'X', SFC_AM_ABS },
    { 'L', 'A', 'X', SFC_AM_ABS },

    { 'B', 'C', 'S', SFC_AM_REL },
    { 'L', 'D', 'A', SFC_AM_INY },
    { 'S', 'T', 'P', SFC_AM_UNK },
    { 'L', 'A', 'X', SFC_AM_INY },
    { 'L', 'D', 'Y', SFC_AM_ZPX },
    { 'L', 'D', 'A', SFC_AM_ZPX },
    { 'L', 'D', 'X', SFC_AM_ZPY },
    { 'L', 'A', 'X', SFC_AM_ZPY },
    { 'C', 'L', 'V', SFC_AM_IMP },
    { 'L', 'D', 'A', SFC_AM_ABY },
    { 'T', 'S', 'X', SFC_AM_IMP },
    { 'L', 'A', 'S', SFC_AM_ABY },
    { 'L', 'D', 'Y', SFC_AM_ABX },
    { 'L', 'D', 'A', SFC_AM_ABX },
    { 'L', 'D', 'X', SFC_AM_ABY },
    { 'L', 'A', 'X', SFC_AM_ABY },

    { 'C', 'P', 'Y', SFC_AM_IMM },
    { 'C', 'M', 'P', SFC_AM_INX },
    { 'N', 'O', 'P', SFC_AM_IMM },
    { 'D', 'C', 'P', SFC_AM_INX },
    { 'C', 'P', 'Y', SFC_AM_ZPG },
    { 'C', 'M', 'P', SFC_AM_ZPG },
    { 'D', 'E', 'C', SFC_AM_ZPG },
    { 'D', 'C', 'P', SFC_AM_ZPG },
    { 'I', 'N', 'Y', SFC_AM_IMP },
    { 'C', 'M', 'P', SFC_AM_IMM },
    { 'D', 'E', 'X', SFC_AM_IMP },
    { 'A', 'X', 'S', SFC_AM_IMM },
    { 'C', 'P', 'Y', SFC_AM_ABS },
    { 'C', 'M', 'P', SFC_AM_ABS },
    { 'D', 'E', 'C', SFC_AM_ABS },
    { 'D', 'C', 'P', SFC_AM_ABS },

    { 'B', 'N', 'E', SFC_AM_REL },
    { 'C', 'M', 'P', SFC_AM_INY },
    { 'S', 'T', 'P', SFC_AM_UNK },
    { 'D', 'C', 'P', SFC_AM_INY },
    { 'N', 'O', 'P', SFC_AM_ZPX },
    { 'C', 'M', 'P', SFC_AM_ZPX },
    { 'D', 'E', 'C', SFC_AM_ZPX },
    { 'D', 'C', 'P', SFC_AM_ZPX },
    { 'C', 'L', 'D', SFC_AM_IMP },
    { 'C', 'M', 'P', SFC_AM_ABY },
    { 'N', 'O', 'P', SFC_AM_IMP },
    { 'D', 'C', 'P', SFC_AM_ABY },
    { 'N', 'O', 'P', SFC_AM_ABX },
    { 'C', 'M', 'P', SFC_AM_ABX },
    { 'D', 'E', 'C', SFC_AM_ABX },
    { 'D', 'C', 'P', SFC_AM_ABX },

    { 'C', 'P', 'X', SFC_AM_IMM },
    { 'S', 'B', 'C', SFC_AM_INX },
    { 'N', 'O', 'P', SFC_AM_IMM },
    { 'I', 'S', 'B', SFC_AM_INX },
    { 'C', 'P', 'X', SFC_AM_ZPG },
    { 'S', 'B', 'C', SFC_AM_ZPG },
    { 'I', 'N', 'C', SFC_AM_ZPG },
    { 'I', 'S', 'B', SFC_AM_ZPG },
    { 'I', 'N', 'X', SFC_AM_IMP },
    { 'S', 'B', 'C', SFC_AM_IMM },
    { 'N', 'O', 'P', SFC_AM_IMP },
    { 'S', 'B', 'C', SFC_AM_IMM },
    { 'C', 'P', 'X', SFC_AM_ABS },
    { 'S', 'B', 'C', SFC_AM_ABS },
    { 'I', 'N', 'C', SFC_AM_ABS },
    { 'I', 'S', 'B', SFC_AM_ABS },

    { 'B', 'E', 'Q', SFC_AM_REL },
    { 'S', 'B', 'C', SFC_AM_INY },
    { 'S', 'T', 'P', SFC_AM_UNK },
    { 'I', 'S', 'B', SFC_AM_INY },
    { 'N', 'O', 'P', SFC_AM_ZPX },
    { 'S', 'B', 'C', SFC_AM_ZPX },
    { 'I', 'N', 'C', SFC_AM_ZPX },
    { 'I', 'S', 'B', SFC_AM_ZPX },
    { 'S', 'E', 'D', SFC_AM_IMP },
    { 'S', 'B', 'C', SFC_AM_ABY },
    { 'N', 'O', 'P', SFC_AM_IMP },
    { 'I', 'S', 'B', SFC_AM_ABY },
    { 'N', 'O', 'P', SFC_AM_ABX },
    { 'S', 'B', 'C', SFC_AM_ABX },
    { 'I', 'N', 'C', SFC_AM_ABX },
    { 'I', 'S', 'B', SFC_AM_ABX },
  };

  uint8_t disassemble(nes_code code, char buf[]) {
    uint8_t length = 0;
    const nes_opname opname = nes_opname_data[code.op];
    buf[0] = opname.name[0];
    buf[1] = opname.name[1];
    buf[2] = opname.name[2];

    switch (opname.mode) {
    case SFC_AM_UNK:
    case SFC_AM_IMP:
      length = 1;
      break;

    case SFC_AM_ACC:
      // INS A
      length = 1;
      buf[4] = 'A';
      break;

    case SFC_AM_IMM:
      // INS #$AB
      length = 2;
      buf[4] = '#';
      buf[5] = '$';
      btoh(buf+6, code.a1);
      break;

    case SFC_AM_ABS:
      // INS #$ABCD
    case SFC_AM_ABX:
      // INS #$ABCD, X
    case SFC_AM_ABY:
      // INS #$ABCD, Y
      length = 3;
      buf[4] = '#';
      buf[5] = '$';
      btoh(buf+6, code.a2);
      btoh(buf+8, code.a1);
      if (opname.mode == SFC_AM_ABS) break;
      buf[10] = ',';
      buf[12] = (opname.mode == SFC_AM_ABX? 'X': 'Y');
      break;

    case SFC_AM_ZPG:
      // INS $AB
    case SFC_AM_ZPX:
      // INS $AB, X
    case SFC_AM_ZPY:
      // INS $AB, Y
      length = 2;
      buf[4] = '$';
      btoh(buf+5, code.a1);
      if (opname.mode == SFC_AM_ZPG) break;
      buf[7] = ',';
      buf[9] = (opname.mode == SFC_AM_ZPX? 'X': 'Y');
      break;

    case SFC_AM_INX:
      // INS ($AB, X)
      length = 2;
      buf[4] = '(';
      buf[5] = '$';
      btoh(buf+6, code.a1);
      buf[8] = ',';
      buf[10] = 'X';
      buf[11] = ')';
      break;

    case SFC_AM_INY:
      // INS ($AB), Y
      length = 2;
      buf[4] = '(';
      buf[5] = '$';
      btoh(buf+6, code.a1);
      buf[8] = ')';
      buf[9] = ',';
      buf[11] = 'Y';
      break;

    case SFC_AM_IND:
      // INS ($ABCD)
      length = 3;
      buf[4] = '(';
      buf[5] = '$';
      btoh(buf+6, code.a2);
      btoh(buf+8, code.a1);
      buf[10] = ')';
      break;

    case SFC_AM_REL:
      // 以十六进制表示，实际区分正负
      // INS $AB
      length = 2;
      buf[4] = '$';
      btoh(buf+5, code.a1);
      buf[7] = '(';
      buf[8] = '+';
      buf[9] = 'P';
      buf[10] = 'C';
      buf[11] = ')';
      break;
    }
    return length;
  }
}
