#ifndef BaseOfferTypeUI_h_
#define BaseOfferTypeUI_h_

#include "../base/UIBaseLayer.h"

class BaseOfferTypeUI
	:public UIBaseLayer
{
public:
	BaseOfferTypeUI();
	~BaseOfferTypeUI();

	virtual void loadData() = 0;

};

#endif