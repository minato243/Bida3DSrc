#include "MatchingInPacket.h"
#include <core/GameMgr.h>

ResponseJoinRoomMsg::ResponseJoinRoomMsg(fr::InPacket * pk)
	:BaseInPacket(pk)
{
}

ResponseJoinRoomMsg::~ResponseJoinRoomMsg()
{
}

void ResponseJoinRoomMsg::readData()
{
	int error = _pk->getError();
	if (error == 0) {
		gameId = _pk->getInt();
		gameMode = (GameMode)_pk->getByte();
		channelId = _pk->getByte();
		matchMode = _pk->getByte();

		int playerCount = _pk->getByte();
		CCLOG("player count: %d", playerCount);
		players.clear();

		for (int i = 0; i < playerCount; i++) {
			Player p;
			readPlayerData(p);
			players.push_back(p);
		}
	}
}

void ResponseJoinRoomMsg::readPlayerData(Player & player)
{
	player.id = _pk->getInt();
	player.team = _pk->getByte();
	player.name = _pk->getString();
	player.avatar = _pk->getString();
	player.gold = (int)(_pk->getLong());
	player.goldBet = (int)(_pk->getLong());
	player.cueId = _pk->getInt();
	player.rankId = _pk->getInt();
	player.glove = _pk->getInt();
	player.exp = _pk->getInt();
	// cc.log("read player: " + JSON.stringify(player));
	player.cues.clear();
	int bagSize = _pk->getByte();
	for (int i = 0; i < bagSize; i++) 
	{
		CueInfoData cue;
		cue.id = _pk->getInt();
		cue.configId = _pk->getInt();
		cue.type = _pk->getInt();
		cue.star = _pk->getInt();
		cue.maxStar = _pk->getInt();
		cue.level = _pk->getInt();
		cue.point = _pk->getInt();
		cue.accuracy = _pk->getDouble();
		player.cues.push_back(cue);
	}
}

const Player & ResponseJoinRoomMsg::getOpponents()
{
	for (int i = 0; i < players.size(); i++) 
	{
		if (players[i].id != gameMgr->_userInfo->_uId)
		{
			return players[i];
		}
	}
}

std::string ResponseJoinRoomMsg::getStatus()
{
	switch (error)
	{
	case 0:
		return "Success";
		
	default:
		return "Error " + std::to_string(error);
		break;
	}
	return "";
}

ChannelInfoMsg::ChannelInfoMsg(fr::InPacket *pk)
	:BaseInPacket(pk)
{
	
}

ChannelInfoMsg::~ChannelInfoMsg()
{

}

void ChannelInfoMsg::readData()
{
	if (error == 0) {
		ccu = _pk->getInt();
		estSec = _pk->getInt();
	}
}

InviteFriendMsg::InviteFriendMsg(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

InviteFriendMsg::~InviteFriendMsg()
{

}

void InviteFriendMsg::readData()
{

}

std::string InviteFriendMsg::getStatus()
{
	switch (error)
	{
	case 0: return "Success";
	case 1: return "User Invalid";
	case 4: return "Friend in another room";
	case 5: return "Friend in game";
	default:
		return "Error";
		break;
	}
}
