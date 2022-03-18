#ifndef CueInspectScene_h_
#define CueInspectScene_h_

#include "../../../core/gui/BaseScene.h"
#include "../../../core/gui/BaseLayer.h"
#include "../../core/inventory/CueInfo.h"

class CueInspectScene
	:public BaseScene
{
public:
	CueInspectScene();
	~CueInspectScene();

	CREATE_FUNC(CueInspectScene);
	bool init();

	void onSceneReload();

	void onEnter();

	void loadData(CueInfo *data, bool isOwn, bool isGacha = false, bool isFusion = false);

private:
	BaseLayer *_ui;
	CueInfo *_data;

	bool _isOwn;
	bool _isGacha;
	bool _isFusion;
};

#endif // !CueInspectScene_h_
