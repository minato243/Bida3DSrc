#ifndef CueGachaElementSingleUI_h_
#define CueGachaElementSingleUI_h_

#include <cocos2d.h>
#include <core/gui/BaseLayer.h>
#include "../popup/FullScreenPopup.h"
#include <data/GameConstant.h>
#include "../../core/shop/ShopItemData.h"
#include "CueGachaElementUI.h"

class CueGachaElementSingleUI
	:public CueGachaElementUI
{
public:
	enum TAG
	{
		btnInspect
	};
	CueGachaElementSingleUI();
	~CueGachaElementSingleUI();

	CREATE_FUNC(CueGachaElementSingleUI);
	bool init();

	void initSprite();

	static CueGachaElementSingleUI* create(cocos2d::Size size);

	void setButtonInspectVisible(bool isVisible);

	void loadData(CueRateInBox *data);

	void highlight(bool isHighlight);

	CueRateInBox *getData();

	void updateRenderTexture();

	void setOpacity(int val);

	cocos2d::Sprite *getSprite();

	void scaleSpriteAndModel(float scale);

private:
	cocos2d::RenderTexture* _rt;
	cocos2d::Sprite *_sprite;
};
#endif