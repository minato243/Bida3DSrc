#ifndef InventoryPacketBuilder_h_
#define InventoryPacketBuilder_h_

#include "../base/OutPacketBuilder.h"


class InventoryPacketBuilder
	:public pk::OutPacketBuilder
{
public:
	InventoryPacketBuilder();
	~InventoryPacketBuilder();

	fr::OutPacket * createOutPacket(int cmdId, int controllerId);
};

#endif
