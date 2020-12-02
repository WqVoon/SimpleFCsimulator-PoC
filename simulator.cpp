#include "include/simulator.h"

namespace fc
{
  simulator::simulator() {
    banks[0] = main_memory;
    banks[3] = sram_memory;
  }

  void simulator::load_rom(const char* path) {
    rom_handler.load_image(path);
    rom_handler.parse_to_info();
    rom_info = rom_handler.get_info();
    rom_info->show_info();
  }

  void simulator::free_rom() {
    rom_handler.unload_image();
    rom_info = NULL;
  }
}
