#ifndef EIGHTBALLPOLLCTRL_H_
#define EIGHTBALLPOLLCTRL_H_

#include "modules/core/ps/internal/ExtensionMath.hpp"
#include "EightBallPoolMatch.h"
#include <modules/network/match/MatchHandler.h>
#include "Player.h"
#include <modules/network/game/GameHandler.h>
#include "../inventory/CueInfo.h"
#include "../../network/game/IngameInPacket.h"

class EightBallPollCtrl
{
public:
	EightBallPollCtrl();
	~EightBallPollCtrl();

	void init();
	
	void showEightPoolModeSelectGUI();

	void getEightPoolModeSelectGUI();

	void getFindMatchLayer();

	void getLobbyUI();

	void gotoModeRT3(int sendQuickMatch, int channelId);

	void showRT3Lobby();

	void cancelRT3Matching();

	void showEightPoolModeSelection();

	void gotoMode(int mode, int quick, int channelId = 1);

	void playWithBot(int mode, double accuracy);

	void onResponeQuickMatch(ResponseJoinRoomMsg * pk);

	void onCannotFoundOpponent(bool quick = false);

	void onReceiveStartingMatchPacket(StartMatchMsg * pk);

	void onReceiveStartNextGame(StartMatchMsg *pk);

	void afterJoinedRoom();

	void getPlayerInfoInMatch(int index);

	void setGameInfo(ResponseJoinRoomMsg * pk);

	void getCurGameChannel();

	void getCurGameFee();

	void getCurGamePrize();

	void getOpponent(int index);

	void updateUIPlayerInfo();

	void receiveStartingMatch(int data);

	void receiveShoot(ShootData shot);

	void opponentReady();

	void startMatch(StartMatchMsg * pk, bool isFirst);

	int getGameFee(int type);

	void createRT3Match(int data);

	void createRT2Match(int data, bool isFirst);

	void createS2Match(int data, bool isFirst);

	void createRT1Match(int data, bool isFirst);

	void receiveSwitchCue(SwitchCueMsg *data);

	void onPlayerSwitchCue(int cueId);

	void onPlayerSwitchCue(int cueId, int playerIdx);

	void onSwitchCue(CueInfo *cueInfo, bool updateAngle = true, bool isCurPlayer = false);
	
	void receiveStartTurn(int pId, bool isPlacingCueBall);

	void onStartPlayerTurn(int pId);

	void onSwitchPlayer(int pId, bool isPlacingCueBall);

	void onFinishCueChallenge(int cueChallenge);

	void onShoot(int cueType, int opponentShoot, int ballId, double force, ps::ExtMath::vector offset, ps::ExtMath::vector direction);

	void onBeforeShoot();

	void onMatchEnd(bool isWinner);

	void onMatchRT1End(bool isWinner);

	void onMatchS2End(bool isWinner);

	void onMatchRT2End(bool isWinner);

	void onMatchRT3End(bool isWinner);

	void setBallPosition(int ballId, ps::ExtMath::vector pos);

	void setShootResult(ShotResultMsg *data);

	void receiveStartNextGame(int data);

	void logHistory();

	void onEndGame(bool isWinner);

	void onReceiveEndGame(bool isWinner, int callback);

	void onCallPocketFinish(int pocketId);

	void receiveCallPocket(int pocketId);

	void curPlayerCallExtension(bool curPlayer = true);

	void hasNextMatch();

	bool hasNextGame();

	void rollbackShotResult(int error);

	void receiveTimeOutTurn();

	void getS2LastMatchResult();

	void getRewards(int mode);

	void receiveSetBall(int ballId, ps::ExtMath::vector pos);

	void receiveRematch(int data);

	void placingCueBallPosition(ps::ExtMath::vector & position);

	EightBallPoolMatch *getCurMatch();

	void setSelectPocketState(SelectPocketState state);

	int getGameId();

	int getPlayerIdx(int userId);

	void receiveChannelInfo(int ccu);

	void receiveStartMatchingUI();

	void onSelectPickFirstTurn(PickFirstTurnStateMsg *msg);

	void sendFakeClientResult(bool isFoul, bool isSwitchPlayer, const std::vector<PocketBallData> & pocketedBall, const std::vector<BallBody *> *activeBalls);

	void resetMatch();

	void onReceiveDisconnect(PlayerDisconnectMsg *msg);

	void onReceiveReconnect(PlayerReconnectMsg *msg);

	bool checkShootResult();

	private:
		EightBallPoolMatch *_curMatch;
		SelectPocketState _callingPocket;
		PlaceCueBallState _placeCueBall;
		int _pocketId;

		bool _receivedJoinRoom;
		
		ResponseJoinRoomMsg *_currentGameInfo;
		int _curGameMode;
		int _gameId;
		int _goldBet;

		std::vector<Player> _players;
		std::vector<CueInfo *> _playersCueData;
		CueInfo *_playerCueData;

		bool _waitingForStartMatch;
		ShootResultData _shootResult;
		bool _receiveShootResult;

	public:
		GameMode _currentMode;
		int _channelId;
		bool _startedMatch;

		static const bool DEBUG_PLAY_ALONE = false;
		static const ps::ExtMath::vector MIN_PLACING_BALL;
		static const ps::ExtMath::vector MAX_PLACING_BALL;
		static const ps::ExtMath::vector MAX_PLACING_BALL_KITCHEN;
};

#endif // !EIGHTBALLPOLLCTRL_H_



