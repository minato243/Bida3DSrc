#ifndef INGAME_PACKET_BUILDER_H
#define INGAME_PACKET_BUILDER_H

#include "../base/OutPacketBuilder.h"


class IngamePacketBuilder
	:public pk::OutPacketBuilder
{
public:
	IngamePacketBuilder();
	~IngamePacketBuilder();

	fr::OutPacket * createOutPacket(int cmdId, int controllerId);
};

#endif // !INGAME_PACKET_BUILDER_H



