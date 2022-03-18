#include "PaymentPacketBuilder.h"
#include "PaymentOutMsg.h"

using namespace pk;

PaymentPacketBuilder::PaymentPacketBuilder()
	:OutPacketBuilder()
{

}

PaymentPacketBuilder::~PaymentPacketBuilder()
{

}

fr::OutPacket * PaymentPacketBuilder::createOutPacket(int cmdId, int controllerId)
{
	switch (cmdId)
	{
	case PacketID::REQUEST_SMS_ZALO:
	{
		auto msg = new CmdSendRequestSMSZalo();
		return msg;
	}

	case PacketID::BUY_GOLD_BY_RUBY:
	{
		auto msg = new CmdSendBuyGoldByRuby();
		return msg;
	}

	case PacketID::VALIDATE_GOOGLE_RECEIPT:
	{
		auto msg = new CmdSendValidateGoogleReceipt();
		return msg;
	}
	case PacketID::VALIDATE_APPLE_RECEIPT:
	{
		auto msg = new CmdSendValidateAppleReceipt();
		return msg;
	}

	case PacketID::REQUEST_CHARGE_TELO_ZALO:
	{
		auto msg = new CmdSendRequestZingCardCharge();
		return msg;
	}

	case PacketID::REQUEST_CHARGE_OF_THAILAND:
	{
		auto msg = new CmdSendRequestChargeOfThailand();
		return msg;
	}

	case PacketID::REQUEST_BUY_FIRST_CHARGE_OFFER:
	{
		auto msg = new CmdSendBuyFirstChargeOffer();
		return msg;
	}

	case PacketID::BUY_CUE:
	{
		auto msg = new CmdSendBuyCue();
		return msg;
	}

	case PacketID::BUY_GOLD_BY_CASH:
	{
		auto msg = new CmdSendBuyGoldByCash();
		return msg;
	}

	case PacketID::BUY_CUE_BOX_BUY_CASH:
	{
		auto msg = new CmdSendBuyCueBoxByCash();
		return msg;
	}

	case PacketID::BUY_HOT_DEAL:
	{
		auto msg = new CmdSendBuyHotDeal();
		return msg;
	}

	default:
		break;
	}
	return nullptr;
}

