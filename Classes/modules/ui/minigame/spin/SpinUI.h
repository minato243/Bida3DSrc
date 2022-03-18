#ifndef SpinUI_h_
#define SpinUI_h_

#include "../../base/UIBaseLayer.h"

class SpinUI
	: public UIBaseLayer
{
public:
	SpinUI();
	~SpinUI();

	enum TAG
	{
		btnSpin,
		btnClose
	};
	CREATE_FUNC(SpinUI);
	bool init();

	void initGUI();

	void initTag();

	void onButtonTouched(cocos2d::ui::Button* button, int id);

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void loadData();

	void spinWheel();

	void spinWheelToCellId(int targetId);

	void updateAll(float dt);

	void show();

	void updateComponent();

	void onRotatingFinished();

	void runAnimReward();

	void runAnimShowWheel();

	void createMovingCoins();

	void onAnimRewardComplete();

	void updateNumberOfSpin();

	void onEnter();

	void onExit();

	float randomAngle(float s, float e);

	cocos2d::Node *goldPanel();

private:
	cocos2d::ui::Text *_lbNotice2;
	cocos2d::ui::Text *_lbNotice3;

	cocos2d::Node *_wheelNode;
	cocos2d::Sprite *_imgHighlight;
	cocos2d::Sprite *_imgLight1;

	cocos2d::ui::ImageView *_imgWheel;
	cocos2d::Node *_spinButtonPanel;
	cocos2d::ui::Text *_lbSpinText;

	cocos2d::Node *_rewardNode;
	cocos2d::ui::Text *_lbGoldReward;
	cocos2d::Sprite *_imgBackLight;

	float _angle, _angleSpeed, _dAngleSpeed;

	float _backLightOpacity;
	float _backLightScale;
	float _dBackLightOpacity;
	float _dBackLightScale;

	bool _isRotating;
	int _targetCellId;
};
#endif
