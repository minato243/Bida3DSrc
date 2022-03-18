#include "ChanelConfig.h"
#include "json/document.h"
#include "cocos2d.h"
#include "core/GameData.h"

USING_NS_CC;

ChanelConfig::ChanelConfig()
{
	betTime = 100;
}

ChanelConfig::~ChanelConfig()
{

}

ChanelConfig* ChanelConfig::instance()
{
	if (!_inst)
	{
		_inst = new ChanelConfig();
	}
	return _inst;
}

void ChanelConfig::setConfig(std::string data)
{
	betTime = 3;

	chanelConfig.clear();

	rapidjson::Document json;
	json.Parse<0>(data.c_str());
	// Print error
	if (json.HasParseError()) {
		size_t offset = json.GetErrorOffset();
		if (offset > 0)
			offset--;
		std::string errorSnippet = data.substr(offset, 10);
		CCLOG("ChanelConfig::setConfig File parse error %d at <%s>\n", json.GetParseError(), errorSnippet.c_str());
	}

	for (rapidjson::Value::MemberIterator itr = json.MemberBegin(); itr != json.MemberEnd(); ++itr)
	{
		rapidjson::Value &channel = itr->value;
		if (channel.IsObject() && channel.HasMember("minGold")) {
			Config config;
			config.minGold = channel["minGold"].GetInt64();
			config.maxGold = channel["maxGold"].GetInt64();
			config.name = channel["name"].GetString();

			const rapidjson::Value& bets = channel["betGold"];
			for (rapidjson::SizeType i = 0; i < bets.Size(); ++i)
			{
				config.bet.push_back(bets[i].GetInt64());
			}
			chanelConfig.push_back(config);
		}
	}
}

int ChanelConfig::getCurChanel()
{
	int i;
	if (gamedata->userData.bean == 0)
		return -1;

	for (i = chanelConfig.size() - 1; i >= 0; i--) {
		if (chanelConfig[i].minGold <= gamedata->userData.bean) {
			return i;
		}
	}

	return i;
}

long long ChanelConfig::getMinBet()
{
	return chanelConfig[0].bet[0];
}

long long ChanelConfig::getBet(int id)
{
	return chanelConfig[gamedata->selectedChanel].bet[id];
}

int ChanelConfig::betLevel(long long gold)
{
	int i;
	if (gold == 0)
		return -1;

	for (i = 0; i < chanelConfig.size(); i++) {
		if (chanelConfig[i].maxGold >= gold) {
			if (chanelConfig[i].bet[0] * betTime <= gold)
				return i;
			else
				return i - 1;

		}
		else {
			if (chanelConfig[i].maxGold == -1) {
				return i;
			}
		}
	}

	return i;
}

long long ChanelConfig::getSuitableBet()
{
	for (int i = chanelConfig[gamedata->selectedChanel].bet.size()- 1; i >= 0; i--) {
		if (chanelConfig[gamedata->selectedChanel].bet[i] * betTime < gamedata->userData.bean) {
			return i;
		}
	}
	return 0;
}

int ChanelConfig::getCurrentChannel()
{
	int i;
	if (gamedata->userData.bean == 0)
		return -1;

	for (i = 0; i < chanelConfig.size(); i++) {

		if (chanelConfig[i].maxGold >= gamedata->userData.bean) {

			if (chanelConfig[i].bet[0] * betTime <= gamedata->userData.bean)
				return i;
			else
				return i - 1;

		}
		else {
			if (chanelConfig[i].maxGold == -1) {
				return i;
			}
		}
	}

	return i;
}


long long ChanelConfig::getMaxGoldCanPlayInChannel()
{
	return chanelConfig[gamedata->selectedChanel].maxGold;
}

bool ChanelConfig::firstInit = true;

ChanelConfig* ChanelConfig::_inst = nullptr;

