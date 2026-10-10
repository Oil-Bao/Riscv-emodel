import Simulator

# 直接创建C++对象
cache = simulator.Cache(65536, 8, simulator.ReplacePolicy.LRU)
sim = simulator.Simulator("gem5_like_simulation", 1000000, cache)

# 直接调用C++方法
sim.run()