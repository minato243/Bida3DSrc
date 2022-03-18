#ifndef GameData_h__
#define GameData_h__
#include <string>
#include "lobby/network/LobbyPackets.h"
#include <vector>
struct ChannelIAPPackage {
	int id;
	int value;
	int coin;
	float br;
	float usd;
	int requirePack;
	int maxBuyTime;
	int timeCountDown;
	std::vector<int> vipBonus;
	int hourBonus;
	long long gold;
	int shopBonus;
	int firstBonus;
	int gStar;
	std::string portalAndroidId;
	std::vector<std::string> packIds;
};
struct ChannelIAP {
	int id;
	std::string name;
	int type;
	int priority;
	std::vector<int> isMaintained;
	bool isShopBonus;
	std::vector<ChannelIAPPackage> packages;
	int numPackage;
};
class GameConfig {
public:
	GameConfig();
	~GameConfig();
public:
	void setChanelConfig(MsgChannelConfig config);
	int getCurrentChanel();
	void setNewShopGoldConfig(std::string stringConfig);
	void setNewShopGConfig(std::string stringConfig);
	int getIsFirstGold(long long value, int id);
	ChannelIAP getShopGoldById(int id);
	//ShopG getShopGById(int id);
	long long convertGStar(long long gold);
	void setVipConfig(std::string configVip);
	void setVipBenefit(std::string vipBenefit);
	void setVipEmotion(std::string vipEmotion);
	void setOldVipConfig(std::string vipOld);
	void setFriendConfig(std::string friendConfig);
	void setGiftOnlineConfig(std::string giftOnlineConfig);
	void setGuestConfig(std::string guestConfig);
public:
	std::string bonusStartDate;
	std::string bonusEndDate;
	std::string timeStartBonus;
	std::string timeEndBonus;
	int versionShopGold;
	std::vector<ChannelIAP> arrayShopGoldConfig;
	std::vector<int> arrayTypeFirst;
	std::vector<std::vector<char>> arrayIsFirstShop;
	std::vector<std::vector<int>> arrayValueShop;
	int buyGoldCount;
};

#define gamedata GameData::getInstance()

class GameData
{
public:
	struct UserData {
		long long bean;
		std::string avatar;
		std::string displayName;
		std::string zName;

		std::string name;
		long long coin;
		int levelScore;
		int winCount;
		int lostCount;
		int star;
		int uID;
		std::string openID;
		//userData.listGiftCode = [];
	};
	GameData();
	~GameData();
	static GameData* getInstance();
public:
	int getTimeLimitLeft(int idx);
	int getTimeLimitWebviewLeft(int idx);
	std::string getTimeLimitString(int idx);
	std::string getDisplayName();
	long long getUserGold();
	long long getUserId();
	std::string getUserAvatar();
	void setUserInfo(MsgUserInfo info);
	void updateMoney(MsgUpdateBean update);
	void saveConfigPayment(std::string paymentConfig);
	void saveConfigShop(MsgShopConfig configShop);
	void parseShopConfig();
	void setChannelConfig(MsgChannelConfig config);
	void getEventBonusTicket();

public:
	static GameData* _instance;

	GameConfig* gameConfig;
	UserData userData;
	std::string source;
	std::string forum;
	std::string support;
	std::string supporturl;
	std::string supportphone;
	int autologin;
	std::vector<bool> payments;
	std::string storeUrl;
	bool voteAppEnable;
	bool isShopBonus;
	bool isShopIAPBonus;
	int giftIndex;
	int numSupport;
	int timeSupport;
	bool showSupportTime;
	int selectedChanel;
	int jackpot;
	std::string appVersion;
	int isAppSupport;
	bool sound;
	bool music;
	bool soundhuman;
	bool soundhumansusun;
	bool vibrate;
	bool acceptInvite;
	bool acceptFriend;
	bool assistance;
	bool completeTooltip;
	bool loggedFacebook;
	bool wasShowPopupSurvey;
	bool doneSurvey;
	bool logged;
	bool timeSendGetListGiftCode;
	int countGame;
	std::string language;
	bool canRateApp;
	std::string countryID;
	bool enablePayement;
	bool waitReconnectBoard;
	std::string openID;
	std::string dataCacheService;
	MsgShopConfig cmdShopConfig;
public:
	static bool IS_RECEIVE_START_UP;
	static bool IS_RECEIVE_SHOW_TUT_PACKAGE;
	static bool IS_FIRST_LOGIN_AFTER_OPEN_APP;
};

#endif // GameData_h__
