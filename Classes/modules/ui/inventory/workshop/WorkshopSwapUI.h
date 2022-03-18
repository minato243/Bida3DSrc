#ifndef WorkshopSwapUI_h_
#define WorkshopSwapUI_h_

#include <core/gui/BaseLayer.h>

class WorkshopSwapUI
	: public BaseLayer
{
public:
	WorkshopSwapUI();
	~WorkshopSwapUI();

	CREATE_FUNC(WorkshopSwapUI);
	bool init();

	void initGUI();
protected:
private:
};

#endif
