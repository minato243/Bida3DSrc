#ifndef ResultGift_h_
#define ResultGift_h_

#include <core/gui/BaseLayer.h>
#include "../base/UIBaseLayer.h"
#include "../popup/BasePopupLayer.h"
#include "data/GameConstant.h"
#include "data/DataStruct.h"

class ResultGift:
	public BasePopupLayer
{
public:

	enum TAG 
	{
		btnRePlay,
		btnLobby
	};

	ResultGift();
	~ResultGift();

	CREATE_FUNC(ResultGift);
	bool init();

	void initGUI();

	void initTag();

	void onButtonRelease(cocos2d::ui::Button* button, int id);

};

#endif