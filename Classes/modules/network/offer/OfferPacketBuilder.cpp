#include "OfferPacketBuilder.h"
#include "OfferOutMsg.h"

using namespace pk;

OfferPacketBuilder::OfferPacketBuilder()
	:OutPacketBuilder()
{

}

OfferPacketBuilder::~OfferPacketBuilder()
{

}

fr::OutPacket * OfferPacketBuilder::createOutPacket(int cmdId, int controllerId)
{
	switch (cmdId)
	{
	case CHEAT_SEND_OFFER_INFO:
	{
		auto msg = new CmdSendCheatOffer();
		return msg;
	}

	case PURCHASE_OFFER:
	{
		auto msg = new CmdPurchaseOffer();
		return msg;
	}

	case OPEN_CUMUL_CHEST:
	{
		auto msg = new CmdSendOpenChestOffer();
		return msg;
	}

	case CHEAT_OFFER_CUMULATIVE:
	{
		auto msg = new CmdSendCheatOfferCumulative();
		return msg;
	}
		
	}

	return NULL;
}

