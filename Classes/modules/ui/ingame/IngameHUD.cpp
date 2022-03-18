#include "IngameHUD.h"
#include <data/GameConstant.h>
#include <modules/core/utils/TimeUtils.h>
#include <data/Resource.h>
#include <core/GameMgr.h>
#include "IngameUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

IngameHUD::IngameHUD()
{
	_curActivePlayer = -1;
	_modeFull = true;
	//interactions = []; todo
	
}

IngameHUD::~IngameHUD()
{
}

bool IngameHUD::init()
{
	if (!Node::init()) {
		return false;
	}
	
	return true;
}

void IngameHUD::initComponent(cocos2d::ui::Widget * hud, cocos2d::ui::ImageView * avatarBox, cocos2d::Node * playerNodes[])
{
	_hud = hud;
	_minimizedHud = avatarBox;
	_avatars[0] = playerNodes[0];
	_avatars[1] = playerNodes[1];

	_imgPanelP1 = (ImageView *)_hud->getChildByName("imgPanelP1");
	_imgPanelP2 = (ImageView *)_hud->getChildByName("imgPanelP2");

	initHUDBalls();
	initGameModeNodes();
	initAnimationsPos();
	setModeFull(false);
	setGameMode(RT1);
	scheduleUpdate();
}

void IngameHUD::initAnimationsPos()
{
	auto p1 = _imgPanelP1;
	auto p2 = _imgPanelP2;

	_showPosition1 = p1->getPosition();
	_showPosition2 = p2->getPosition();
	_hidePosition1 = Vec2(0, _showPosition1.y);
	_hidePosition2 = Vec2(0, _showPosition2.y);

	for (int i = 0; i < _gameModeNodes.size(); i++) {
		Vec2 pos = _gameModeNodes[i]->getPosition();
		_gameModeShowPositions.push_back(pos);
		_gameModeHidePositions.push_back(Vec2(0, pos.y));
	}
}

void IngameHUD::initHUDBalls()
{
	char text[20];
	char boxText[10];
	for (auto pId = 0; pId < 2; pId++) {
		sprintf(text, "imgPanelP%d", (pId + 1));
		auto parentNode = _hud->getChildByName(text);
		auto nodeBalls = parentNode->getChildByName("nodeBalls");
		for (auto i = 0; i < 7; i++) {
			auto sprite = Sprite::create();
			sprintf(boxText, "box_%d", i);
			auto box = nodeBalls->getChildByName(boxText);
			nodeBalls->addChild(sprite, box->getLocalZOrder()+1, "sprite_"+ std::to_string(i));
			sprite->setPosition(box->getPosition());
		}
	}
}

void IngameHUD::initGameModeNodes()
{
	const int numChild = 4;
	std::string childNames[numChild] = {"imgGameModeRT1", "imgGameModeS2", "imgGameModeRT2", "imgGameModeRT3"};
	for (int i = 0; i < numChild; i++) {
		ImageView *imgGameMode = (ImageView*)_hud->getChildByName("imgGameModeRT1");
		_gameModeNodes.push_back(imgGameMode);
	}
}

void IngameHUD::scheduleAutoHide()
{
	if (!_modeFull) return;

	if (_interactions.size() > 0 && _interactions[_interactions.size() - 1] < timeUtils->current() - 2000) {
		_interactions.clear();
	}
	_interactions.push_back(timeUtils->current());
	_lastInteraction = timeUtils->current();
	if (_interactions[_interactions.size() - 1] - _interactions[0] > IngameHUD::AUTO_HIDE_THRESHOLD_AFTER_TOUCH && _interactions.size() > 10) {
		setModeFull(false);
	}
}

void IngameHUD::setModeFull(bool full)
{
//	if (_modeFull == full) {
//		return;
//	}
//	auto ANIM_TIME = 0.25;
//	auto p1 = _imgPanelP1;
//	auto p2 = _imgPanelP2;
//	auto avatar0 = _avatars[0];
//	auto avatar1 = _avatars[1];
//	_minimizedHud->setVisible(!full);
//	_minimizedHud->getParent()->getChildByName("btnExpand")->setVisible(!full);
//	_interactions.clear();
//	avatar0->getChildByName("timer")->setVisible(_curActivePlayer == 0);
//	avatar1->getChildByName("timer")->setVisible(_curActivePlayer == 1);
//	if (full) {
//		_lastInteraction = timeUtils->current();
//		avatar0->setVisible(true);
//
//		p1->runAction(FadeTo::create(ANIM_TIME, 255));
//		p2->runAction(FadeTo::create(ANIM_TIME, 255));
//
//		p1->setPosition(_hidePosition1);
//		p1->setScaleX(0);
//		p1->runAction(Spawn::create(MoveTo::create(ANIM_TIME, _showPosition1), ScaleTo::create(ANIM_TIME, 1, 1)));
//
//		p2->setPosition(_hidePosition2);
//		p2->setScaleX(0);
//		p2->runAction(Spawn::create(MoveTo::create(ANIM_TIME, _showPosition2), ScaleTo::create(ANIM_TIME, 1, 1)));
//
//		avatar1->setVisible(true);
//		// avatar1->timer.setVisible(true);
//
//		if (_curActivePlayer == 0)
//			avatar1->setOpacity(0);
//		avatar1->setScaleX(-1);
//		avatar1->getChildByName("timeAlert")->setScaleX(-1);
//		avatar1->getChildByName("imgBtnExt")->setScaleX(-1);
//		avatar1->runAction(Spawn::create(
//			Sequence::create(FadeTo::create(ANIM_TIME, 255), 
//				CallFunc::create([&avatar1]()
//					{
//						avatar1->setOpacity(255); 
//					}
//				)),
//			MoveTo::create(ANIM_TIME, gameMgr->_ingameUI->getNodeP2ShowPosition())
//		));
//
//		for (int i = 0; i < _gameModeNodes.size(); i++) 
//		{
//			auto node = _gameModeNodes[i];
//			node->setScaleX(0);
//			node->setPosition(_gameModeHidePositions[i]);
//			node->runAction(Spawn::create(
//				MoveTo::create(ANIM_TIME, _gameModeShowPositions[i]), 
//				ScaleTo::create(ANIM_TIME, 1, 1), 
//				FadeTo::create(ANIM_TIME, 255)
//			));
//		}
//	}
//	else {
//		avatar0->setVisible(_curActivePlayer == 0);
//		p1->runAction(FadeTo::create(ANIM_TIME, 0));
//		p2->runAction(FadeTo::create(ANIM_TIME, 0));
//
//		p1->setPosition(_showPosition1);
//		p1->setScaleX(1);
//		p1->runAction(Spawn::create(MoveTo::create(ANIM_TIME, _hidePosition1), ScaleTo::create(ANIM_TIME, 0, 1), nullptr));
//
//		p2->setPosition(_showPosition2);
//		p2->setScaleX(1);
//		p2->runAction(Spawn::create(MoveTo::create(ANIM_TIME, _hidePosition2), ScaleTo::create(ANIM_TIME, 0, 1), nullptr));
//
//		avatar1->setScaleX(1);
//		avatar1->getChildByName("timeAlert")->setScaleX(1);
//		avatar1->getChildByName("imgBtnExt")->setScaleX(1);
//		CUSTOM_CALLBACK callback = std::bind(&IngameHUD::setModeFullComplete, this, avatar1);
//		auto action = Sequence::create(
//			FadeTo::create(ANIM_TIME, 0),
//			CallFunc::create(callback), 
//			nullptr
//		);
//		avatar1->runAction(Spawn::create(
//			_curActivePlayer == 1 ? DelayTime::create(0) : (FiniteTimeAction*)action,
//			MoveTo::create(ANIM_TIME,((IngameUINew*)_ingameUI)->getNodeP2HidePosition()),
//			nullptr
//		));
//
//		for (int i = 0; i < _gameModeNodes.size(); i++) 
//		{
//			auto node = _gameModeNodes[i];
//			node->runAction(
//				Spawn::create(
//					MoveTo::create(ANIM_TIME, _gameModeHidePositions[i]), 
//					ScaleTo::create(ANIM_TIME, 0, 1), 
//					FadeTo::create(ANIM_TIME, 0),
//					nullptr
//				)
//			);
//		}
//	}
//	_modeFull = full;
}

void IngameHUD::setGameMode(int mode)
{
	std::string gameModeName;
	switch (mode) {
	case RT1: gameModeName = "RT1"; break;
	case RT2_MODE: gameModeName = "RT2"; break;
	case RT3_MODE: gameModeName = "RT3"; break;
	case STREAK_MODE: gameModeName = "S2"; break;
	}
	auto node = _hud->getChildByName("imgGameMode" + gameModeName);
	_gameMode = mode;
	CCLOG("GAME MODE %d", mode);
	for (int i = 0; i < _gameModeNodes.size(); i++)
	{
		_gameModeNodes[i]->setVisible(false);
	}
	node->setVisible(true);
	_imgPanelP1->getChildByName("nodeEightBall")->setVisible(false);
	_imgPanelP2->getChildByName("nodeEightBall")->setVisible(false);
}

void IngameHUD::update(float dt)
{
	auto curTime = timeUtils->current();
	auto time = 2 - (curTime % 2000) / 1000;
	char text[20];
	for (auto i = 1; i <= 2; i++) {
		sprintf(text, "imgPanelP%d", i);
		auto node = _hud->getChildByName(text)->getChildByName("nodeEightBall");
		for (auto j = 1; j <= 6; j++) {
			node->getChildByName("green"+std::to_string(j))->setOpacity(255 - 255 / 3 * (1 + abs((j - 1) % 3 - time)));
		}
	}
	if (_modeFull && _lastInteraction < curTime - IngameHUD::AUTO_HIDE_THRESHOLD) {
		setModeFull(false);
	}
}

void IngameHUD::resetActivePlayer()
{
	_curActivePlayer = -1;
}

void IngameHUD::setActivePlayer(int idx, Player info)
{
	if (_curActivePlayer == idx) return;
	_curActivePlayer = idx;

	CCLOG("set active player %d", idx);
	// Chi co 2 avatar nhung idx = 2 nen bi loi TODO
	_avatars[idx]->getChildByName("timer")->setVisible(true);
	_avatars[1 - idx]->getChildByName("timer")->setVisible(false);
	if (_modeFull) {
	}
	else {
		_avatars[idx]->setVisible(true);
		_avatars[1 - idx]->setVisible(false);
	}

	//todo
	//if (idx == 0) {
	//	((Text*)_hud->getChildByName("imgPanelp1")->getChildByName("lbName"))->setSttring(_players[info.realIndex].name);
	//	CCLOG("PLAYER name", _players[info.realIndex].name);
	//}
}

void IngameHUD::updateRemainingBalls(int pid, std::vector<int> balls)
{ 
	auto parentNode = _imgPanelP1;
	if (pid == 1) parentNode = _imgPanelP2;
	auto nodeBalls = parentNode->getChildByName("nodeBalls");
	auto nodeEightBall = parentNode->getChildByName("nodeEightBall");
	if (balls.size() == 1 && balls[0] == 8) {
		nodeEightBall->setVisible(true);
		nodeBalls->setVisible(false);
	}
	else {
		nodeEightBall->setVisible(false);
		nodeBalls->setVisible(true);

		char boxText[20];
		for (auto i = 0; i < 7; i++) {
			sprintf(boxText, "sprite_%d", i);
			if (i >= balls.size()) {
				nodeBalls->getChildByName(boxText)->setVisible(false);
			}
			else {
				auto sprite = (Sprite*)nodeBalls->getChildByName(boxText);
				sprite->setVisible(true);
				sprite->setTexture(Res::BALLNUMS[balls[i] -1]);
			}
		}
	}
}

void IngameHUD::setIngameUI(cocos2d::Node * ingameUI)
{
	_ingameUI = ingameUI;
}

void IngameHUD::setModeFullComplete(cocos2d::Node * avatar1)
{
	avatar1->setOpacity(255);
	avatar1->setVisible(_curActivePlayer == 1);
}
