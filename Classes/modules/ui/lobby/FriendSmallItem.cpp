#include "FriendSmallItem.h"
#include "../../../data/Resource.h"

USING_NS_CC;
using namespace cocos2d::ui;

FriendSmallItem::FriendSmallItem()
{

}

FriendSmallItem::~FriendSmallItem()
{

}

bool FriendSmallItem::init()
{
	if (!UIBaseLayer::init())
		return false;

	initWithBinaryFile(Res::GUI::getCsbPathFromJson(Res::GUI::FriendSmallItem));
	return true;

}

void FriendSmallItem::initGUI()
{
	initTag();
	autoInitComponent(_layout);

	_bg = (Widget *)_nodeMap["bg"];
}

void FriendSmallItem::initTag()
{

}

void FriendSmallItem::onButtonRelease(cocos2d::ui::Button* button, int id)
{

}

void FriendSmallItem::loadData(FriendData *data)
{
}

cocos2d::Size FriendSmallItem::getOriginalSize()
{
	return _bg->getContentSize();
}

