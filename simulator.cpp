#include "include/simulator.h"

namespace fc
{
  simulator::simulator() {}

  void simulator::load_rom(const char* path) {
    rom_handler.load_image(path);
    rom_handler.parse_to_info();
    rom_info = rom_handler.get_info();
    // TODO: 暂时传递 NULL，后面会根据 mapper_number 来传递具体的 mapper 实例
    memory_pool.init(rom_info, mappers[rom_info->mapper_number]);
    cpu.init(&memory_pool);

    // rom_info->show_info();
  }

  void simulator::free_rom() {
    rom_handler.unload_image();
    rom_info = NULL;
  }
}
