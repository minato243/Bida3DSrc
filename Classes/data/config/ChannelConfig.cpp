#include "ChannelConfig.h"
#include "ConfigMgr.h"
#include "../GameConstant.h"
#include <cocos2d.h>

using namespace rapidjson;


ChannelConfig::ChannelConfig()
{
	loadConfig();
}

ChannelConfig::~ChannelConfig()
{

}

void ChannelConfig::loadConfig()
{
	data = configMgr->loadJsonFile("res/config/ChannelConfig.json");
}

ChannelConfigData ChannelConfig::getChannelConfig(int gameMode, int channel)
{
	CCLOG("getChannelConfig %d, %d", gameMode, channel);
	ChannelConfigData channelData;
	Value &config = (*data)[cto_string(gameMode)][cto_string(channel)];

	channelData.channelId = config["channelId"].GetInt();
	channelData.entryFee = config["entryFee"].GetInt();
	channelData.entryFeeUnit = config["entryFeeUnit"].GetString();
	channelData.prize = config["prize"].GetInt64();
	channelData.prizeUnit = config["prizeUnit"].GetString();
	channelData.cardLevel = config["cardLevel"].GetInt();

	return channelData;
}

long long ChannelConfig::getChannelPrize(int gameMode, int channel)
{
	CCLOG("getChannelPrize %d, %d", gameMode, channel);
	Value &config = (*data)[cto_string(gameMode)][cto_string(channel)];
	auto prize = config["prize"].GetInt64();
	return prize;
}

