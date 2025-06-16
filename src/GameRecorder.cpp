#include "GameRecorder.h"
#include <sstream>
#include "ioUtils.h"

GameRecorder::GameRecorder(bool shouldSaveSteps) {
    this->shouldSaveSteps = shouldSaveSteps;
    this->tick = 0;
    std::vector<CollisionEvent> collisions = {};
    std::vector<MineDeathEvent> mineDeaths = {};
}

void GameRecorder::logAction(const int playerId, const int tankId, ActionType action) {
    if (shouldSaveSteps) {
        steps.push_back({ tick, playerId, tankId, action });
    }
}

void GameRecorder::saveToFile(const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error: Cannot open " << filename << " for writing.\n";
        return;
    }

    for (const auto& step : steps) {
        outFile << step.tick << "," << step.playerId << "," << step.tankId << "," << static_cast<int>(step.action) << "\n";
    }

    outFile.close();
}

std::vector<GameStep> GameRecorder::loadGameStepsFromCSV(const std::string& filePath) {
    std::vector<GameStep> steps;
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filePath);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string tickStr, playerStr, tankStr, actionStr;

        if (!std::getline(ss, tickStr, ',')) continue;
        if (!std::getline(ss, playerStr, ',')) continue;
        if (!std::getline(ss, tankStr, ',')) continue;
        if (!std::getline(ss, actionStr, ',')) continue;

        try {
            GameStep step;
            step.tick = std::stoi(tickStr);
            step.playerId = std::stoi(playerStr);
            step.tankId = std::stoi(tankStr);

            int actionInt = std::stoi(actionStr);
            if (actionInt < 0 || actionInt > static_cast<int>(ActionType::END_GAME)) {
                throw std::out_of_range("Invalid action value: " + actionStr);
            }
            step.action = static_cast<ActionType>(actionInt);

            steps.push_back(step);
        }
        catch (const std::exception& e) {
            std::cerr << "Skipping invalid line: " << line << " (" << e.what() << ")\n";
        }
    }

    return steps;
}


std::string GameRecorder::writeResultFile(
    const std::string& screenFileName,
    int player1Score,
    int player2Score
) {
    std::string resultFileName = screenFileName.substr(0, screenFileName.rfind('.')) + ".result";
    std::ofstream out(resultFileName);

    if (!out) {
        std::cerr << "Failed to open result file: " << resultFileName << std::endl;
        // throw error
        return "Error";
    }

    out << "Shell collisions:\n";
    for (const auto& col : collisions) {
        out << "Tick " << col.tick << ": Shell hit " << col.whatWasHit
            << " at (" << col.x << "," << col.y << ")\n";
    }

    out << "\nMine deaths:\n";
    for (const auto& death : mineDeaths) {
        out << "Tick " << death.tick << ": Player " << death.playerId
            << "'s tank " << death.tankId << " stepped on a mine at ("
            << death.x << "," << death.y << ")\n";
    }

    out << "\nFinal score:\n";
    out << "Player 1: " << player1Score << "\n";
    out << "Player 2: " << player2Score << "\n";

    out.close();
    return resultFileName;
}
