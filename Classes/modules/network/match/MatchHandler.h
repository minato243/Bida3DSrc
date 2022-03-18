#ifndef MATCH_HANDLER_H_
#define MATCH_HANDLER_H_

#include "../PacketIDDefine.h"
#include <BaseFramework.h>
#include "MatchingInPacket.h"
#include "MatchingOutMsg.h"
#include "../base/BaseHandler.h"

#define matchHandler MatchHandler::getInstance()

class MatchHandler
	:public BaseHandler
{
public:
	MatchHandler();
	~MatchHandler();

	bool onReceived(fr::InPacket* pk);

	void fetchChannelInfo(int gameMode, int matchMode);

	void requestQuickMatch(int gameMode, int matchMode, int channelId);

	void requestCancelMatching();

	void sendJoinRT3Room(int roomId);

	void sendInviteFriend(int friendId);

	void sendLeaveRT3Room();

	void sendFindMatchRT3();

	void sendAcceptJoin(int ownerId, bool accept);

	void sendKickPlayerRT3(int id);

private:
	static MatchHandler *_instance;

public:
	static void send(fr::OutPacket *msg);

	static MatchHandler *getInstance();
};

#endif // !MATCH_HANDLER_H_


