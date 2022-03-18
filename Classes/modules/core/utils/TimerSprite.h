#ifndef TimerSprite_h_
#define TimerSprite_h_

#include <cocos2d.h>

class TimerSprite : public cocos2d::Sprite
{
public:
	TimerSprite();
	~TimerSprite();

	bool init();

	CREATE_FUNC(TimerSprite);

	static TimerSprite *create(const std::string& fileName);

	void setTimer(float time);

	float getTimer();

	void setStateByTimer();

	void update(float dt);

	void onEnter();

	void onExit();

private:
	float _timer;
};
#endif // !TimerSprite_h_
