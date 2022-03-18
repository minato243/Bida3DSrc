#ifndef OfferInPacket_h_
#define OfferInPacket_h_

#include "../base/BasePacket.h"

class PurchaseOfferMsg
	:public BaseInPacket
{
public:
	PurchaseOfferMsg(fr::InPacket *pk);
	~PurchaseOfferMsg();

	void readData();

public:
	int packId;
};


class OfferInfoMsg
	:public BaseInPacket
{
public:
	OfferInfoMsg(fr::InPacket *pk);
	~OfferInfoMsg();

	void readData();

public:
	int offerID;
	int packId;
	long long remainTime;

};

class OpenCumulChestMsg
	:public BaseInPacket
{
public:
	OpenCumulChestMsg(fr::InPacket *pk);
	~OpenCumulChestMsg();

	void readData();

};

class CumulOfferDataMsg
	:public BaseInPacket
{
public:
	CumulOfferDataMsg(fr::InPacket *pk);
	~CumulOfferDataMsg();

	void readData();

public:
	int point;
	int level;
};

class CheatOfferCumulativeMsg
	:public BaseInPacket
{
public:
	CheatOfferCumulativeMsg(fr::InPacket *pk);
	~CheatOfferCumulativeMsg();

	void readData();
};
#endif
