// 修改 readelf.hh 头文件
#ifndef _READELF_H_
#define _READELF_H_

#include <string> 
#include <libelf.h>


void uint32_to_hex_le(uint32_t value, char* buffer);
void uint16_to_hex_le(uint16_t value, char* buffer);
void extract_sections(const std::string& filename);  


#endif