#include <cstdio>
#include <cstdlib>
#include <cassert>
#include "include/simulator.h"

int main(int argc, char const *argv[])
{
  fc::simulator simulator;
  
  if (argc == 2) {
    simulator.load_rom(argv[1]);
  } else {
    assert(!"请提供参数");
  }

  simulator.free_rom();
  return 0;
}
