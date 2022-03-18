#ifndef ShopTabLimited_h_
#define ShopTabLimited_h_

#include <core/gui/BaseLayer.h>
#include <data/GameConstant.h>
#include "ShopTabItem.h"

class ShopTabLimited
	: public ShopTabBase
{
public:
	ShopTabLimited();
	~ShopTabLimited();

	CREATE_FUNC(ShopTabLimited);

	bool init();

	void initTimer();

	void updateRemainTime(float dt);

	void initGUI();

	void setSizeTabLayer();

	void loadData(int idx);

	void loadDataLimited();

	void loadDataHotDeals();

	void reloadHotDeals();

	void updateDataHotDeals();

	cocos2d::Size getOriginalSize();

	void scaleToSize(cocos2d::Size size);
private:
	cocos2d::ui::Widget *_pnContent_0;
	cocos2d::ui::Widget *_pnContent_1;
	cocos2d::ui::Widget *_bgTop;

	cocos2d::ui::Text *_time_remain;

	int _remainTimeDeal;

	cocos2d::Layer *_listItemLimited;
	cocos2d::Layer *_listItem;
};

#endif // !ShopTabLimited_h_

