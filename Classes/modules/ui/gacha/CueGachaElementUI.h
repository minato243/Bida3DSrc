#ifndef CueGachaElementUI_h_
#define CueGachaElementUI_h_

#include <core/gui/BaseLayer.h>
#include "../popup/FullScreenPopup.h"
#include <data/GameConstant.h>
#include "../../core/shop/ShopItemData.h"

class CueGachaElementUI
	:public BaseLayer
{
public:
	enum TAG
	{
		btnInspect
	};
	CueGachaElementUI();
	~CueGachaElementUI();

	CREATE_FUNC(CueGachaElementUI);
	bool init();
	
	void initGUI();

	void initTag();

	void setContainerRect(cocos2d::Rect rect);

	cocos2d::Size getOriginalSize();

	void addCueModel(const std::string & model, const std::string & tex);

	void loadData(CueRateInBox *data);

	std::string loadCueName(int type);

	void setNumStar(int num);

	void onButtonRelease(cocos2d::ui::Button* button, int id);

protected:
	cocos2d::Size _size;
	cocos2d::ui::Text *_lbName;
	cocos2d::ui::Text *_lbGeneralPoint;
	
	cocos2d::ui::Button *_btnInspect;

	cocos2d::Rect _rect;
	bool _isInitRect;
	
	std::map<std::string, cocos2d::Sprite3D*> _cues;
	cocos2d::Sprite3D* _cue3D;
	std::map<cocos2d::Sprite3D *, float > _cueOriginalScale;
	std::map<cocos2d::Sprite3D *, cocos2d::Vec2> _cueRatio;

	CueRateInBox* _data;
protected:
};
#endif