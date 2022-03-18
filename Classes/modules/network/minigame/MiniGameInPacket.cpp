#include "MiniGameInPacket.h"

SpinDataMsg::SpinDataMsg(fr::InPacket *pk)
	:BaseInPacket(pk)
{
	cellId = -1;
}

SpinDataMsg::~SpinDataMsg()
{

}

void SpinDataMsg::readData()
{
	if (error == 0)
	{
		cellId = _pk->getInt();
	}
}

SpinInfoMsg::SpinInfoMsg(fr::InPacket *pk)
	:BaseInPacket(pk)
{
	numberOfSpin = 0;
}

SpinInfoMsg::~SpinInfoMsg()
{

}

void SpinInfoMsg::readData()
{
	if (error == 0)
	{
		numberOfSpin = _pk->getInt();
	}
}
