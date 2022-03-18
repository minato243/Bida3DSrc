#ifndef ChannelConfig_h_
#define ChannelConfig_h_

#include <string>
#include <json/document.h>

class ChannelConfigData
{
public:
	int channelId;
	int entryFee;
	std::string entryFeeUnit;
	long long prize;
	std::string prizeUnit;
	int cardLevel;

};

class ChannelConfig
{
public:
	ChannelConfig();
	~ChannelConfig();


	void loadConfig();

	ChannelConfigData getChannelConfig(int gameMode, int channel);

	long long getChannelPrize(int gameMode, int channel);

private:
	rapidjson::Document *data;
};
#endif