#ifndef POWERUI_H_
#define POWERUI_H_

#include <cocos2d.h>
#include <ui/CocosGUI.h>
#include <functional>


struct PowerUIListener {
	std::function<void(float)> onChange;
	std::function<void(float)> onRelease;

	PowerUIListener() {};
};

class PowerUI: public cocos2d::Node
{
public:
	PowerUI();
	~PowerUI();

	bool init();
	CREATE_FUNC(PowerUI);

	static PowerUI *create(cocos2d::Node *ui);

	void onEnter();

	void setPosition(cocos2d::Vec2 pos);

	void setForceValue(float force);

	void initFrame();

	void initListener();
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);

	void setListener(PowerUIListener *listener);

	void enable();

	void disable();
private:
	cocos2d::Node *_ui;
	cocos2d::Rect _frameRect;
	cocos2d::ui::Text *_force;

	cocos2d::ui::ImageView *_cue;
	cocos2d::ui::ImageView *_imgForceOverlay;
	cocos2d::ui::ImageView *_imgCueGhost;

	bool _touching;
	cocos2d::Vec2 _start;
	bool _disabled;
	PowerUIListener *_listener;

	float _percent;
	
	cocos2d::Vec2 _cueInitPos;

public:
	static int CUE_RANGE;
	const static float TRANSITION_TIME;
};

#endif // !POWERUI_H_


