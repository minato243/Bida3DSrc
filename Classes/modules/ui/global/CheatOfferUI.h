#ifndef CheatOfferUI_h_
#define CheatOfferUI_h_

#include "../base/UIBaseLayer.h"

class CheatOfferUI
	:public UIBaseLayer
{
public:
	CheatOfferUI();
	~CheatOfferUI();

	enum TAG
	{
		btnStarterOffer,
		btnOnlyCashOffer,
		btnOnlyGoldOffer,
		btnOfferCumulative,
		btnSpin
	};
	CREATE_FUNC(CheatOfferUI);
	bool init();

	void initGUI();

	void initTag();

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void cheateOfferCumulative();

private:
	cocos2d::ui::TextField *_tfLevel;
	cocos2d::ui::TextField *_tfPoint;
};



#endif