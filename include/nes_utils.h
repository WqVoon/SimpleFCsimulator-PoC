#include <cstdlib>

#ifndef NES_UTILS_H
#define NES_UTILS_H

namespace fc
{
  // 将 uint8_t 类型的数据的十六进制表示写入到 buf 中
  inline void btoh(char buf[], uint8_t data) {
    static const char hexcode[] =
      "0123456789ABCDEF";

    buf[0] = hexcode[data >> 4];
    buf[1] = hexcode[data & (uint8_t)0x0F];
  }
}

#endif