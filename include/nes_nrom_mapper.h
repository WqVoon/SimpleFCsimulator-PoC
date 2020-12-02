#include "./nes_mapper.h"

#ifndef NES_NROM_MAPPER
#define NES_NROM_MAPPER

namespace fc
{
  // Mapper000，支持 16KB 的 NROM-128 和 32KB 的 NROM-256
  class nes_nrom_mapper: public nes_mapper
  {
  public:
    void reset(nes_rom_info* info, uint8_t** banks);
  };
}

#endif