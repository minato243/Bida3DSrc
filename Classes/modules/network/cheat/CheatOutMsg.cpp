#include "CheatOutMsg.h"

using namespace pk;

CmdCheatGold::CmdCheatGold()
	:BaseOutMsg(PacketID::CHEAT_SET_GOLD)
{

}

CmdCheatGold::~CmdCheatGold()
{

}

void CmdCheatGold::packData(int gold)
{
	putLong(gold);
}

CmdCheatCup::CmdCheatCup()
	:BaseOutMsg(PacketID::CHEAT_ADD_CUP)
{

}

CmdCheatCup::~CmdCheatCup()
{

}

void CmdCheatCup::packData(int level)
{
	putInt(level);
}

CmdCheatS2Result::CmdCheatS2Result()
	:BaseOutMsg(PacketID::CHEAT_S2_FIRST_MATCH)
{

}

CmdCheatS2Result::~CmdCheatS2Result()
{

}

void CmdCheatS2Result::packData(int channel, bool win)
{
	putInt(channel);
	putByte(win ? 1 : 0);
}

CmdCheatCueInfo::CmdCheatCueInfo()
	:BaseOutMsg(PacketID::CHEAT_CUE_INFO)
{

}

CmdCheatCueInfo::~CmdCheatCueInfo()
{

}

void CmdCheatCueInfo::packData(int type, int star, int level, int battery)
{
	putInt(type);
	putInt(star);
	putInt(battery);
	putInt(level);
}

CmdCheatNewChallenges::CmdCheatNewChallenges()
	:BaseOutMsg(PacketID::CHEAT_NEW_SPONSORSHIP_CHALLENGES)
{

}

CmdCheatNewChallenges::~CmdCheatNewChallenges()
{

}

void CmdCheatNewChallenges::packData(std::vector<ChallengeData> challenges)
{
	for (int i = 0; i < 3; i++) {
		putInt(challenges[i].id);
		putInt(challenges[i].groupIdx);
	}
}

CmdCheatFinishSponChallenge::CmdCheatFinishSponChallenge()
	:BaseOutMsg(PacketID::CHEAT_COMPLETE_SPONSORSHIP_CHALLENGE)
{

}

CmdCheatFinishSponChallenge::~CmdCheatFinishSponChallenge()
{

}

void CmdCheatFinishSponChallenge::packData(int id)
{
	putInt(id);
}

CmdCheatPlayWithBot::CmdCheatPlayWithBot()
	:BaseOutMsg(PacketID::CHEAT_PLAY_WITH_BOT)
{

}

CmdCheatPlayWithBot::~CmdCheatPlayWithBot()
{

}

void CmdCheatPlayWithBot::packData(int gameMode, double accuracy)
{
	putInt(gameMode);
	putDouble(accuracy);
}

CmdCheatEditCueInfo::CmdCheatEditCueInfo()
	:BaseOutMsg(PacketID::CHEAT_EDIT_CUE_INFO)
{

}

CmdCheatEditCueInfo::~CmdCheatEditCueInfo()
{

}

void CmdCheatEditCueInfo::packData(int id, int level, int star, int configId, int battery, int point)
{
	putInt(id);
	putInt(level);
	putInt(star);
	putInt(configId);
	putInt(battery);
	putInt(point);
}

CmdCheatAddCueInfo::CmdCheatAddCueInfo()
	:BaseOutMsg(PacketID::CHEAT_ADD_CUE_INFO)
{

}

CmdCheatAddCueInfo::~CmdCheatAddCueInfo()
{

}

void CmdCheatAddCueInfo::packData(std::vector<CueInfoData> &list)
{
	putInt(list.size());
	for (auto i = 0; i < list.size(); i++) {
		auto item = list[i];
		putInt(item.type);
		putInt(item.level);
		putInt(item.star);
		putInt(item.configId);
		putInt(item.battery);
		putInt(item.point);
	}
}

CmdCheatCash::CmdCheatCash()
	:BaseOutMsg(PacketID::CHEAT_SET_CASH)
{

}

CmdCheatCash::~CmdCheatCash()
{

}

void CmdCheatCash::packData(long cash)
{
	putLong(cash);
}

CmdCheatClearInventory::CmdCheatClearInventory()
	:BaseOutMsg(PacketID::CHEAT_CLEAR_INVENTORY)
{
	
}

CmdCheatClearInventory::~CmdCheatClearInventory()
{

}
