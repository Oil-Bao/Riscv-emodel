#!/usr/bin/env python3
import add

# 注意：SimConfig和ReplacePolicy现在在add.config子模块中
config = add.config.SimConfig()  # 使用 add.config.SimConfig()
config.cache_config.size = 65536  # 64KB
config.cache_config.assoc = 16
config.cache_config.block_size = 128
config.cache_config.replace_policy = add.config.ReplacePolicy.PLRU  # 使用 add.config.ReplacePolicy

print("=== 设置全局配置 ===")
add.set_global_config(config)

print("=== 获取并打印当前配置 ===")
#current_config = add.get_global_config()
#current_config.print_config()

print("\n=== 使用配置进行计算 ===")
result1 = add.add(10, 20)
print(f"add(10, 20) = {result1}")

result2 = add.sub(50, 30)
print(f"sub(50, 30) = {result2}")

print("\n=== 修改配置后重新计算 ===")
# 修改配置
config.cache_config.size = 16384  # 16KB
config.cache_config.replace_policy = add.config.ReplacePolicy.LRU
add.set_global_config(config)

result3 = add.add(10, 20)
print(f"New add(10, 20) = {result3}")

result4 = add.sub(50, 30)
print(f"New sub(50, 30) = {result4}")

print("\n=== 重置为默认配置 ===")
add.reset_config()
result5 = add.add(10, 20)
print(f"After reset add(10, 20) = {result5}")