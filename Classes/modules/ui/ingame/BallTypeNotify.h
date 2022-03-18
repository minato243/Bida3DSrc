#ifndef BALLTYPENOTIFY_H_
#define BALLTYPENOTIFY_H_

#include <cocos2d.h>
#include <core/gui/BaseLayer.h>
#include <cocos/ui/CocosGUI.h>

class BallTypeNotify : public BaseLayer
{
public:
	BallTypeNotify();
	~BallTypeNotify();

	bool init();
	CREATE_FUNC(BallTypeNotify);

	void initGUI();

	void showSelectEightBallPocket();

	void showWithType(bool isSolid);

	void flyOut(double showTime = 2);

	void flyOutDone(double x);

	void setAllBallSuperSmall();

	void setBgDisappear();

	void runActionBalls();

	void runActionBackground();

	void runActionHide();

private:
	std::vector<cocos2d::ui::ImageView *> _balls;
	cocos2d::ui::Text *_lbNotify;
	cocos2d::ui::ImageView *_imgEffect;
	cocos2d::ui::ImageView *_bgLight;

	std::map<cocos2d::Node *, cocos2d::Vec2> _hidePos;

	bool _isSolid;

public:
	static const int NUM_BALL_SELECT = 7;

	static const float EFFECT_PAUSE;
	static const float EFFECT_ZOOM_TIME;
	static const float EFFECT_FLY_TIME;
	static const float EFFECT_FLY_DELAY;
	static const float EFFECT_SHOW_TIME;
};

#endif // !BALLTYPENOTIFY_H_


