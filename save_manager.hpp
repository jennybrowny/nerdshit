#pragma once
#include <nlohmann/json.hpp>
#include <fstream>

class SaveManager {
public:
    static SaveManager& getInstance();
    void save(const std::string& path, const nlohmann::json& data);
    nlohmann::json load(const std::string& path);

private:
    SaveManager() = default;
};