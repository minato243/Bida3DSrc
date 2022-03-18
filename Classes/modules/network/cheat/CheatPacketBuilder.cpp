#include "CheatPacketBuilder.h"
#include "../game/IngameOutMsg.h"
#include "CheatOutMsg.h"

using namespace pk;

CheatPacketBuilder::CheatPacketBuilder()
	:OutPacketBuilder()
{

}

CheatPacketBuilder::~CheatPacketBuilder()
{

}

fr::OutPacket * CheatPacketBuilder::createOutPacket(int cmdId, int controllerId)
{
	switch (cmdId)
	{

	case PacketID::START_CHEAT:
	{
		auto msg = new CmdStartCheat();
		return msg;
	}

	case PacketID::END_CHEAT:
	{
		auto msg = new CmdEndCheat();
		return msg;
	}
	case PacketID::CHEAT_CUE_INFO:
	{
		auto msg = new CmdCheatCueInfo();
		return msg;
	}

	case PacketID::CHEAT_ADD_CUE_INFO:
	{
		auto msg = new CmdCheatAddCueInfo();
		return msg;
	}

	case PacketID::CHEAT_ADD_CUP:
	{
		auto msg = new CmdCheatCup();
		return msg;
	}

	case PacketID::CHEAT_SET_GOLD:
	{
		auto msg = new CmdCheatGold();
		return msg;
	}

	case PacketID::CHEAT_SET_CASH:
	{
		auto msg = new CmdCheatCash();
		return msg;
	}

	case PacketID::CHEAT_S2_FIRST_MATCH:
	{
		auto msg = new CmdCheatS2Result();
		return msg;
	}

	case PacketID::CHEAT_NEW_SPONSORSHIP_CHALLENGES:
	{
		auto msg = new CmdCheatNewChallenges();
		return msg;
	}

	case PacketID::CHEAT_COMPLETE_SPONSORSHIP_CHALLENGE:
	{
		auto msg = new CmdCheatFinishSponChallenge();
		return msg;
	}

	case PacketID::CHEAT_PLAY_WITH_BOT:
	{
		auto msg = new CmdCheatPlayWithBot();
		return msg;
	}

	case PacketID::CHEAT_EDIT_CUE_INFO:
	{
		auto msg = new CmdCheatEditCueInfo();
		return msg;
	}

	case PacketID::CHEAT_CLEAR_INVENTORY:
	{
		auto msg = new CmdCheatClearInventory();
		return msg;
	}

	default:
		break;
	}
	return nullptr;
}

