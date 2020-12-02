#include <cstdio>
#include <cstdlib>
#include "./nes_rom.h"

#ifndef SIMULATOR_H
#define SIMULATOR_H

namespace fc
{
  // 模拟器主体
  /*
    内存布局：
    Bank0 [$0000, $2000) 系统主内存，从 $0800 开始
    Bank1 [$2000, $4000) PPU 寄存器
    Bank2 [$4000, $6000) pAPU寄存器以及扩展区域
    Bank3 [$6000, $8000) SRAM区
    剩下的全是程序代码区 PRG-ROM
  */
  class simulator
  {
  private:
    // 当前加载的 rom 信息
    nes_rom_info* rom_info;
    // 小霸王的 2k 主要内存
    uint8_t main_memory[2 * 1024];
    // 工作,存档用内存 SRAM
    uint8_t sram_memory[8 * 1024];
    // 方便 Mapper 的 banks，每 8KB 一个，因此 64 KB 一共有 8 个
    uint8_t* banks[8];

    nes_rom_handler rom_handler;
  public:
    // Constructor，初始化一些状态
    simulator();
    // 根据路径加载 rom 到 rom_info 中
    void load_rom(const char* path);
    // 释放当前加载的 rom_info
    void free_rom();
  };
}


#endif