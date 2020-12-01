#ifndef NES_READER_H
#define NES_READER_H

#include <cstdio>
#include <cstdlib>

namespace fc
{
  // 暂时记录 NES 镜像的文件头信息，仅支持 NES2.0 的 ROM 镜像
  struct nes_header_info_buffer
  {
    // 魔数 "NES<EOF>"
    uint32_t id;
    // 以 0x4000 字节为单位的 PRG-ROM 大小数量
    uint8_t prg_rom_count;
    // 以 0x2000 字节为单位的 CHR-ROM 大小数量
    uint8_t chr_rom_count;
    // flags6: NNNN FTBM
    //  - 高4位为Mapper编号的低四位
    //  - F 为4屏幕标志位，如果被置1则忽略M位
    //  - T 为 Trainer 标志位，如果被置1则从 $7000-$71FF 处加载之
    //  - B 如果被置1则表示 $6000-$7FFF 拥有电池供电的 SRAM
    //  - M 如果被置1则表示垂直镜像，0则表示水平镜像
    uint8_t flags6;
    // flags7: NNNN xxPV
    //  - 高4位为Mapper编号的高四位
    //  - P 为 Playchoice 10 标志位，被设置则表示为 PC-10 游戏
    //  - V 为 Vs. Unisystem 标志位，被设置则表示为 Vs. 游戏
    //  - xx 如果为 10b 且魔数格式正确则表示镜像为 NES2.0
    uint8_t flags7;
    // 文件头中的 8-15 字节暂时保留用
    uint64_t reserved;
  };
  
  // nes_reader 会根据 nes_header_info_buffer 的内容生成程序可用的 nes_rom_info
  struct nes_rom_info
  {
    // 指向 prg_rom 内存的开始处
    uint8_t* prg_rom_ptr;
    // 指向 chr_rom 内存的开始处
    uint8_t* chr_rom_ptr;

    // 以 0x4000 字节为单位的 PRG-ROM 大小数量
    uint8_t prg_rom_count;
    // 以 0x2000 字节为单位的 CHR-ROM 大小数量
    uint8_t chr_rom_count;
    // mapper 编号
    uint8_t mapper_number;
    // 是否4屏幕
    bool is_four_sreen;
    // 是否拥有 Trainer
    bool have_trainer;
    // 是否拥有供电的 SRAM
    bool have_sram;
    // 是否为垂直镜像
    bool is_vertical;

    // 展示当前的 ROM 信息
    void show_info();
  };

    // 具备读取 NES 镜像文件与解析文件内容的功能
  class nes_rom_handler
  {
  private:
    FILE* fp;
    nes_header_info_buffer buffer;
    nes_rom_info info;

  public:
    // 加载一个文件内容到 buffer 中
    void load_image(const char* path);
    // 解析 buffer 中的内容为 nes_header_info，同时申请内存空间
    void parse_to_info();
    // 返回内部的 nes_header_info
    nes_rom_info& get_info();
    // 卸载当前的镜像
    void unload_image();
  };
  
}


#endif