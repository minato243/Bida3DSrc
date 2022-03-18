#ifndef GAME_HANDLER_H_
#define GAME_HANDLER_H_

#include "BaseFramework.h"
#include <data/DataStruct.h>
#include "IngameInPacket.h"
#include "IngameOutMsg.h"
#include "../base/BaseHandler.h"
#include "IngamePacketBuilder.h"
#include "../../core/ps/internal/BallBodyV2.h"

#define gameHandler GameHandler::getInstance()

class GameHandler
	:BaseHandler
{
public:
	GameHandler();
	~GameHandler();

	bool onReceived(fr::InPacket* pk);

	void sendShoot(const ShootData & shot);

	void waitForStartingMatch();

	void onReceiveStartMatch();

	void sendSetBall(int ballId, ps::ExtMath::vector ballPos, bool submit = false);

	void sendReportOutOfSync();

	void sendOpenCard(int cardIdx);

	void sendUpdateCueDirection(const ps::ExtMath::vector & direction, const double & offsetX, const double & offsetY);

	void sendEndTurn();

	void sendCallPocket(int pocketId);

	void sendActiveExtension();

	void sendUpdateCueBallPosition();

	void sendFakeResultClient(bool foul, bool isSwitchPlayer, const std::vector<PocketBallData> & pocketedBall, const std::vector<ps::BallBody *> * activeBalls);

	void sendPickKeoBuabao(int cardId);

	void sendSwitchCue(int cueId);

	void sendFakeEndGame();

private:

	static GameHandler *_instance;

public:
	static GameHandler * getInstance();
};

#endif // !GAME_HANDLER_H_

