#ifndef OfferCumulativeUI_h_
#define OfferCumulativeUI_h_

#include "../base/UIBaseLayer.h"
#include "../../core/offer/OfferEntity.h"
#include "BaseOfferTypeUI.h"

class OfferCumulativeUI
	:public BaseOfferTypeUI
{
public:
	enum TAG
	{
		btnOpen
	};
	OfferCumulativeUI();
	~OfferCumulativeUI();

	CREATE_FUNC(OfferCumulativeUI);
	bool init();

	void initGUI();

	void initTag();

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void loadData();

	void loadGift(OfferCumulativeData *data);

	void show();

	void loadProgressBar();

	void updateProgressBar(float dt);

	void setEnabledButton(bool isEnable);

private:
	cocos2d::ui::LoadingBar *_progressBar;
	cocos2d::ui::Text *_lbProgress;

	cocos2d::ui::Button *_btnOpen;

	int _level;
	int _point;

	float _curPercent;
	float _maxPercent;
	int _maxPoint;

	int _curPoint;
	int _require;

};
#endif
