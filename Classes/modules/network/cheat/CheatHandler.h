#ifndef CheatHandler_h_
#define CheatHandler_h_

#include <BaseFramework.h>
#include <data/DataStruct.h>
#include "modules/network/base/BaseHandler.h"
#include "../base/OutPacketBuilder.h"

#define cheatHandler CheatHandler::getInstance()

class CheatHandler
	:BaseHandler
{
public:
	CheatHandler();
	~CheatHandler();

	bool onReceived(fr::InPacket* pk);

	void requestStartCheat();

	void requestEndCheat();

	void requestCheatCueInfo(int type, int star, int point, int battery);

	void requestCheatAddCueInfo(std::vector<CueInfoData> &list);

	void requestCheatGold(int gold);

	void requestCheatCash(int cash);

	void requestCheatS2Result(int channel, bool win);

	void requestCheatNewChallenges(std::vector<ChallengeData> &chanllenges);

	void requestFinishSponChallenge(int id);

	void requestCheatPlayWithBot(int gameMode, double accuracy);

	void requestCheatClearInventory();

private:
	pk::OutPacketBuilder *_builder;

	static CheatHandler *_instance;
	

public:
	static CheatHandler *getInstance();
	
	static void destroyInstance();
};

#endif // CheatHandler_h_
