#include "PaymentMgr.h"


PaymentMgr::PaymentMgr()
{
}

PaymentMgr::~PaymentMgr()
{
}

void PaymentMgr::init()
{

}

void PaymentMgr::purchaseSMS(int price, int itemId)
{

}

void PaymentMgr::sendRequestSMSZalo(int price, std::string mNetworkOperator, int itemId)
{

}

void PaymentMgr::receiveSMSZalo(RequestSMSZaloInPacket *data)
{
	//TODO
}

void PaymentMgr::receiveValidateAppleReceipt(ValidateAppleReceiptInPacket *msg)
{
	//TODO
}

void PaymentMgr::receiveValidateGoogleReceipt(ValidateGoogleReceiptInPacket *msg)
{
	//TODO
}

void PaymentMgr::receiveChargeTelcoZalo(RequestSMSZaloInPacket *msg)
{
	//TODO
}

void PaymentMgr::receiveChargeSuccess(PaymentChargeSuccessInPacket *msg)
{
	//TODO
}

void PaymentMgr::receiveChargeOfThailan(RequestChargeOfThailandInPacket* msg)
{
	//TODO
}

void PaymentMgr::onReceiveEnablePayment(EnableLocalPaymentInPacket *msg)
{
	//TODO
}

PaymentMgr * PaymentMgr::_isntance = nullptr;

PaymentMgr * PaymentMgr::getInstance()
{
	if (_isntance == nullptr)
	{
		_isntance = new PaymentMgr();
	}
	return _isntance;
}
