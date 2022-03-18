#ifndef OfferBaseUI_h_
#define OfferBaseUI_h_

#include "../base/UIBaseLayer.h"
#include "../../core/offer/OfferEntity.h"

class OfferBaseUI
	:public UIBaseLayer
{
public:
	OfferBaseUI();
	~OfferBaseUI();

	enum TAG
	{
		btnClose
	};

	CREATE_FUNC(OfferBaseUI);
	bool init();

	void initGUI();

	void initTag();

	void show();

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void initTabOffer(OfferType offerId);

	void runAnimation();

	void initListOffer();

	void switchOffer(OfferType offerId);

	void loadListOffer();

	void changeStatusListOffer(OfferType offerId);
private:
	cocos2d::ui::Widget *_pnOffer;
	cocos2d::ui::Widget *_pnListOffer;

	cocos2d::ui::ImageView *_imgLight;
	cocos2d::ui::ImageView *_imgLightGround;

	std::map<OfferType, cocos2d::Node *> _listTabOffer;

	cocos2d::Node *_listBtnOffer;
	OfferType _curTab;
};
#endif