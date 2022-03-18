#include "MatchingOutMsg.h"
#include <modules/network/PacketIDDefine.h>

using namespace pk;
CmdSendQuickMatch::CmdSendQuickMatch()
	:BaseOutMsg(PacketID::QUICK_MATCH)
{

}

CmdSendQuickMatch::~CmdSendQuickMatch()
{
}

void CmdSendQuickMatch::packData(int gameMode, int matchMode, int channelId)
{
	putInt(gameMode);
	putInt(matchMode);
	putInt(channelId);
}

CmdSendGetChannelInfo::CmdSendGetChannelInfo()
	:BaseOutMsg(pk::PacketID::CHANNEL_INFO)
{
}

CmdSendGetChannelInfo::~CmdSendGetChannelInfo()
{
}

void CmdSendGetChannelInfo::packData(int gameMode, int matchMode)
{
	putInt(gameMode);
	putInt(matchMode);
}

//CmdJoinRT3Room---------------------
CmdJoinRT3Room::CmdJoinRT3Room()
	:BaseOutMsg(pk::PacketID::JOIN_WAITING_ROOM)
{
}

CmdJoinRT3Room::~CmdJoinRT3Room()
{
}

void CmdJoinRT3Room::packData(int id)
{
	putInt(id);
}

CmdInviteFriend::CmdInviteFriend()
	:BaseOutMsg(pk::PacketID::INVITE_FRIEND)
{
}

CmdInviteFriend::~CmdInviteFriend()
{
}

void CmdInviteFriend::packData(int id)
{
	putInt(id);
}

CmdSendAcceptInviteJoinRT3::CmdSendAcceptInviteJoinRT3()
	:BaseOutMsg(PacketID::CONFIRM_INVITE_FRIEND)
{
}

CmdSendAcceptInviteJoinRT3::~CmdSendAcceptInviteJoinRT3()
{
}

void CmdSendAcceptInviteJoinRT3::packData(int ownerId, bool isAccept)
{
	putInt(ownerId);
	putByte(isAccept);
}

CmdKickPlayerRT3Room::CmdKickPlayerRT3Room()
	:BaseOutMsg(PacketID::KICK_PLAYER)
{
}

CmdKickPlayerRT3Room::~CmdKickPlayerRT3Room()
{
}

void CmdKickPlayerRT3Room::packData(int pId)
{
	putInt(pId);
}
