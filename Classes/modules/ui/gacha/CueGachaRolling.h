#ifndef CueGachaRolling_h_
#define CueGachaRolling_h_

#include <core/gui/BaseLayer.h>
#include "../popup/FullScreenPopup.h"
#include <data/GameConstant.h>
#include "../../../data/config/CueConfig.h"
#include "CueGachaElementSingleUI.h"

class CueGachaRolling
	:public BaseLayer
{
public:
	enum TAG
	{
		btnSkip
	};
	CueGachaRolling();
	~CueGachaRolling();

	CREATE_FUNC(CueGachaRolling);
	bool init();

	void initGUI();

	void initTag();

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void setRandomData(const std::vector<CueRateInBox*> &data, int cueItemIdx);

	void onOpenBoxSuccess(CueRateInBox *lastData);

	CueRateInBox* getARandomResult();

	void beginSlowEndRoll(float dt);

	void slowEndRoll();

	void calcDistanceFromChosenElementToCenter();

	void onAutoRollStopped();

	void postRollUpdate(float dt);

	void postRollUpdateDone(float dt);

	void rollUpdate(float dt);

	void skipAnimation();

	void moveAllElementDy(float dy);

	void highlightCenterElement();

private:
	cocos2d::ui::Widget *_pnBoxSide;
	cocos2d::ui::Button *_btnSkip;
	cocos2d::ui::Widget *_pnList;
	cocos2d::ui::ImageView *_imgCurrent;

	std::vector<CueGachaElementSingleUI*> _elements;

	float _upperBound;
	float _lowerBound;
	int _topElement;
	float _elementHeight;

	std::vector<CueRateInBox*> _data;
	CueRateInBox *_lastData;
	bool _rolling;
	int _boxCode;

	bool _endingAnimation;
	float _endAnimationTime;
	int _elementTilLast;
	float _distanceToMiddle;
};
#endif