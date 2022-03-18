#include "ChatConfig.h"
#include "json/document.h"
#include "cocos2d.h"
USING_NS_CC;
ChatConfig::ChatConfig()
{

}

ChatConfig::~ChatConfig()
{

}

ChatConfig* ChatConfig::getInstance()
{
	if (!instance) {
		instance = new ChatConfig();
	}
	return instance;
}

void ChatConfig::resetData()
{

}

void ChatConfig::setChatConfig(std::string config)
{
	rapidjson::Document	json;
	json.Parse<0>(config.c_str());
	if (json.HasParseError()) {
		size_t offset = json.GetErrorOffset();
		if (offset > 0)
			offset--;
		std::string errorSnippet = config.substr(offset, 10);
		log("ChatConfig::setChatConfig File parse error %d at <%s>\n", json.GetParseError(), errorSnippet.c_str());
	}
	//emotion
	emoConfig.clear();
	rapidjson::Value& emotion = json["emotion"];
	for (rapidjson::Value::MemberIterator itr = emotion.MemberBegin(); itr != emotion.MemberEnd(); ++itr)
	{
		std::string key = itr->name.GetString();
		rapidjson::Value &emotions = itr->value;
		std::vector<int> emotionIds;
		for (rapidjson::SizeType i = 0; i < emotions.Size(); ++i)
		{
			emotionIds.push_back(emotions[i].GetInt());
		}
		emoConfig[key] = emotionIds;
	}

	//interactive
	interactiveConfig.clear();
	rapidjson::Value& interactive = json["interactive"];
	for (rapidjson::Value::MemberIterator itr = interactive.MemberBegin(); itr != interactive.MemberEnd(); ++itr)
	{
		std::string key = itr->name.GetString();
		rapidjson::Value &interactives = itr->value;
		std::vector<int> interactiveIds;
		for (rapidjson::SizeType i = 0; i < interactives.Size(); ++i)
		{
			interactiveIds.push_back(interactives[i].GetInt());
		}
		interactiveConfig[key] = interactiveIds;
	}
	//VIP
	vipCanUseConfig.clear();
	if (json.HasMember("vip")) {
		rapidjson::Value& vip = json["vip"];
		for (rapidjson::Value::MemberIterator itr = vip.MemberBegin(); itr != vip.MemberEnd(); ++itr)
		{
			std::string key = itr->name.GetString();
			rapidjson::Value &vips = itr->value;
			VipChatConfig vipConfig;
			rapidjson::Value& emotions = vips["emotion"];
			for (rapidjson::SizeType i = 0; i < emotions.Size(); ++i)
			{
				vipConfig.emotion.push_back(emotions[i].GetInt());
			}
			rapidjson::Value& interactives = vips["interactive"];
			for (rapidjson::SizeType i = 0; i < interactives.Size(); ++i)
			{
				vipConfig.interactive.push_back(interactives[i].GetInt());
			}
			vipCanUseConfig[key] = vipConfig;
		}
	}
}

std::map<std::string, std::vector<int>> ChatConfig::getEmoConfig()
{
	return emoConfig;
}

std::map<std::string, std::vector<int>> ChatConfig::getInteractConfig()
{
	return interactiveConfig;
}

std::map<std::string, VipChatConfig> ChatConfig::getVipUseConfig()
{
	return vipCanUseConfig;
}

std::string ChatConfig::getLevelCanUseAnItem(int itemIdx, bool isEmo)
{
	std::map<std::string, std::vector<int>> itemConfig = isEmo ? emoConfig : interactiveConfig;
	std::string key = "";
	for (std::map<std::string, std::vector<int>>::iterator iter = itemConfig.begin(); iter != itemConfig.end(); iter++) {
		key = iter->first;
		std::vector<int> ids = itemConfig[key];
		if (std::find(ids.begin(), ids.end(), itemIdx) != ids.end()) {
			break;
		}
	}
	if (key.empty()) 
		return "";

	for (std::map<std::string, VipChatConfig>::iterator iter = vipCanUseConfig.begin(); iter != vipCanUseConfig.end(); iter++) {
		VipChatConfig vipConfig = iter->second;
		std::string vipLevel = iter->first;
		std::vector<int> ids = isEmo ? vipConfig.emotion : vipConfig.interactive;
		int value = std::stoi(key);
		if (std::find(ids.begin(), ids.end(), value) != ids.end()) {
			return vipLevel;
		}
	}
	return "";
}

bool ChatConfig::canUseItem(int itemIdx, bool isEmo)
{
	//TODO:VIP
	//var vipLevel = NewVipManager.getInstance().getVipLevel();
	std::string vipLevel = "0";
	std::map<std::string, std::vector<int>> itemConfig = isEmo ? emoConfig : interactiveConfig;
	std::string key = "";
	for (std::map<std::string, std::vector<int>>::iterator iter = itemConfig.begin(); iter != itemConfig.end(); iter++) {
		key = iter->first;
		std::vector<int> ids = itemConfig[key];
		if (std::find(ids.begin(), ids.end(), itemIdx) != ids.end()) {
			break;
		}
	}
	if (key.empty())
		return false;
	for (std::map<std::string, VipChatConfig>::iterator iter = vipCanUseConfig.begin(); iter != vipCanUseConfig.end(); iter++) {
		VipChatConfig vipConfig = iter->second;
		std::string vipLevel = iter->first;
		std::vector<int> ids = isEmo ? vipConfig.emotion : vipConfig.interactive;
		int value = std::stoi(key);
		if (std::find(ids.begin(), ids.end(), value) != ids.end()) {
			return true;
		}
	}
	return false;
}

ChatConfig* ChatConfig::instance = nullptr;

