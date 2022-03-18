#include "MiniGamePacketBuilder.h"
#include "../PacketIDDefine.h"
#include "MiniGameOutMsg.h"

using namespace pk;

MiniGamePacketBuilder::MiniGamePacketBuilder()
	:OutPacketBuilder()
{

}

MiniGamePacketBuilder::~MiniGamePacketBuilder()
{

}

fr::OutPacket * MiniGamePacketBuilder::createOutPacket(int cmdId, int controllerId)
{
	switch (cmdId)
	{
	case SPIN_WHEEL:
	{
		auto msg = new CmdSendSpinWheel();
		return msg;
	}

	case CHEAT_SPIN:
	{
		auto msg = new CmdSendCheatSpin();
		return msg;
	}
	}

	return NULL;
}

