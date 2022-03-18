#ifndef ShopItemLimited_h_
#define ShopItemLimited_h_

#include <core/gui/BaseLayer.h>
#include <data/GameConstant.h>
#include "../../../data/DataStruct.h"

class ShopItemLimited
	: public BaseLayer
{
public:
	ShopItemLimited();
	~ShopItemLimited();

	CREATE_FUNC(ShopItemLimited);
	bool init();

	void initGUI();

	void loadData(int * data);

	cocos2d::Size getOriginalSize();

private:
	cocos2d::ui::ImageView *_img;
	cocos2d::ui::Text *_lbName;
};
#endif
