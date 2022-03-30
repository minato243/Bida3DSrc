#include "GloveConfig.h"
#include "ConfigMgr.h"

using namespace rapidjson;

GloveConfig::GloveConfig()
{
	loadConfig();
}

GloveConfig::~GloveConfig()
{

}

void GloveConfig::loadConfig()
{
	data = configMgr->loadJsonFile("res/config/GloveConfig.json");

	matchingGloveConfigs.reserve(10);

	Value &matchingConfig = (*data)["Matching"];
	for (auto i = 0; i < matchingConfig.Size(); i++) {
		MatchingGloveConfigData item;
		item.min = matchingConfig[i]["min"].GetInt();
		item.max = matchingConfig[i]["max"].GetInt();
		item.require = matchingConfig[i]["require"].GetInt();
		item.prize = matchingConfig[i]["prize"].GetInt();
		matchingGloveConfigs.push_back(item);
	}
}

MatchingGloveConfigData GloveConfig::getMatchingGloveConfig(int glove)
{
	for (auto i = 0; i < matchingGloveConfigs.size(); i++) {
		MatchingGloveConfigData &config = matchingGloveConfigs[i];
		if (glove <= config.max && glove >= config.min)
			return config;
	}

	return matchingGloveConfigs[0];
}

