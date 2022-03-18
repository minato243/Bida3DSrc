#include "UserInPacket.h"

UserInfoInPacket::UserInfoInPacket(fr::InPacket * pk)
	:BaseInPacket(pk)
{
}

UserInfoInPacket::~UserInfoInPacket()
{
}

void UserInfoInPacket::readData()
{
	userId = _pk->getInt();
	userName = _pk->getString();
	displayName = _pk->getString();
	avatar = _pk->getString();
	gold = _pk->getLong();
	cash = _pk->getLong();
	usedGold = _pk->getLong();
	exp = _pk->getInt();
	level = _pk->getInt();
	serverTime = _pk->getLong();
}

UpdateHistoryMsg::UpdateHistoryMsg(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

UpdateHistoryMsg::~UpdateHistoryMsg()
{

}

void UpdateHistoryMsg::readData()
{
	if (error == 0)
	{
		auto num = _pk->getInt();
		for (auto i = 0; i < num; i++)
		{
			int key = _pk->getInt();
			bool val = _pk->getByte() == 1;
			history[key] = val;
		}
	}
}
