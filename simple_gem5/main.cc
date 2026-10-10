#include <stdio.h>
#include <string>
#include "readelf.h"
#include <iostream>
#include "BaseParams.hh"
#include "ConfigManager.hh"
#include "ifu/ras.hh"

// 模拟的统计头文件
// namespace statistics {
//     class Scalar {};
//     class Group {
//     public:
//         Group(void* parent) {}
//     };
// }

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <config.json>" << std::endl;
        return 1;
    }
    extract_sections(argv[1]);

    ConfigManager& config = ConfigManager::getInstance();
    
    // 默认配置文件，也可以通过命令行参数指定
    std::string configFile = "config.json";
    if (argc > 1) {
        configFile = argv[1];
    }
    
    if (!config.loadConfig(configFile)) {
        std::cerr << "Failed to load config, using defaults." << std::endl;
    }
    
    // 2. 打印整个配置
    config.printConfig();
    
    // 3. 创建参数对象（参数从配置文件自动加载）
    ReturnAddrStackParams rasParams;
    
    // 4. 创建 ReturnAddrStack 实例
    ReturnAddrStack ras(rasParams);
    ras.printConfig();
    
    // 5. 运行仿真
    std::cout << "\n=== Starting Simulation ===" << std::endl;
    
    // ... 仿真代码 ...
    

    
    return 0;
}