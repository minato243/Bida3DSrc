#include "IngamePacketBuilder.h"
#include "IngameOutMsg.h"

using namespace pk;

IngamePacketBuilder::IngamePacketBuilder()
	:OutPacketBuilder()
{
}

IngamePacketBuilder::~IngamePacketBuilder()
{
}

fr::OutPacket * IngamePacketBuilder::createOutPacket(int cmdId, int controllerId)
{
	switch (cmdId)
	{
	case PacketID::UPDATE_CUE_DIR:
	{
		auto msg = new CmdSendUpdateCueDirection();
		return msg;
	}
	case PacketID::PICK_KEOBUABAO:
	{
		auto msg = new CmdSelectKeoBuaDao();
		return msg;
	}
	case PacketID::SWITCH_CUE:
	{
		auto msg = new CmdSwitchCue();
		return msg;
	}
	default:
		break;
	}
	return nullptr;
}
