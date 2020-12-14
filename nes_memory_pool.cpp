#include <cassert>
#include "include/nes_memory_pool.h"
#include "include/nes_mapper.h"

namespace fc
{
  void nes_memory_pool::init(nes_rom_info* rom_info, nes_mapper* mapper) {
    // puts("Banks (before mapper reset):");
    // for (int i=0; i<8; i++) {
    //   printf(" idx(%d): %p\n", i, banks[i]);
    // }

    banks[0] = main_memory;
    banks[3] = sram_memory;

    if (mapper) mapper->reset(rom_info, banks);

    // puts("Banks (after mapper reset):");
    // for (int i=0; i<8; i++) {
    //   printf(" idx(%d): %p\n", i, banks[i]);
    // }
  }

  uint8_t nes_memory_pool::read(uint16_t addr) {
    switch (addr >> 13) {
    case 0:
      // TODO: 这里是否需要地址的映射？
      return main_memory[addr & (uint16_t)0x07ff];
    case 1:
      assert(!"未实现");
    case 2:
      assert(!"未实现");
    case 3:
      return sram_memory[addr & (uint16_t)0x1fff];
    case 4: case 5: case 6: case 7:
      return banks[addr >> 13][addr & (uint16_t)0x1fff];
    }
    assert(!"无效的地址");
  }

  void nes_memory_pool::write(uint16_t addr, uint8_t data) {
  }
}