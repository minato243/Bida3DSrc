#ifndef CheatOutMsg_h_
#define CheatOutMsg_h_

#include <BaseFramework.h>
#include "../base/BasePacket.h"
#include <data/DataStruct.h>

class CmdCheatGold
	:public BaseOutMsg
{
public:
	CmdCheatGold();
	~CmdCheatGold();

	void packData(int gold);
};

class CmdCheatCup
	:public BaseOutMsg
{
public:
	CmdCheatCup();
	~CmdCheatCup();

	void packData(int level);
};

class CmdCheatS2Result
	:public BaseOutMsg
{
public:
	CmdCheatS2Result();
	~CmdCheatS2Result();

	void packData(int channel, bool win);
};

class CmdCheatCueInfo
	:public BaseOutMsg
{
public:
	CmdCheatCueInfo();
	~CmdCheatCueInfo();

	void packData(int type, int star, int level, int battery);
};

class CmdCheatNewChallenges
	:public BaseOutMsg
{
public:
	CmdCheatNewChallenges();
	~CmdCheatNewChallenges();

	void packData(std::vector<ChallengeData> challenges);
};

class CmdCheatFinishSponChallenge
	:public BaseOutMsg
{
public:
	CmdCheatFinishSponChallenge();
	~CmdCheatFinishSponChallenge();

	void packData(int id);
};

class CmdCheatPlayWithBot
	:public BaseOutMsg
{
public:
	CmdCheatPlayWithBot();
	~CmdCheatPlayWithBot();

	void packData(int gameMode, double accuracy);

};

class CmdCheatEditCueInfo
	:public BaseOutMsg
{
public:
	CmdCheatEditCueInfo();
	~CmdCheatEditCueInfo();

	void packData(int id, int level, int star, int configId, int battery, int point);

};

class CmdCheatAddCueInfo
	:public BaseOutMsg
{
public:
	CmdCheatAddCueInfo();
	~CmdCheatAddCueInfo();

	void packData(std::vector<CueInfoData> &list);

};

class CmdCheatCash
	:public BaseOutMsg
{
public:
	CmdCheatCash();
	~CmdCheatCash();

	void packData(long cash);

};

class CmdCheatClearInventory 
	: public BaseOutMsg
{
public:
	CmdCheatClearInventory();
	~CmdCheatClearInventory();
};
#endif