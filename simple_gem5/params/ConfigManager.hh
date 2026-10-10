#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <fstream>
#include "json.hh" // 需要安装 nlohmann/json

using json = nlohmann::json;

class ConfigManager {
private:
    static ConfigManager* instance;
    json configData;
    std::string configPath;
    
    ConfigManager() = default;
    
public:
    // 单例模式
    static ConfigManager& getInstance() {
        static ConfigManager instance;
        return instance;
    }
    
    // 禁止复制
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
    
    // 加载配置文件
    bool loadConfig(const std::string& filepath);
    
    // 获取配置参数
    template<typename T>
    T getParameter(const std::string& component, const std::string& param, 
                   T defaultValue = T()) {
        try {
            return configData["components"][component][param].get<T>();
        } catch (...) {
            std::cerr << "Warning: Parameter " << component << "." 
                      << param << " not found, using default: " 
                      << defaultValue << std::endl;
            return defaultValue;
        }
    }
    
    // 获取整个组件配置
    json getComponentConfig(const std::string& component) {
        return configData["components"][component];
    }
    
    // 打印所有配置
    void printConfig() const {
        std::cout << "=== Simulation Configuration ===" << std::endl;
        std::cout << configData.dump(4) << std::endl;
        std::cout << "================================" << std::endl;
    }
};

#endif // CONFIGMANAGER_H