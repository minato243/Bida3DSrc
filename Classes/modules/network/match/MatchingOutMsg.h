#ifndef MATCHING_OUT_MSG_H_
#define MATCHING_OUT_MSG_H_

#include <BaseFramework.h>
#include "../base/BasePacket.h"

class CmdSendQuickMatch
	:public BaseOutMsg
{
public:
	CmdSendQuickMatch();
	~CmdSendQuickMatch();

	void packData(int gameMode, int matchMode, int channelId);

public:

};

class CmdSendGetChannelInfo
	:public BaseOutMsg
{
public:
	CmdSendGetChannelInfo();
	~CmdSendGetChannelInfo();

	void packData(int gameMode, int matchMode);
};

class CmdJoinRT3Room
	:public BaseOutMsg
{
public:
	CmdJoinRT3Room();
	~CmdJoinRT3Room();

	void packData(int id);

};

class CmdInviteFriend
	:public BaseOutMsg
{
public:
	CmdInviteFriend();
	~CmdInviteFriend();

	void packData(int id);
};

class CmdSendAcceptInviteJoinRT3
	:public BaseOutMsg
{
public:
	CmdSendAcceptInviteJoinRT3();
	~CmdSendAcceptInviteJoinRT3();

	void packData(int ownerId, bool isAccept);
};

class CmdKickPlayerRT3Room
	:public BaseOutMsg
{
public:
	CmdKickPlayerRT3Room();
	~CmdKickPlayerRT3Room();

	void packData(int pId);

};

#endif // !MATCHING_OUT_MSG_H

