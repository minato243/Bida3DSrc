#include "CheatHandler.h"
#include "../base/PacketPool.h"
#include "CheatOutMsg.h"
#include "../game/IngameOutMsg.h"
#include "CheatPacketBuilder.h"
#include "../../ui/GuiMgr.h"
#include "../../scene/GameScene.h"
#include "core/GameMgr.h"

using namespace pk;

CheatHandler::CheatHandler()
	:BaseHandler()
{
	_builder = new CheatPacketBuilder();
}

CheatHandler::~CheatHandler()
{
	CC_SAFE_DELETE(_builder);
}

bool CheatHandler::onReceived(fr::InPacket* pk)
{
	CCLOG("CheatHandler::onReceived %d", pk->getCmdId());
	int cmd = pk->getCmdId();
	int controllerID = pk->getControllerId();

	switch (cmd)
	{
	case PacketID::START_CHEAT:
	{
		CCLOG("CheatHandler::onReceived -------------- START_CHEAT");
		break;
	}

	case PacketID::END_CHEAT:
	{
		CCLOG("CheatHandler::onReceived -------------- END_CHEAT");
		break;
	}
	case PacketID::CHEAT_CUE_INFO:
	{
		CCLOG("CheatHandler::onReceived -------------- CHEAT_CUE_INFO");
		break;
	}

	case PacketID::CHEAT_SET_GOLD:
	{
		CCLOG("CheatHandler::onReceived -------------- CHEAT_SET_GOLD");
		break;
	}

	case PacketID::CHEAT_SET_CASH:
	{
		CCLOG("CheatHandler::onReceived -------------- CHEAT_SET_CASH");
		break;
	}

	case PacketID::CHEAT_PLAY_WITH_BOT:
	{
		CCLOG("CheatHandler::onReceived -------------- CHEAT_PLAY_WITH_BOT");
		if (pk->getError() != 0)
		{
			guiMgr->showMessagePopup("Cannot join new game", NULL);
			guiMgr->getGui(Popup::FIND_MATCH_LAYER)->hide();
			auto gameScene = (GameScene *)gameMgr->_scene;
			gameScene->toLobby();
		}
		break;
	}

	default:
		return false;
	}
	return true;
}

void CheatHandler::requestStartCheat()
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::START_CHEAT);
	auto msg = dynamic_cast<CmdStartCheat *>(pk);
	send(msg);
}

void CheatHandler::requestEndCheat()
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::END_CHEAT);
	auto msg = dynamic_cast<CmdEndCheat *>(pk);
	send(msg);
}

void CheatHandler::requestCheatCueInfo(int type, int star, int point, int battery)
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::CHEAT_CUE_INFO);
	auto msg = dynamic_cast<CmdCheatCueInfo *>(pk);
	msg->packData(type, star, point, battery);
	send(msg);
}

void CheatHandler::requestCheatAddCueInfo(std::vector<CueInfoData> &list)
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::CHEAT_ADD_CUE_INFO);
	auto msg = dynamic_cast<CmdCheatAddCueInfo *>(pk);
	msg->packData(list);
	send(msg);
}

void CheatHandler::requestCheatGold(int gold)
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::CHEAT_SET_GOLD);
	auto msg = dynamic_cast<CmdCheatGold *>(pk);
	msg->packData(gold);
	send(msg);
}

void CheatHandler::requestCheatCash(int cash)
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::CHEAT_SET_CASH);
	auto msg = dynamic_cast<CmdCheatCash *>(pk);
	msg->packData(cash);
	send(msg);
}

void CheatHandler::requestCheatS2Result(int channel, bool win)
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::CHEAT_S2_FIRST_MATCH);
	auto msg = dynamic_cast<CmdCheatS2Result *>(pk);
	msg->packData(channel, win);
	send(msg);
}

void CheatHandler::requestCheatNewChallenges(std::vector<ChallengeData> &chanllenges)
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::CHEAT_NEW_SPONSORSHIP_CHALLENGES);
	auto msg = dynamic_cast<CmdCheatNewChallenges *>(pk);
	msg->packData(chanllenges);
	send(msg);
}

void CheatHandler::requestFinishSponChallenge(int id)
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::CHEAT_COMPLETE_SPONSORSHIP_CHALLENGE);
	auto msg = dynamic_cast<CmdCheatFinishSponChallenge *>(pk);
	msg->packData(id);
	send(msg);
}

void CheatHandler::requestCheatPlayWithBot(int mode, double accuracy)
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::CHEAT_PLAY_WITH_BOT);
	auto msg = dynamic_cast<CmdCheatPlayWithBot *>(pk);
	msg->packData(mode, accuracy);
	send(msg);
}

void CheatHandler::requestCheatClearInventory()
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::CHEAT_CLEAR_INVENTORY);
	auto msg = dynamic_cast<CmdCheatClearInventory *>(pk);
	send(msg);
}

CheatHandler * CheatHandler::_instance;

CheatHandler * CheatHandler::getInstance()
{
	if (_instance == nullptr) 
	{
		_instance = new CheatHandler();
	}
	return _instance;
}

void CheatHandler::destroyInstance()
{
	CC_SAFE_DELETE(_instance);
}

