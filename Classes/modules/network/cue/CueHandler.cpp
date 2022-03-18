#include "CueHandler.h"
#include "../PacketIDDefine.h"
#include "../base/PacketPool.h"
#include "CueOutMsg.h"
#include "CueInPacket.h"
#include "../../core/inventory/InventoryMgr.h"
#include "CuePacketBuilder.h"
#include "../../../lobby/network/GameClient.h"

using namespace pk;

CueHandler::CueHandler()
{
	auto builder = new CuePacketBuilder();
}

CueHandler::~CueHandler()
{

}

bool CueHandler::onReceived(fr::InPacket* pk)
{
	CCLOG("CueHandler::onReceived %d", pk->getCmdId());
	int cmd = pk->getCmdId();
	int controllerID = pk->getControllerId();

	switch (cmd)
	{
	case PacketID::CREATE_CUE:
	{
		CCLOG("CueHandler::onReceived -------------- CREATE_CUE");
		CreateCueInPacket *msg = new CreateCueInPacket(pk);
		msg->readData();
		if (msg->error == 0)
		{
			inventoryMgr->onReceiveNewCue(msg->cue);
		}
		CC_SAFE_DELETE(msg);
		break;
	}
	
	case PacketID::UPGRADE_CUE:
	{
		CCLOG("CueHandler::onReceived -------------- UPGRADE_CUE");
		UpgradeCueInPacket *msg = new UpgradeCueInPacket(pk);
		msg->readData();
		if (msg->error == 0)
		{
			//TODO
				
		}
		CC_SAFE_DELETE(msg);
		break;
	}

	default:
		return false;
	}

	return true;
}

void CueHandler::requestCreateCue(int type)
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::CREATE_CUE);
	auto msg = dynamic_cast<CmdCreateCue *>(pk);
	if (msg)
	{
		msg->packData(type);
		send(msg);
	}
}

void CueHandler::requestRenameCue(int id, std::string name)
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::CHANGE_EXTRA_CUE_NAME);
	auto msg = dynamic_cast<CmdChangeCueName *>(pk);
	if (msg)
	{
		msg->packData(id, name);
		send(msg);
	}
}

void CueHandler::requestChargeCue(int id)
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::RESET_CUE_BATTERY);
	auto msg = dynamic_cast<CmdChargeCueBattery *>(pk);
	if (msg)
	{
		msg->packData(id);
		send(msg);
	}
}

void CueHandler::requestDeleteCue(int id)
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::DELETE_CUE);
	auto msg = dynamic_cast<CmdDeleteCue *>(pk);
	if (msg)
	{
		msg->packData(id);
		send(msg);
	}
}

void CueHandler::requestAccumulateDust(int cueId, int dust)
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::ACCUMULATE_DUST);
	auto msg = dynamic_cast<CmdAccumulateDust *>(pk);
	if (msg)
	{
		msg->packData(cueId, dust);
		send(msg);
	}
}

void CueHandler::requestBuyCueBox(int type, int quantity)
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::BUY_CUE_BOX);
	auto msg = dynamic_cast<CmdBuyCueBox *>(pk);
	if (msg)
	{
		msg->packData(type, quantity);
		send(msg);
	}
}

void CueHandler::requestCheatInventoryItem(int type, int quantity)
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::CHEAT_SET_INVENTORY_ITEM);
	auto msg = dynamic_cast<CmdCheatSetInventoryItem *>(pk);
	if (msg)
	{
		msg->packData(type, quantity);
		send(msg);
	}
}

void CueHandler::requestUpgradeCue(int cueId)
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::UPGRADE_CUE);
	auto msg = dynamic_cast<CmdSendUpgradeCue *>(pk);
	if (msg)
	{
		msg->packData(cueId);
		send(msg);
	}
}

void CueHandler::requestWorkshopUpgradeCue(int cueId, const std::vector<MatData> & mats)
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::WORKSHOP_UPGRADE_CUE);
	auto msg = dynamic_cast<CmdSendWorkshopUpgradeCue *>(pk);
	if (msg)
	{
		msg->packData(cueId, mats);
		send(msg);
	}
}

void CueHandler::requestWorkshopFusionCues(const int* cueIds)
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::WORKSHOP_FUSION_CUES);
	auto msg = dynamic_cast<CmdSendWorkshopFusionCues *>(pk);
	if (msg)
	{
		msg->packData(cueIds);
		send(msg);
	}
}

void CueHandler::requestWorkshopSwapCues(int src, int dst)
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::WORKSHOP_SWAP_CUES);
	auto msg = dynamic_cast<CmdSendWorkshopSwapCues *>(pk);
	if (msg)
	{
		msg->packData(src, dst);
		send(msg);
	}
}

CueHandler * CueHandler::_instance = nullptr;

CueHandler * CueHandler::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new CueHandler();
	}
	return _instance;
}

