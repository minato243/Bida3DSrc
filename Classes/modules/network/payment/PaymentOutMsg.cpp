#include "PaymentOutMsg.h"

using namespace pk;
CmdSendRequestSMSZalo::CmdSendRequestSMSZalo()
	:BaseOutMsg(PacketID::REQUEST_SMS_ZALO)
{

}

CmdSendRequestSMSZalo::~CmdSendRequestSMSZalo()
{

}

void CmdSendRequestSMSZalo::packData(std::string mNetworkOperator, std::string amount, std::string item)
{
	putString(mNetworkOperator);
	putString(amount);
	putString(item);
}

CmdSendBuyGoldByRuby::CmdSendBuyGoldByRuby()
	:BaseOutMsg(PacketID::BUY_GOLD_BY_RUBY)
{

}

CmdSendBuyGoldByRuby::~CmdSendBuyGoldByRuby()
{

}

void CmdSendBuyGoldByRuby::packData(int type)
{
	putByte(type);
}

CmdSendValidateGoogleReceipt::CmdSendValidateGoogleReceipt()
	:BaseOutMsg(PacketID::VALIDATE_GOOGLE_RECEIPT)
{

}

CmdSendValidateGoogleReceipt::~CmdSendValidateGoogleReceipt()
{

}

void CmdSendValidateGoogleReceipt::packData(std::string purchaseData, std::string signature, std::string key, std::string packageName)
{
	putString(purchaseData);
	putString(signature);
	putString(key);
	putString(packageName);
}

CmdSendValidateAppleReceipt::CmdSendValidateAppleReceipt()
	:BaseOutMsg(PacketID::VALIDATE_APPLE_RECEIPT)
{

}

CmdSendValidateAppleReceipt::~CmdSendValidateAppleReceipt()
{

}

void CmdSendValidateAppleReceipt::packData(std::string receipt, std::string store, std::string deviceInfo)
{
	putString(receipt);
	putString(store);
	putString(deviceInfo);
}

CmdSendRequestZingCardCharge::CmdSendRequestZingCardCharge()
	:BaseOutMsg(PacketID::REQUEST_CHARGE_TELO_ZALO)
{

}

CmdSendRequestZingCardCharge::~CmdSendRequestZingCardCharge()
{

}

void CmdSendRequestZingCardCharge::packData(std::string seri, std::string code, int type, std::string item)
{
	putString(seri);
	putString(code);
	putInt(type);
	putString(item);
}

CmdSendRequestChargeOfThailand::CmdSendRequestChargeOfThailand()
	:BaseOutMsg(PacketID::REQUEST_CHARGE_OF_THAILAND)
{

}

CmdSendRequestChargeOfThailand::~CmdSendRequestChargeOfThailand()
{

}

void CmdSendRequestChargeOfThailand::packData(int userId, std::string cardNo, std::string pinCode, int paymentType, bool isVip, std::string itemInfo)
{
	putInt(userId);
	putString(pinCode);
	putInt(paymentType);
	putByte(isVip);
	putString(itemInfo);
}

CmdSendBuyFirstChargeOffer::CmdSendBuyFirstChargeOffer()
	:BaseOutMsg(PacketID::REQUEST_BUY_FIRST_CHARGE_OFFER)
{

}

CmdSendBuyFirstChargeOffer::~CmdSendBuyFirstChargeOffer()
{

}

void CmdSendBuyFirstChargeOffer::packData()
{

}

CmdSendBuyCue::CmdSendBuyCue()
	:BaseOutMsg(PacketID::BUY_CUE)
{

}

CmdSendBuyCue::~CmdSendBuyCue()
{

}

void CmdSendBuyCue::packData(int itemId)
{
	putInt(itemId);
}

CmdSendBuyGoldByCash::CmdSendBuyGoldByCash()
	:BaseOutMsg(PacketID::BUY_GOLD_BY_CASH)
{

}

CmdSendBuyGoldByCash::~CmdSendBuyGoldByCash()
{

}

void CmdSendBuyGoldByCash::packData(int itemId)
{
	putInt(itemId);
}

CmdSendBuyCueBoxByCash::CmdSendBuyCueBoxByCash()
	:BaseOutMsg(PacketID::BUY_CUE_BOX_BUY_CASH)
{

}

CmdSendBuyCueBoxByCash::~CmdSendBuyCueBoxByCash()
{

}

void CmdSendBuyCueBoxByCash::packData(int boxId)
{
	putInt(boxId);
}

CmdSendBuyHotDeal::CmdSendBuyHotDeal()
	:BaseOutMsg(PacketID::BUY_HOT_DEAL)
{

}

CmdSendBuyHotDeal::~CmdSendBuyHotDeal()
{

}

void CmdSendBuyHotDeal::packData(int hotDealIdx)
{
	putInt(hotDealIdx);
}
