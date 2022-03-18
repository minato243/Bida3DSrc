#ifndef USER_INPACKET_H_
#define USER_INPACKET_H_

#include "../base/BasePacket.h"

class UserInfoInPacket
	: public BaseInPacket
{
public:
	UserInfoInPacket(fr::InPacket *pk);
	~UserInfoInPacket();

	void readData();

public:
	int userId;
	std::string userName;
	std::string displayName;
	std::string avatar;
	int gold;
	int cash;
	int usedGold;
	int exp;
	int level;
	long serverTime;
};

class UpdateHistoryMsg
	:public BaseInPacket
{
public:
	UpdateHistoryMsg(fr::InPacket *pk);
	~UpdateHistoryMsg();

	void readData();

public:
	std::map<int, bool> history;
};

#endif // !USER_INPACKET_H_

