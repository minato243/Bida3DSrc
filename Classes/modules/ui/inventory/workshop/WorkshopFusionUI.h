#ifndef WorkshopFusionUI_h_
#define WorkshopFusionUI_h_

#include <core/gui/BaseLayer.h>

class WorkshopFusionUI
	: public BaseLayer
{
public:
	WorkshopFusionUI();
	~WorkshopFusionUI();

	CREATE_FUNC(WorkshopFusionUI);
	bool init();

	void initGUI();
protected:
private:
};
#endif