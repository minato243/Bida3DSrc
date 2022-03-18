#include "PowerUI.h"
#include <base/UIFactory.h>
#include <data/Resource.h>
#include <cmath>
#include <core/GameMgr.h>

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;


PowerUI::PowerUI()
	: _touching(false), _disabled(false), _listener(nullptr)
{
}

PowerUI::~PowerUI()
{
	delete _listener;
	_listener = nullptr;
}

bool PowerUI::init()
{
	if (!Node::init()) {
		return false;
	}

	
	initFrame();
	initListener();
	setForceValue(0);

	return true;
}

PowerUI * PowerUI::create(Node *ui)
{
	PowerUI *powerBar = new PowerUI();
	powerBar->_ui = ui;
	powerBar->init();
	powerBar->autorelease();

	return powerBar;
}

void PowerUI::onEnter()
{
	Node::onEnter();
}

void PowerUI::setPosition(Vec2 pos)
{
	Node::setPosition(Vec2(pos.x - _frameRect.size.width/2, pos.y));
}

void PowerUI::setForceValue(float force)
{
	int value = (int)force;
	_force->setString(std::to_string(value));
}

void PowerUI::initFrame()
{
	auto imgPower = (ImageView*)_ui->getChildByName("imgPower");
	auto imgPowerBox = (ImageView*)_ui->getChildByName("imgPowerBox");
	_frameRect = imgPower->getBoundingBox();
	auto extendWidth = imgPowerBox->getContentSize().width - _frameRect.size.width;
	_frameRect.origin.x -= extendWidth / 2;
	_frameRect.size.width += extendWidth;
	_cue = (ImageView*)imgPower->getChildByName("clipping")->getChildByName("imgCue");
	_imgForceOverlay = (ImageView*)imgPower->getChildByName("imgPowerOverlay");
	_imgCueGhost = dynamic_cast<ImageView*>(imgPower->getChildByName("imgCueGhost"));
	if (_imgCueGhost) {
		_imgCueGhost->setVisible(false);
	}
	_cueInitPos = _cue->getPosition();
	PowerUI::CUE_RANGE = -_cue->getContentSize().height + 5;
	_force = (Text*)_ui->getChildByName("lbForce");
}

void PowerUI::initListener()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(PowerUI::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(PowerUI::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(PowerUI::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool PowerUI::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event)
{
	if (_touching) return false;
	auto location = _ui->convertToNodeSpace(touch->getLocation());
	if (_frameRect.containsPoint(location)) {
		_start = location;
		_touching = true;
		return true;
	}
	else {
		return false;
	}
}

void PowerUI::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event)
{
	if (_disabled) return;
	auto location = _ui->convertToNodeSpace(touch->getLocation());
	auto delta = Vec2(location.x - _start.x, location.y - _start.y);
	if (delta.y ==0) return;

	auto posY = _cueInitPos.y + std::max(std::min(0.f, delta.y), CUE_RANGE*1.f);
	auto percent = 100 * ((posY - _cueInitPos.y) / CUE_RANGE);
	_cue->setPositionY(posY);
	gameMgr->_table->getCue()->cueMoveByForce(percent);

	if (_listener) {
		_listener->onChange(percent);
	}
}

void PowerUI::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event)
{
	_touching = false;
	if (_disabled) return;
	auto y = _cue->getPositionY();
	_cue->runAction(MoveTo::create(PowerUI::TRANSITION_TIME, _cueInitPos));
	_percent = 100 * ((y -_cueInitPos.y) / PowerUI::CUE_RANGE);
	if (_percent < 5) return;

	if (_listener) {
		_disabled = true;
		scheduleOnce([this](float dt) {
			_disabled = false;
			_listener->onRelease(_percent);
		}, PowerUI::TRANSITION_TIME, "onPowerBarRelease");
	}
}

void PowerUI::setListener(PowerUIListener* listener)
{
	_listener = listener;
}

void PowerUI::enable()
{
	_disabled = false;
}

void PowerUI::disable()
{
	_disabled = true;
}

const float PowerUI::TRANSITION_TIME = 0.1f;
int PowerUI::CUE_RANGE = 280;
