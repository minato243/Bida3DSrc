#ifndef IngameBaseUI_h_
#define IngameBaseUI_h_

#include <cocos2d.h>
#include <functional>
#include <core/gui/BaseLayer.h>
#include <cocos/ui/CocosGUI.h>
#include <data/DataStruct.h>
#include "../elements/PowerUI.h"
#include "PlacingBallControlUI.h"
#include "BallTypeNotify.h"
#include "CueTypeSocket.h"
#include "../../core/match/EightBallPoolMatch.h"

class IngameBaseUI: public BaseLayer
{
public:
	typedef std::function<void(float)> ChangeCueAngleCallBack;

	typedef std::function<void(bool)> ToggleCamModeCallBack;

	enum TAG
	{
		TAG_BTN_BALL	= 0,
		TAG_BTN_ANGLE	= 1,
		TAG_BTN_RULE	= 2,
		TAG_BTN_EYE		= 3,
		TAG_BTN_MOVE_BALL	= 4,
		TAG_BTN_CUE		= 5
	};

	IngameBaseUI();
	~IngameBaseUI();

	CREATE_FUNC(IngameBaseUI);
	virtual bool init();

	void initGUI();

	void initUIStatesPos();
	
	void initUIEffects();

	void initPowerbar();

	void initPlacingBall();

	void initBallTypeNotify();

	void initAngleNode();

	void initRulerNode();

	void initCueTypeSocket();

	void onEnter();

	virtual void update(float dt);

	void onPowerBarRelease(float percent);

	void onPowerBarChange(float percent);

	void setMyTurnLayout();

	void setEnemyTurnLayout();

	void updateTurnLayout(bool isMyTurn, bool onlyHUD = false);

	void setCamMode3DLayout();

	void setCamMode2DLayout();

	void setCamModeFreeLayout();

	void notifyBallType(bool isSolid);

	void notifySelectPocket();

	void subscribeChangeCueAngle(const ChangeCueAngleCallBack& callback);

	void subscribeToggleCamMode(const ToggleCamModeCallBack & callback);

	void onButtonTouched(cocos2d::ui::Button* button, int id);

	void onButtonTouchedMove(cocos2d::ui::Button* button, int id);
	
	void onButtonRelease(cocos2d::ui::Button* button, int id);
	
	void onButtonCanceled(cocos2d::ui::Button* button, int id);
	
	void onCustomTouchEventHandler(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void onBelowButtonTouchingUpdate(float dt);

	void submitBallTarget(cocos2d::Vec2 pos, bool moving = false);

	void submitMoveMili(float posX);

	void moveDXButtonRuler(float dx);

	void enablePlacingBall(cocos2d::Vec2 startPosition, const PlacingBallControlUI::PlaceBallCallBack & listener);

	void setPlacingPosition(cocos2d::Vec2 position);

	void disablePlacingBall();

	void setBallIndicatorOffset(cocos2d::Vec2 offset, bool percent = true);

	void setUIAngle(float angle);

	void setCueIcon(const int & cueType, double cueRange[]);

	void forceHideBigIcon();

	void changePnCueIconBig();

	PowerUI * getPowerUI();

	void setCallPocketLayout(bool placing);

	void enable();

	void disable();

	void startMatch(EightBallPoolMatch *match);

protected:
	cocos2d::ui::Button *_btnBall;
	cocos2d::ui::Button *_btnAngle;
	cocos2d::ui::Button *_btnRuler;
	cocos2d::ui::Button *_btnEye;
	cocos2d::ui::Button *_ballIndicator;
	
	cocos2d::ui::ImageView *_imgPowerBar;
	cocos2d::ui::ImageView *_imgBgBtnCue;
	cocos2d::ui::Button *_btnCue;

	cocos2d::Node *_nodePlaceBall;
	cocos2d::ui::ImageView *_imgCollideBall;
	cocos2d::ui::ImageView *_imgPlaceBall;

	cocos2d::ui::ImageView *_imgAngleCue;
	cocos2d::ui::Text *_lbAngleVal;

	cocos2d::ui::ImageView *_ballIndicatorTarget;

	cocos2d::Sprite * _imgBarsBtnRuller;
	cocos2d::Sprite * _imgBars1BtnRuller;

	cocos2d::ui::Widget *_pnCueIcon;

	cocos2d::ui::ImageView *_imgTimerBg;

	cocos2d::DrawNode *_angleStencil;

	PowerUI *_powerBar;
	BallTypeNotify *_ballTypeNotify;
	PlacingBallControlUI *_placingBallControl;

	std::vector<cocos2d::Node *> _uis;
	std::vector<cocos2d::Vec2> _uiShowPoss;
	std::vector<cocos2d::Vec2> _uiHidePoss;

	bool _isMyTurn;
	bool _isDisable;
	BallIndicatorData _ballIndicatorData;

	ChangeCueAngleCallBack _changeCueAngleCallBack;
	ChangeCueAngleData _changeCueAngleData;

	CueTypeSocket *_cueTypeSocket;
	CueTypeSocket *_cueTypeSocketBig;

	ToggleCamModeCallBack _toggleCamModeCallBack;

	std::vector<cocos2d::ui::Button *> _buttonList;
	std::vector<float> _btnDistList;

	long _touchBeganTimeBtnBall;
	cocos2d::Vec2 _beforePosBtnBall;

	float _lastYBtnAngle;
	float _lastXBtnRuler;
	float _midPointBtnRuler;
	float _curAngle;

	int _cueType;

	std::vector<cocos2d::ui::ImageView *> _sockets;
	std::vector<cocos2d::Vec2> _socketPos;
	std::map<cocos2d::ui::ImageView *, int> _socketTypes;

	EightBallPoolMatch *_match;

public:
	static const float BALL_INDICATOR_REGION_SCALE;
	static const float CHANGE_ANGLE_SPEED;
	static const float MAX_ANGLE;
	static const float RADIAN_PER_DEGREE;

};

#endif // !IngameBaseUI_h_



