#ifndef 	_INGAMEUINEW_H_
#define 	_INGAMEUINEW_H_

#include <cocos2d.h>
#include <functional>
#include <core/gui/BaseLayer.h>
#include <cocos/ui/CocosGUI.h>
#include <data/DataStruct.h>
#include "../elements/PowerUI.h"
#include "BallTypeNotify.h"
#include "PlacingBallControlUI.h"
#include "IngameHUD.h"

class IngameUINew: 
	public BaseLayer
{
public:
	typedef std::function<void(float)> ChangeCueAngleCallBack;
	CREATE_FUNC(IngameUINew);
	IngameUINew();
	~IngameUINew();
	bool init();
	void initGUI();

	void initComponent();
	void initBallTypeNotify();
	void initPlacingBall();
	void initTurnStates();
	void notifyBallType(int isSolid);
	void notifySelectPocket();
	void initAngleNode();
	void initHUD();
	void initHUDBalls();
	void initAvatarNodes();
	void initTimers();
	void createCircleAvatarFromImgView(cocos2d::ui::ImageView *image, int i);

	void onEnter();

	void updateAngle(float dt);

	void onButtonTouched(cocos2d::ui::Button* button, int id);
	void onButtonTouchedMove(cocos2d::ui::Button* button, int id);
	void onButtonRelease(cocos2d::ui::Button* button, int id);
	void onButtonCanceled(cocos2d::ui::Button* button, int id);
	void onCustomTouchEventHandler(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void submitBallTarget(cocos2d::Vec2 pos, bool moving = false);
	void setBallIndicatorOffset(cocos2d::Vec2 offset, bool percent = true);

	void initRulerAndPowerBar();
	void onPowerBarRelease(float percent);
	void onPowerBarChange(float percent);

	void subscribeChangeCueAngle(const ChangeCueAngleCallBack& callback);

	IngameHUD *getHUD();
	PowerUI *getPowerUI();

	cocos2d::Vec2 getNodeP2ShowPosition();
	cocos2d::Vec2 getNodeP2HidePosition();

	void disablePlacingBall();

	void setMyTurnLayout();

	void setEnemyTurnLayout();

	void updateTurnLayout(bool isMyTurn);
	
	void enablePlacingBall(cocos2d::Vec2 startPosition, const PlacingBallControlUI::PlaceBallCallBack & listener);

	void setPlacingPosition(cocos2d::Vec2 position);

	void setCamMode3DLayout();

	void setCamMode2DLayout();

	void setCamModeFreeLayout();

private:
	bool _showAngle;
	cocos2d::ui::ImageView *_bg;
	cocos2d::ui::Button *_btnBall;
	cocos2d::ui::Button *_ballIndicator;
	cocos2d::ui::ImageView *_ballIndicatorTarget;
	cocos2d::ui::Widget *_pnAvatars;
	cocos2d::ui::Widget *_pnHud;
	cocos2d::ui::ImageView *_imgAvatarBox;
	cocos2d::Node *_nodeP1;
	cocos2d::Node *_nodeP2;
	cocos2d::ui::ImageView *_imgBgBtnCue;
	cocos2d::ui::ImageView *_imgBallCueGroup;
	cocos2d::Node *_nodePlaceBall;
	cocos2d::ui::ImageView *_imgPlaceBall;
	cocos2d::ui::ImageView *_imgCollideBall;

	cocos2d::Vec2 _nodeP1ShowPosition;
	cocos2d::Vec2 _nodeP1HidePosition;
	cocos2d::Vec2 _nodeP2ShowPosition;
	cocos2d::Vec2 _nodeP2HidePosition;

	cocos2d::ProgressTimer* _timers[2];

	BallIndicatorData _ballIndicatorData;
	BallTypeNotify *_ballTypeNotify;
	PlacingBallControlUI *_placingBallControl;
	IngameHUD *_hud;
	PowerUI *_powerBar;

	cocos2d::ui::Button *_btnAngle;
	cocos2d::ui::ImageView *_imgAngleCue;
	cocos2d::ui::Button *_btnMoveBall;
	cocos2d::ui::Button *_btnSelectPocket;

	std::vector<cocos2d::Node *> _stateObjects;
	std::vector<float> _stateOriginal;
	std::vector<float> _stateTransform;

	ChangeCueAngleCallBack _changeCueAngleCallBack;
	ChangeCueAngleData _changeCueAngleData;
	
	std::vector<cocos2d::Sprite*> _avatarSprite;

	bool _isMyTurn;

	public:
		static const int TAG_BTN_BALL = 0;
		static const int TAG_BTN_ANGLE = 1;
		static const int TAG_BTN_MOVE_BALL = 2;
};
#endif