#include "PlacingBallControlUI.h"
#include "modules/core/utils/TimeUtils.h"
#include <modules/core/ps/internal/PhysicsConstants.hpp>
#include <core/GameMgr.h>
#include <modules/core/interaction/InteractionManager.h>
#include <base/UIFactory.h>

USING_NS_CC;
using namespace cocos2d::ui;

PlacingBallControlUI::PlacingBallControlUI()
{
}

PlacingBallControlUI::~PlacingBallControlUI()
{
}

bool PlacingBallControlUI::init()
{
	if (!Node::init()) {
		return false;
	}
	
	return true;
}

bool PlacingBallControlUI::initNode(cocos2d::ui::ImageView * node)
{
	_control = node;
	initControl();
	initDrawNode();
	colorize(false);
	disablePlacingBall();
	_lockExternal = false;
	return true;
}

void PlacingBallControlUI::initDrawNode()
{
	_dn = DrawNode::create();
	addChild(_dn);
}

void PlacingBallControlUI::initControl()
{
	_control->addTouchEventListener(CC_CALLBACK_2(PlacingBallControlUI::touchListener, this));

	UIFactory::grayNode(_control);
}

void PlacingBallControlUI::touchListener(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType type)
{
	CCLOG("touchListener %d", (int)(type));
	Button *btn = (Button *)(sender);
	Vec2 position;
	switch (type) {
	case ui::Widget::TouchEventType::BEGAN:
		_lockExternal = true;
		position = btn->getTouchBeganPosition();
		colorize(true);
		_position = position;
		_lastUpdate = timeUtils->current();
		break;
	case ui::Widget::TouchEventType::MOVED:
		position = btn->getTouchMovePosition();
		move(position);
		break;
	case ui::Widget::TouchEventType::ENDED:
	case ui::Widget::TouchEventType::CANCELED:
		position = btn->getTouchEndPosition();
		_lockExternal = false;
		end(position);
		colorize(false);
		break;
	}
}

void PlacingBallControlUI::update(float dt)
{
	//if (timeUtils->current() - _lastUpdate > 500) {
	//	move(_position);
	//}

	_dn->clear();
	if (_listener && _control->getParent()->isVisible()) {
		auto pos = ps::ExtMath::vector(gameMgr->_table->getWhiteBall()->body()->position());
		pos.z += PhysicsConstants::BALL_RADIUS;
		auto cueBallPos = interactionMgr->getCamera()->worldPositionToWindowLocation(pos);
		auto color = _isTouching ? Color3B(0, 255, 243) : Color3B(177, 177, 177);

		auto ctrlPos = _control->getPosition();
		auto vector = Vec2(cueBallPos.x - ctrlPos.x, cueBallPos.y - ctrlPos.y);
		auto multiplier = (_control->getContentSize().width / 4) / sqrt(vector.x*vector.x + vector.y*vector.y);
		_dn->drawLine(Vec2(ctrlPos.x + vector.x * multiplier, ctrlPos.y + vector.y * multiplier), cueBallPos, Color4F(color));
	}
}

void PlacingBallControlUI::move(cocos2d::Vec2 position)
{
	auto delta = Vec2(position.x - _position.x, position.y - _position.y);
	auto pos = _control->getPosition();
	pos.x += delta.x;
	pos.y += delta.y;
	_control->setPosition(pos);
	_position = position;

	if (_listener != nullptr) {
		_listener(position, false);
	}
	_lastUpdate = timeUtils->current();
}

void PlacingBallControlUI::end(cocos2d::Vec2 position)
{
	auto delta = Vec2(position.x - _position.x, position.y - _position.y);
	auto pos = _control->getPosition();
	pos.x += delta.x;
	pos.y += delta.y;
	_control->setPosition(pos);
	_position = position;
	if (_listener) {
		_listener(position, true);
	}
	_lastUpdate = timeUtils->current();
}

void PlacingBallControlUI::enablePlacingBall(cocos2d::Vec2 startPosition, const PlaceBallCallBack & listener)
{
	_listener = listener;
	_control->setPosition(startPosition);
	_control->setVisible(true);
}

void PlacingBallControlUI::setPlacingPosition(cocos2d::Vec2 position)
{
	if (_lockExternal) return;
	_control->setPosition(position);
}

void PlacingBallControlUI::disablePlacingBall()
{
	_control->setVisible(false);
	_listener = nullptr;
}

void PlacingBallControlUI::onEnter()
{
	Node::onEnter();
	scheduleUpdate();
}

void PlacingBallControlUI::colorize(bool val) 
{
	if (val) {
		UIFactory::normalNode(_control);
	}
	else {
		UIFactory::grayNode(_control);
	}
}
