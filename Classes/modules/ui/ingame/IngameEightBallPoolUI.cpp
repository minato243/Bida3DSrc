#include "IngameEightBallPoolUI.h"
#include <data/Resource.h>
#include "../../core/utils/Utils.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

IngameEightBallPoolUI::IngameEightBallPoolUI()
{
	_showedEfxEnemyBalls = false;
	_effectShowBallType = false;

	_timeInLastUpdate = 0;
	_curPlayerIdx = 0;
	_match = NULL;
}

IngameEightBallPoolUI::~IngameEightBallPoolUI()
{
}

bool IngameEightBallPoolUI::init()
{
	IngameBaseUI::init();

	auto size = Director::getInstance()->getVisibleSize();
	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::IngameEBPUI);
	_hud = CSLoader::createNode(filePath);
	_hud->setContentSize(size);
	Helper::doLayout(_hud);
	addChild(_hud);
	autoInitComponent(_hud);
	localizedText(_hud);

	initHUD();

	initRemainBalls();
	initAvatars();
	initTimers();
	initUIStatesPos2();

	initEightBallEffect();

	return true;
}

void IngameEightBallPoolUI::initHUD()
{
	_hud->getChildByName("fake")->setVisible(false);
	auto bgHUD = getControl("bg", _hud);
	_imgP1 = (Widget *)getControl("imgP1", bgHUD);
	_nodeRemainBall1 = getControl("nodeRemainBalls", _imgP1);
	_nodeEightBall1 = getControl("nodeEightBall", _imgP1);
	
	_imgP2 = (Widget *)getControl("imgP2", bgHUD);
	_nodeRemainBall2 = getControl("nodeRemainBalls", _imgP2);
	_nodeEightBall2 = getControl("nodeEightBall", _imgP2);

	_playerNode[0] = _imgP1;
	_playerNode[1] = _imgP2;

	_nodeEightBall1->setVisible(false);
	_nodeEightBall2->setVisible(false);
	_nodeRemainBall1->setVisible(false);
	_nodeRemainBall2->setVisible(false);
}

void IngameEightBallPoolUI::initRemainBalls()
{
	_balls.clear();
	char text[10];
	
	for (auto player = 1; player <= 2; player++) {
		std::vector<Sprite*> playerBalls;
		sprintf(text, "imgP%d", player);
		auto playerNode = _hud->getChildByName("bg")->getChildByName(text);
		auto node = playerNode->getChildByName("nodeRemainBalls");
		node->setLocalZOrder(-2);
		for (auto i = 1; i <= 7; i++) 
		{
			auto blank = node->getChildByName("box"+std::to_string(i));
			auto sprite = Sprite::create(Res::BALLNUM_1);
			sprite->setPosition(blank->getPosition());
			_originalXBalls.insert(std::pair<Sprite*, float>(sprite, sprite->getPositionX()));
			sprite->setScale(blank->getContentSize().width / sprite->getContentSize().width);
			node->addChild(sprite);
			sprite->setLocalZOrder(7 - i);
			playerBalls.push_back(sprite);

			if (player == 2) {
				sprite->setScaleX(-1);
			}
		}
		_balls.push_back(playerBalls);
	}
}

void IngameEightBallPoolUI::initAvatars()
{
	_avatars.clear();
	for (auto player = 0; player < 2; player++) {
		auto node = _playerNode[player];
		auto mask = node->getChildByName("mask");
		_avatarsWidth = mask->getContentSize().width;
		mask->removeFromParent();
		auto avatar = Sprite::create(Res::INGAME_AVATAR_FAKE_1);
		avatar->setPosition(mask->getPosition());
		_avatars.push_back(avatar);

		auto clippingNode = ClippingNode::create();
		clippingNode->setLocalZOrder(-1);
		clippingNode->setAlphaThreshold(0.5);
		node->addChild(clippingNode);
		clippingNode->setStencil(mask);
		clippingNode->addChild(avatar);
		avatar->setScale(mask->getContentSize().width / _avatarsWidth);
	}
}

void IngameEightBallPoolUI::initTimers()
{
	_timer.clear();
	for (auto player = 0; player < 2; player++) {
		auto node = _playerNode[player];
		auto imgTimer = (Sprite*)node->getChildByName("imgTimer");
		imgTimer->setVisible(false);
		imgTimer->setFlippedX(true);
		imgTimer->setFlippedY(true);
		imgTimer->setVisible(false);
		auto timer = ProgressTimer::create(imgTimer);
		timer->cleanup();
		timer->setMidpoint(Vec2(0.5, 0.5));
		timer->setType(ProgressTimer::Type::RADIAL);
		node->addChild(timer, node->getLocalZOrder(), "timer");
		timer->setPosition(node->getChildByName("imgTimer")->getPosition());
		timer->setLocalZOrder(2);
		timer->setPercentage(0);
		auto timeAlert = (Text*)getControl("timeAlert", node);
		timeAlert->setLocalZOrder(node->getLocalZOrder()+1);

		_timeAlert.push_back(timeAlert);
		_timer.push_back(timer);
	}

	_timeInLastUpdate = 0;
	_timeBar = (ImageView *)_bg->getChildByName("imgTimerBg")->getChildByName("img");
}

void IngameEightBallPoolUI::initUIStatesPos2()
{
	auto uis = _uis;
	//cocos2d::Node* newUIs[1] = {};
	//uis.push_back(newUIs);

	//newUIs.forEach(ui = > {
	//	ui._showPos = cc.p(ui.x, ui.y);
	//	if (ui.y < cc.winSize.height / 2) {
	//		auto uiPosToOutside = -(1 - ui.getAnchorPoint().x) * ui.height - 10; // x offset to put ui totally outside of left side + 10 more px
	//		ui._hidePos = cc.p(ui.x, uiPosToOutside);
	//	}
	//	else {
	//		auto uiPosToOutside = cc.winSize.width + ui.getAnchorPoint().x * ui.height + 10; // x offset to put ui totally outside of right side + 10 more px
	//		ui._hidePos = cc.p(ui.x, uiPosToOutside);
	//	}
	//})
}

void IngameEightBallPoolUI::initEightBallEffect()
{
	_eightBallEfx.push_back(_nodeEightBall1);
	_eightBallEfx.push_back(_nodeEightBall2);

	for (int i = 0; i < 2; i ++)
	{
		for (int j = 0; j < 6; j++)
		{
			auto item = getControl("arrow_" + std::to_string(j), _eightBallEfx[i]);
			auto action = Sequence::create(
				DelayTime::create(1.f/3 * abs(j - 2.5f) - 0.5f),
				CallFuncN::create([](Node* item)
			{
				item->stopAllActions();
				item->runAction(
					RepeatForever::create(
						Sequence::create(
							EaseInOut::create(FadeTo::create(0.5, 0), 1),
							EaseInOut::create(FadeTo::create(0.5, 255), 1),
							NULL
						)
					)
				);
			}
				),
				NULL
			);
		}

		_eightBallEfx[i]->setVisible(false);
	}
}

void IngameEightBallPoolUI::update(float dt)
{
	IngameBaseUI::update(dt);
	if (_isDisable) return;
	if (_match == NULL) return;

	activePlayer(_match->getCurPlayerIdx());
	float timeRemain = _match->getTimeRemain();
	setTimeRemain(_curPlayerIdx, timeRemain);
}

void IngameEightBallPoolUI::activePlayer(int idx)
{
	if (_curPlayerIdx == idx)
		return;

	_curPlayerIdx = idx;
	_timer[idx]->setVisible(true);
	_timer[1- idx]->setVisible(false);
}

void IngameEightBallPoolUI::setTimeRemain(int idx, float time)
{
	float percent = time / GameConstant::TURN_TIME * 100;
	auto timer = _timer[idx];
	timer->setPercentage(percent);

	auto winSize = Director::getInstance()->getWinSize();
	if (time < 10 && _timeInLastUpdate > 10)
	{
		_imgTimerBg->runAction(MoveTo::create(0.3, Vec2(winSize.width /2, 0)));
	}
	else if (time > 10 && _timeInLastUpdate < 10)
	{
		_imgTimerBg->runAction(MoveTo::create(0.3, Vec2(winSize.width/2, -10)));
	}

	time = std::max(time, 0.f);
	_timeAlert[1 - idx]->setVisible(false);
	_timeAlert[idx]->setVisible(time < 10);
	_timeAlert[idx]->setOpacity((time - (int)(time)) * (255 - 70) + 70);
	_timeAlert[idx]->setString(std::to_string((int)time));

	_timeInLastUpdate = time;
	_timeBar->setScaleX(std::min(1.f, time /10));

	if (time > 5)
	{
		_timeBar->setColor(Utils::mix(Color3B(166, 160, 8), Color3B(8, 166, 33), (time - 5) / 5));
	}
	else
	{
		_timeBar->setColor(Utils::mix(Color3B(166, 8, 8), Color3B(166, 160, 8), time / 5));
	}
}

void IngameEightBallPoolUI::startMatch(EightBallPoolMatch *match)
{
	IngameBaseUI::startMatch(match);
	_curPlayerIdx = -1;

	_nodeRemainBall1->setVisible(false);
	_nodeRemainBall2->setVisible(false);
	_nodeEightBall1->setVisible(false);
	_nodeEightBall2->setVisible(false);
}

void IngameEightBallPoolUI::updateRemainingBalls(int pId, std::vector<int> balls)
{
	_nodeRemainBall1->setVisible(true);
	_nodeRemainBall2->setVisible(true);

	auto sprites = _balls[pId];

	bool onlyEightBall = (balls.size() == 1) && (balls[0] == 8);

	if (!onlyEightBall)
	{
		for (int i = 0; i < sprites.size(); i++) 
		{
			auto sprite = sprites[i];
			if (i < balls.size()) 
			{
				sprite->setVisible(true);
				sprite->setTexture(Res::BALLNUMS[balls[i] - 1]);
			}
			else 
			{
				sprite->setVisible(false);
			}
		}
	}
	else
	{
		for (int i = 0; i < sprites.size(); i++)
		{
			auto sprite = sprites[i];
			sprite->setVisible(i == 3);
			if (i == 3) sprite->setTexture(Res::BALLNUM_8);
		}
		if(pId == 0) _nodeEightBall1->setVisible(true);
		if(pId == 1) _nodeEightBall2->setVisible(true);
	}
	
}

void IngameEightBallPoolUI::resetRemainingBalls()
{
	setDefaultUI();
	for (int i = 0; i < _balls.size(); i++)
	{
		auto sprites = _balls[i];
		for (int j = 0; j < _balls[i].size(); j++)
		{
			sprites[j]->setVisible(false);
		}
	}

	_eightBallEfx[0]->setVisible(false);
	_eightBallEfx[1]->setVisible(false);
}

void IngameEightBallPoolUI::setGameMode(GameMode mode)
{
	auto score1 = getControl("nodeScore", _imgP1);
	auto score2 = getControl("nodeScore", _imgP2);

	bool visible = (mode != GameMode::QUICK_MODE && mode != GameMode::RT2_MODE);
	score1->setVisible(visible);
	score2->setVisible(visible);
}

void IngameEightBallPoolUI::resetActivePlayer()
{
	showAllPlayers();
}

void IngameEightBallPoolUI::showAllPlayers()
{
	auto action = EaseInOut::create(MoveTo::create(0.5, _defaultPos[_imgP1]), 2);
	_imgP1->runAction(action);

	auto action2 = EaseInOut::create(MoveTo::create(0.5, _defaultPos[_imgP2]), 2);
	_imgP2->runAction(action2);
}

void IngameEightBallPoolUI::hideAllPlayers()
{
	std::vector<Node*> huds = { _imgP1, _imgP2 };
	for (int i = 0; i < huds.size(); i++)
	{
		Node *img = huds[i];
		auto pos = _defaultPos[img];
		auto action = EaseInOut::create(MoveTo::create(0.5, Vec2(pos.x, -pos.y)), 2);
		img->runAction(action);
	}
}

void IngameEightBallPoolUI::setDefaultUI()
{
	std::vector<Node*> list= {_imgP1, _imgP2};
	for (int i = 0; i < list.size(); i++)
	{
		auto img = list[i];
		getControl("nodeRemainBalls", img)->setOpacity(0);
		getControl("boxAvatar_0", img)->setOpacity(255);
		getControl("boxAvatar_1", img)->setOpacity(0);
		getControl("boxBia", img)->setScale(0);
	}
}

void IngameEightBallPoolUI::setScore(const int & userScore, const int & enemyScore)
{
	//TODO
}
