#ifndef ChatConfig_h__
#define ChatConfig_h__
#include <string>
#include <map>
#include <vector>

#define chatConfig ChatConfig::getInstance()

struct VipChatConfig {
	std::vector<int> emotion;
	std::vector<int> interactive;
};
class ChatConfig {
public:
	ChatConfig();
	~ChatConfig();
	static ChatConfig* getInstance();
public:
	void resetData();
	void setChatConfig(std::string config);
	std::map<std::string, std::vector<int>> getEmoConfig();
	std::map<std::string, std::vector<int>> getInteractConfig();
	std::map<std::string, VipChatConfig> getVipUseConfig();
	std::string getLevelCanUseAnItem(int itemIdx, bool isEmo);
	bool canUseItem(int itemIdx, bool isEmo);
public:
	static ChatConfig* instance;

	std::map<std::string, std::vector<int>> emoConfig;
	std::map<std::string, std::vector<int>> interactiveConfig;
	std::map<std::string, VipChatConfig> vipCanUseConfig;
};
#endif // ChatConfig_h__
