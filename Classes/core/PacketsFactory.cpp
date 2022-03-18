#include "PacketsFactory.h"

PacketsFactory* PacketsFactory::_instance = nullptr;
fr::InPacket* PacketsFactory::getInPacket(fr::sPackage* pkg)
{
	initInstanceIfNull();
	fr::InPacket* packet = new fr::InPacket();
	packet->init(pkg);
	//TODO: get from queue;
	return packet;
}

fr::OutPacket* PacketsFactory::getOutPacket(int msgId, int controllerId)
{
	initInstanceIfNull();
	fr::OutPacket* outPacket = new fr::OutPacket();
	outPacket->initData(100);
	outPacket->setCmdId(msgId);
	outPacket->setControllerId(controllerId);
	outPacket->packHeader();
	return outPacket;
	//TODO: get from queue
}
void PacketsFactory::putInPacket(fr::InPacket* packet)
{
	delete packet;//TODO: add queue
}
void PacketsFactory::putOutPacket(fr::OutPacket* packet)
{
	delete packet;//TODO; add queue
}

void PacketsFactory::initInstanceIfNull()
{

}
void PacketsFactory::destroyInstance()
{
	if (_instance)
	{
		delete _instance;
		_instance = nullptr;
	}
}

PacketsFactory::PacketsFactory()
{

}
PacketsFactory::~PacketsFactory()
{

}