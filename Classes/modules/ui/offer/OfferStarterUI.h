#ifndef OfferStarterUI_h_
#define OfferStarterUI_h_

#include "../base/UIBaseLayer.h"
#include "BaseOfferTypeUI.h"
#include "../../../data/config/OfferConfig.h"
#include "../../core/offer/OfferEntity.h"

class OfferStarterUI
	:public BaseOfferTypeUI
{
public:
	OfferStarterUI();
	~OfferStarterUI();

	enum TAG
	{
		btnGift_0,
		btnGift_1,
		btnGift_2,
		btnGift_3,
		btnGift_4, 
		btnGift_5,
		btnGift_6,
		btnBuy
	};

	CREATE_FUNC(OfferStarterUI);
	bool init();

	void initGUI();

	void initTag();

	void onButtonTouched(cocos2d::ui::Button* button, int id);

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void loadData();

	void setEnabledButtonBuy(bool isEnable);

	void loadListGift(const std::vector<OfferItemData> & listGift);

	void loadNodeGift(cocos2d::Node *node, const OfferItemData & gift);

	void update(float dt);

	void onEnter();

	void onExit();

	void showTooltip(int idx);

	void hideTooltip();

private:
	cocos2d::ui::Button *_btnBuy;
	cocos2d::ui::Text *_lbCost;
	cocos2d::ui::Text *_timeRemain;

	cocos2d::ui::ImageView *_bgTooltip;
	cocos2d::ui::Text *_lbTooltip;
	cocos2d::ui::Widget *_pnGift;

	OfferStarter *_data;

};
#endif
