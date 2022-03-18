#ifndef ShopUI_h_
#define ShopUI_h_

#include <core/gui/BaseLayer.h>
#include <data/GameConstant.h>
#include "ShopTabLimited.h"
#include "ShopTabItem.h"

class ShopUI
	:public BaseLayer
{
public:
	enum TAG
	{
		btnBack,
		btnTab0,
		btnTab1,
		btnTab2,
		btnTab3
	};
	ShopUI();
	~ShopUI();

	bool init();
	CREATE_FUNC(ShopUI);

	void initGUI();

	void initTag();

	void initTimerScroll();

	void updateScroll(float dt);

	int convertPosToOffset(float posY);

	void onSceneReload();

	void reloadGUIHotDeals();

	void initInterval();

	void initData();

	void initTabs();

	void initSubscribers();

	void refresh();

	void setGoldValue(int gold);

	void setCashValue(int cash);

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void switchTab(int id, bool isInstant = false);

	void showTabSelected(int id);

	void onRequestClose();
private:
	static const int numTab = 4;

	cocos2d::ui::Button *_btnBack;
	cocos2d::ui::Button *_btnTab0;
	cocos2d::ui::Button *_btnTab1;
	cocos2d::ui::Button *_btnTab2;
	cocos2d::ui::Button *_btnTab3;

	cocos2d::ui::Button * _listBtnTab[numTab];

	cocos2d::ui::Widget *_container;

	cocos2d::ui::Text *_lbGold;
	cocos2d::ui::Text *_lbCC;

	int _curOffset;
	cocos2d::ui::ScrollView *_scrollView;
	
	std::vector<float> _listHeight;
	float _totalHeight;
	std::vector<ShopTabBase *> _listTabs;

public:
	static std::string TAB_IMG_BUTTON[numTab];
	static std::string SELECT_IMG_BUTTON[numTab];

	static const int OFFER_TAB = 0;
	static const int GACHA_TAB = 1;
	static const int ITEM_TAB = 2;
};


#endif