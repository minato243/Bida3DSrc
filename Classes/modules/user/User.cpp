#include "User.h"

User::User()
	:_uId(0), _uName("Player")
{
	init(0, "", "", 0, 0, 0, 0);
}

User::User(int uId, std::string uName, std::string avatar)
{
	init(uId, uName, avatar, 0, 0, 0 ,0);
}

User::User(int uId, std::string uName, std::string avatar, int gold, int cash, int usedGold, int exp)
{
	init(uId, uName, avatar, gold, cash, usedGold, exp);
}

User::~User()
{
}

void User::init(int uId, std::string uName, std::string avatar, int gold, int cash, int usedGold, int exp)
{
	_uId = uId;
	_uName = uName;
	_avatar = avatar;
	_gold = gold;
	_cash = cash;
	_usedGold = usedGold;
	_exp = exp;
	_level = 0;
}

void User::setGold(int gold)
{
	_gold = gold;
}

long long User::getGold()
{

	return _gold;
}

void User::setCash(int cash)
{
	_cash = cash;
}

int User::getCash()
{
	return _cash;
}

void User::addGold(long long add)
{
	_gold += add;
}

void User::addCash(int add)
{
	_cash += add;
}

void User::updateInfo(const UserInfoInPacket * pk)
{
	_uId = pk->userId;
	_uName = pk->displayName;
	_avatar = pk->avatar;
	_gold = pk->gold;
	_cash = pk->cash;
	_usedGold = pk->usedGold;
	_exp = pk->exp;
}

void User::updateHistory(const std::map<int, bool> & history)
{
	_history.clear();
	for (auto ittr = history.begin(); ittr != history.end(); ittr++)
	{
		_history[ittr->first] = ittr->second;
	}
}

void User::addS2History(int channelId, bool isWin)
{
	_history[channelId] = isWin;
}

void User::resetS2History(int channelId)
{
	_history[channelId] = false;
}

bool User::getLastS2MatchResult(int channelId)
{
	return _history[channelId];
}

User * User::_instance = nullptr;

User * User::getInstance()
{
	if (_instance == nullptr) {
		_instance = new User();
	}
	return _instance;
}
