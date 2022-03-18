#include "OfferHandler.h"
#include "../PacketIDDefine.h"
#include "../../core/offer/OfferMgr.h"
#include "../base/PacketPool.h"
#include "OfferOutMsg.h"
#include "OfferPacketBuilder.h"
#include "OfferInPacket.h"
#include "../../ui/GuiMgr.h"
#include "../../core/offer/OfferEntity.h"

using namespace pk;

OfferHandler::OfferHandler()
	:BaseHandler()
{
	auto builder = new OfferPacketBuilder();
}

OfferHandler::~OfferHandler()
{

}

OfferHandler * OfferHandler::getInstance()
{
	if (_instance == NULL)
	{
		_instance = new OfferHandler();
	}

	return _instance;
}

bool OfferHandler::onReceived(fr::InPacket* pk)
{
	int cmd = pk->getCmdId();
	int controllerID = pk->getControllerId();

	switch (cmd)
	{
	case PacketID::OFFER_INFO:
	{
		CCLOG("OfferHandler::onReceived OFFER_INFO");
		OfferInfoMsg *msg = new OfferInfoMsg(pk);
		msg->readData();
		if (msg->error == 0)
		{
			offerMgr->addOfferData((OfferType)msg->offerID, msg->packId, msg->remainTime);
		}
		CC_SAFE_DELETE(msg);
		break;
	}
	case PacketID::CUMUL_OFFER_DATA:
	{
		CCLOG("OfferHandler::onReceived CUMUL_OFFER_DATA");
		CumulOfferDataMsg *msg = new CumulOfferDataMsg(pk);
		msg->readData();
		if (msg->error == 0)
		{
			offerMgr->addOfferCumulative(msg->level, msg->point);
		}
		CC_SAFE_DELETE(msg);
		break;
	}

	case PURCHASE_OFFER:
	{
		CCLOG("OfferHandler::onReceived PURCHASE_OFFER");
		PurchaseOfferMsg *msg = new PurchaseOfferMsg(pk);
		msg->readData();
		if (msg->error == 0)
		{
			offerMgr->onResponePurchase((OfferType)msg->packId);
		}
		else
		{
			guiMgr->notifyDebugText("purchase fail!");
		}
		CC_SAFE_DELETE(msg);
		break;
	}

	case OPEN_CUMUL_CHEST:
	{
		CCLOG("OfferHandler::onReceived OPEN_CUMUL_CHEST");
		OpenCumulChestMsg *msg = new OpenCumulChestMsg(pk);
		msg->readData();
		if (msg->error == 0)
		{
			offerMgr->onResponeOpenChestOffer();
		}
		else
		{
			guiMgr->notifyDebugText("purchase fail!");
		}
		CC_SAFE_DELETE(msg);
		break;
	}
	default:
		return false;
		break;
	}
	return true;
}

void OfferHandler::sendPurchaseOffer(int offerId)
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::PURCHASE_OFFER);
	auto msg = dynamic_cast<CmdPurchaseOffer *>(pk);
	msg->packData(offerId);
	send(msg);
}

void OfferHandler::sendCheatOffer(int offerId)
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::CHEAT_SEND_OFFER_INFO);
	auto msg = dynamic_cast<CmdSendCheatOffer *>(pk);
	msg->packData(offerId);
	send(msg);
}

void OfferHandler::sendOpenChestOffer(int level)
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::OPEN_CUMUL_CHEST);
	auto msg = dynamic_cast<CmdSendOpenChestOffer *>(pk);
	msg->packData(level);
	send(msg);
}

void OfferHandler::sendCheatOfferCumulative(int level, int point)
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::CHEAT_OFFER_CUMULATIVE);
	auto msg = dynamic_cast<CmdSendCheatOfferCumulative *>(pk);
	msg->packData(level, point);
	send(msg);
}

OfferHandler * OfferHandler::_instance = NULL;

