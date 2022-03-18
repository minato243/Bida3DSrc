#include "UserInfo.h"

UserInfo::UserInfo()
{

}

UserInfo::~UserInfo()
{

}

UserInfo* UserInfo::getInstance()
{
	if (!_instance)
	{
		_instance = new UserInfo();
	}
	return _instance;
}

void UserInfo::setUserInfo(MsgUserInfo info)
{
	avatar = info.avatar;
	displayName = info.userName;
	zName = info.zName;

	name = displayName;
	bean = info.gold;
	coin = info.zMoney;
	levelScore = info.levelScore;
	winCount = info.winCount;
	lostCount = info.lostCount;
	star = 0;
	uID = info.uId;
	openID = "";
}

void UserInfo::updateMoney(MsgUpdateBean obj)
{
	if (uID == obj.uID) {
		bean = obj.bean;
		coin = obj.coin;
		winCount = obj.winCount;
		lostCount = obj.lostCount;
		levelScore = obj.levelScore;
	}
}

std::string UserInfo::getDisplayName(int nLength)
{
	if (nLength && nLength > displayName.size()) {
		return displayName.substr(0, nLength - 2) + "...";
	}
	return displayName;
}

std::string UserInfo::getUserName()
{
	return zName;
}

std::string UserInfo::getOpenID()
{
	return openID;
}

int UserInfo::getUserID()
{
	return userId;
}

std::string UserInfo::getAvatar()
{
	return avatar;
}

long long UserInfo::getGold()
{
	return bean;
}

int UserInfo::getLevel()
{
	return levelScore;
}

int UserInfo::getWinCount()
{
	return winCount;
}

int UserInfo::getLoseCount()
{
	return lostCount;
}

UserInfo* UserInfo::_instance = nullptr;

