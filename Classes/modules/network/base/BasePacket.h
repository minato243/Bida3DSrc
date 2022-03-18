#ifndef BASE_PACKET_H
#define BASE_PACKET_H

#include <BaseFramework.h>
#include <modules/network/PacketIDDefine.h>

class BaseOutMsg
	:public fr::OutPacket
{
public:
	BaseOutMsg(pk::PacketID cmdId, int controllerId = 1);
	~BaseOutMsg();
};


class BaseInPacket
{
public:
	BaseInPacket(fr::InPacket *pk);
	~BaseInPacket();

	virtual void readData()=0;

	virtual std::string getStatus();
public:
	fr::InPacket *_pk;
	int error;
};

#endif // !BASE_PACKET_h



