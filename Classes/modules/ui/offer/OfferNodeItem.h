#ifndef OfferNodeItem_h_
#define OfferNodeItem_h_

#include "../base/UIBaseLayer.h"
#include "../../core/offer/OfferEntity.h"

class OfferNodeItem
	:public UIBaseLayer
{
public:
	OfferNodeItem();
	~OfferNodeItem();

	enum TAG
	{
		btn
	};

	CREATE_FUNC(OfferNodeItem);
	bool init();

	void initGUI();

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void loadData(BaseOffer *data);

	cocos2d::Size getOriginalSize();

	void switchOffer();

	void setStatusSelect(bool isSelect);

private:
	cocos2d::ui::Text *_lb;
	cocos2d::ui::ImageView *_imgButton;
	cocos2d::ui::ImageView *_iconButton;

public:
	OfferType _type;

};

#endif