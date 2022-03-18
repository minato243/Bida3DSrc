#include "BallTypeNotify.h"
#include <data/Resource.h>
#include <core/utils/LanguageMgr.h>
#include <data/GameConstant.h>

USING_NS_CC;
using namespace cocos2d::ui;

BallTypeNotify::BallTypeNotify()
{
}

BallTypeNotify::~BallTypeNotify()
{
}

bool BallTypeNotify::init()
{
	if (!BaseLayer::init("BallTypeNotify")) {
		return false;
	}
	
	Size winSize = Director::getInstance()->getVisibleSize();
	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::BallTypeNotify);
	initWithJsonFile(filePath);
	_layerColor->setPosition(Vec2(winSize.width / 2, winSize.height / 2));

	return true;
}

void BallTypeNotify::initGUI()
{
	autoInitComponent(_layout);
	_bg = getControl("bg");
	((Widget*)_bg)->setTouchEnabled(false);

	char text[10];
	for (int i = 0; i < NUM_BALL_SELECT; i++) {
		sprintf(text, "ball_%d", i);
		ImageView * ball = (ImageView*)getControl(text, _bg);
		_balls.push_back(ball);
	}

	_lbNotify	= (Text*)getControl("lbNotify", _bg);
	_bgLight	= (ImageView *)_nodeMap["bgLight"];
	_imgEffect	= (ImageView *)_nodeMap["imgEffect"];

	_hidePos[_lbNotify] = Vec2(_lbNotify->getPositionX(), _lbNotify->getPositionY() - 40);
}

void BallTypeNotify::showSelectEightBallPocket()
{
	for (auto i = 0; i < 7; i++) {
		_balls.at(i)->setVisible(i== 3);
	}
	ImageView *ball = (ImageView*)_balls[3]->getChildByName("ball");
	ball->loadTexture("big_balls_8.png", Widget::TextureResType::PLIST);

	_lbNotify->setString(languageMgr->localizedText("select_eight_ball_pocket_phase"));
	flyOut(3.5);
}

void BallTypeNotify::showWithType(bool isSolid)
{
	_isSolid = isSolid;
	auto offset = isSolid ? 1 : 9;

	for (auto i = 0; i < 7; i++) {
		_balls[i]->setVisible(true);
		ImageView *ball = (ImageView*)_balls[i]->getChildByName("ball");
		ball->loadTexture("big_balls_" + std::to_string(i + offset) + ".png", Widget::TextureResType::PLIST);
	}
	_lbNotify->setString(languageMgr->localizedText(isSolid ? "you_are_solid" : "you_are_striped"));
	flyOut();
}

void BallTypeNotify::flyOut(double showTime)
{
	Size winSize = Director::getInstance()->getVisibleSize();
	auto x = this->getPositionX();
	float y = this->getPositionY();
	setScale(1);
	setVisible(true);
	setAllBallSuperSmall();
	setBgDisappear();

	auto totalTimeForShow = BallTypeNotify::EFFECT_SHOW_TIME;
	auto action = Sequence::create(
		EaseExponentialIn::create(FadeTo::create(0.2, 150)),
		CallFunc::create([this]() {
		runActionBalls();
		runActionBackground();
	}),
		DelayTime::create(totalTimeForShow + EFFECT_PAUSE), 
		CallFunc::create([this]()
	{
		runActionHide();
	}),
		NULL
	);
	_layerColor->runAction(action);
}
	
void BallTypeNotify::flyOutDone(double x)
{
	setVisible(false);
	setPositionX(x);
}


void BallTypeNotify::setAllBallSuperSmall()
{
	for (auto i = 0; i < _balls.size(); i++)
	{
		_balls[i]->setScale(0.01);
		_balls[i]->setPositionX(_defaultPos[_balls[i]].x);
		_balls[i]->getChildByName("ball")->setOpacity(0);
	}
}

void BallTypeNotify::setBgDisappear()
{
	_imgEffect->setVisible(false);
	_imgEffect->setScaleX(0.005);
	_bgLight->setOpacity(0);
	_lbNotify->setOpacity(0);
	_lbNotify->setPosition(_hidePos[_lbNotify]);
}

void BallTypeNotify::runActionBalls()
{
	auto mid = 3;
	auto EFFECT_ZOOM_TIME = BallTypeNotify::EFFECT_ZOOM_TIME;
	for (auto i = 0; i < 7; i++) {
		auto diff = abs(mid - i);
		auto ball = _balls[i];
		auto action = Sequence::create(
			DelayTime::create(EFFECT_ZOOM_TIME * diff),
			ScaleTo::create(EFFECT_ZOOM_TIME, 1),
			NULL
		);
		ball->runAction(action);
		
		auto img = ball->getChildByName("ball");
		auto imgAction = Sequence::create(
			DelayTime::create(EFFECT_ZOOM_TIME * diff),
			EaseBounceIn::create(FadeTo::create(EFFECT_ZOOM_TIME, 255)),
			NULL
		);
		img->runAction(imgAction);
	}
}

void BallTypeNotify::runActionBackground()
{
	auto bg = _imgEffect;
	auto light = _bgLight;
	auto lb = _lbNotify;
	auto winSize = Director::getInstance()->getWinSize();
	auto scale = winSize.width / bg->getContentSize().width;

	light->runAction(Sequence::create(
		DelayTime::create(EFFECT_ZOOM_TIME * 1.5),
		EaseOut::create(FadeTo::create(EFFECT_ZOOM_TIME * 2, 255), 1),
		NULL
	));

	bg->runAction(Sequence::create(
		DelayTime::create(EFFECT_ZOOM_TIME * 1.5),
		CallFuncN::create([](Node* node) {
			node->setVisible(true);
		}),
		EaseSineInOut::create(ScaleTo::create(EFFECT_ZOOM_TIME * 2, scale, 1)),
			NULL
		));

	lb->runAction(Sequence::create(
		DelayTime::create(EFFECT_ZOOM_TIME),
		Spawn::create(
			FadeTo::create(EFFECT_ZOOM_TIME, 255),
			MoveTo::create(EFFECT_ZOOM_TIME, _defaultPos[lb])
		), 
		NULL
	));
}

void BallTypeNotify::runActionHide()
{
	auto winSize = Director::getInstance()->getWinSize();
	auto offset = winSize.width / 2 - _bg->getContentSize().width / 2;
	auto timeBefore = 0;
	for (auto i = 0; i < 7; i++) {
		auto ball = _balls[i];
		auto curNum = _isSolid ? i + 1 : i + 9;
		
		auto pos = ball->getPosition();
		ball->runAction(Sequence::create(
			DelayTime::create(EFFECT_FLY_DELAY * i),
			EaseOut::create(MoveTo::create(EFFECT_FLY_TIME * (pos.x + offset + 200) / 200, Vec2(-(offset + 200), pos.y)), 1),
			NULL
		));
		if (i < 6)
			timeBefore += EFFECT_FLY_TIME * (pos.x + offset + 200) / 200;
	}


	_imgEffect->runAction(Sequence::create(
		DelayTime::create(EFFECT_FLY_DELAY * 3),
		FadeTo::create(EFFECT_ZOOM_TIME * 1, 0), 
		NULL
	));
	_bgLight->runAction(Sequence::create(
		DelayTime::create(EFFECT_FLY_DELAY * 3),
		FadeTo::create(EFFECT_ZOOM_TIME * 1, 0),
		NULL

	));
	_lbNotify->runAction(Sequence::create(
		DelayTime::create(EFFECT_FLY_DELAY * 3),
		Spawn::create(
			FadeTo::create(EFFECT_ZOOM_TIME * 1, 0),
			MoveTo::create(EFFECT_ZOOM_TIME * 1, _hidePos[_lbNotify ])
		),
		NULL
	));

	auto totalTimeForHide = timeBefore - 1.2;
	_layerColor->runAction(Sequence::create(
		DelayTime::create(totalTimeForHide),
		FadeTo::create(0.3, 0),
		CallFunc::create([this]() {
			setVisible(false);
		}),
		NULL
	));
}

const float BallTypeNotify::EFFECT_PAUSE = 0.5f;

const float BallTypeNotify::EFFECT_ZOOM_TIME = 0.15;

const float BallTypeNotify::EFFECT_FLY_TIME = 0.1;

const float BallTypeNotify::EFFECT_FLY_DELAY = 0.1;

const float BallTypeNotify::EFFECT_SHOW_TIME = BallTypeNotify::EFFECT_ZOOM_TIME * 4;
