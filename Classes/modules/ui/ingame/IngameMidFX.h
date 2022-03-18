#ifndef IngameMidFX_h_
#define IngameMidFX_h_

#include "../base/UIBaseLayer.h"
#include <spine-cocos2dx.h>
#include "data/GameConstant.h"

class IngameMidFX
	:public UIBaseLayer
{
public:
	IngameMidFX();
	~IngameMidFX();

	CREATE_FUNC(IngameMidFX);
	bool init();

	void initGUI();

	void initFX();

	void loadData(int userScore, int enemyScore);

	void runFX(int userScore, int enemyScore, int winnerIdx);

	void updateNewScore();

	void onRunFXComplete();

	void setCallback(CUSTOM_CALLBACK callback);

private:
	cocos2d::Sprite *_text_Image_0;
	cocos2d::Sprite *_text_Image_1;

	cocos2d::Node *_panelFX;

	spine::SkeletonAnimation *_fxMidGame;

	int _userScore, _enemyScore;

	CUSTOM_CALLBACK _callback;
};

#endif
