#ifndef PaymentOutMsg_h_
#define PaymentOutMsg_h_

#include <BaseFramework.h>
#include "../base/BasePacket.h"
#include <data/DataStruct.h>

class CmdSendRequestSMSZalo
	:public BaseOutMsg
{
public:
	CmdSendRequestSMSZalo();
	~CmdSendRequestSMSZalo();

	void packData(std::string mNetworkOperator, std::string amount, std::string item);
};

class CmdSendBuyGoldByRuby
	:public BaseOutMsg
{
public:
	CmdSendBuyGoldByRuby();
	~CmdSendBuyGoldByRuby();

	void packData(int type);
};

class CmdSendValidateGoogleReceipt
	:public BaseOutMsg
{
public:
	CmdSendValidateGoogleReceipt();
	~CmdSendValidateGoogleReceipt();

	void packData(std::string purchaseData, std::string signature, std::string key, std::string packageName);
};

class CmdSendValidateAppleReceipt
	:public BaseOutMsg
{
public:
	CmdSendValidateAppleReceipt();
	~CmdSendValidateAppleReceipt();

	void packData(std::string receipt, std::string store, std::string deviceInfo);
};


class CmdSendRequestZingCardCharge
	:public BaseOutMsg
{
public:
	CmdSendRequestZingCardCharge();
	~CmdSendRequestZingCardCharge();

	void packData(std::string seri, std::string code,int type, std::string item);
};


class CmdSendRequestChargeOfThailand
	:public BaseOutMsg
{
public:
	CmdSendRequestChargeOfThailand();
	~CmdSendRequestChargeOfThailand();

	void packData(int userId, std::string cardNo, std::string pinCode, int paymentType, bool isVip, std::string itemInfo);
};

class CmdSendBuyFirstChargeOffer
	:public BaseOutMsg
{
public:
	CmdSendBuyFirstChargeOffer();
	~CmdSendBuyFirstChargeOffer();

	void packData();
};

class CmdSendBuyCue
	:public BaseOutMsg
{
public:
	CmdSendBuyCue();
	~CmdSendBuyCue();

	void packData(int itemId);
};

class CmdSendBuyGoldByCash
	:public BaseOutMsg
{
public:
	CmdSendBuyGoldByCash();
	~CmdSendBuyGoldByCash();

	void packData(int itemId);
};

class CmdSendBuyCueBoxByCash
	:public BaseOutMsg
{
public:
	CmdSendBuyCueBoxByCash();
	~CmdSendBuyCueBoxByCash();

	void packData(int boxId);
};

class CmdSendBuyHotDeal
	:public BaseOutMsg
{
public:
	CmdSendBuyHotDeal();
	~CmdSendBuyHotDeal();

	void packData(int hotDealIdx);
};

#endif