#ifndef ShopItemCueBox_h_
#define ShopItemCueBox_h_

#include <core/gui/BaseLayer.h>
#include <data/GameConstant.h>
#include "../../../data/DataStruct.h"
#include "../../core/shop/ShopMgr.h"
#include "../../core/shop/ShopItemData.h"

class ShopItemCueBox
	: public BaseLayer
{
public:
	enum TAG
	{
		btnImgItem
	};

	ShopItemCueBox();
	~ShopItemCueBox();

	CREATE_FUNC(ShopItemCueBox);
	bool init();

	void initGUI();

	void loadData(ShopItemData *data);

	cocos2d::Size getOriginalSize();

	void loadDataCueBox(ShopItemData *data);

	void loadDataHotDeal(ShopItemData *data);

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void showPopupConfirm();

private:
	cocos2d::ui::Button *_btnImgItem;

	cocos2d::Node *_ndValue;
	cocos2d::Node *_ndCost;
	cocos2d::Node *_ndCostOld;
	cocos2d::ui::ImageView *_imgBoxNameBox;

	cocos2d::ui::Text *_lbNdValue;
	cocos2d::ui::Text *_lbNdCost;
	cocos2d::ui::Text *_lbNdCostOld;
	cocos2d::ui::Text *_lbBoxName;

	cocos2d::Sprite *_sprNdValue;
	cocos2d::Sprite *_sprNdCost;
	cocos2d::Sprite *_sprNdCostOld;

	cocos2d::ui::Widget *_bgVisible;

	ShopItemData *_data;
};
#endif
