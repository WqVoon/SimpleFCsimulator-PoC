#include "./nes_rom.h"

#ifndef NES_MAPPER_H
#define NES_MAPPER_H

namespace fc
{
  // 从 rom 中加载 8k 的 PRG-ROM/CHR-ROM 到 banks 中
  #define load_8K_from_rom_to_banks(dst, src)\
    banks[4 + (dst)] = info->prg_rom_ptr + 8 * 1024 * (src)

  // 抽象 mapper
  class nes_mapper
  {
  public:
    virtual void reset(nes_rom_info* info, uint8_t** banks) = 0;
  };
}

#endif