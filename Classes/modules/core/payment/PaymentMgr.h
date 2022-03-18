#ifndef PaymentMgr_h_
#define PaymentMgr_h_

#include <data/DataStruct.h>
#include <data/GameConstant.h>
#include "../../network/payment/PaymentInPacket.h"

#define paymentMgr PaymentMgr::getInstance()

class PaymentMgr
{
public:
	PaymentMgr();
	~PaymentMgr();

	void init();

	void purchaseSMS(int price, int itemId);

	void sendRequestSMSZalo(int price, std::string mNetworkOperator, int itemId);

	void receiveSMSZalo(RequestSMSZaloInPacket *data);

	void receiveValidateAppleReceipt(ValidateAppleReceiptInPacket *msg);

	void receiveValidateGoogleReceipt(ValidateGoogleReceiptInPacket *msg);

	void receiveChargeTelcoZalo(RequestSMSZaloInPacket *msg);

	void receiveChargeSuccess(PaymentChargeSuccessInPacket *msg);

	void receiveChargeOfThailan(RequestChargeOfThailandInPacket* msg);
	
	void onReceiveEnablePayment(EnableLocalPaymentInPacket *msg);

private:
	static PaymentMgr *_isntance;

public:
	static PaymentMgr *getInstance();
};



#endif