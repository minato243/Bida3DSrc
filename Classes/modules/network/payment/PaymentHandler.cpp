#include "PaymentHandler.h"
#include "PaymentPacketBuilder.h"
#include "PaymentInPacket.h"
#include "../../core/payment/PaymentMgr.h"
#include "../../core/shop/ShopMgr.h"
#include "../base/PacketPool.h"
#include "PaymentOutMsg.h"
#include "core/GameMgr.h"
#include "lobby/network/GameClient.h"
#include "../../ui/shop/ShopPopupResult.h"

using namespace pk;


PaymentHandler::PaymentHandler()
	:BaseHandler()
{
	auto pmBuilder = new PaymentPacketBuilder();
}


PaymentHandler::~PaymentHandler()
{
}

bool PaymentHandler::onReceived(fr::InPacket* pk)
{
	CCLOG("InventoryHandler::onReceived %d", pk->getCmdId());
	int cmd = pk->getCmdId();
	int controllerID = pk->getControllerId();

	switch (cmd)
	{
	case PacketID::REQUEST_SMS_ZALO:
	{
		CCLOG("PaymentHandler::onReceived -------------- REQUEST_SMS_ZALO");
		auto msg = new RequestSMSZaloInPacket(pk);
		msg->readData();
		paymentMgr->receiveSMSZalo(msg);
		CC_SAFE_DELETE(msg);
		break;
	}
	case PacketID::VALIDATE_APPLE_RECEIPT:
	{
		CCLOG("PaymentHandler::onReceived -------------- VALIDATE_APPLE_RECEIPT");
		auto msg = new ValidateAppleReceiptInPacket(pk);
		msg->readData();
		paymentMgr->receiveValidateAppleReceipt(msg);
		CC_SAFE_DELETE(msg);
		break;
	}
		
	case PacketID::VALIDATE_GOOGLE_RECEIPT:
	{
		CCLOG("PaymentHandler::onReceived -------------- VALIDATE_GOOGLE_RECEIPT");
		auto msg = new ValidateGoogleReceiptInPacket(pk);
		msg->readData();
		paymentMgr->receiveValidateGoogleReceipt(msg);
		CC_SAFE_DELETE(msg);
		break;
	}
		
	case PacketID::REQUEST_CHARGE_TELO_ZALO:
	{
		CCLOG("PaymentHandler::onReceived -------------- REQUEST_CHARGE_TELO_ZALO");
		auto msg = new RequestSMSZaloInPacket(pk);
		msg->readData();
		paymentMgr->receiveChargeTelcoZalo(msg);
		CC_SAFE_DELETE(msg);
		break;
	}
		
	case PacketID::RESPONSE_PAYMENT_CHARGE_SUCCESS:
	{
		CCLOG("PaymentHandler::onReceived -------------- RESPONSE_PAYMENT_CHARGE_SUCCESS");
		auto msg = new PaymentChargeSuccessInPacket(pk);
		msg->readData();
		paymentMgr->receiveChargeSuccess(msg);
		CC_SAFE_DELETE(msg);
		break;
	}
		
	case PacketID::REQUEST_CHARGE_OF_THAILAND:
	{
		CCLOG("PaymentHandler::onReceived -------------- REQUEST_CHARGE_OF_THAILAND");
		auto msg = new RequestChargeOfThailandInPacket(pk);
		msg->readData();
		paymentMgr->receiveChargeOfThailan(msg);
		CC_SAFE_DELETE(msg);

		break;
	}
		
	case PacketID::RESPONSE_ENABLE_LOCALPAYMENT:
	{
		CCLOG("PaymentHandler::onReceived -------------- RESPONSE_ENABLE_LOCALPAYMENT");
		auto msg = new EnableLocalPaymentInPacket(pk);
		msg->readData();
		paymentMgr->onReceiveEnablePayment(msg);
		CC_SAFE_DELETE(msg);
		break;
	}
		
	case PacketID::UPDATE_SHOP_DATA:
	{
		CCLOG("PaymentHandler::onReceived -------------- UPDATE_SHOP_DATA");
		auto msg = new UpdateShopDataInPacket(pk);
		shopMgr->syncServerData(msg);
		CC_SAFE_DELETE(msg);
		break;
	}
		
	case PacketID::RESET_HOT_DEALS:
	{
		CCLOG("PaymentHandler::onReceived -------------- RESET_HOT_DEALS");
		auto msg = new ResetHotDealInPacket(pk);
		msg->readData();
		reloadGuiShop(msg);
		CC_SAFE_DELETE(msg);
		break;
	}

	case PacketID::BUY_GOLD_BY_CASH:
	{
		CCLOG("PaymentHandler::onReceived -------------- BUY_GOLD_BY_CASH");
		auto msg = new BuyGoldByCashInPacket(pk);
		if(msg->error == 0)
			msg->readData();
		shopMgr->onResponeBuyGold(msg);
		CC_SAFE_DELETE(msg);
		break;
	}

	case PacketID::BUY_CUE_BOX_BUY_CASH:
	{
		CCLOG("PaymentHandler::onReceived -------------- BUY_CUE_BOX_BUY_CASH");
		auto msg = new BuyCueBoxByCashInPacket(pk);
		if (msg->error == 0) 
		{
			msg->readData();
			shopMgr->onResponeBuyCueBox(msg);
		}
		else
		{
			auto popup = (ShopPopupResult*)guiMgr->getGui(Popup::POPUP_RESULT);
			popup->noticeError(SHOP_CATEGORY_CASH);
			popup->show();
		}
			
		CC_SAFE_DELETE(msg);
		break;
	}

	default:
		return false;
	}

	return true;
}

void PaymentHandler::reloadGuiShop(ResetHotDealInPacket *msg)
{

}

void PaymentHandler::waitForShopData()
{

}

void PaymentHandler::buyShopCue(int id)
{

}

void PaymentHandler::buyGoldByCash(int id)
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::BUY_GOLD_BY_CASH);
	auto msg = dynamic_cast<CmdSendBuyGoldByCash *>(pk);
	msg->packData(id);
	send(msg);
}

void PaymentHandler::buyCueBoxByCash(int boxId)
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::BUY_CUE_BOX_BUY_CASH);
	auto msg = dynamic_cast<CmdSendBuyCueBoxByCash *>(pk);
	msg->packData(boxId);
	send(msg);
}

void PaymentHandler::buyHotDeal(int hotDealIdx)
{
	auto pk = packetPool->getOrCreateOutPacket(PacketID::BUY_HOT_DEAL);
	auto msg = dynamic_cast<CmdSendBuyHotDeal *>(pk);
	msg->packData(hotDealIdx);
	send(msg);
}

void PaymentHandler::sendRequestSMSZalo(std::string mNetworkOperator, int amount, std::string item)
{

}

void PaymentHandler::sendRequestZingCardCharge(std::string seri, std::string code, int paymentType, std::string item)
{

}

void PaymentHandler::sendRequestChargeOfThailan(int userId, std::string cardNo, std::string pinCode, int paymentType, bool isVip, std::string itemInfo)
{

}

void PaymentHandler::sendValidateGoogleReceipt(std::string purchaseData, std::string signature, std::string key, std::string packageName)
{

}

void PaymentHandler::sendValidateAppleReceipt(std::string receipt, std::string store, std::string deviceInfo)
{

}

PaymentHandler *PaymentHandler::_instance = nullptr;

PaymentHandler * PaymentHandler::getInstance()
{
	if (!_instance)
	{
		_instance = new PaymentHandler();
	}
	return _instance;
}
