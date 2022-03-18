#include "PaymentInPacket.h"
#include "../../../data/GameConstant.h"

RequestSMSZaloInPacket::RequestSMSZaloInPacket(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

RequestSMSZaloInPacket::~RequestSMSZaloInPacket()
{

}

void RequestSMSZaloInPacket::readData()
{
	returnCode = _pk->getInt();
	transId = _pk->getString();
	smsSyntax = _pk->getString();
	servicePhone = _pk->getString();
}

BuyGoldByRubyInPacket::BuyGoldByRubyInPacket(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

BuyGoldByRubyInPacket::~BuyGoldByRubyInPacket()
{

}

void BuyGoldByRubyInPacket::readData()
{

}

ValidateGoogleReceiptInPacket::ValidateGoogleReceiptInPacket(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

ValidateGoogleReceiptInPacket::~ValidateGoogleReceiptInPacket()
{

}

void ValidateGoogleReceiptInPacket::readData()
{
	purchaseData = _pk->getString();
	signature = _pk->getString();
}

ValidateAppleReceiptInPacket::ValidateAppleReceiptInPacket(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

ValidateAppleReceiptInPacket::~ValidateAppleReceiptInPacket()
{

}

void ValidateAppleReceiptInPacket::readData()
{
	auto length = _pk->getInt();
	listProductId.clear();
	for (auto i = 0; i < length; i++) {
		listProductId.push_back(_pk->getString());
	}
}

RequestZingCardChargeInPacket::RequestZingCardChargeInPacket(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

RequestZingCardChargeInPacket::~RequestZingCardChargeInPacket()
{

}

void RequestZingCardChargeInPacket::readData()
{
	returnCode = _pk->getInt();
	returnMessage = _pk->getString();
	zmpTransID = _pk->getString();
}

PaymentChargeSuccessInPacket::PaymentChargeSuccessInPacket(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

PaymentChargeSuccessInPacket::~PaymentChargeSuccessInPacket()
{

}

void PaymentChargeSuccessInPacket::readData()
{
	itemType = _pk->getInt();
	itemNum = _pk->getLong();
	itemId = _pk->getString();
}

RequestChargeOfThailandInPacket::RequestChargeOfThailandInPacket(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

RequestChargeOfThailandInPacket::~RequestChargeOfThailandInPacket()
{

}

void RequestChargeOfThailandInPacket::readData()
{
	transId = _pk->getString();
}

EnableLocalPaymentInPacket::EnableLocalPaymentInPacket(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

EnableLocalPaymentInPacket::~EnableLocalPaymentInPacket()
{

}

void EnableLocalPaymentInPacket::readData()
{
	enable = _pk->getByte();
}

UpdateShopDataInPacket::UpdateShopDataInPacket(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

UpdateShopDataInPacket::~UpdateShopDataInPacket()
{

}

void UpdateShopDataInPacket::readData()
{
	auto chargedPacksNum = _pk->getInt();
	chargedPackId.clear();
	for (auto i = 0; i < chargedPacksNum; i++) {
		chargedPackId.push_back(_pk->getString());
	}
	listHotDeals.clear();
	for (auto i = 0; i < 6; i++) {
		HotDealData item;
		item.hotDealType = _pk->getString();
		item.rate = _pk->getInt();
		item.cost = _pk->getInt();
		item.discount = _pk->getInt();
		item.isSold = _pk->getBool();
		item.category = ShopTabId::HOT_DEALS;
		item.hotDealIdx = i;
		if (i < 3)
			item.typeCost = "gold";
		else item.typeCost = "cash";
		listHotDeals.push_back(item);
	}
}

BuyCueInPacket::BuyCueInPacket(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

BuyCueInPacket::~BuyCueInPacket()
{

}

void BuyCueInPacket::readData()
{

}

BuyGoldByCashInPacket::BuyGoldByCashInPacket(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

BuyGoldByCashInPacket::~BuyGoldByCashInPacket()
{

}

void BuyGoldByCashInPacket::readData()
{
	newGold = _pk->getLong();
	newCash = _pk->getLong();
}

BuyCueBoxByCashInPacket::BuyCueBoxByCashInPacket(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

BuyCueBoxByCashInPacket::~BuyCueBoxByCashInPacket()
{

}

void BuyCueBoxByCashInPacket::readData()
{
	cueItemIdx = _pk->getInt();
	newCash =_pk->getLong();
}

BuyHotDealInPacket::BuyHotDealInPacket(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

BuyHotDealInPacket::~BuyHotDealInPacket()
{

}

void BuyHotDealInPacket::readData()
{
	hotDealIdx = _pk->getInt();
	cueItemIdx = _pk->getInt();
	newGold = _pk->getLong();
	newCash =_pk->getLong();
}

ResetHotDealInPacket::ResetHotDealInPacket(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

ResetHotDealInPacket::~ResetHotDealInPacket()
{

}

void ResetHotDealInPacket::readData()
{
	serverTime = _pk->getLong();
	listHotDeals.clear();
	for (auto i = 0; i < 6; i++) {
		HotDealData item = {};
		item.hotDealType = _pk->getString();
		item.rate = _pk->getInt();
		item.cost = _pk->getInt();
		item.discount = _pk->getInt();
		item.isSold = _pk->getBool();
		item.category = ShopTabId::HOT_DEALS;
		item.hotDealIdx = i;
		if (i < 3)
			item.typeCost = "gold";
		else item.typeCost = "cash";
		listHotDeals.push_back(item);
	}
}
