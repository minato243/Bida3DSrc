#include "OfferInPacket.h"

PurchaseOfferMsg::PurchaseOfferMsg(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

PurchaseOfferMsg::~PurchaseOfferMsg()
{

}

void PurchaseOfferMsg::readData()
{
	if (error == 0)
	{
		packId = _pk->getInt();
	}
}


OfferInfoMsg::OfferInfoMsg(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

OfferInfoMsg::~OfferInfoMsg()
{

}

void OfferInfoMsg::readData()
{
	error = _pk->getInt();
	offerID = _pk->getInt();
	packId = _pk->getInt();
	remainTime = _pk->getLong();
}

OpenCumulChestMsg::OpenCumulChestMsg(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

OpenCumulChestMsg::~OpenCumulChestMsg()
{

}

void OpenCumulChestMsg::readData()
{
	
}

CumulOfferDataMsg::CumulOfferDataMsg(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

CumulOfferDataMsg::~CumulOfferDataMsg()
{

}

void CumulOfferDataMsg::readData()
{
	point = _pk->getInt();
	level = _pk->getInt();
}

CheatOfferCumulativeMsg::CheatOfferCumulativeMsg(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

CheatOfferCumulativeMsg::~CheatOfferCumulativeMsg()
{

}

void CheatOfferCumulativeMsg::readData()
{

}
