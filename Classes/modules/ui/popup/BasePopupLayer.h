#ifndef BasePopupLayer_h_
#define BasePopupLayer_h_

#include "../../../core/gui/BaseLayer.h"

class BasePopupLayer
	:public BaseLayer
{
public:
	BasePopupLayer();
	~BasePopupLayer();

	CREATE_FUNC(BasePopupLayer);
	virtual bool init(std::string id= "");

	virtual void initGUI();

	void initBlockListener();

	virtual void onEnter();

	virtual void onCloseDone();

protected:
	bool _swallowTouchBackground;
};
#endif