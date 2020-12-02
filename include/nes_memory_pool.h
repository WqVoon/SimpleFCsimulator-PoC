#include <cstdio>
#include <cstdlib>
#include "./nes_rom.h"
#include "./nes_mapper.h"

#ifndef NES_MEMORY_POOL_H
#define NES_MEMORY_POOL_H

namespace fc
{
  // 用于处理模拟器的内存的读写动作
  /*
    内存布局：
    Bank0 [$0000, $2000) 系统主内存，从 $0800 开始
    Bank1 [$2000, $4000) PPU 寄存器
    Bank2 [$4000, $6000) pAPU寄存器以及扩展区域
    Bank3 [$6000, $8000) SRAM区
    剩下的全是程序代码区 PRG-ROM
  */
  class nes_memory_pool
  {
  private:
    // 小霸王的 2k 主要内存
    uint8_t main_memory[2 * 1024] = {0};
    // 工作,存档用内存 SRAM
    uint8_t sram_memory[8 * 1024] = {0};
    // 方便 Mapper 的 banks，每 8KB 一个，因此 64 KB 一共有 8 个
    uint8_t* banks[8] = {0};

  public:
    // 绑定 simulator 实例
    void init(nes_rom_info* rom_info, nes_mapper* mapper);
    // 读取内存
    uint8_t read(uint16_t addr);
    // 写入内存
    void write(uint16_t addr, uint8_t data);
  };
}

#endif