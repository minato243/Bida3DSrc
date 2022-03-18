#ifndef MiniGameOutMsg_h_
#define MiniGameOutMsg_h_

#include "../base/BasePacket.h"

class CmdSendSpinWheel
	:public BaseOutMsg
{
public:
	CmdSendSpinWheel();
	~CmdSendSpinWheel();

	void packData();

};

class CmdSendCheatSpin
	:public BaseOutMsg
{
public:
	CmdSendCheatSpin();
	~CmdSendCheatSpin();

	void packData();

};
#endif