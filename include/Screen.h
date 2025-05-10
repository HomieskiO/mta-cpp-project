#pragma once

#include <string>
#include <vector>

class Screen {
public:
    std::string name;

    Screen(const std::string& screenName);

    static bool loadAllScreenFiles(std::vector<Screen>& screens);
	static void displayScreensOptionsMenu(const std::vector<Screen>& screens);
};
