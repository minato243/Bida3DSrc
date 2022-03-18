#include "PacketPool.h"
#include <data/DataStruct.h>

using namespace pk;

PacketPool::PacketPool()
{
}

PacketPool::~PacketPool()
{
	for (auto itt = _outPacketPool.begin(); itt != _outPacketPool.end();)
	{
		CC_SAFE_DELETE(itt->second);

		itt = _outPacketPool.erase(itt);
	}

	deleteVector(_builderList);
}

fr::OutPacket * PacketPool::getOrCreateOutPacket(int cmdId, int controllerId)
{
	if (_outPacketPool.find(cmdId) != _outPacketPool.end())
	{
		_outPacketPool[cmdId]->reset();
		_outPacketPool[cmdId]->packHeader();
		return _outPacketPool[cmdId];
	}
	else
	{
		return createOutPacket(cmdId, controllerId);
	}
}

fr::OutPacket * PacketPool::createOutPacket(int cmdId, int controllerId)
{
	for (int i = 0; i < _builderList.size(); i++)
	{
		auto msg = _builderList[i]->createOutPacket(cmdId, controllerId);
		if (msg != nullptr)
		{
			_outPacketPool.insert(std::pair<int, fr::OutPacket*>(cmdId, msg));
			return msg;
		}
	}

	fr::OutPacket* outPacket = new fr::OutPacket();
	outPacket->initData(100);
	outPacket->setCmdId(cmdId);
	outPacket->setControllerId(controllerId);
	outPacket->packHeader();
	_outPacketPool.insert(std::pair<int, fr::OutPacket*>(cmdId, outPacket));

	return outPacket;
}

void PacketPool::addPacketBuilder(OutPacketBuilder * builder)
{
	_builderList.push_back(builder);
}

PacketPool * PacketPool::_instance = nullptr;
PacketPool * PacketPool::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new PacketPool();
	}
	return _instance;
}
