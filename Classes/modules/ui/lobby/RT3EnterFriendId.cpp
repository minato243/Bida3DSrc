#include "RT3EnterFriendId.h"
#include "../../../data/Resource.h"
#include "../../network/match/MatchHandler.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

RT3EnterFriendId::RT3EnterFriendId()
{

}

RT3EnterFriendId::~RT3EnterFriendId()
{

}

bool RT3EnterFriendId::init()
{
	if (!UIBaseLayer::init())
	{
		return false;
	}

	initWithBinaryFile(Res::GUI::getCsbPathFromJson(Res::GUI::RT3EnterFriendID));

	initGUI();
	return true;
}

void RT3EnterFriendId::initGUI()
{
	initTag();
	autoInitComponent(_layout);

	_UserId = (TextField *)_nodeMap["tfUserId"];
}

void RT3EnterFriendId::initTag()
{
	_tagMap["btnSubmit"] = btnSubmit;
	_tagMap["btnBack"] = btnBack;
}

void RT3EnterFriendId::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	switch (id)
	{
	case TAG::btnBack:
	{
		setVisible(false);
		break;
	}
	case TAG::btnSubmit:
	{
		inviteFriend();
		break;
	}
	default:
		break;
	}
}

void RT3EnterFriendId::inviteFriend()
{
	auto idStr = _UserId->getString();
	int userId = atoi(idStr.c_str());
	matchHandler->sendInviteFriend(userId);
	setVisible(false);
}

