#ifndef PACKET_POOL_H_
#define PACKET_POOL_H_

#include "BaseFramework.h"
#include <modules/network/PacketIDDefine.h>
#include <map>
#include "OutPacketBuilder.h"

#define packetPool PacketPool::getInstance()

namespace pk
{
	class PacketPool
	{
	public:
		PacketPool();
		~PacketPool();

		fr::OutPacket *getOrCreateOutPacket(int cmdId, int controllerId = 1);

		fr::OutPacket *createOutPacket(int cmdId, int controllerId);

		void addPacketBuilder(OutPacketBuilder *builder);
	private:
		static PacketPool *_instance;

		std::map<int, fr::OutPacket *> _outPacketPool;
		std::vector<OutPacketBuilder *> _builderList;

	public:
		static PacketPool *getInstance();
	};
};




#endif // !PACKET_POOL_H


