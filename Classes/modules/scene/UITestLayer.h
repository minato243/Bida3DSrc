#ifndef 	_UITESTLAYER_H_
#define 	_UITESTLAYER_H_


#include <cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "TestBallIndicator.h"

struct UITestCallBack {
	BallIndicatorCallback indicatorCallback;
};

class UITestLayer: public cocos2d::Layer{
public:
	UITestLayer();
	virtual bool init();
	void initPowerSlider();
	void initTimescaleSlider();
	void initcueMovingSpeedSlider();
	void initBallIndicator();
	float getPowerValue();
	float getCueSpeedValue();
	void setMaxPower(int force);
	double getMaxPower();
	void sliderEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type);
	void setDelegate(UITestCallBack delegate);
	void setBallIndicatorCallback(BallIndicatorCallback callback);
	UITestCallBack& getDelegate();
	void initFoVSliders();
	void updateFoVValues();
	void updateAngle(const char* text);
	CREATE_FUNC(UITestLayer);

private:
	int maxForce;
	cocos2d::ui::Slider* powerSlider;
	cocos2d::ui::Text* powerPercentText;
	cocos2d::ui::Text* powerText;
	cocos2d::ui::Slider* timescaleSlider;
	cocos2d::ui::Text* timescaleText;
	cocos2d::ui::Slider* cueSlider;
	cocos2d::ui::Text* cueText;
	TestBallIndicator* ballIndicator;
	UITestCallBack delegate;
	cocos2d::ui::Slider* fovSlider_1;
	cocos2d::ui::Slider* fovSlider_2;
	cocos2d::ui::Slider* fovSlider_3;

public:
	static const int POWER_SLIDER_TAG = 1001;
	static const int CUE_SPEED_SLIDER_TAG = 1002;
	static const int TIMESCALE_SLIDER_TAG = 1003;
	static const int FOV_SLIDER = 1004;
};
#endif