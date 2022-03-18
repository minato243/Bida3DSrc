#ifndef ShopPopupConfirm_h_
#define ShopPopupConfirm_h_

#include <core/gui/BaseLayer.h>
#include <data/GameConstant.h>
#include "../../core/shop/ShopItemData.h"

class ShopPopupConfirm
	:public BaseLayer
{
public:
	enum TAG
	{
		btnConfirm,
		btnCancel
	};
	ShopPopupConfirm();
	~ShopPopupConfirm();

	CREATE_FUNC(ShopPopupConfirm);
	bool init();

	void initGUI();

	void initTag();

	cocos2d::Size getOriginalSize();

	void loadData(ShopItemData * data);

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void processConfirm();

public:
	cocos2d::Node *_ndValue;
	cocos2d::Node *_ndCost;
	cocos2d::Node *_ndCostOld;

	cocos2d::ui::Text *_lbNdValue;
	cocos2d::Sprite *_sprNdValue;
	cocos2d::ui::Text *_lbNdCost;
	cocos2d::Sprite *_sprNdCost;
	cocos2d::ui::Text *_lbNdCostOld;
	cocos2d::Sprite *_sprNdCostOld;

	cocos2d::ui::Button *_btnCancel;
	cocos2d::ui::Button *_btnConfirm;
	cocos2d::ui::Text *_text;

	ShopItemData *_data;
};

#endif