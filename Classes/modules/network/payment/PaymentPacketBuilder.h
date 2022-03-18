#ifndef PaymentPacketBuilder_h_
#define PaymentPacketBuilder_h_

#include "../base/OutPacketBuilder.h"


class PaymentPacketBuilder
	:public pk::OutPacketBuilder
{
public:
	PaymentPacketBuilder();
	~PaymentPacketBuilder();

	fr::OutPacket * createOutPacket(int cmdId, int controllerId);
};

#endif
