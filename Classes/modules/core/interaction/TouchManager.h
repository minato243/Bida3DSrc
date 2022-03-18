#ifndef TOUCHMANAGER_H_
#define TOUCHMANAGER_H_


#include <cocos2d.h>
#include <functional>
#include <modules/core/ps/internal/ExtensionMath.hpp>
#include <modules/utils/MoveDetector.h>

class TouchManager: public cocos2d::Node
{
public:
	typedef std::function<void(cocos2d::Vec2, float)> TouchCallBack;
	typedef std::function<void(int, cocos2d::Vec2, cocos2d::Vec2, cocos2d::Vec2)> EventCallBack;

	enum Events
	{
		BEGAN,
		MOVED,
		ENDED
	};

	TouchManager();
	~TouchManager();

	bool init();
	CREATE_FUNC(TouchManager);

	void initEvents();

	void onEnter();

	void update(float dt);

	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);

	void onBegan(cocos2d::Vec2 location, float timestamp);

	void onMoved(cocos2d::Vec2 location, cocos2d::Vec2 velocity);

	void onEnded(cocos2d::Vec2 location, float timestamp);

	void dispatch(Events event, cocos2d::Vec2 location, cocos2d::Vec2 delta, cocos2d::Vec2 velocity);

	void dispatchTouch(cocos2d::Vec2 location, float timestamp);

	int subscribe(const EventCallBack& callback);

	int subscribeTouch(const TouchCallBack& callback);

	void lock(int callbackId);

	void unlock(int callbackId);

	bool isLock(int callbackId);
private:
	bool _enableUpdate;
	float _timestamp;
	float _startTime;
	cocos2d::Vec2 _location;
	cocos2d::Vec2 _prevLocation;
	MoveDetector _moveDetect;

	std::map<int, EventCallBack> _subscribers;
	std::map<int, TouchCallBack> _touchSubscribers;
	std::map<int, bool> _locks;

	int _idCounter;
public:
	void cancelLockUtilRelease();
	void lockUntilReleaseIfTouching();
};


#endif // !TOUCHMANAGER_H_


