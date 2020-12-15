#include <cstdio>
#include <cstdlib>
#include "./nes_rom.h"
#include "./nes_cpu.h"
#include "./nes_nrom_mapper.h"
#include "./nes_memory_pool.h"

#ifndef SIMULATOR_H
#define SIMULATOR_H

namespace fc
{
  static nes_mapper* mappers[] = {
    new nes_nrom_mapper()
  };

  // 模拟器主体
  class simulator
  {
  private:
    // 当前加载的 rom 信息
    nes_rom_info* rom_info;
    // 用来处理 rom 镜像
    nes_rom_handler rom_handler;
    // 用来读写内存
    nes_memory_pool memory_pool;
    // 用来解释和执行指令
    nes_cpu cpu;

  public:
    // Constructor，初始化一些状态
    simulator();
    // 根据路径加载 rom 到 rom_info 中
    void load_rom(const char* path);
    // 释放当前加载的 rom_info
    void free_rom();
    // 获取内存池对象
    nes_memory_pool& get_memory_pool() { return memory_pool; }
    // 获取 cpu 对象
    nes_cpu& get_cpu() { return cpu; }
    // 输出当前寄存器的值
  };
}


#endif