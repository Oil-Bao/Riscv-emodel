#include "ConfigManager.hh"

bool ConfigManager::loadConfig(const std::string& filepath) {
    try {
        std::ifstream configFile(filepath);
        if (!configFile.is_open()) {
            std::cerr << "Error: Cannot open config file: " << filepath << std::endl;
            return false;
        }
        
        configFile >> configData;
        configPath = filepath;
        
        std::cout << "Successfully loaded config from: " << filepath << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error loading config: " << e.what() << std::endl;
        return false;
    }
}