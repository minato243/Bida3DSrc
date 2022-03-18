#ifndef InventoryItemElementUI_h_
#define InventoryItemElementUI_h_

#include <cocos2d.h>
#include <core/gui/BaseLayer.h>
#include "../../../data/DataStruct.h"
#include "../../core/inventory/ItemInfo.h"

class InventoryItemElementUI
	: public BaseLayer
{
public:
	enum TAG
	{
		btnOpenMulti,
		btnOpen,
		btnInfo
	};
	InventoryItemElementUI();
	~InventoryItemElementUI();

	bool init();

	void initGUI();

	void initTag();

	void autoResize();

	cocos2d::Size getOriginalSize();

	void loadData(ItemInfo *data);

	void onButtonRelease(cocos2d::ui::Button* button, int id);

private:
	cocos2d::ui::Button *_btnOpenMulti;
	cocos2d::ui::Button *_btnOpen;
	cocos2d::ui::Button *_btnInfo;

	cocos2d::ui::Text *_lbTitle;
	cocos2d::ui::Text *_lbQuantity;
	cocos2d::ui::ImageView *_imgItem;

	cocos2d::Size _size;
	ItemInfo *_data;

public:
};
#endif // !InventoryItemElementUI_h_
