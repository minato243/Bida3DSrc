#include "CuePacketBuilder.h"
#include "CueOutMsg.h"

using namespace pk;

CuePacketBuilder::CuePacketBuilder()
	:pk::OutPacketBuilder()

{

}

CuePacketBuilder::~CuePacketBuilder()
{

}

fr::OutPacket * CuePacketBuilder::createOutPacket(int cmdId, int controllerId)
{
	switch (cmdId)
	{
	case PacketID::CREATE_CUE:
	{
		auto msg = new CmdCreateCue();
		return msg;
	}

	case PacketID::CHANGE_EXTRA_CUE_NAME:
	{
		auto msg = new CmdChangeCueName();
		return msg;
	}

	case PacketID::RESET_CUE_BATTERY:
	{
		auto msg = new CmdChargeCueBattery();
		return msg;
	}

	case PacketID::DELETE_CUE:
	{
		auto msg = new CmdDeleteCue();
		return msg;
	}

	case PacketID::BUY_CUE_BOX:
	{
		auto msg = new CmdBuyCueBox();
		return msg;
	}

	case PacketID::CHEAT_SET_INVENTORY_ITEM:
	{
		auto msg = new CmdCheatSetInventoryItem();
		return msg;
	}

	case PacketID::UPGRADE_CUE:
	{
		auto msg = new CmdSendUpgradeCue();
		return msg;
	}

	case PacketID::WORKSHOP_UPGRADE_CUE:
	{
		auto msg = new CmdSendWorkshopUpgradeCue();
		return msg;
	}

	case PacketID::WORKSHOP_FUSION_CUES:
	{
		auto msg = new CmdSendWorkshopFusionCues();
		return msg;
	}

	case PacketID::WORKSHOP_SWAP_CUES:
	{
		auto msg = new CmdSendWorkshopFusionCues();
		return msg;
	}

	case PacketID::SET_CUE_AUTO_CHARGE:
	{
		auto msg = new CmdSetAutoChargeCue();
		return msg;
	}

	default:
		break;
	}
	return nullptr;
}
