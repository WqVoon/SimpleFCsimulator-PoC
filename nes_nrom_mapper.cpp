#include <cstdio>
#include <cassert>
#include "include/nes_mapper.h"
#include "include/nes_nrom_mapper.h"

namespace fc
{
  void nes_nrom_mapper::reset(nes_rom_info* info, uint8_t** banks) {
    assert(info->prg_rom_count && info->prg_rom_count <= 2 && "错误的 PRG-ROM 数量");

    // 用于判断是 16KB 还是 32KB，如果是前者，那么会在 banks 中加载两次 PRG-ROM
    // 即 banks[0] = banks[2], banks[1] = banks[3]
    const int base_idx = info->prg_rom_count & 2;
    load_8K_from_rom_to_banks(0, 0);
    load_8K_from_rom_to_banks(1, 1);
    load_8K_from_rom_to_banks(2, base_idx + 0);
    load_8K_from_rom_to_banks(3, base_idx + 1);
  }
}