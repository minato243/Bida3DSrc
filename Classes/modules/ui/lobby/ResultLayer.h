#ifndef RESULT_LAYER_H_
#define RESULT_LAYER_H_

#include <core/gui/BaseLayer.h>
#include "../base/UIBaseLayer.h"
#include "../popup/BasePopupLayer.h"
#include "data/GameConstant.h"
#include "data/DataStruct.h"

struct ResultData 
{
	GameMode gameMode;
	int matchMode;
	int winnerIdx;

	ResultData() {};

	ResultData(GameMode gameMode, int matchMode, int winnerIdx)
	{
		this->gameMode = gameMode;
		this->matchMode = matchMode;
		this->winnerIdx = winnerIdx;
	}
};

class ResultLayer
	:public BasePopupLayer
{
public:
	ResultLayer();
	~ResultLayer();

	CREATE_FUNC(ResultLayer);
	bool init();

	void initGUI();
	
	void initComponent();

	void setInfo();

	void setPlayerWinInfo(int winerIdx);

	void onEnter();

	void setMode(GameMode gameMode, int matchMode);

	void setDataResult(const ResultData & data);

	void showResultGift(float dt);

private:
	cocos2d::Node *_nodeP0;
	cocos2d::Node *_nodeP1;
	cocos2d::Sprite *_imgWinLose;

	ResultData _result;
	std::vector<ItemInfoData> rewards;

};

#endif // !RESULT_LAYER_H_


