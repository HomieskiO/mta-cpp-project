#include "GameManager.h"
#include "ioUtils.h"

class GameFromFileManager : GameManager {
private:
	bool isSilent;
	std::string stepsFile;
	std::vector<GameStep> steps;
	void applyGameStep(const GameStep& step);
	Tank* findTankById(int playerId, int tankId);
	void gameOver();
	//void replayGame(const std::string& recordFile);
	//void replayAction(const GameStep& action);
public:
	GameFromFileManager(const std::string& screenFile, const std::string& stepsFile, bool isSilent);
	void startGame();
	void gameLoop();
};