#include "BasePacket.h"

//BaseOutMsg

BaseOutMsg::BaseOutMsg(pk::PacketID cmdId, int controllerId)
	:fr::OutPacket()
{
	initData(100);
	setCmdId((int)cmdId);
	setControllerId(controllerId);
	packHeader();
}

BaseOutMsg::~BaseOutMsg()
{
}


//BaseInPacket

BaseInPacket::BaseInPacket(fr::InPacket * pk)
{
	_pk = pk;
	error = pk->getError();
}

BaseInPacket::~BaseInPacket()
{
}

std::string BaseInPacket::getStatus()
{
	if (error == 0)
		return "Success";
	return "Error " + std::to_string(error);
}
