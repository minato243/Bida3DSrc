#include "MatchHandler.h"
#include "MatchingOutMsg.h"
#include <lobby/network/GameClient.h>
#include <core/PacketsFactory.h>
#include "MatchingInPacket.h"
#include <core/GameMgr.h>
#include "../../ui/lobby/RT3LobbyLayer.h"

USING_NS_CC;
using namespace pk;

MatchHandler::MatchHandler()
	:BaseHandler()
{


}

MatchHandler::~MatchHandler()
{
}

bool MatchHandler::onReceived(fr::InPacket * pk)
{
	int cmd = pk->getCmdId();
	int controllerID = pk->getControllerId();

	switch (cmd)
	{
		case PacketID::QUICK_MATCH:
		{
			CCLOG("MatchHandler::onReceived -------------- QUICK_MATCH");
			break;
		}

		case PacketID::RESPONSE_JOIN_ROOM:
		{
			CCLOG("MatchHandler::onReceived -------------- RESPONSE_JOIN_ROOM");
			ResponseJoinRoomMsg *msg = new ResponseJoinRoomMsg(pk);
			if (msg->error == 0)
			{
				msg->readData();
				gameMgr->_ebpCtrl->onResponeQuickMatch(msg);
			}
			else guiMgr->showMessagePopup("Error", msg->getStatus(), NULL);
			break;
		}

		case PacketID::CANCEL_MATCHING:
		{
			CCLOG("MatchHandler::onReceived -------------- CANCEL_MATCHING");
			break;
		}

		case  PacketID::CHANNEL_INFO:
		{
			CCLOG("MatchHandler::onReceived -------------- CHANNEL_INFO");
			ChannelInfoMsg *msg = new ChannelInfoMsg(pk);
			msg->readData();
			if (msg->error == 0)
			{
				gameMgr->_ebpCtrl->receiveChannelInfo(msg->ccu);
			}
			CC_SAFE_DELETE(msg);
			break;
		}

		case JOIN_WAITING_ROOM:
		{
			CCLOG("MatchHandler::onReceived -------------- JOIN_WAITING_ROOM");
			JoinWaitingRoomMsg *msg = new JoinWaitingRoomMsg(pk);
			msg->readData();
			if (msg->error == 0)
			{
				auto rt3Gui = (RT3LobbyLayer *)guiMgr->getGui(Popup::RT3_FIND_MATCH_LAYER);
				if (!rt3Gui->_isShow)
					rt3Gui->show();
				rt3Gui->setRoomInfo(msg);
			}
			else
			{
				guiMgr->showMessagePopup("Error", msg->getStatus(), NULL);
			}
			CC_SAFE_DELETE(msg);
			break;
		}

		case MATCHING_ROOM_RT3:
		{
			CCLOG("MatchHandler::onReceived -------------- MATCHING_ROOM_RT3");
			MatchingRoomRT3Msg *msg = new MatchingRoomRT3Msg(pk);
			msg->readData();
			if (msg->error == 0)
			{
				
			}
			else if (msg->error == 1 || msg->error == 4)
			{
				auto rt3Gui = (RT3LobbyLayer *)guiMgr->getGui(Popup::RT3_FIND_MATCH_LAYER);
				if(rt3Gui->_isShow) rt3Gui->hide();
			}
			CC_SAFE_DELETE(msg);
			break;
		}

		case INVITE_FRIEND:
		{
			CCLOG("MatchHandler::onReceived -------------- MATCHING_ROOM_RT3");
			InviteFriendMsg *msg = new InviteFriendMsg(pk);
			msg->readData();
			if (msg->error != 0)
			{
				guiMgr->showMessagePopup("Error", msg->getStatus(), NULL);
			}
			CC_SAFE_DELETE(msg);
			break;
		}

	default:
		return false;
		break;
	}
	return true;
}

void MatchHandler::fetchChannelInfo(int gameMode, int matchMode)
{
	CmdSendGetChannelInfo *msg = new CmdSendGetChannelInfo();
	msg->packData(gameMode, matchMode);
	send(msg);
	CC_SAFE_DELETE(msg);
}

void MatchHandler::requestQuickMatch(int gameMode, int matchMode, int channelId)
{
	CmdSendQuickMatch *msg = new CmdSendQuickMatch();
	msg->packData(gameMode, matchMode, channelId);
	send(msg);
	CC_SAFE_DELETE(msg);
}

void MatchHandler::requestCancelMatching()
{
	fr::OutPacket *msg = PacketsFactory::getOutPacket(PacketID::CANCEL_MATCHING);
	send(msg);
	CC_SAFE_DELETE(msg);
}

void MatchHandler::sendJoinRT3Room(int roomId)
{
	CmdJoinRT3Room *msg = new CmdJoinRT3Room();
	msg->packData(roomId);
	send(msg);
	CC_SAFE_DELETE(msg);
}

void MatchHandler::sendInviteFriend(int friendId)
{
	CmdInviteFriend *msg = new CmdInviteFriend();
	msg->packData(friendId);
	send(msg);
	CC_SAFE_DELETE(msg);
}

void MatchHandler::sendLeaveRT3Room()
{
	fr::OutPacket *msg = PacketsFactory::getOutPacket(PacketID::LEAVE_ROOM);
	send(msg);
	CC_SAFE_DELETE(msg);
}

void MatchHandler::sendFindMatchRT3()
{
	fr::OutPacket *msg = PacketsFactory::getOutPacket(PacketID::MATCHING_ROOM_RT3);
	send(msg);
	CC_SAFE_DELETE(msg);
}

void MatchHandler::sendAcceptJoin(int ownerId, bool accept)
{
	CmdSendAcceptInviteJoinRT3 *msg = new CmdSendAcceptInviteJoinRT3();
	msg->packData(ownerId, accept);
	send(msg);
	CC_SAFE_DELETE(msg);
}

void MatchHandler::sendKickPlayerRT3(int id)
{
	CmdKickPlayerRT3Room *msg = new CmdKickPlayerRT3Room();
	msg->packData(id);
	send(msg);
	CC_SAFE_DELETE(msg);
}

void MatchHandler::send(fr::OutPacket * msg)
{
	GameClient::getInstance()->sendPacket(msg);
}

MatchHandler * MatchHandler::_instance = nullptr;

MatchHandler * MatchHandler::getInstance()
{
	if (_instance == nullptr) 
	{
		_instance = new MatchHandler();
	}

	return _instance;
}
