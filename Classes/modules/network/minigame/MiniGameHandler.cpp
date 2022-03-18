#include "MiniGameHandler.h"
#include "../base/PacketPool.h"
#include "MiniGameOutMsg.h"
#include "MiniGameInPacket.h"
#include "../../core/minigame/spin/SpinMgr.h"
#include "MiniGamePacketBuilder.h"

using namespace pk;

MiniGameHandler::MiniGameHandler()
	:BaseHandler()
{
	auto builder = new MiniGamePacketBuilder();
}

MiniGameHandler::~MiniGameHandler()
{

}

bool MiniGameHandler::onReceived(fr::InPacket* pk)
{
	int cmd = pk->getCmdId();
	int controllerID = pk->getControllerId();

	switch (cmd)
	{
	case SPIN_INFO:
	{
		CCLOG("MiniGameHandler::onReceived SPIN_INFO");
		SpinInfoMsg *msg = new SpinInfoMsg(pk);
		
		if (msg->error == 0)
		{
			msg->readData();
			spinMgr->setNumberOfSpin(msg->numberOfSpin);
		}
		CC_SAFE_DELETE(msg);
		break;
	}

	case SPIN_DATA:
	{
		CCLOG("MiniGameHandler::onReceived SPIN_INFO");
		SpinDataMsg *msg = new SpinDataMsg(pk);
		if (msg->error == 0)
		{
			msg->readData();
			spinMgr->onResponeSpinWheel(msg->cellId);
		}
	}
	
	default:
		return false;
		break;
	}
	return true;
}

void MiniGameHandler::sendSpinWheel()
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::SPIN_WHEEL);
	auto msg = dynamic_cast<CmdSendSpinWheel *>(pk);
	msg->packData();
	send(msg);
}

void MiniGameHandler::sendCheatSpin()
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::CHEAT_SPIN);
	auto msg = dynamic_cast<CmdSendCheatSpin *>(pk);
	msg->packData();
	send(msg);
}

MiniGameHandler * MiniGameHandler::_instance = NULL;

MiniGameHandler * MiniGameHandler::getInstance()
{
	if (_instance == NULL)
	{
		_instance = new MiniGameHandler();
	}
	return _instance;
}

