#ifndef OfferPacketBuilder_h_
#define OfferPacketBuilder_h_

#include "../base/OutPacketBuilder.h"

class OfferPacketBuilder
	:pk::OutPacketBuilder
{
public:
	OfferPacketBuilder();
	~OfferPacketBuilder();

	fr::OutPacket * createOutPacket(int cmdId, int controllerId);
};
#endif // !CuePacketBuilder_h_
