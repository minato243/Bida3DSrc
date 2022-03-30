#ifndef ConfigMgr_h_
#define ConfigMgr_h_

#include <json/document.h>
#include "TypeCodecConfig.h"
#include "CueConfig.h"
#include "CueRangeConfig.h"
#include "ShopV3Config.h"
#include "ChannelConfig.h"
#include "OfferConfig.h"
#include "SpinConfig.h"
#include "GloveConfig.h"

#define configMgr ConfigMgr::getInstance()

class ConfigMgr
{
public:
	ConfigMgr();
	~ConfigMgr();

	void loadConfig();

	rapidjson::Document* loadJsonFile(const char *pzName);

public:
	TypeCodecConfig	*typeCodecConfig;
	CueConfig		*cueConfig;
	CueRangeConfig	*cueRangeConfig;
	ShopConfig		*shopConfig;
	ChannelConfig	*channelConfig;
	OfferConfig		*offerConfig;
	SpinConfig		*spinConfig;
	GloveConfig		*gloveConfig;

private:
	static ConfigMgr * _instance;

public:
	static ConfigMgr *getInstance();
};

#endif // !ConfigMgr_h_


