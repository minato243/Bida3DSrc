#ifndef LobbyPackets_h__
#define LobbyPackets_h__
#include <string>
#include <vector>

struct MsgChannelConfig {
	std::string channelConfig;
	std::string vipConfig;
	std::string vipBenefit;
	std::string vipEmotion;
	std::string vipOld;
	std::string friendConfig;
	std::string playingGiftConfig;
	std::string guestConfig;
};

struct MsgShopConfig {
	char type;
	bool isShopBonus;
	bool isShopBonusG;

	std::string stringConfigGold;
	std::string stringConfigG;

	std::string paymentConfig;
};
struct EventSocketHandshake {
	EventSocketHandshake() {}
};

struct EventSocketLogin {
	EventSocketLogin(int _errorCode) :errorCode(_errorCode) {}
	int errorCode;
};

struct MsgUserInfo {
	int uId;
	std::string userName;
	std::string zName;
	std::string avatar;
	long long gold;
	long long zMoney;
	int levelScore;
	int winCount;
	int lostCount;
};

struct MsgUpdateBean {
	int uID;
	char nChair;
	long long bean;
	long long coin;
	long long levelScore;
	int winCount;
	int lostCount;
	char iapProgress;
	long long exp;
};
struct EventQuickPlayError {
	EventQuickPlayError(int _errorCode) :errorCode(_errorCode) {}
	int errorCode;
};

struct EventShowTutorial {
	char willShow;
};

struct EventSupportBean {
	long long nBean;
	int numSupport;
	char bonusPercent;
	long long facebookBonus;
};

struct EventGetGiftInfo {
	double currentPlayingTime;
};

struct MsgMissionIAP {
	std::vector<bool> arrayPackageIAP;
	std::vector<double> arrayPackageTime;
};
struct PurchaseItem {
	std::string id; 
	int num;
};
struct MsgPurchaseIAPApple {
	std::vector<PurchaseItem> success;
	std::vector<PurchaseItem> failed;
	std::string idSuccess;
	std::string idFaileds;
	std::vector<std::string> ids;
};

struct MsgUpdateBuyGold {
	int nBuyGold;
	int sizeMission;
	std::vector<int> arrayTypeFirst;
	std::vector<std::vector<char>> arrayIsFirst;
	std::vector<std::vector<int>> arrayValue;
};
#endif // LobbyPackets_h__
