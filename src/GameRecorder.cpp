#include "GameRecorder.h"
#include <sstream>

void GameRecorder::logAction(const int playerId, const int tankId, ActionType action) {
    steps.push_back({ tick, playerId, tankId, action });
}

void GameRecorder::saveToFile(const std::string& filename) {

    // TODO add flag should record or not

    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error: Cannot open " << filename << " for writing.\n";
        return;
    }

    for (const auto& step : steps) {
        outFile << step.tick << " " << step.playerId << " " << step.tankId << " " << static_cast<int>(step.action) << "\n";
    }

    outFile.close();
}
