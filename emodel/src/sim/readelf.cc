#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <libelf.h>
#include <gelf.h>
#include <vector>
#include "params/GlobalParams.hh"

// 将32位整数转换为小端序字节序的十六进制字符串
void uint32_to_hex_le(uint32_t value, char* buffer) {
    sprintf(buffer, "%02x%02x%02x%02x",
            value & 0xFF,
            (value >> 8) & 0xFF,
            (value >> 16) & 0xFF,
            (value >> 24) & 0xFF);
}

// 将16位整数转换为小端序字节序的十六进制字符串
void uint16_to_hex_le(uint16_t value, char* buffer) {
    sprintf(buffer, "%02x%02x",
            value & 0xFF,
            (value >> 8) & 0xFF);
}

void extract_sections(const std::string& filename) {
    Elf *elf;
    Elf_Scn *scn = NULL;
    GElf_Shdr shdr;
    Elf_Data *data;
    int fd;
    
    // 初始化 libelf
    if (elf_version(EV_CURRENT) == EV_NONE) {
        fprintf(stderr, "ELF library initialization failed\n");
        return;
    }
    
    // 打开 ELF 文件
    if ((fd = open(filename.c_str(), O_RDONLY, 0)) < 0) {
        perror("open");
        std::cerr << "文件路径: " << filename << std::endl;
        return;
    }
    
    if ((elf = elf_begin(fd, ELF_C_READ, NULL)) == NULL) {
        fprintf(stderr, "elf_begin() failed: %s\n", elf_errmsg(-1));
        close(fd);
        return;
    }
    
    // 检查文件类型
    if (elf_kind(elf) != ELF_K_ELF) {
        fprintf(stderr, "Not an ELF file\n");
        elf_end(elf);
        close(fd);
        return;
    }
    
    // 获取 ELF 头
    GElf_Ehdr ehdr;
    if (gelf_getehdr(elf, &ehdr) == NULL) {
        fprintf(stderr, "getehdr() failed: %s\n", elf_errmsg(-1));
        elf_end(elf);
        close(fd);
        return;
    }
    
    printf("Entry point (start PC): 0x%lx\n", (unsigned long)ehdr.e_entry);
    
    // 遍历所有段，提取 .data 和 .text
    while ((scn = elf_nextscn(elf, scn)) != NULL) {
        if (gelf_getshdr(scn, &shdr) != &shdr) {
            continue;
        }
        
        // 获取段名
        const char *sec_name = elf_strptr(elf, ehdr.e_shstrndx, shdr.sh_name);
        if (!sec_name) continue;
        
        // 处理 .text 段
        if (strcmp(sec_name, ".text") == 0 && shdr.sh_size > 0) {
            //printf("Extracting .text section: 0x%lx bytes at 0x%lx\n", 
            //       (unsigned long)shdr.sh_size, (unsigned long)shdr.sh_addr);
            
            data = elf_getdata(scn, NULL);
            if (data && data->d_size > 0) {
                FILE *f = fopen("inst.log", "w");
                if (f) {
                    uint64_t current_addr = shdr.sh_addr;
                    unsigned char *buf = (unsigned char*)data->d_buf;
                    char hex_buffer[9];
                    
                    // 按4字节（32位指令）处理
                    for (size_t i = 0; i < data->d_size; i += 4) {
                        if (i + 3 < data->d_size) {
                            // 以小端序读取指令
                            uint32_t instruction = buf[i] | (buf[i+1] << 8) | 
                                                  (buf[i+2] << 16) | (buf[i+3] << 24);
                            // 转换为正确的十六进制表示
                            uint32_to_hex_le(instruction, hex_buffer);
                            Global::workload.memoryData[current_addr] = instruction;
                            fprintf(f, "@%lx %s\n", current_addr, hex_buffer);
                            current_addr += 4;
                        }
                    }
                    fclose(f);
                    //printf("Text section saved to inst.log (%lu instructions)\n", 
                    //       (unsigned long)(data->d_size / 4));
                } else {
                    perror("Failed to open inst.log");
                }
            }
        }
        
        // 处理 .data 段
        if (strcmp(sec_name, ".data") == 0 && shdr.sh_size > 0) {
            //printf("Extracting .data section: 0x%lx bytes at 0x%lx\n", 
            //    (unsigned long)shdr.sh_size, (unsigned long)shdr.sh_addr);
            
            data = elf_getdata(scn, NULL);
            if (data && data->d_size > 0) {
                FILE *f = fopen("data.log", "w");
                if (f) {
                    uint64_t current_addr = shdr.sh_addr;
                    unsigned char *buf = (unsigned char*)data->d_buf;
                    
                    // 按4字节（32位）处理
                    for (size_t i = 0; i < data->d_size; i += 4) {
                        if (i + 3 < data->d_size) {
                            // 以小端序读取32位数据
                            uint32_t value = buf[i] | (buf[i+1] << 8) | 
                                            (buf[i+2] << 16) | (buf[i+3] << 24);
                            char hex_buffer[9];
                            uint32_to_hex_le(value, hex_buffer);
                            fprintf(f, "@%lx %s\n", current_addr, hex_buffer);
                            
                            // 存入memoryData（假设使用map）
                            Global::workload.memoryData[current_addr] = value;
                            
                            current_addr += 4;
                        } else {
                            // 处理剩余不足4字节的数据
                            uint32_t value = 0;
                            for (size_t j = i; j < data->d_size; j++) {
                                value |= ((uint32_t)buf[j] << ((j - i) * 8));
                            }
                            char hex_buffer[9];
                            uint32_to_hex_le(value, hex_buffer);
                            fprintf(f, "@%lx %s\n", current_addr, hex_buffer);
                            
                            // 存入memoryData
                            Global::workload.memoryData[current_addr] = value;
                            
                            current_addr += (data->d_size - i);
                        }
                    }
                    fclose(f);
                    //printf("Data section saved to data.log (%lu entries)\n", 
                    //    (unsigned long)((data->d_size + 3) / 4));
                } else {
                    perror("Failed to open data.log");
                }
            }
        }
    }
    
    // 查找 print 相关符号
    //printf("\nSearching for print-related functions...\n");
    
    // 获取符号表
    Elf_Scn *symtab_scn = NULL;
    Elf_Data *symtab_data = NULL;
    int symtab_entries = 0;
    
    scn = NULL;
    while ((scn = elf_nextscn(elf, scn)) != NULL) {
        if (gelf_getshdr(scn, &shdr) != &shdr) {
            continue;
        }
        
        if (shdr.sh_type == SHT_SYMTAB) {
            symtab_data = elf_getdata(scn, NULL);
            symtab_entries = shdr.sh_size / shdr.sh_entsize;
            break;
        }
    }
    
    // 查找包含 "print" 的符号
    if (symtab_data && symtab_entries > 0) {
        for (int i = 0; i < symtab_entries; i++) {
            GElf_Sym sym;
            const char *name;
            
            if (gelf_getsym(symtab_data, i, &sym) != &sym) {
                continue;
            }
            
            name = elf_strptr(elf, shdr.sh_link, sym.st_name);
            //if (name && (strstr(name, "print") || strstr(name, "printf") || 
            //             strstr(name, "puts") || strstr(name, "putchar"))) {
                //printf("Found symbol: %s at address 0x%lx, size: 0x%lx\n",
                //       name, (unsigned long)sym.st_value, (unsigned long)sym.st_size);
            //}
        }
    }
    
    elf_end(elf);
    close(fd);
}
