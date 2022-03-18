#include "GameHandler.h"
#include <core/GameMgr.h>
#include <modules/core/interaction/InteractionManager.h>
#include "../base/PacketPool.h"
#include <core/utils/Utility.h>
#include "lobby/network/GameClient.h"

using namespace pk;

GameHandler::GameHandler()
	:BaseHandler()
{
	auto builder = new IngamePacketBuilder();
}

GameHandler::~GameHandler()
{
}

bool GameHandler::onReceived(fr::InPacket * pk)
{
	CCLOG("GameHandler::onReceived %d", pk->getCmdId());
	int cmd = pk->getCmdId();
	int controllerID = pk->getControllerId();

	switch (cmd)
	{
	case PacketID::START_MATCH:
	{
		CCLOG("GameHandler::onReceived -------------- START_MATCH");
		StartMatchMsg *msg = new StartMatchMsg(pk);
		msg->readData();
		if (msg->error == 0)
		{
			gameMgr->_ebpCtrl->onReceiveStartingMatchPacket(msg);
		}
		CC_SAFE_DELETE(msg);
		break;
	}
	case PacketID::SHOOT:
	{
		CCLOG("GameHandler::onReceived -------------- SHOOT");
		ShootMsg *msg = new ShootMsg(pk);
		msg->readData();
		if (msg->error == 0)
		{
			gameMgr->_ebpCtrl->receiveShoot(msg->shot);
		}
		
		CC_SAFE_DELETE(msg);
		break;
	}

	case PacketID::SEND_FAKE_SHOOT:
	{
		CCLOG("GameHandler::onReceived -------------- SEND_FAKE_SHOOT");
		ShootMsg *msg = new ShootMsg(pk);
		msg->readData();
		if (msg->error == 0)
		{
			gameMgr->_ebpCtrl->receiveShoot(msg->shot);
		}

		CC_SAFE_DELETE(msg);
		break;
	}

	case PacketID::END_GAME:
	{
		CCLOG("GameHandler::onReceived -------------- END_GAME TODO");
		EndGameMsg *msg = new EndGameMsg(pk);
		msg->readData();
		if (msg->error == 0)
		{
			gameMgr->_ebpCtrl->onEndGame(msg->isWinner);
		}
		break;
	}

	case PacketID::PLAYER_READY:
	{
		CCLOG("GameHandler::onReceived -------------- PLAYER_READY TODO");
		gameMgr->_ebpCtrl->opponentReady();
		break;
	}

	case PacketID::CALL_EXTENSION:
	{
		CCLOG("GameHandler::onReceived -------------- CALL_EXTENSION TODO");
		break;
	}

	case PacketID::REMATCH:
	{
		CCLOG("GameHandler::onReceived -------------- REMATCH TODO");
		break;
	}

	case PacketID::CALL_POCKET:
	{
		CCLOG("GameHandler::onReceived -------------- REMATCH TODO");

		break;
	}

	case PacketID::SET_BALL:
	{
		CCLOG("GameHandler::onReceived -------------- SET_BALL");
		SetBallMsg *msg = new SetBallMsg(pk);

		if (msg->error == 0) {
			msg->readData();
			CCLOG("onReceived SetBallMsg ballId = %d, pos(%f, %f, %f)",
				msg->ballId, msg->ballPos.x, msg->ballPos.y, msg->ballPos.z);
			char text[200];
			sprintf(text, "onReceived SetBallMsg ballId = %d, pos(%f, %f, %f)\n",
				msg->ballId, msg->ballPos.x, msg->ballPos.y, msg->ballPos.z);
			Utility::writeToFile(("Debug_" + std::to_string(gameMgr->_userInfo->_uId)).c_str(), text);
			if (msg->submit) {
				gameMgr->_ebpCtrl->receiveSetBall(msg->ballId, msg->ballPos);
			}
			interactionMgr->receiveCueBallPosition(msg->ballPos);
		}
		else {
			guiMgr->notifyDebugText("DEBUG: " + msg->getStatus());
		}
		break;
	}

	case PacketID::SHOOT_RESULT:
	{
		CCLOG("GameHandler::onReceived -------------- SHOOT_RESULT TODO");
		ShotResultMsg *msg = new ShotResultMsg(pk);
		msg->readData();
		if (msg->error == 14) {
			gameMgr->_ebpCtrl->receiveTimeOutTurn();
			break;
		}
		if (msg->error != 0) {
			guiMgr->notifyDebugText("DEBUG: " + msg->getVNStatus());
		}
		gameMgr->_ebpCtrl->setShootResult(msg);
		CC_SAFE_DELETE(msg);
		break;
	}

	case PacketID::UPDATE_CUE_DIR:
	{
		CCLOG("GameHandler::onReceived -------------- UPDATE_CUE_DIR TODO");
		UpdateCueDirMsg *msg = new UpdateCueDirMsg(pk);
		msg->readData();
		if (msg->error == 0)
		{
			interactionMgr->receiveCueDirection(msg->direction);
		}
		CC_SAFE_DELETE(msg);
		break;
	}

	case PacketID::UPDATE_CUE_BALL_POS:
	{
		CCLOG("GameHandler::onReceived -------------- UPDATE_CUE_BALL_POS TODO");
		break;
	}

	case PacketID::START_TURN:
	{
		CCLOG("GameHandler::onReceived -------------- START_TURN");
		StartTurnMsg *msg = new StartTurnMsg(pk);
		msg->readData();
		CCLOG("PacketID::START_TURN %d %s", msg->curTurnPlayerId, msg->isPlacingCueBall ? "true" : "false");
		if (msg->error == 0)
		{
			gameMgr->_ebpCtrl->receiveStartTurn(msg->curTurnPlayerId, msg->isPlacingCueBall);
		}
		CC_SAFE_DELETE(msg);
		break;
	}

	case PacketID::NEW_CUE_CHALLENGE:
	{
		CCLOG("GameHandler::onReceived -------------- NEW_CUE_CHALLENGE TODO");
		break;
	}

	case PacketID::SWITCH_CUE:
	{
		CCLOG("GameHandler::onReceived -------------- SWITCH_CUE TODO");
		SwitchCueMsg *msg = new SwitchCueMsg(pk);
		msg->readData();
		if (msg->error == 0)
		{
			gameMgr->_ebpCtrl->receiveSwitchCue(msg);
		}
		CC_SAFE_DELETE(msg);
		break;
	}

	case PacketID::START_CHEAT:
	{
		break;
	}

	case PacketID::END_CHEAT:
	{
		break;
	}

	case PacketID::OPEN_CARD:
	{
		break;
	}

	case PacketID::REVEAL_CARD:
	{
		
		break;
	}

	case PacketID::START_CARD_OPENING:
	{
		break;
	}

	case PacketID::SEND_PING_FIRST_TURN_STATE:
	{
		CCLOG("GameHandler::onReceived -------------- SEND_PING_FIRST_TURN_STATE");
		gameMgr->_ebpCtrl->receiveStartMatchingUI();
		break;
	}

	case PacketID::SEND_PICK_FIRST_TURN_STATE:
	{
		CCLOG("GameHandler::onReceived -------------- SEND_PICK_FIRST_TURN_STATE");
		PickFirstTurnStateMsg *msg = new PickFirstTurnStateMsg(pk);
		msg->readData();
		if (msg->error == 0)
		{
			gameMgr->_ebpCtrl->onSelectPickFirstTurn(msg);
		}
		
		CC_SAFE_DELETE(msg);
		break;
	}

	case PacketID::PLAYER_DISCONNECT:
	{
		CCLOG("GameHandler::onReceived -------------- PLAYER_DISCONNECT");
		PlayerDisconnectMsg *msg = new PlayerDisconnectMsg(pk);
		msg->readData();
		if (msg->error == 0)
		{
			gameMgr->_ebpCtrl->onReceiveDisconnect(msg);
		}

		CC_SAFE_DELETE(msg);
		break;
	}
		
	case PacketID::PLAYER_RECONNECT:
	{
		CCLOG("GameHandler::onReceived -------------- PLAYER_RECONNECT");
		PlayerReconnectMsg * msg = new PlayerReconnectMsg(pk);
		msg->readData();
		if (msg->error == 0)
		{
			gameMgr->_ebpCtrl->onReceiveReconnect(msg);
		}

		CC_SAFE_DELETE(msg);
		break;
	}

	default:
		return false;
	}
	return true;
}

void GameHandler::sendShoot(const ShootData & shot)
{
	if (!FAKE_CLIENT_RESULT)
	{
		auto *msg = new CmdSendShoot();
		msg->packData(shot);
		send(msg);
		CC_SAFE_DELETE(msg);
	}
	else
	{
		auto *msg = new CmdSendFakeShoot();
		msg->packData(shot);
		send(msg);
		CC_SAFE_DELETE(msg);
	}
}

void GameHandler::sendSetBall(int ballId, ps::ExtMath::vector ballPos, bool submit)
{
	CCLOG("sendSetBall ballId %d, ballPos(%f %f %f)", ballId, ballPos.x, ballPos.y, ballPos.z);
	auto *msg = new CmdSetBall();
	msg->packData(ballId, ballPos, submit);
	send(msg);
	CC_SAFE_DELETE(msg);
}

void GameHandler::sendUpdateCueDirection(const ps::ExtMath::vector & direction, const double & offsetX, const double & offsetY)
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::UPDATE_CUE_DIR);
	auto msg = dynamic_cast<CmdSendUpdateCueDirection *>(pk);
	msg->packData(direction, offsetX, offsetY);
	send(msg);
}

void GameHandler::sendEndTurn()
{
	CCLOG("GameHandler::sendEndTurn ---------------------");
	auto pk = packetPool->getOrCreateOutPacket(PacketID::END_TURN);
	send(pk);
}

void GameHandler::sendCallPocket(int pocketId)
{
	CCLOG("GameHandler::sendCallPocket ---------------------");
	auto pk = (CmdCallPocket *)packetPool->getOrCreateOutPacket(PacketID::CALL_POCKET);
	pk->packData(pocketId);
	send(pk);
}

void GameHandler::sendFakeResultClient(bool foul, bool isSwitchPlayer, const std::vector<PocketBallData> & pocketedBall, const std::vector<BallBody *> * activeBalls)
{
	auto pk = (CmdFakeClientResult *)packetPool->getOrCreateOutPacket(PacketID::SEND_FAKE_RESULT_CLIENT);
	pk->packData(foul, isSwitchPlayer, pocketedBall, activeBalls);
	send(pk);
}

void GameHandler::sendPickKeoBuabao(int cardId)
{
	auto pk = (CmdSelectKeoBuaDao *) packetPool->getOrCreateOutPacket(PacketID::PICK_KEOBUABAO);
	pk->packData(cardId);
	send(pk);
}

void GameHandler::sendSwitchCue(int cueId)
{
	auto pk = (CmdSwitchCue *)packetPool->getOrCreateOutPacket(PacketID::SWITCH_CUE);
	pk->packData(cueId);
	send(pk);
}

void GameHandler::sendFakeEndGame()
{
	auto pk = (CmdFakeEndGame *)packetPool->getOrCreateOutPacket(PacketID::FAKE_END_GAME);
	send(pk);
}

GameHandler * GameHandler::_instance = nullptr;
GameHandler * GameHandler::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new GameHandler();
	}
	return _instance;
}
