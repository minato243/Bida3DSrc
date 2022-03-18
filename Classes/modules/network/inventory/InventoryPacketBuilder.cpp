#include "InventoryPacketBuilder.h"
#include "InventoryOutMsg.h"

using namespace pk;

InventoryPacketBuilder::InventoryPacketBuilder()
	:OutPacketBuilder()
{

}

InventoryPacketBuilder::~InventoryPacketBuilder()
{

}

fr::OutPacket * InventoryPacketBuilder::createOutPacket(int cmdId, int controllerId)
{
	switch (cmdId)
	{
	case PacketID::ADD_CUE_TO_BAG:
	{
		auto msg = new CmdAddCueToBag();
		return msg;
	}

	case PacketID::RM_CUE_FROM_BAG:
	{
		auto msg = new CmdRmCueFromBag();
		return msg;
	}

	default:
		break;
	}
	return nullptr;
}
