#ifndef CHOOSE_LOGIN_TYPE_LAYER_H
#define CHOOSE_LOGIN_TYPE_LAYER_H

#include "core/gui/BaseLayer.h"
#include <data/GameConstant.h>

class ChooseLoginTypeLayer 
	: public BaseLayer
{
public:
	ChooseLoginTypeLayer();
	~ChooseLoginTypeLayer();

	bool init();
	CREATE_FUNC(ChooseLoginTypeLayer);

	void initGUI();

	void renderLoginTypeButtons();

	cocos2d::ui::Button* createCustomButton(const char *text, const CUSTOM_CALLBACK & callback = nullptr);

	void onButtonClick();

	void setParentLayer(BaseLayer *parent);
private:
	BaseLayer *_parentLayer;
};

#endif // !CHOOSE_LOGIN_TYPE_LAYER_H


