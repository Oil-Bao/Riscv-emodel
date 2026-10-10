#!/usr/bin/env python3
# test.py
import emodel

# 直接修改全局配置
emodel.Params.IFU.FetchWidth = 32
emodel.Params.IFU.DecodeWidth = 16
emodel.Params.IFU.BootPC = 128
emodel.workload.elfPath = "hello.elf"
print("=== 运行仿真 ===")
result = emodel.simulation()