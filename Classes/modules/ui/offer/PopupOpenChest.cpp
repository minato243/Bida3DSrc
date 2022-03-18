#include "PopupOpenChest.h"
#include "../../../data/Resource.h"
#include "../GuiMgr.h"
#include "../../../core/GameMgr.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

PopupOpenChest::PopupOpenChest()
{

}

PopupOpenChest::~PopupOpenChest()
{

}

bool PopupOpenChest::init()
{
	if (!UIBaseLayer::init("PopupOpenChest"))
		return false;

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::PopupOpenChest);
	this->initWithBinaryFile(filePath);
	return true;
}

void PopupOpenChest::initGUI()
{
	autoInitComponent(_layout);
	_layerColor->setVisible(true);
	_layerColor->setOpacity(200);

	_imgChest = (ImageView *)_nodeMap["imgChest"];
	_imgChestOpen = (Sprite *)_nodeMap["imgChestOpen"];

	_nodeStar = _nodeMap["nodeStar"];
	_nodeCard = _nodeMap["nodeCard"];
	_nodeGold = _nodeMap["nodeGold"];

	_giftNodes = {_nodeStar, _nodeCard, _nodeGold};
}

void PopupOpenChest::runAnimation()
{
	_imgChestOpen->setOpacity(0);
	_imgChest->setOpacity(255);

	auto action = Sequence::create(
		CallFunc::create(CC_CALLBACK_0(PopupOpenChest::runActionRingChest, this)),
		DelayTime::create(1.5),
		CallFunc::create(CC_CALLBACK_0(PopupOpenChest::runActionOpenChest, this)),
		DelayTime::create(1),
		CallFunc::create(CC_CALLBACK_0(PopupOpenChest::runReceiveGift, this)),
		DelayTime::create(1),
		CallFunc::create(CC_CALLBACK_0(PopupOpenChest::updateInventory, this)),
		NULL
	);
	runAction(action);
}

void PopupOpenChest::runActionRingChest()
{
	auto action = Repeat::create(
		Sequence::create(
			Spawn::create(
				RotateTo::create(0.2, 5),
				MoveBy::create(0.2, Vec2(0, 5))
			),
			DelayTime::create(0.05),
			Spawn::create(
				RotateTo::create(0.2, -5),
				MoveBy::create(0.2, Vec2(0, -5))
			),
			DelayTime::create(0.05),
			NULL
		),
		3
	);
	
	_imgChest->runAction(action);

	_layerColor->runAction(FadeOut::create(1));
}

void PopupOpenChest::runActionOpenChest()
{
	_imgChest->setRotation(0);
	_imgChest->runAction(FadeOut::create(0.5));
	_imgChestOpen->runAction(FadeIn::create(0.5));

	for (auto i = 0; i < _giftNodes.size(); i++)
	{
		auto node = _giftNodes[i];
		node->runAction(
			Spawn::create(
				ScaleTo::create(1, 0.8),
				MoveBy::create(1, Vec2(50 * (i - 1), 0)), 
				NULL
			)
		);
	}
}

void PopupOpenChest::runReceiveGift()
{
	for (auto i = 0; i < _giftNodes.size(); i++)
	{
		flyToPos(_giftNodes[i], Vec2(0, 640), i == 0);
	}
}

void PopupOpenChest::updateInventory()
{
	gameMgr->_userInfo->addGold(500000);
	guiMgr->refreshAllGui();
	hide();
}

void PopupOpenChest::flyToPos(cocos2d::Node *node, cocos2d::Vec2 pos, bool isStar)
{
	auto startPos = node->getPosition();
	float rate = rand() % 100;
	ccBezierConfig bezier;
	bezier.endPosition = pos;
	bezier.controlPoint_1 = startPos;
	bezier.controlPoint_2 = Vec2((startPos.x * rate + pos.x * (100 -rate)) /100, (startPos.y * rate + pos.y * (100 - rate)) / 100 - 100);
	
	auto action = Sequence::create(
		EaseIn::create(BezierTo::create(1, bezier),2),
		FadeOut::create(0.5),
		NULL
	);
	node->runAction(action);
}

void PopupOpenChest::show()
{
	UIBaseLayer::show();
	runAnimation();
}

