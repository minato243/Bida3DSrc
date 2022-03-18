#ifndef CuePacketBuilder_h_
#define CuePacketBuilder_h_

#include "../base/OutPacketBuilder.h"

class CuePacketBuilder
	:public pk::OutPacketBuilder
{
public:
	CuePacketBuilder();
	~CuePacketBuilder();

	fr::OutPacket * createOutPacket(int cmdId, int controllerId);
protected:
private:
};
#endif