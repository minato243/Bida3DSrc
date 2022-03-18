#ifndef CheatPacketBuilder_h_
#define CheatPacketBuilder_h_

#include "../base/OutPacketBuilder.h"


class CheatPacketBuilder
	:public pk::OutPacketBuilder
{
public:
	CheatPacketBuilder();
	~CheatPacketBuilder();

	fr::OutPacket * createOutPacket(int cmdId, int controllerId);
};

#endif
