#ifndef PaymentHandler_h_
#define PaymentHandler_h_

#include <BaseFramework.h>
#include <data/DataStruct.h>
#include "modules/network/base/BaseHandler.h"
#include "PaymentInPacket.h"

#define paymentHandler PaymentHandler::getInstance()

class PaymentHandler
	:public BaseHandler
{
public:
	PaymentHandler();
	~PaymentHandler();

	bool onReceived(fr::InPacket* pk);

	void reloadGuiShop(ResetHotDealInPacket *msg);

	void waitForShopData();

	void buyShopCue(int id);

	void buyGoldByCash(int id);

	void buyCueBoxByCash(int boxId);

	void buyHotDeal(int hotDealIdx);

	void sendRequestSMSZalo(std::string mNetworkOperator, int amount, std::string item);

	void sendRequestZingCardCharge(std::string seri, std::string code, int paymentType, std::string item);

	void sendRequestChargeOfThailan(int userId, std::string cardNo, std::string pinCode, int paymentType, bool isVip, std::string itemInfo);

	void sendValidateGoogleReceipt(std::string purchaseData, std::string signature, std::string  key, std::string  packageName);

	void sendValidateAppleReceipt(std::string receipt, std::string store, std::string deviceInfo);

private:
	static PaymentHandler *_instance;

public:
	static PaymentHandler *getInstance();
};


#endif