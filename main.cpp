#include <cstdio>
#include <cstdlib>
#include <cassert>
#include "include/nes_rom.h"

int main(int argc, char const *argv[])
{
  fc::nes_rom_handler rom_handler;
  
  if (argc == 2) {
    rom_handler.load_image(argv[1]);
    rom_handler.parse_to_info();
    rom_handler.get_info().show_info();
  } else {
    assert(!"请提供参数");
  }

  rom_handler.unload_image();
  return 0;
}
