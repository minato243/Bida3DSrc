#ifndef PlacingBallControlUI_H_
#define PlacingBallControlUI_H_

#include <cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <data/GameConstant.h>
#include <functional>
#include <modules/core/ps/internal/ExtensionMath.hpp>

class PlacingBallControlUI: public cocos2d::Node
{
public:
	typedef std::function<void(cocos2d::Vec2, bool)> PlaceBallCallBack;

	PlacingBallControlUI();
	~PlacingBallControlUI();

	bool init();
	CREATE_FUNC(PlacingBallControlUI);

	bool initNode(cocos2d::ui::ImageView *node);

	void initDrawNode();

	void initControl();

	void touchListener(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType type);

	void update(float dt);

	void colorize(bool val);

	void move(cocos2d::Vec2 position);

	void end(cocos2d::Vec2 position);

	void enablePlacingBall(cocos2d::Vec2 startPosition, const PlaceBallCallBack &listener);

	void setPlacingPosition(cocos2d::Vec2 position);

	void disablePlacingBall();

	void onEnter();

private:
	cocos2d::ui::Widget *_control;
	cocos2d::DrawNode *_dn;
	bool _lockExternal;
	long _lastUpdate;
	bool _isTouching;
	PlaceBallCallBack _listener;
};


#endif // !PlacingBallControlUI_H_



