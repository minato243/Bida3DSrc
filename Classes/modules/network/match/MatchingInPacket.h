#ifndef MATCHING_IN_PACKET_H_
#define MATCHING_IN_PACKET_H_

#include <BaseFramework.h>
#include "../base/BasePacket.h"
#include <data/GameConstant.h>
#include <modules/core/match/Player.h>

class ResponseJoinRoomMsg
	:public BaseInPacket
{
public:
	ResponseJoinRoomMsg(fr::InPacket *pk);
	~ResponseJoinRoomMsg();

	void readData();

	void readPlayerData(Player & p);

	const Player & getOpponents();

	std::string getStatus();

public:
	int gameId;
	GameMode gameMode;
	int matchMode;
	int channelId;

	std::vector<Player> players;
};

class ChannelInfoMsg
	: public BaseInPacket
{
public:
	ChannelInfoMsg(fr::InPacket *pk);
	~ChannelInfoMsg();

	void readData();

public:
	int ccu;
	int estSec;
};

class InviteFriendMsg
	:public BaseInPacket
{
public:
	InviteFriendMsg(fr::InPacket *pk);
	~InviteFriendMsg();

	void readData();

	std::string getStatus();
};


#endif // !MATCHING_IN_PACKET_H_

