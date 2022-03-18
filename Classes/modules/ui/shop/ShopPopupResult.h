#ifndef ShopPopupResult_h_
#define ShopPopupResult_h_

#include <core/gui/BaseLayer.h>
#include <data/GameConstant.h>
#include "../../core/shop/ShopItemData.h"
#include "../popup/BasePopupLayer.h"

class ShopPopupResult
	:public BasePopupLayer
{
public:
	enum TAG
	{
		btnBuy
	};
	ShopPopupResult();
	~ShopPopupResult();

	CREATE_FUNC(ShopPopupResult);
	bool init();

	void initGUI();

	void initTag();

	void noticeSuccess();

	void noticeError(std::string tag);

	void onButtonRelease(cocos2d::ui::Button* button, int id);

private:
	cocos2d::Node *_ndSuccess;
	cocos2d::Node *_ndError;
	cocos2d::ui::Button *_btnBuy;

	int _idx;
};

#endif