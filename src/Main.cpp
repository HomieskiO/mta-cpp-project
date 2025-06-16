#include <iostream>
#include <vector>
#include <string>

#include "IOUtils.h"
#include "MenuManager.h"
#include "GameFromFileManager.h"

enum class RunMode {
    DEFAULT,
    SAVE,
    LOAD,
    LOAD_SILENT
};

struct RunConfig {
    RunMode mode = RunMode::DEFAULT;
    std::string stepsFile = "tanks-game_01.steps";
};

RunConfig parseArgs(int argc, char* argv[]) {
    RunConfig config;

    if (argc == 1) return config;

    std::vector<std::string> args(argv + 1, argv + argc);

    if (args[0] == "-save") {
        if (args.size() != 1) {
            std::cerr << "Usage: tanks.exe -save" << std::endl;
            exit(1);
        }
        config.mode = RunMode::SAVE;
    }
    else if (args[0] == "-load") {
        if (args.size() < 2 || args.size() > 3) {
            std::cerr << "Usage: tanks.exe -load <steps_file> [-silent]" << std::endl;
            exit(1);
        }
        config.mode = RunMode::LOAD;
        config.stepsFile = args[1];
        if (args.size() == 3 && args[2] == "-silent") {
            config.mode = RunMode::LOAD_SILENT;
        }
    }
    else {
        std::cerr << "Usage:\n"
            << "  tanks.exe                        (play normally)\n"
            << "  tanks.exe -save\n"
            << "  tanks.exe -load <steps_file> [-silent]" << std::endl;
        exit(1);
    }

    return config;
}

int main(int argc, char* argv[]) {
    RunConfig config = parseArgs(argc, argv);

    switch (config.mode) {
    case RunMode::DEFAULT: {
        MenuManager menuManager(false);
        menuManager.openMenu();
        break;
    }

    case RunMode::SAVE: {
        MenuManager menuManager(true);
        menuManager.openMenu();
        break;
    }

    case RunMode::LOAD:
    case RunMode::LOAD_SILENT: {
        std::string screenFile = config.stepsFile.substr(0, config.stepsFile.rfind('.')) + ".screen";
        bool silent = (config.mode == RunMode::LOAD_SILENT);
        GameFromFileManager gameManager(screenFile, config.stepsFile, silent);
        gameManager.startGame();
        break;
    }
    }

    return 0;
}
