#include "IngameOutMsg.h"
#include <core/GameMgr.h>

using namespace pk;

CmdSendShoot::CmdSendShoot()
	:BaseOutMsg(pk::PacketID::SHOOT)
{
}

CmdSendShoot::~CmdSendShoot()
{
}

void CmdSendShoot::packData(const ShootData & shot)
{
	putDouble(shot.direction.x);
	putDouble(shot.direction.y);
	putDouble(shot.direction.z);
	putDouble(shot.offsetX);
	putDouble(shot.offsetY);
	putDouble(shot.force);
}

CmdSendUpdateCueDirection::CmdSendUpdateCueDirection()
	: BaseOutMsg(PacketID::UPDATE_CUE_DIR)
{
}

CmdSendUpdateCueDirection::~CmdSendUpdateCueDirection()
{
}

void CmdSendUpdateCueDirection::packData(const ps::ExtMath::vector & direction, const double & offsetX, const double & offsetY)
{
	putDouble(direction.x);
	putDouble(direction.y);
	putDouble(direction.z);

	putDouble(offsetX);
	putDouble(offsetY);
}

CmdSendUpdateCueBallPosition::CmdSendUpdateCueBallPosition()
	:BaseOutMsg(PacketID::UPDATE_CUE_BALL_POS)
{
}

CmdSendUpdateCueBallPosition::~CmdSendUpdateCueBallPosition()
{
}

void CmdSendUpdateCueBallPosition::putData(ps::ExtMath::vector pos, bool submit)
{
	putInt(gameMgr->_ebpCtrl->getGameId());
	putInt(0);
	putInt(0);
	putDouble(pos.x);
	putDouble(pos.y);
	putDouble(pos.z);
	putByte(submit ? 1 : 0);
}

CmdSendBet::CmdSendBet()
	:BaseOutMsg(PacketID::BET_GOLD)
{
}

CmdSendBet::~CmdSendBet()
{
}

void CmdSendBet::packData(int gameId, int gold)
{
	putInt(gameId);
	putInt(gold);
}

CmdSendReady::CmdSendReady()
	:BaseOutMsg(PacketID::PLAYER_READY)
{
}

CmdSendReady::~CmdSendReady()
{
}

CmdRematch::CmdRematch()
	:BaseOutMsg(PacketID::REMATCH)
{
}

CmdRematch::~CmdRematch()
{
}

CmdLeaveGame::CmdLeaveGame()
	:BaseOutMsg(PacketID::LEAVE_GAME)
{
}

CmdLeaveGame::~CmdLeaveGame()
{
}

CmdCallPocket::CmdCallPocket()
	:BaseOutMsg(PacketID::CALL_POCKET)
{
}

CmdCallPocket::~CmdCallPocket()
{
}

void CmdCallPocket::packData(int pocketId)
{
	putInt(pocketId);
}

CmdActiveExtension::CmdActiveExtension()
	:BaseOutMsg(PacketID::CALL_EXTENSION)
{
}

CmdActiveExtension::~CmdActiveExtension()
{
}

CmdEndTurn::CmdEndTurn()
	:BaseOutMsg(PacketID::END_TURN)
{
}

CmdEndTurn::~CmdEndTurn()
{
}

CmdFakeEndGame::CmdFakeEndGame()
	:BaseOutMsg(PacketID::FAKE_END_GAME)
{
}

CmdFakeEndGame::~CmdFakeEndGame()
{
}

CmdSwitchCue::CmdSwitchCue()
	:BaseOutMsg(PacketID::SWITCH_CUE)
{
}

CmdSwitchCue::~CmdSwitchCue()
{
}

void CmdSwitchCue::packData(int cueId)
{
	putInt(cueId);
}

CmdSetBall::CmdSetBall()
	:BaseOutMsg(PacketID::SET_BALL)
{
}

CmdSetBall::~CmdSetBall()
{
}

void CmdSetBall::packData(int ballId, ps::ExtMath::vector ballPos, bool submit)
{
	putInt(ballId);
	putDouble(ballPos.x);
	putDouble(ballPos.y);
	putDouble(ballPos.z);
	putByte(submit ? 1 : 0);
}

CmdStartCheat::CmdStartCheat()
	:BaseOutMsg(PacketID::START_CHEAT)
{
}

CmdStartCheat::~CmdStartCheat()
{
}

CmdEndCheat::CmdEndCheat()
:BaseOutMsg(PacketID::END_CHEAT)
{
}

CmdEndCheat::~CmdEndCheat()
{
}

CmdFakeClientResult::CmdFakeClientResult()
	:BaseOutMsg(PacketID::SEND_FAKE_RESULT_CLIENT)
{

}

CmdFakeClientResult::~CmdFakeClientResult()
{

}


void CmdFakeClientResult::packData(bool foul, bool isSwitchPlayer, const std::vector<PocketBallData> & pocketedBall, const std::vector<BallBody *> *activeBalls)
{
	putByte(foul);
	putByte(isSwitchPlayer);
	
	putInt(pocketedBall.size());
	for (int i = 0; i < pocketedBall.size(); i++)
	{
		putByte(pocketedBall[i].number);
		putByte(pocketedBall[i].pocketId);
	}

	putInt(activeBalls->size());
	for (int i = 0; i < activeBalls->size(); i++)
	{
		auto ballBody = activeBalls->at(i);
		putByte(ballBody->id());
		putDouble(ballBody->position().x);
		putDouble(ballBody->position().y);
		putDouble(ballBody->position().z);
	}
}

CmdSelectKeoBuaDao::CmdSelectKeoBuaDao()
	:BaseOutMsg(PacketID::PICK_KEOBUABAO)
{

}

CmdSelectKeoBuaDao::~CmdSelectKeoBuaDao()
{

}

void CmdSelectKeoBuaDao::packData(int cardId)
{
	putInt(cardId);
}

CmdSendFakeShoot::CmdSendFakeShoot()
	:BaseOutMsg(PacketID::SEND_FAKE_SHOOT)
{

}

CmdSendFakeShoot::~CmdSendFakeShoot()
{

}

void CmdSendFakeShoot::packData(const ShootData & shot)
{
	putDouble(shot.direction.x);
	putDouble(shot.direction.y);
	putDouble(shot.direction.z);
	putDouble(shot.offsetX);
	putDouble(shot.offsetY);
	putDouble(shot.force);
}
