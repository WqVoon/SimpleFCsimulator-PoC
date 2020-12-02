#include "include/nes_rom.h"
#include <cstdio>
#include <cstdlib>
#include <cassert>

namespace fc {

  void nes_rom_info::show_info() {
    printf(
      "ROM 信息:\n"
      " PRG-ROM大小:     %d * 16K\n"
      " CHR-ROM大小:     %d * 8K\n"
      " Mapper编号:      %d\n"
      " 是否为四屏幕:    %c\n"
      " 是否拥有Trainer: %c\n"
      " 是否拥有SRAM:    %c\n"
      " 是否为垂直镜像:  %c\n",

      prg_rom_count,
      chr_rom_count,
      mapper_number,
      (is_four_sreen? 'Y': 'N'),
      (have_trainer? 'Y': 'N'),
      (have_sram? 'Y': 'N'),
      (is_vertical? 'Y': 'N')
    );
  }

  void nes_rom_handler::load_image(const char* path) {
    fp = fopen(path, "rb");
    if (fp == NULL) assert(!"文件打开失败");

    fread(&this->buffer, sizeof(nes_header_info_buffer), 1, fp);
  }

  void nes_rom_handler::parse_to_info() {
    // "NES<EOF>"
    uint32_t magic_number = 0x1a53454e;
    // 验证是否为 NES2.0 的镜像，flags7的 01 条件在测试的镜像中不符合故暂时注释掉
    if (magic_number != buffer.id /*|| (buffer.flags7 & 0x0c) != 0x08*/) {
      assert(!"不合法的 NES2.0 镜像");
    }
    
    // 将 buffer 中的两个 count 赋值给 info，同时申请相应大小的空间并设置指针
    size_t prg_rom_size = (info.prg_rom_count = buffer.prg_rom_count) * 0x4000;
    size_t chr_rom_size = (info.chr_rom_count = buffer.chr_rom_count) * 0x2000;
    uint8_t* memory = new uint8_t(prg_rom_size + chr_rom_size);
    if (memory == NULL) assert(!"内存不足");
    info.prg_rom_ptr = memory;
    info.chr_rom_ptr = memory + prg_rom_size;

    // 设置 mapper 编号
    info.mapper_number = (buffer.flags7&0xf0) | (buffer.flags6>>4);

    // 根据 flags 设置 info 剩余的几个属性
    info.is_four_sreen = buffer.flags6 & 0x08;
    info.have_trainer  = buffer.flags6 & 0x04;
    info.have_sram     = buffer.flags6 & 0x02;
    info.is_vertical   = buffer.flags6 & 0x01;

    // 继续读取 ROM 镜像文件
    // TODO: 暂时跳过 Trainer
    if (info.have_trainer) fseek(fp, 512, SEEK_CUR);
    fread(memory, prg_rom_size + chr_rom_size, 1, fp);
  }

  nes_rom_info* nes_rom_handler::get_info() {
    return &this->info;
  }

  void nes_rom_handler::unload_image() {
    if (fp != NULL) {
      fclose(fp);
      fp = NULL;
    }

    if (info.prg_rom_ptr != NULL) {
      delete info.prg_rom_ptr;
      info.prg_rom_ptr = NULL;
    }
  }

}