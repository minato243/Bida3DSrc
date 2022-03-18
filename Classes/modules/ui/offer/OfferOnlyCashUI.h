#ifndef OfferOnlyCashUI_h_
#define OfferOnlyCashUI_h_

#include "../base/UIBaseLayer.h"
#include "BaseOfferTypeUI.h"

class OfferOnlyCashUI
	: public BaseOfferTypeUI
{
public:
	enum TAG
	{
		btnBuy
	};
	OfferOnlyCashUI();
	~OfferOnlyCashUI();

	CREATE_FUNC(OfferOnlyCashUI);

	bool init();

	void initGUI();

	void initTag();

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void loadData();

	void setEnabledButtonBuy(bool isEnable);

	void update(float dt);

	void onEnter();

	void onExit();

private:
	cocos2d::ui::Text *_lbOldCash;
	cocos2d::ui::Text *_lbNewCash;
	cocos2d::ui::Text *_lbCost;

	cocos2d::ui::Button *_btnBuy;

	cocos2d::ui::Text *_timeRemain;
};
#endif