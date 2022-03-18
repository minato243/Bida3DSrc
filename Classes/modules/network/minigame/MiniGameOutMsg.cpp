#include "MiniGameOutMsg.h"

using namespace pk;

CmdSendSpinWheel::CmdSendSpinWheel()
	:BaseOutMsg(PacketID::SPIN_WHEEL)
{

}

CmdSendSpinWheel::~CmdSendSpinWheel()
{

}

void CmdSendSpinWheel::packData()
{

}

CmdSendCheatSpin::CmdSendCheatSpin()
	:BaseOutMsg(PacketID::CHEAT_SPIN)
{

}

CmdSendCheatSpin::~CmdSendCheatSpin()
{

}

void CmdSendCheatSpin::packData()
{

}
