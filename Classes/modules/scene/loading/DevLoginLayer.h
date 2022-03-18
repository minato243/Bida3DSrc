#ifndef DEV_LOGIN_LAYER_H_
#define DEV_LOGIN_LAYER_H_

#include "core/gui/BaseLayer.h"
#include <data/GameConstant.h>
class DevLoginLayer
	: public BaseLayer
{
public:
	DevLoginLayer();
	~DevLoginLayer();

	bool init();
	CREATE_FUNC(DevLoginLayer);

	void initGUI();

	void renderInputBoxes();

	void renderSocialIDBox();

	void onButtonTouch(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type);

	void doLogin();

	void setParentLayer(BaseLayer *parent);

private:
	cocos2d::ui::EditBox *_inputBox;
	cocos2d::ui::EditBox *_socialIDBox;
	BaseLayer *_parentLayer;
};

#endif // !DEV_LOGIN_LAYER_H_




