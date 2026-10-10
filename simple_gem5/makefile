# 编译器设置
CC := gcc
CXX := g++
CFLAGS := -Wall -Wextra -O2 -std=c11 -D_GNU_SOURCE -D_XOPEN_SOURCE=700
CXXFLAGS := -Wall -Wextra -O2 -std=c++17 -D_GNU_SOURCE
LDFLAGS := -lm

# 目标文件
TARGET := simulator

# 源代码目录
SRC_DIRS := src params

# 排除 libelf 目录，单独处理
C_SOURCES := $(shell find $(SRC_DIRS) -name "*.c" -type f 2>/dev/null | grep -v "inlcude/libelf" | grep -v -E "(~|\.bak|\.swp)$$" | sort)
CPP_SOURCES := $(shell find $(SRC_DIRS) \( -name "*.cc" -o -name "*.cpp" \) -type f 2>/dev/null | grep -v -E "(~|\.bak|\.swp)$$" | sort)

# 单独处理 libelf 的 C 文件
LIBELF_DIR := src/inlcude/libelf
LIBELF_C_SOURCES := $(shell find $(LIBELF_DIR) -name "*.c" -type f 2>/dev/null | sort)

# 主文件
MAIN_SOURCE := main.cc

# 头文件目录
INCLUDE_DIRS := -I. -Iparams -Isrc -Isrc/ifu -Isrc/inlcude -Isrc/inlcude/base -Isrc/inlcude/isa -Isrc/inlcude/libelf -Isrc/lsu

# 生成目标文件列表（确保不重复）
C_OBJS := $(C_SOURCES:.c=.o)
CPP_OBJS := $(CPP_SOURCES:.cc=.o)
CPP_OBJS := $(CPP_OBJS:.cpp=.o)
LIBELF_OBJS := $(LIBELF_C_SOURCES:.c=.o)

# 所有目标文件（排除重复）
ALL_OBJS := $(C_OBJS) $(CPP_OBJS) $(LIBELF_OBJS)

# 去重（确保每个目标文件只出现一次）
UNIQUE_OBJS := $(sort $(ALL_OBJS))

# 主要目标
all: $(TARGET)

$(TARGET): $(MAIN_SOURCE) $(UNIQUE_OBJS)
	@echo "链接目标文件..."
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) $(MAIN_SOURCE) $(UNIQUE_OBJS) -o $@ $(LDFLAGS)

# 编译普通 C 文件的规则
%.o: %.c
	@echo "编译 $<..."
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@

# 编译 C++ 文件的规则
%.o: %.cc
	@echo "编译 $<..."
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@

%.o: %.cpp
	@echo "编译 $<..."
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@

# 专门编译 libelf 的 C 文件
$(LIBELF_DIR)/%.o: $(LIBELF_DIR)/%.c
	@echo "编译 libelf: $<..."
	$(CC) $(CFLAGS) -D_GNU_SOURCE -D_XOPEN_SOURCE=700 $(INCLUDE_DIRS) -c $< -o $@

# 清理
clean:
	rm -f $(UNIQUE_OBJS) $(TARGET) *.o

# 深度清理
distclean: clean
	rm -f *~ *.bak *.swp core
	find . -name "*.o" -delete
	find . -name "*.d" -delete

# 显示信息
info:
	@echo "=== Build Configuration ==="
	@echo "Target: $(TARGET)"
	@echo "Main Source: $(MAIN_SOURCE)"
	@echo ""
	@echo "C Sources (non-libelf): $(words $(C_SOURCES)) files"
	@echo "C++ Sources: $(words $(CPP_SOURCES)) files"
	@echo "LibELF C Sources: $(words $(LIBELF_C_SOURCES)) files"
	@echo ""
	@echo "Total Objects: $(words $(UNIQUE_OBJS))"
	@echo ""
	@echo "Include Directories:"
	@echo $(INCLUDE_DIRS)
	@echo ""
	@echo "To build: make"
	@echo "To clean: make clean"

# 列出所有源文件
list-sources:
	@echo "=== C Sources (non-libelf) ==="
	@for src in $(C_SOURCES); do echo "  $$src"; done
	@echo ""
	@echo "=== C++ Sources ==="
	@for src in $(CPP_SOURCES); do echo "  $$src"; done
	@echo ""
	@echo "=== LibELF C Sources ==="
	@for src in $(LIBELF_C_SOURCES); do echo "  $$src"; done

# 列出所有目标文件
list-objects:
	@echo "=== Object Files ==="
	@for obj in $(UNIQUE_OBJS); do echo "  $$obj"; done

.PHONY: all clean distclean info list-sources list-objects