#ifndef INGAME_OUT_MSG_H
#define INGAME_OUT_MSG_H

#include <BaseFramework.h>
#include "../base/BasePacket.h"
#include <data/DataStruct.h>
#include "../../core/ps/internal/BallBodyV2.h"

class CmdSendShoot 
	: public BaseOutMsg
{
public:
	CmdSendShoot();
	~CmdSendShoot();

	void packData(const ShootData & data);
};

class CmdSendFakeShoot
	: public BaseOutMsg
{
public:
	CmdSendFakeShoot();
	~CmdSendFakeShoot();

	void packData(const ShootData & data);
};

class CmdSendUpdateCueDirection
: public BaseOutMsg
{
public:
	CmdSendUpdateCueDirection();
	~CmdSendUpdateCueDirection();

	void packData(const ps::ExtMath::vector & direction, const double & offsetX, const double & offsetY);
};

class CmdSendUpdateCueBallPosition
	:public BaseOutMsg
{
	CmdSendUpdateCueBallPosition();
	~CmdSendUpdateCueBallPosition();

	void putData(ps::ExtMath::vector pos, bool submit);
};

class CmdSendBet
	: public BaseOutMsg
{
public:
	CmdSendBet();
	~CmdSendBet();

	void packData(int gameId, int gold);
};

class CmdSendReady
	: public BaseOutMsg
{
public:
	CmdSendReady();
	~CmdSendReady();

	void packData() {};
};

class CmdRematch
	: public BaseOutMsg
{
public:
	CmdRematch();
	~CmdRematch();

	void packData() {};
};

class CmdLeaveGame
	: public BaseOutMsg
{
public:
	CmdLeaveGame();
	~CmdLeaveGame();

	void packData() {};
};


class CmdCallPocket
	: public BaseOutMsg
{
public:
	CmdCallPocket();
	~CmdCallPocket();

	void packData(int pocketId);
};

class CmdActiveExtension
	: public BaseOutMsg
{
public:
	CmdActiveExtension();
	~CmdActiveExtension();

	void packData() {};
};

class CmdEndTurn
	: public BaseOutMsg
{
public:
	CmdEndTurn();
	~CmdEndTurn();

	void packData() {};
};

class CmdFakeEndGame
	: public BaseOutMsg
{
public:
	CmdFakeEndGame();
	~CmdFakeEndGame();

	void packData() {};
};

class CmdSwitchCue
	: public BaseOutMsg
{
public:
	CmdSwitchCue();
	~CmdSwitchCue();

	void packData(int cueId);
};

class CmdSetBall
	: public BaseOutMsg
{
public:
	CmdSetBall();
	~CmdSetBall();

	void packData(int ballId, ps::ExtMath::vector ballPos, bool submit);
};

class CmdStartCheat
	: public BaseOutMsg
{
public:
	CmdStartCheat();
	~CmdStartCheat();

	void packData() {};
};


class CmdEndCheat
	: public BaseOutMsg
{
public:
	CmdEndCheat();
	~CmdEndCheat();

	void packData() {};
};

class CmdFakeClientResult
	:public BaseOutMsg
{
public:
	CmdFakeClientResult();
	~CmdFakeClientResult();

	void packData(bool foul, bool isSwitchPlayer, const std::vector<PocketBallData> & pocketedBall, const std::vector<ps::BallBody*> *activeBalls);
};

class CmdSelectKeoBuaDao
	:public BaseOutMsg
{
public:
	CmdSelectKeoBuaDao();
	~CmdSelectKeoBuaDao();

	void packData(int cardId);
};
#endif // !INGAME_OUT_MSG_H
