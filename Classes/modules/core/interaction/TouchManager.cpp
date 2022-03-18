#include "TouchManager.h"

USING_NS_CC;


TouchManager::TouchManager()
	:_enableUpdate(false), _timestamp(0)
{
	_idCounter = 0;
}

TouchManager::~TouchManager()
{
}

bool TouchManager::init()
{
	if (!Node::init()) {
		return false;
	}

	initEvents();
	setLocalZOrder(-1);

	return true;
}

void TouchManager::initEvents()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(TouchManager::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(TouchManager::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(TouchManager::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void TouchManager::onEnter()
{
	Node::onEnter();
	scheduleUpdate();
}

void TouchManager::update(float dt)
{
	if (_enableUpdate) {
		_timestamp += dt;
		if (_location == _prevLocation) return;

		auto velocity = Vec2(
			(_location.x - _prevLocation.x) / dt,
			(_location.y - _prevLocation.y) / dt
		);

		onMoved(_location, velocity);

		_prevLocation = _location;
	}
}

bool TouchManager::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event)
{
	_enableUpdate = true;
	_timestamp = 0;
	_location = touch->getLocation();
	_prevLocation = _location;
	onBegan(_location, _timestamp);
	return true;
}

void TouchManager::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event)
{
	_location = touch->getLocation();
}

void TouchManager::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event)
{
	_enableUpdate = false;
	onEnded(touch->getLocation(), _timestamp);
}

void TouchManager::onBegan(cocos2d::Vec2 location, float timestamp)
{
	_location = location;
	_startTime = timestamp;
	_moveDetect.init(location);
	dispatch(Events::BEGAN, location, Vec2(0,0), Vec2(0,0));
}

void TouchManager::onMoved(cocos2d::Vec2 location, cocos2d::Vec2 velocity)
{
	_moveDetect.setPoint(location);
	auto delta = Vec2(
		location.x - _location.x,
		location.y - _location.y
	);
	dispatch(Events::MOVED, location, delta, velocity);
}

void TouchManager::onEnded(cocos2d::Vec2 location, float timestamp)
{
	_moveDetect.setPoint(location);
	auto delta = Vec2(
		location.x - _location.x,
		location.y - _location.y
	);
	dispatch(Events::ENDED, location, delta, Vec2(0,0));
	if (!_moveDetect.isMoved()) {
		dispatchTouch(location, timestamp - _startTime);
	}
}

void TouchManager::dispatch(Events event, cocos2d::Vec2 location, cocos2d::Vec2 delta, cocos2d::Vec2 velocity)
{
	std::map<int, EventCallBack>::iterator itr;
	for (itr = _subscribers.begin(); itr != _subscribers.end(); itr++) {
		if (isLock(itr->first)) continue;
		auto callback = itr->second;
		callback(event, location, delta, velocity);
	}
}

void TouchManager::dispatchTouch(cocos2d::Vec2 location, float timestamp)
{
	std::map<int, TouchCallBack>::iterator itr;
	for (itr = _touchSubscribers.begin(); itr != _touchSubscribers.end(); itr++) {
		if (isLock(itr->first)) continue;
		auto callback = itr->second;
		callback(location, timestamp);
	}
}

int TouchManager::subscribe(const EventCallBack & callback)
{
	int id = _idCounter;
	_subscribers.insert(std::pair<int, EventCallBack>(id, callback));
	_idCounter++;
	return id;
}

int TouchManager::subscribeTouch(const TouchCallBack & callback)
{
	int id = _idCounter;
	_touchSubscribers.insert(std::pair<int, TouchCallBack>(id, callback));
	_idCounter++;
	return id;
}

void TouchManager::lock(int callbackId)
{
	if (!isLock(callbackId))
	{
		_locks.insert(std::pair<int, bool>(callbackId, true));
	}
}

void TouchManager::unlock(int callbackId)
{
	if (isLock(callbackId))
	{
		_locks[callbackId] = false;
	}
}

bool TouchManager::isLock(int callbackId)
{
	std::map<int, bool>::iterator ittr = _locks.find(callbackId);
	if (ittr != _locks.end())
		return ittr->second;
	return false;
}

void TouchManager::cancelLockUtilRelease()
{
	//TODO
}

void TouchManager::lockUntilReleaseIfTouching()
{
	//TODO
}
