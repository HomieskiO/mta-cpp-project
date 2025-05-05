#include <algorithm>
#include <filesystem>
#include <regex>
#include <iostream>
#include "Screen.h"

namespace fs = std::filesystem;

Screen::Screen(const std::string& screenName)
    : name(screenName) {
}

bool Screen::loadAllScreenFiles(std::vector<Screen>& screens) {
	screens.clear();
	// ChatGPT prompt - sort a directory and find all files matching a regex pattern
    const std::string path = "../screens";
    const std::regex pattern(R"(.*\.screen)");

    for (auto& entry : fs::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            std::string filename = entry.path().filename().string();
            if (std::regex_match(filename, pattern)) {
                screens.emplace_back(filename);
            }
        }
    }

    std::sort(screens.begin(), screens.end(), [](const Screen& a, const Screen& b) {
        return a.name < b.name;
        });

    return !screens.empty();
}


void Screen::displayScreensOptionsMenu(const std::vector<Screen>& screens) {
    std::cout << "Available Screens:\n";
    for (size_t i = 0; i < screens.size(); ++i) {
        std::cout << "  (" << i + 1 << ") " << screens[i].name << "\n";
    }
}
