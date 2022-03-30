#ifndef MATCHWRAPPER_H_
#define MATCHWRAPPER_H_

#include <vector>
#include <cocos2d.h>
#include <functional>
#include <modules/ui/GuiMgr.h>
#include "../../network/game/IngameInPacket.h"

enum MatchMode
{
	DEBUG = -1,
	NONE = 0,
	GAMBLE = 1,
	BEST_OF_N = 2,
	COMPETITIVE = 4,
	STREAK_2 = 8
};

class MatchWrapper
{
public:

	MatchWrapper();
	~MatchWrapper();

	void init(GameMode mode);

	void notifyBallType(bool isSolid);
	
	void setOnPocketedBall(int pId, std::vector<int> balls);

	void sendFinishAnim();

	void resetRack();

	void onGameFinished(int pId);

	void startPlacingCueBall();

	void highlightBalls(std::vector<int> *ballls);

	void undoLetPlayerCallPocket();

	void letPlayerCallPocket(int pId);

	void onMatchEnd(int pId);

	void onNotifyEndGameComplete();

	void notify(AnnounceType announType, const std::string & message = "", const CUSTOM_CALLBACK & callback = nullptr);

	void notifyComplete(const CUSTOM_CALLBACK & callback);

	int* getScore();

	void onReceiveStartNextGame(StartMatchMsg *pk);

	void nextGameClient(const int & userScore, const int &  enemyScore, const int & winnerIdx);

	void doNextGameClient();

	void updateScore(int userWin, int enemyWin);

	bool hasNextGame();

	void nextGame();

	void onNotifyNewRoundComplete();

	int getWinner();
	
	void onNextGame();

	void applySeed();

private:
	bool _animatingNotify;

	std::vector<CUSTOM_CALLBACK> _notifyQueues;
	
	MatchMode _matchMode;
	int _curGameIdx;
	double _seed;

	GameMode _gameMode;
	int _gameCount;
	std::vector<int> _gameResults;
	int _winner;

	//new game
	bool _receivedNextGame;
	bool _clientDonePreviousGame;

};

#endif // !MATCHWRAPPER_H_



