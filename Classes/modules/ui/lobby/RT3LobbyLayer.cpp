#include "RT3LobbyLayer.h"
#include "../../../data/Resource.h"
#include "../../core/utils/TimeUtils.h"
#include "../../../core/utils/StringUtility.h"
#include "../../../base/UIFactory.h"
#include "../../../core/utils/LanguageMgr.h"
#include "../../../core/GameMgr.h"
#include "FriendSmallItem.h"
#include "../elements/QuickCustomTableView.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

RT3LobbyLayer::RT3LobbyLayer()
{
	_keep = true;
	_startClockTime = 0;
	_isHost = true;
	_isShowListFriend = false;
	_enterRoomIdPopup = NULL;
	_inviteFriendPopup = NULL;
}

RT3LobbyLayer::~RT3LobbyLayer()
{
	_enterRoomIdPopup = NULL;
}

bool RT3LobbyLayer::init()
{
	if (!UIBaseLayer::init())
	{
		return false;
	}

	initWithBinaryFile(Res::GUI::getCsbPathFromJson(Res::GUI::RT3LobbyLayer));

	for (auto i = 0; i < NUM_PLAYER; i++)
	{
		auto slot = PlayerInfoItem::create();
		slot->setDelegate(this);
		_playerNodes[i]->addChild(slot);
		_playerSlots.push_back(slot);
		_playerNodes[i]->setLocalZOrder(2);
		slot->setSide(i < 2);
		slot->setUnknown();
	}

	initBgListFriend();

	return true;
}

void RT3LobbyLayer::initGUI()
{
	initTag();
	autoInitComponent(_layout);

	_lbTime = (Text*)_nodeMap["lbTime"];
	_nodeP1 = _nodeMap["nodeP1"];
	_nodeP2 = _nodeMap["nodeP2"];
	_nodeP3 = _nodeMap["nodeP3"];
	_nodeP4 = _nodeMap["nodeP4"];
	_playerNodes[0] = _nodeP1; _playerNodes[1] = _nodeP2; 
	_playerNodes[2] = _nodeP3; _playerNodes[3] = _nodeP4;

	_bgListFriend = (ImageView *)_nodeMap["bgListFriend"];
	_btnFriendList = (Button*)_nodeMap["btnListFriend"];

	_lbRoomId = (cocos2d::ui::Text *) _nodeMap["lbRoomId"];
	_btnFind = (cocos2d::ui::Button *) _nodeMap["btnFind"];
}

void RT3LobbyLayer::initTag()
{
	_tagMap.insert(pair<string, int>("btnBack", btnBack));
	_tagMap.insert(pair<string, int>("btnFind", btnFind));
	_tagMap.insert(pair<string, int>("btnJoinRoom", btnJoinRoom));
	_tagMap["btnListFriend"] = btnListFriend;
}

void RT3LobbyLayer::initEnterRoomId()
{
	_enterRoomIdPopup = RT3EnterRoomId::create();
	addChild(_enterRoomIdPopup);
}

void RT3LobbyLayer::updateClock(float dt)
{
	auto cur = timeUtils->current();
	auto time = cur - _startClockTime;

	_lbTime->setString(StringUtility::getShortTimeString(time));
}

void RT3LobbyLayer::startClock()
{
	_startClockTime = timeUtils->current();
	schedule(schedule_selector(RT3LobbyLayer::updateClock), 0.5);
}

void RT3LobbyLayer::stopClock()
{
	unschedule(schedule_selector(RT3LobbyLayer::updateClock));
}

void RT3LobbyLayer::onEnter()
{
	UIBaseLayer::onEnter();
}

void RT3LobbyLayer::setRoomInfo(JoinWaitingRoomMsg *msg)
{
	_roomId = msg->roomId;
	_hostId = msg->hostId;
	_isHost = msg->hostId == gameMgr->_userInfo->_uId;

	_lbRoomId->setString(languageMgr->localizedText("Room") + ":" + to_string(_roomId));
	loadData(msg->players);
}

void RT3LobbyLayer::loadData(const std::vector<Player> & players)
{
	if (_isHost)
	{
		auto size = _btnFind->getSize();
		UIFactory::loadButtonTextures(_btnFind, "Button_Yellow.png");
		_btnFind->setTouchEnabled(true);
		_btnFind->setSize(size);
	}
	else
	{
		auto size = _btnFind->getSize();
		UIFactory::loadButtonTextures(_btnFind, "Button_Gray.png");
		_btnFind->setTouchEnabled(false);
		_btnFind->setSize(size);
	}

	for (auto i = 0; i < NUM_PLAYER; i++)
	{
		if(i < players.size())
			_playerSlots[i]->loadData(&players.at(i), _isHost);
		else _playerSlots[i]->loadData(NULL, false);
	}
}

void RT3LobbyLayer::initBgListFriend()
{
	CCLOG("RT3LobbyLayer::initBgListFriend TODO");
	auto pnContent = getControl("pnContent", _bgListFriend);
	auto listFriend = new QuickCustomTableView<FriendData, FriendSmallItem>();
	listFriend->init(pnContent->getContentSize(), extension::ScrollView::Direction::VERTICAL, -1, 1);

	std::vector<FriendData *> listDataFriend;
	for (int i = 0; i < 5; i++)
	{
		FriendData *data = new FriendData(i, "userName");
		listDataFriend.push_back(data);
	}
	listFriend->reloadData(listDataFriend);

	pnContent->addChild(listFriend);
	_bgListFriend->setLocalZOrder(2);
}

void RT3LobbyLayer::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	switch (id)
	{
	case TAG::btnBack:
	{
		leave();
		break;
	}
	case TAG::btnFind:
	{
		startClock();
		matchHandler->sendFindMatchRT3();
		break;
	}

	case TAG::btnJoinRoom:
	{
		if (_enterRoomIdPopup == NULL)
		{
			initEnterRoomId();
		}
		_enterRoomIdPopup->setVisible(true);

		break;
	}

	case TAG::btnListFriend:
	{
		CCLOG("RT3LobbyLayer::onButtonRelease btnJoinRoom TODO");
		moveListFriend();
		break;
	}
	default:
		break;
	}
}

void RT3LobbyLayer::leave()
{
	matchHandler->sendLeaveRT3Room();
	gameMgr->_ebpCtrl->cancelRT3Matching();
	hide();
}

void RT3LobbyLayer::showInviteFriendPopup()
{
	if (!_inviteFriendPopup)
	{
		_inviteFriendPopup = RT3EnterFriendId::create();
		addChild(_inviteFriendPopup);
	}
	_inviteFriendPopup->setVisible(true);
}

void RT3LobbyLayer::onKickPlayer(int userId, const std::string & userName)
{
	std::string confirmMsg = languageMgr->localizedText("text_confirm_kick_player");
	StringUtility::replaceAll(confirmMsg, "@0", userName);
	
	MessagePoup::PopupListener callback = std::bind(&RT3LobbyLayer::onConfirm, this, placeholders::_1, userId);
	guiMgr->showOkCancelPopup(
		languageMgr->localizedText("kick_player"),
		confirmMsg,
		callback,
		languageMgr->localizedText("kick_player"),
		languageMgr->localizedText("kick_player")
		);

}

void RT3LobbyLayer::onConfirm(int btnId, int userId)
{
	if (btnId == MessagePoup::TAG::btnPositive) {
		doKickPlayer(userId);
	}
}

void RT3LobbyLayer::doKickPlayer(int userId)
{
	matchHandler->sendKickPlayerRT3(userId);
}

void RT3LobbyLayer::moveListFriend()
{
	float height = _bgListFriend->getContentSize().height * .9;
	int dir = _isShowListFriend ? -1 : 1;
	Vec2 pos = Vec2(_bgListFriend->getPositionX(), _bgListFriend->getPositionY() + height * dir);
	_bgListFriend->runAction(MoveTo::create(0.5, pos));
	_btnFriendList->setRotation(_isShowListFriend?180:0);
	_isShowListFriend = !_isShowListFriend;
}

