#ifndef OUT_PACKET_BUILDER_H_
#define OUT_PACKET_BUILDER_H_

#include "BaseFramework.h"
#include <modules/network/PacketIDDefine.h>

namespace pk
{
	class OutPacketBuilder
	{
	public:
		OutPacketBuilder();
		~OutPacketBuilder();

		virtual fr::OutPacket* createOutPacket(int cmdId, int controllerId) = 0;
	};

};

#endif // !OUT_PACKET_BUILDER_H_


