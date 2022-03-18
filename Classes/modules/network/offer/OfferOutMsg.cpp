#include "OfferOutMsg.h"

using namespace pk;

CmdPurchaseOffer::CmdPurchaseOffer()
	:BaseOutMsg(PacketID::PURCHASE_OFFER)
{

}

CmdPurchaseOffer::~CmdPurchaseOffer()
{

}

void CmdPurchaseOffer::packData(int offerId)
{
	putInt(offerId);
}

CmdSendCheatOffer::CmdSendCheatOffer()
	:BaseOutMsg(PacketID::CHEAT_SEND_OFFER_INFO)
{

}

CmdSendCheatOffer::~CmdSendCheatOffer()
{

}

void CmdSendCheatOffer::packData(int offerId)
{
	putInt(offerId);
}

CmdSendOpenChestOffer::CmdSendOpenChestOffer()
	:BaseOutMsg(OPEN_CUMUL_CHEST)
{

}

CmdSendOpenChestOffer::~CmdSendOpenChestOffer()
{

}

void CmdSendOpenChestOffer::packData(int level)
{
	putInt(level);
}

CmdSendCheatOfferCumulative::CmdSendCheatOfferCumulative()
	:BaseOutMsg(CHEAT_OFFER_CUMULATIVE)
{

}

CmdSendCheatOfferCumulative::~CmdSendCheatOfferCumulative()
{

}

void CmdSendCheatOfferCumulative::packData(int level, int point)
{
	putInt(level);
	putInt(point);
}
