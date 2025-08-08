#include "save_manager.hpp"

SaveManager& SaveManager::getInstance() {
    static SaveManager instance;
    return instance;
}

void SaveManager::save(const std::string& path, const nlohmann::json& data) {
    std::ofstream file(path);
    if (file) {
        file << data.dump(4); // Pretty-print
    }
}

nlohmann::json SaveManager::load(const std::string& path) {
    std::ifstream file(path);
    if (file) {
        nlohmann::json data;
        file >> data;
        return data;
    }
    return nlohmann::json(); // Return empty JSON if file doesn't exist
}