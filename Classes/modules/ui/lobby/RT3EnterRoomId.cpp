#include "RT3EnterRoomId.h"
#include "../../../data/Resource.h"
#include "../../../core/utils/StringUtility.h"
#include "../../network/match/MatchHandler.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

RT3EnterRoomId::RT3EnterRoomId()
{
	_roomId = 0;
}

RT3EnterRoomId::~RT3EnterRoomId()
{

}

bool RT3EnterRoomId::init()
{
	if (!UIBaseLayer::init())
	{
		return false;
	}

	initWithBinaryFile(Res::GUI::getCsbPathFromJson(Res::GUI::RT3EnterRoomID));

	initGUI();
	return true;
}

void RT3EnterRoomId::initGUI()
{
	initTag();
	autoInitComponent(_layout);
	_nodeMap["fake"]->setVisible(false);
	
	_lbRoomId = (Text*)_nodeMap["lbRoomId"];
	_lbTitle = (Text*)_nodeMap["lbTitle"];
}

void RT3EnterRoomId::initTag()
{
	_tagMap["btnOne"] = btnOne;
	_tagMap["btnTwo"] = btnTwo;
	_tagMap["btnThree"] = btnThree;
	_tagMap["btnFour"] = btnFour;
	_tagMap["btnFive"] = btnFive;
	_tagMap["btnSix"] = btnSix;
	_tagMap["btnSeven"] = btnSeven;
	_tagMap["btnEight"] = btnEight;
	_tagMap["btnNine"] = btnNine;
	_tagMap["btnZero"] = btnZero;
	_tagMap["btnBackSpace"] = btnBackSpace;
	_tagMap["btnOk"] = btnOk;
	_tagMap["btnBack"] = btnBack;
}

void RT3EnterRoomId::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	switch (id)
	{
	case btnZero:
	case btnOne:
	case btnTwo:
	case btnThree:
	case btnFour:
	case btnFive:
	case btnSix:
	case btnSeven:
	case btnEight:
	case btnNine:
		updateRoomId(id);
		break;
	case btnBackSpace:
		backSpace();
		break;
	case btnOk:
		joinRoom(_roomId);
		break;
	case btnBack:
		setVisible(false);
		break;
	default:
		break;
	}
}

void RT3EnterRoomId::updateRoomId(int newCharacter)
{
	if (_roomId < MAX_ROOM_ID)
	{
		_roomId = _roomId * 10 + newCharacter;
	}
	
	updateRoomLabel();
}

void RT3EnterRoomId::backSpace()
{
	_roomId = _roomId / 10;
	updateRoomLabel();
}

void RT3EnterRoomId::updateRoomLabel()
{
	string roomIdStr = to_string(_roomId);
	StringUtility::fillChar(roomIdStr, 6, '0');
	_lbRoomId->setString(roomIdStr);
}

void RT3EnterRoomId::joinRoom(int roomId)
{
	matchHandler->sendJoinRT3Room(roomId);
	setVisible(false);
}

