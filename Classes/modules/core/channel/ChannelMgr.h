#ifndef ChannelMgr_h_
#define ChannelMgr_h_

#include <data/DataStruct.h>
#include "data/GameConstant.h"
#include "data/config/GloveConfig.h"

#define channelMgr ChannelMgr::getInstance()

class ChannelMgr
{
public:
	ChannelMgr();
	~ChannelMgr();

	static ChannelMgr *getInstance();

	static void destroyInstance();

	std::vector<ItemInfoData> getListItemWin(GameMode gameMode, int channelId);

	std::vector<ItemInfoData> getListItemFee(GameMode gameMode, int channelId);

	ItemInfoData *getListFee(int gameMode, int channelId);

	bool joinChannel(GameMode gameMide, int channelId);

	MatchingGloveConfigData getGloveMatchingConfigByCurrentGlove(int glove);

	int getGloveFeeByCurrentGlove();

	int getGlovePrizeByCurrentGlove();

private:
	static ChannelMgr *_intstance;

};

#endif // !ChannelMgr_h_
