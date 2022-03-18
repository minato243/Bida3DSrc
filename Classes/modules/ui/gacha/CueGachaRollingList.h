#ifndef CueGachaRollingList_h_
#define CueGachaRollingList_h_

#include <core/gui/BaseLayer.h>
#include "../popup/FullScreenPopup.h"
#include <data/GameConstant.h>
#include "../../core/shop/ShopItemData.h"
#include "../elements/QuickCustomTableView.h"
#include "../../../data/config/CueConfig.h"
#include "CueGachaElementUI.h"
#include "CueGachaRolling.h"

class CueGachaRollingList
	:public BaseLayer
{
public:
	enum TAG
	{
		btnBack,
		btnStart
	};
	CueGachaRollingList();
	~CueGachaRollingList();

	CREATE_FUNC(CueGachaRollingList);
	bool init();

	void initGUI();

	void initTag();

	void loadDataCueBox(ShopItemData *data);

	void loadDataCueBoxDeal(HotDealData *data);

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void refresh();

	void setGoldValue(int gold);

	void setCashValue(int cash);

	void buyCueBox(int cueItemIdx);

	void onOpenBoxSuccess(int cueItemIdx);

	CueGachaRolling* getTrueRollingUI(int cueItemIdx);

private:
	cocos2d::ui::Button *_btnBack;
	cocos2d::ui::Widget *_pnBoxSide;
	cocos2d::ui::Widget *_pnList;

	cocos2d::ui::Button *_btnStart;
	cocos2d::ui::Text *_lbBoxName;
	cocos2d::ui::Text *_lbBtnStart;
	cocos2d::ui::ImageView *_imgCCBtnStart;

	cocos2d::ui::Text *_lbGold;
	cocos2d::ui::Text *_lbCC;

	ShopItemData *_data;
	std::vector<CueRateInBox *> _boxesData;
	CueRateInBox *_lastData;

	cocos2d::Rect _boudingBoxItem;
	QuickCustomTableView<CueRateInBox, CueGachaElementUI> *_list;

	CueGachaRolling *_rollingUI;

	int _preTab;
	bool _locked;
	bool _rolled;

public:
	static const int END_ANIMATION_TIME = 3;
	static const int ANIMATION_SPEED = 1000;
	static const int ANIMATION_SPEED_AFTER = 300;
};
#endif