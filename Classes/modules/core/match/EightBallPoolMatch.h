#ifndef EIGHTBALLPOLLMATCH_H_
#define EIGHTBALLPOLLMATCH_H_

#include <data/GameConstant.h>
#include <modules/core/ps/internal/ExtensionMath.hpp>
#include "Player.h"
#include <data/DataStruct.h>
#include <modules/core-implement/Table.h>
#include <modules/core/utils/TableEvent.h>
#include <modules/ui/GuiMgr.h>
#include "MatchWrapper.h"
#include "MatchBallSuggestion.h"

class EightBallPoolMatch
{
public:
	EightBallPoolMatch();
	~EightBallPoolMatch();

	void reset();

	void resetGameData();

	void setPlayerListOrder(const std::vector<Player> & players);

	void setRule();

	void start(int firstPlayer);

	void subscribeToTable(Table *table);

	void unsubscribeEverything();

	void onTableEvent(TableEventType eventType, TableEventData *event);

	void canCurPlayerActiveExt();

	void activeExt(double force);

	bool isExtActivating();

	bool isInExt();

	bool isCurTeam(int pId);

	void letPlayerSetCueBallPosition(int pId);

	void afterPlayerSetCueBallPosition(int pId, ps::ExtMath::vector pos);

	bool isCapCueBallToKitchen();

	void setManager(MatchWrapper *manager);

	void onPlayerWin(int pId, std::string msg);

	void processAchievement();

	void notify(AnnounceType type, std::string str = "", const CUSTOM_CALLBACK & callback = nullptr);

	void notifyComplete(const CUSTOM_CALLBACK & callback);

	void resetRack();

	void selectBallType(int pId, std::vector<PocketBallData> & pocketBalls);

	void onFoul(TableEventType eventType);

	void onFoul();

	void onNotifyTimeOutDone(int playerIdx);

	int getBallCollideCushionCount();

	bool isCueBallCollideCushion();

	void checkFoul(BallCollisionData & event);

	void checkFoul(BallFellHoleData & event);

	void checkFoul(BallOutOfTableData & event);

	bool checkFoulLag();

	void onPlayerFinishLag(int idx);

	void logAndCompareBallPosition();

	bool isCurplayerPlacingBall();

	void onWorldPaused();

	int getPocketOfBall(int ballNum);

	int getCushionEventsOfBall(int number);

	int getFinishedCueChallenge();

	int getCurTurnCushion();

	int getCurTurnCombin();

	BallPocketResult getBallPocketResult(std::vector<PocketBallData> &pocketBallData);

	int getCurGroupIdx(int idx = -1);

	void callPocket(int pId, int pocketId);

	void receiveStartNextGame();

	void hasNextMatch();

	void addOnBallPocketedDelegate(int delegate);

	void addOnWorldPauseDelegate(int delegate);

	void onBallPocketedDelegate(int pId, int pocketData, int cushion, int combin, int isFoul);

	bool onBreakTurnFinished(std::vector<PocketBallData> & pocketBalls, bool isCueBallFell, bool isEightBallFell, WinData * win = nullptr);

	void rollbackShotResult(const ShootResultData & data);

	void forceChangePlayerTurn();

	int getNextPlayerIdx(int idx = -1);

	void notifyFoul(int player = -1, WinData * win = nullptr, bool placeBall = true);

	void notifyFoulDone(int player, WinData * win, bool placeBall);

	void updatePocketedBalls();

	bool isLag();

	void onLagDone();

	int getPlayersCount();

	void onFinishCueChallenge();

	bool isPlayerFinishAllBallOfType(int idx = -1);

	void setPlayerBallType(int id, BallType type);

	BallType getAnotherType(BallType type);

	bool canShoot();

	bool canSetBallPosition();

	int getTurn();

	int getCurPlayerIdx();

	void resetTimer();

	void update(float dt);

	bool isCurPlayer(int idx = -1);

	int getTimePercent();

	float getTimeRemain();

	bool curPlayerNextTurn();

	void changePlayer();

	void startLag(int idx);

	void startNextTurnByUid(int uId);

	void startNextTurn(bool isNextTurn);

	void startTurnPlayerClientOrServer(int idx, bool ignoreNotifyTurn, int server);

	int getCombineShotCount();

	int getCushionShotCount();

	int getShotCounter();

	int getTotalTurnCount();

	int getPlayerTurnCount();

	int getPocketedBall();

	int getPlayerPocketedBall(int index = 0);

	void startTurnTeamPlayer(int teamId, int ignoreNotify);

	void startTurnPlayer(int idx, bool ignoreNotifyTurn);

	void letPlayerSelectPocket(int idx);

	void forcePause();

	void forceResume();

	std::vector<int> *getValidBall(int idx = -1);

	Ball *getSuggestedBall();

	bool isBallInPocket(int number);

	BallType getBallType(int ballNum);

	void highlightValidBall(int idx);

	void debugAllBallPosition();

	void onShoot(int cueType, bool opponentShoot = false);

	void onTurnTimeOut();

	int getDebugInfo();

	void handleBallCollision(BallCollisionData *ballCollisionData);

	void handleBallOutOfTable(BallOutOfTableData * ballOutTable);

	void handleBallFellInHole(BallFellHoleData *ballFellInHoleData);

	void handleCushionCollision(int ballNumber, int cushionId);

	int getPlayerIdFromIdx(int idx);

	void applySeed(long long seed);

	void loadFakeDataMatch();

	void notifyBallType(bool isSolid);

	Player *getPlayerFromId(int uId);

	void onEndGame(int winner);

	public:
		const static int NUM_PLAYER = 2;

	private:
		bool _lag;
		bool _started;
		bool _paused;
		bool _counting;
		bool _extActivated;
		bool _selectedBallType;

		int _turn;
		bool _breakTurn;
		bool _isSetBallPosition;
		int _waitingPlayerIdx;
		int _lastCollide;
		int _curPlayerIdx;
		int _oldPlayerIdx;
		double _curPlayerRemainTime;
		int _countTurnSkip;
		bool _ignoreServerStartTurn;
		int _playerTurns[NUM_BALL_TYPE];

		int _lagPlayerIdx;
		int _playerThatSetBallPos;

		std::vector<Player> _players;
		std::vector<PocketBallData> _pocketedBall[NUM_BALL_TYPE];
		std::vector<PocketBallData> _curTurnPocketBall;
		std::vector<int> _curTurnOutOfTableBall;
		std::vector<int> _listPockedBallByPlayer[2];

		bool _justShoot;
		int _shotCounter[4];
		bool _foul;
		int _calledPocket[NUM_PLAYER];
		
		int _combineShotCount[NUM_PLAYER];
		int _cushionShotCount[NUM_PLAYER];

		Table *_table;
		std::vector<std::string> _foulList;
		std::vector<BallCollisionData> _ballCollisionsCurTurn;
		std::vector<CushionCollisionData> _ballTouchCushionCurTurn;
		std::vector<TableEventData> _generalCollision;

		int _pauseDelegateIdx;
		int _eventDelegateIdx;

		std::vector<CUSTOM_CALLBACK> _notifyQueues;
		bool _animatingNotify;
		bool _ignoreNotifyTurn;
		
		MatchBallSuggestion *_ballSuggestion;

	public:
		bool _serverFinish;
		MatchWrapper *_manager;
		int winner;
		
		const static double TURN_TIME;
		
};

#endif // !EIGHTBALLPOLLMATCH


