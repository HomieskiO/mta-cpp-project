#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>
#include <iostream>

class Screen {
public:
    //ChatGPT prompt to read files
    static void loadScreens(const std::string& directory, std::vector<std::string>& screenFiles) {
        for (const auto& entry : std::filesystem::directory_iterator(directory)) {
            if (entry.path().extension() == ".screen") {
                screenFiles.push_back(entry.path().string());
            }
        }

        // Sort the screen files in lexicographical order
        std::sort(screenFiles.begin(), screenFiles.end());
    }
};

