#ifndef ShopScene_h_
#define ShopScene_h_

#include "../../../core/gui/BaseScene.h"
#include "../../../core/gui/BaseLayer.h"

class ShopScene
	:public BaseScene
{
public:
	ShopScene();
	~ShopScene();

	CREATE_FUNC(ShopScene);
	bool init();

	void initCameras();

	void onSceneReload();

	void onEnter();

	void toShopTab();

private:
	BaseLayer *_ui;
};

#endif // !ShopScene_h_
