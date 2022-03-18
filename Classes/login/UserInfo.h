#ifndef UserInfo_h__
#define UserInfo_h__
#include <string>
#include "lobby/network/LobbyPackets.h"

#define userInfo UserInfo::getInstance()

class UserInfo {
public:
	UserInfo();
	~UserInfo();

	static UserInfo* getInstance();
public:
	void setUserInfo(MsgUserInfo info);
	void updateMoney(MsgUpdateBean obj);
	std::string getDisplayName(int nLength);
	std::string getUserName();
	std::string getOpenID();
	int getUserID();
	std::string getAvatar();
	long long getGold();
	int getLevel();
	int getWinCount();
	int getLoseCount();
public:
	static UserInfo* _instance;
	std::string avatar;
	std::string displayName;
	std::string zName;
	std::string name;

	long long bean;
	long long coin;

	int levelScore;
	int winCount;
	int lostCount;
	int star;
	long long uID;
	std::string openID;
	int userId;
};
#endif // UserInfo_h__
