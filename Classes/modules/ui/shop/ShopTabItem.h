#ifndef ShopTabItem_h_
#define ShopTabItem_h_

#include <core/gui/BaseLayer.h>
#include <data/GameConstant.h>
#include "../../core/shop/ShopMgr.h"

class ShopTabBase
	:public BaseLayer
{
public:
	ShopTabBase() {};
	~ShopTabBase() {};

	virtual cocos2d::Size getOriginalSize() = 0;
};

class ShopTabItem
	:public ShopTabBase
{
public:
	enum TAG
	{
		btn_price_descending,
		btn_price_ascending,
		btn_prefer_break,
		btn_prefer_jump,
		btn_prefer_masses,
		box_sort
	};
	ShopTabItem();
	~ShopTabItem();

	CREATE_FUNC(ShopTabItem);
	bool init();

	static ShopTabItem *create(ShopCategory category);

	void initGUI();

	void initTag();

	void initListener();

	void loadData(ShopCategory category);

	void setSizeTabLayer(ShopCategory category);

	cocos2d::Size getOriginalSize();

	void scaleToSize(cocos2d::Size size);

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void sortListByCost(bool asc);

	bool compairCost(ShopItemData* i1, ShopItemData *i2);

	bool compairCostDec(ShopItemData* i1, ShopItemData *i2);

	void sortListByTypeBox(std::string typeBox);

	void searchBoxByName(const std::string & name);

	void resetListData();

	void isCorrectSearch(std::string item, std::string name);

	void updateGui(std::string title);

	void hidePanelSelection();

private:
	cocos2d::ui::ImageView *_boxSearch;
	cocos2d::ui::Widget *_pnContent;
	cocos2d::Node *_ndTop;
	cocos2d::ui::Text *_title;
	cocos2d::ui::Widget *_nd_selection;
	cocos2d::ui::ImageView *_imgArrow;
	cocos2d::ui::Widget *_lbText;
	cocos2d::ui::Button *_box_sort;
	cocos2d::ui::Text *_lbTextBoxSort;

	cocos2d::ui::TextField *_tfSearch;

	std::vector<ShopItemData *> _listData;
	cocos2d::Layer *_listItem;
public:
	bool _checkSelect;
};
#endif
