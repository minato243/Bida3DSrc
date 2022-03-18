#ifndef OfferOutMsg_h_
#define OfferOutMsg_h_

#include "../base/BasePacket.h"

class CmdPurchaseOffer
	:public BaseOutMsg
{
public:
	CmdPurchaseOffer();
	~CmdPurchaseOffer();

	void packData(int offerId);
};

class CmdSendCheatOffer
	:public BaseOutMsg
{
public:
	CmdSendCheatOffer();
	~CmdSendCheatOffer();

	void packData(int offerId);
};

class CmdSendOpenChestOffer
	:public BaseOutMsg
{
public:
	CmdSendOpenChestOffer();
	~CmdSendOpenChestOffer();

	void packData(int level);
};

class CmdSendCheatOfferCumulative
	:public BaseOutMsg
{
public:
	CmdSendCheatOfferCumulative();
	~CmdSendCheatOfferCumulative();

	void packData(int level, int point);
};

#endif