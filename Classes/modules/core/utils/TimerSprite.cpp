#include "TimerSprite.h"

USING_NS_CC;

TimerSprite::TimerSprite()
	:Sprite(),_timer(0)
{

}

TimerSprite::~TimerSprite()
{
}

bool TimerSprite::init()
{
	Sprite::init();
	return true;
}

TimerSprite * TimerSprite::create(const std::string& fileName)
{
	TimerSprite *ts = new TimerSprite();
	ts->initWithSpriteFrameName(fileName);
	ts->autorelease();
	return ts;
}

void TimerSprite::setTimer(float time)
{
	_timer = time;
	setStateByTimer();
}

float TimerSprite::getTimer()
{
	return _timer;
}

void TimerSprite::setStateByTimer()
{
	auto timer = _timer;
	timer = timer - 2 * floor(timer / 2);

	if (timer < 1) {
		setOpacity(255 - timer * 205);
		setScale(0.01 - timer * 0.0025);
	}
	else {
		setOpacity(50 + (timer - 1) * 205);
		setScale(0.0075 + (timer - 1) * 0.0025);
	}
}

void TimerSprite::update(float dt)
{
	_timer += dt;
	setStateByTimer();
}

void TimerSprite::onEnter()
{
	Sprite::onEnter();
	scheduleUpdate();
}

void TimerSprite::onExit()
{
	unscheduleUpdate();
	Sprite::onExit();
}

