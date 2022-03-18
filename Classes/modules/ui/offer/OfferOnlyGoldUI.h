#ifndef OfferOnlyGoldUI_h_
#define OfferOnlyGoldUI_h_

#include "../base/UIBaseLayer.h"
#include "BaseOfferTypeUI.h"

class OfferOnlyGoldUI
	: public BaseOfferTypeUI
{
public:
	enum TAG
	{
		btnBuy
	};
	OfferOnlyGoldUI();
	~OfferOnlyGoldUI();

	CREATE_FUNC(OfferOnlyGoldUI);
	bool init();

	void initGUI();

	void initTag();

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void loadData();

	void update(float dt);

	void setEnabledButtonBuy(bool isEnable);

	void show();

private:
	cocos2d::ui::Text *_timeRemain;
	cocos2d::ui::Button *_btnBuy;

	cocos2d::ui::Text *_lbValue;
	cocos2d::ui::Text *_lbCost;

	float remainTime;
};
#endif