#ifndef MiniGamePacketBuilder_h_
#define MiniGamePacketBuilder_h_

#include "../base/OutPacketBuilder.h"

class MiniGamePacketBuilder
	:pk::OutPacketBuilder
{
public:
	MiniGamePacketBuilder();
	~MiniGamePacketBuilder();

	fr::OutPacket * createOutPacket(int cmdId, int controllerId);
};
#endif // !CuePacketBuilder_h_
