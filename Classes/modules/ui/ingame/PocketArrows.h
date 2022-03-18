#ifndef PocketArrows_h_
#define PocketArrows_h_

#include <cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <data/GameConstant.h>

class PocketArrows
	:public cocos2d::Node
{
public:
	PocketArrows();
	~PocketArrows();

	CREATE_FUNC(PocketArrows);

	bool init();

	void initArrows();

	void update(float dt);

	void setVisible(bool isVisible) override;

private:
	std::vector<cocos2d::Node *> _arrows;
	float _passedTime;

public:
	static const float ANIM_TIME;
	static const int ARROWS_COUNT = 3;

};
#endif
