#ifndef PaymentInPacket_h_
#define PaymentInPacket_h_

#include "../base/BasePacket.h"
#include <data/DataStruct.h>
#include "../../core/shop/ShopItemData.h"

class RequestSMSZaloInPacket
	:public BaseInPacket
{
public:
	RequestSMSZaloInPacket(fr::InPacket *pk);
	~RequestSMSZaloInPacket();

	void readData();

public:
	int returnCode;
	std::string transId;
	std::string smsSyntax;
	std::string servicePhone;
};

class BuyGoldByRubyInPacket
	:public BaseInPacket
{
public:
	BuyGoldByRubyInPacket(fr::InPacket *pk);
	~BuyGoldByRubyInPacket();

	void readData();
};

class ValidateGoogleReceiptInPacket
	:public BaseInPacket
{
public:
	ValidateGoogleReceiptInPacket(fr::InPacket *pk);
	~ValidateGoogleReceiptInPacket();

	void readData();
public:
	std::string purchaseData;
	std::string signature;
};


class ValidateAppleReceiptInPacket
	:public BaseInPacket
{
public:
	ValidateAppleReceiptInPacket(fr::InPacket *pk);
	~ValidateAppleReceiptInPacket();

	void readData();
public:public:
	std::vector<std::string> listProductId;
};

class RequestZingCardChargeInPacket
	:public BaseInPacket
{
public:
	RequestZingCardChargeInPacket(fr::InPacket *pk);
	~RequestZingCardChargeInPacket();

	void readData();
public:
	int returnCode;
	std::string returnMessage;
	std::string zmpTransID;
};

class PaymentChargeSuccessInPacket
	:public BaseInPacket
{
public:
	PaymentChargeSuccessInPacket(fr::InPacket *pk);
	~PaymentChargeSuccessInPacket();

	void readData();
public:
	int itemType;
	int itemNum;
	std::string itemId;
};

class RequestChargeOfThailandInPacket
	:public BaseInPacket
{
public:
	RequestChargeOfThailandInPacket(fr::InPacket *pk);
	~RequestChargeOfThailandInPacket();

	void readData();
public:
	std::string transId;
};

class EnableLocalPaymentInPacket
	:public BaseInPacket
{
public:
	EnableLocalPaymentInPacket(fr::InPacket *pk);
	~EnableLocalPaymentInPacket();

	void readData();
public:
	bool enable;
};

class UpdateShopDataInPacket
	:public BaseInPacket
{
public:
	UpdateShopDataInPacket(fr::InPacket *pk);
	~UpdateShopDataInPacket();

	void readData();
public:
	std::vector<std::string> chargedPackId;
	std::vector<HotDealData> listHotDeals;
};

class BuyCueInPacket
	:public BaseInPacket
{
public:
	BuyCueInPacket(fr::InPacket *pk);
	~BuyCueInPacket();

	void readData();
};

class BuyGoldByCashInPacket
	:public BaseInPacket
{
public:
	BuyGoldByCashInPacket(fr::InPacket *pk);
	~BuyGoldByCashInPacket();

	void readData();
public:
	long newGold;
	long newCash;
};

class BuyCueBoxByCashInPacket
	:public BaseInPacket
{
public:
	BuyCueBoxByCashInPacket(fr::InPacket *pk);
	~BuyCueBoxByCashInPacket();

	void readData();
public:
	int cueItemIdx;
	int newCash;
};

class BuyHotDealInPacket
	:public BaseInPacket
{
public:
	BuyHotDealInPacket(fr::InPacket *pk);
	~BuyHotDealInPacket();

	void readData();
public:
	int hotDealIdx;
	int cueItemIdx;
	long newGold;
	long newCash;
};

class ResetHotDealInPacket
	:public BaseInPacket
{
public:
	ResetHotDealInPacket(fr::InPacket *pk);
	~ResetHotDealInPacket();

	void readData();
public:
	long serverTime;
	std::vector<HotDealData> listHotDeals;
};
#endif