#include "InventoryHandler.h"
#include "../PacketIDDefine.h"
#include "InventoryOutMsg.h"
#include "../base/PacketPool.h"
#include "../../../lobby/network/GameClient.h"
#include "InventoryInPacket.h"
#include "../../core/inventory/InventoryMgr.h"
#include "../cue/CueOutMsg.h"
#include "../cue/CueInPacket.h"

using namespace pk;

InventoryHandler::InventoryHandler()
	:BaseHandler()
{
	auto inventoryBuilder = new InventoryPacketBuilder();
}

InventoryHandler::~InventoryHandler()
{

}

bool InventoryHandler::onReceived(fr::InPacket* pk)
{
	CCLOG("InventoryHandler::onReceived %d", pk->getCmdId());
	int cmd = pk->getCmdId();
	int controllerID = pk->getControllerId();

	switch (cmd)
	{
	case PacketID::SET_CUE_AUTO_CHARGE:
	{
		CCLOG("InventoryHandler::onReceived -------------- SET_CUE_AUTO_CHARGE");
		SetCueAutoChargeInPacket *msg = new SetCueAutoChargeInPacket(pk);
		msg->readData();
		if (msg->error == 0)
		{
			inventoryMgr->updateCueAutoRecharge(msg->cueId, msg->on);
		}
		CC_SAFE_DELETE(msg);
		
		break;
	}

	case PacketID::ADD_CUE_TO_BAG:
	{
		CCLOG("InventoryHandler::onReceived -------------- ADD_CUE_TO_BAG");
		
		AddCueToBagInPacket *msg = new AddCueToBagInPacket(pk);
		msg->readData();
		if (msg->error == 0)
		{
			inventoryMgr->addCueToBag(msg->id);
		}
		CC_SAFE_DELETE(msg);
		break;
	}

	case PacketID::RM_CUE_FROM_BAG:
	{
		CCLOG("InventoryHandler::onReceived -------------- RM_CUE_FROM_BAG");
		RmCueFromBagInPacket *msg = new RmCueFromBagInPacket(pk);
		msg->readData();
		if (msg->error == 0)
		{
			inventoryMgr->removeCueFromBag(msg->id);
		}
		CC_SAFE_DELETE(msg);
		break;
	}

	case PacketID::UPDATE_INVENTORY_INFO:
	{
		CCLOG("InventoryHandler::onReceived -------------- UPDATE_INVENTORY_INFO");
		UpdateInventoryInfo *msg = new UpdateInventoryInfo(pk);
		msg->readData();
		if (msg->error == 0)
		{
			inventoryMgr->onReceiveInventoryInfo(msg);
		}
		CC_SAFE_DELETE(msg);
		break;
	}

	default:
		return false;
	}

	return true;
}

void InventoryHandler::requestAddCueToBag(int id)
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::ADD_CUE_TO_BAG);
	auto msg = dynamic_cast<CmdAddCueToBag *>(pk);
	msg->packData(id);
	send(msg);
}

void InventoryHandler::requestRemoveCueFromBag(int id)
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::RM_CUE_FROM_BAG);
	auto msg = dynamic_cast<CmdRmCueFromBag *>(pk);
	msg->packData(id);
	send(msg);
}

void InventoryHandler::requestAutoChargeCue(int id, bool value)
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::SET_CUE_AUTO_CHARGE);
	auto msg = dynamic_cast<CmdSetAutoChargeCue*>(pk);
	msg->packData(id, value);
	send(pk);
}

void InventoryHandler::getInventoryInfo()
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::UPDATE_INVENTORY_INFO);
	send(pk);
}

InventoryHandler * InventoryHandler::_instance = nullptr;
InventoryHandler * InventoryHandler::getInstance()
{
	if (!_instance)
	{
		_instance = new InventoryHandler();
	}
	return _instance;
}

