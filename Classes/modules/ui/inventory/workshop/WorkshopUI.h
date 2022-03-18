#ifndef WorkShopUI_h_
#define WorkShopUI_h_

#include <core/gui/BaseLayer.h>

class WorkshopUI
	: public BaseLayer
{
public:
	enum Tab
	{
		UNKNOWN = -1,
		UPGRADE = 0,
		FUSION = 1,
		SWAP =2
	};
	enum TAG
	{
		BTN_TAB_UPGRADE,
		BTN_TAB_FUSHION,
		BTN_TAB_SWAP,
		BTN_BACK,
		BTN_INVENTORY,
	};

	WorkshopUI();
	~WorkshopUI();

	CREATE_FUNC(WorkshopUI);
	bool init();

	void initGUI();

	void initTabs();

	void refresh();

	void setTrackingCue(int cue);

	void switchTab(Tab tabIdx);

	void setParentScene(cocos2d::Scene *scene);

	void onButtonRelease(cocos2d::ui::Button* button, int id);

private:
	cocos2d::ui::Widget *_bg;
	cocos2d::ui::ImageView * _imgTabs;
	cocos2d::ui::Button * _btnTabUpgrade;
	cocos2d::ui::Button * _btnTabFusion;
	cocos2d::ui::Button * _btnTabSwap;

	Tab _curTab;
	std::vector<BaseLayer *> _tabs;
	std::vector<cocos2d::ui::Button *> _btnTabs;
	std::vector<std::string> _texNameActives;
	std::vector<std::string> _texNameInactives;
};

#endif