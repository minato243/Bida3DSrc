#ifndef StatBarPanel_h_
#define StatBarPanel_h_

#include <cocos2d.h>
#include "cocos/ui/CocosGUI.h"

class StatBarPanel
	:public cocos2d::Ref
{
public:
	StatBarPanel(cocos2d::ui::Widget * panel);
	~StatBarPanel();

	void setPostfix(const std::string & str);

	void setDefaultValue();

	void initPanel();

	void setInfo(int val, float percent);

	void updateInfo(float dt);

	void _setInfo(int val, float percent);

	void setPercent(float percentOnOne);

protected:
	cocos2d::ui::Widget *_panel;
	cocos2d::ui::Text *_lbValue;
	cocos2d::ui::ImageView *_bar;
	

	std::string _postfix;

	float _animationTime;
	float _animationStartTime;
	bool _valid;
	float _endVal;
	float _endPercent;

	float _stencilOriginalX;

	const static int ANIMATION_TIME = 1000;
public:
	cocos2d::DrawNode *_stencil;
};

class StatBarPanelAngle:
	public StatBarPanel
{
public:
	StatBarPanelAngle(cocos2d::ui::Widget * panel);
	~StatBarPanelAngle();

	void setInfo(float val1, float val2, float percent);

	void setDefaultValue();

	void updateInfo(float dt);

	void _setInfo(int val1, int val2, float percent);

public:
	float _endVal1;
	float _endVal2;
};

#endif