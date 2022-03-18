#include "OutPacketBuilder.h"
#include "PacketPool.h"

using namespace pk;

OutPacketBuilder::OutPacketBuilder()
{
	packetPool->addPacketBuilder(this);
}

OutPacketBuilder::~OutPacketBuilder()
{
}
