#ifndef ChannelMgr_h_
#define ChannelMgr_h_

#include <data/DataStruct.h>
#include "data/GameConstant.h"

#define channelMgr ChannelMgr::getInstance()

class ChannelMgr
{
public:
	ChannelMgr();
	~ChannelMgr();

	static ChannelMgr *getInstance();

	static void destroyInstance();

	std::vector<ItemInfoData> getListItemWin(int gameMode, int channelId);

	ItemInfoData *getListFee(int gameMode, int channelId);

	bool joinChannel(GameMode gameMide, int channelId);

private:
	static ChannelMgr *_intstance;

};

#endif // !ChannelMgr_h_
