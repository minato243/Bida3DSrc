#ifndef USER_H_
#define USER_H_

#include <string>
#include <modules/network/user/UserInPacket.h>

#define userInst User::getInstance()

class User
{
public:
	User();
	User(int uId, std::string uName, std::string avatar);
	User(int uId, std::string uName, std::string avatar, int gold, int cash, int useGold, int exp);
	~User();

	void init(int uId, std::string uName, std::string avatar, int gold, int cash, int useGold, int exp);

	void setGold(int gold);

	long long getGold();

	void setCash(int cash);

	int getCash();

	void addGold(long long add);

	void addCash(int add);

	void updateInfo(const UserInfoInPacket *pk);

	void updateHistory(const std::map<int, bool> & history);

	void addS2History(int channelId, bool isWinn);

	void resetS2History(int channelId);

	bool getLastS2MatchResult(int channelId);

public:

	static User * getInstance();
public:
	int _uId;
	std::string _uName;
	std::string _avatar;
	
	std::map<int, bool> _history;

	int _level;
private:
	long long _gold;
	int _cash;
	int _usedGold;
	int _exp;
	

	static User * _instance;

};

#endif // !USER_H_

