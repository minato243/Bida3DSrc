#ifndef InteractionManager_H_
#define InteractionManager_H_

#include <modules/core/ps/internal/ExtensionMath.hpp>
#include <data/DataStruct.h>
#include <modules/ui/ingame/IngameUI.h>
#include <modules/core-implement/Table.h>
#include "TouchManager.h"
#include <modules/camera-view/GameCamera.h>
#include <modules/core/utils/LerpUtils.h>
#include <modules/core/utils/TriggerUtil.h>
#include <modules/ui/ingame/IngameBaseUI.h>
#include "CameraPosition.h"
#include "CameraTransition.h"
#include "CueAngleSupporter.h"
#include "../ps/internal/CueAngle.h"


#define interactionMgr  InteractionManager::getInstance()

#define CC_CALLBACK_4(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, ##__VA_ARGS__)

class InteractionManager
{
public:
	enum CamModes {
		MODE_3D,
		MODE_FREE,
		MODE_2D,
		MODE_CALL_POCKET,
		MODE_VIEWER
	};

	enum Turns{
		UNDEFINED,
		MY_TURN,
		COMPETITOR_TURN
	};

	InteractionManager();
	~InteractionManager();

	static InteractionManager *getInstance();
	static void destroyInstance();

	void setEntities(GameCamera *camera, Cue *cue, Ball *cueBall, IngameBaseUI *ingameUI, TouchManager *touchManager, CueAngle *cueAngle);

	void startShoting();

	void onWorldPaused();

	void startNewMatch();

	void resetFlagTurn();

	void switchTurn(Turns turn, bool isPlacingCueBall, bool isCallingPocket);

	void startChangingCueOffset();

	void stopChangingCueOffset();

	void onSubmitBallOrPocket();

	void stopPlacingCueBall();

	void setCueOffset(ps::ExtMath::vector offset);

	void toggleCamMode(bool isPress);

	void onChangeAngle(float angle);

	void updateCueRPTDirection(Rpt rpt);

	void updateUIAngle();

	void handleShoot(float percent);

	void subscribeTouchHandlers();

	void handleInteractMoving(int event, cocos2d::Vec2 location, cocos2d::Vec2 delta, cocos2d::Vec2 velocity);

	void handleInteractMoving3D(cocos2d::Vec2 velocity);

	void updateCueAngleMinValues();

	void handleTouchDirection(cocos2d::Vec2 location, float duration);

	void handleTouchOnPocket(cocos2d::Vec2 location, float duration);

	void submitSelectPocket();

	void notifyDirection(bool force = false);

	void handleCueLerp(Rpt rpt);

	void handleCamLerp(Rpt rpt, ps::ExtMath::vector offset);

	void handerCueBallLerp(ps::ExtMath::vector pos);

	void update(float dt);

	ps::ExtMath::vector cueBallLockAt();

	CamCoord calcCameraCoord(Rpt rpt);

	void updateIngamePlacingPosition();

	void updateSuggestedDirection();

	void updateSpecialCameraPosition();

	double calcMinimumDepthForRetangle(float visibleWidth, float visibleHeight, float tableWidth, float tableHeight);

	double calcMinimumDepthForPoint();

	double calcMinimumDepthForTable();

	//change mode
	void enable2D();

	void enable3D();

	void enableFreeCam();

	void enablePlayingMode();

	void startMovingOverview(CUSTOM_CALLBACK callback);

	void startFocusOnTarget();

	void topOverView();

	void onMovingOverviewComplete();

	void onFocusOnTargetComplete();

	void ballOutsideDetect();

	void movingOverView(float dt, tu::TransitionData * data);

	void focusOnTarget(float dt, tu::TransitionData * data);

	GameCamera *getCamera();

	void setSelectingPocket(bool isSelecting);

	void enableCallPocketMode(bool placing);

	void enablePlacingMode();

	void enableViewerMode();

	void restorePlayingMode();

	void handleChangePlacingBall(cocos2d::Vec2 location, bool finish = false);

	void handleChangePlacingBallNoLocation();

	void receiveCueDirection(const ps::ExtMath::vector & direction);

	void receiveCueBallPosition(const ps::ExtMath::vector & pos);

	void receiveStopInterpolateCueBall(const ps::ExtMath::vector & pos);

	void doMovingMili(float d);

	ps::ExtMath::vector cueBallLookAt();

	double findNearestAngle(const double & current, const double candidates[], int length);

	void lockTransition();

	void notifyWaitTransition();

	void setGhostBallPosition(const vector & pos);
	
	void setGhostBallPosition(const vector& pos, const vector& dir);

private:
	static InteractionManager* _instance;

	double _width;
	double _height;

	ps::ExtMath::vector _secondViewCamPos;
	ps::ExtMath::vector _topViewCamPos;
	ps::ExtMath::vector _callPocketCamPos;

	double _minBallViewAlpha;
	double _maxBallViewAlpha;

	ps::ExtMath::AABB _safeViewZone;

	Cue *_cue;
	Ball *_cueBall;
	
	IngameBaseUI *_ingameUI;

	GameCamera *_camera;

	TouchManager *_touchMgr;

	int _handleInteractMovingId;
	int _handleTouchDirectionId;
	int _handleTouchOnPocketId;

	CueAngle *_cueAngle;

	SphericalLerp *_cueLerp;
	RPTOLerp *_camLerp;
	VectorLerp *_cueBallLerp;

	tu::TimeTrigger *_ballOutsideDetect;
	tu::LimitedTimeTrigger *_movingOverview;
	tu::LimitedTimeTrigger *_focusOnTarget;
	tu::TimeoutTrigger *_topOverview;

	CameraTransition *_ct;
	CueAngleSupporter *_cas;

	ps::ExtMath::vector _freeCamRoot;

	long _lastDirectionUpdateTime;
	
	int _pocketId;

	bool _selectingPocket;

	bool _flagPlacing		= false;
	bool _flagCallPocket;
	CamModes _flagCamMode	= CamModes::MODE_3D;
	Turns _flagTurn			= Turns::UNDEFINED;
	bool _flagRunning		= false; 
	bool _camLerpWhileSetBall;

	long _lastCueBallUpdateTime;

	bool _firstTurn;

	CUSTOM_CALLBACK _movingOverViewCallback;

	bool _lockTransition;
	CUSTOM_CALLBACK _waitTransitionCB;
public:
	Rpt _cueRPT;
	Rpt _viewRPT;

	static const double ACCEPTABLE_TOUCH_DURATION;
	static const double MIN_TOUCH_X;
	static const double MAX_TOUCH_X;
	static const double MIN_TOUCH_Y;
	static const double MAX_TOUCH_Y;
	static const double MIN_RESTRAIN_LOW;
	static const double MAX_RESTRAIN_HIGH;
	static const double FREE_CAM_MIN_BETA;
	static const double FREE_CAM_MAX_BETA;

	static const double DX_DELTA_MOVING;
	static const double DY_DELTA_MOVING;
	static const double DY_DELTA_BETA;
	static const double MIN_INTERACT_T;
	static const double MAX_INTERACT_T;

	static const double DELTA_CAM_CUE_T_MIN;
	static const double DELTA_CAM_CUE_T_MAX;
	static const double DELTA_CAM_CUE_P_HIGH;

	static const double ELIPSE_WIDTH;
	static const double ELIPSE_HEIGHT;
	static const double RECT_WIDTH;
	static const double RECT_HEIGHT;
	static const double RECT_RADIUS;

	static const double TABLE_PADDING;
	static const double SECOND_VIEW_ANGLE;
 
	static const double TARGET_THETA_OVERVIEW;
	static const double OVERVIEW_TOTAL_TIME;
	static const double OVER_TO_TOP_TIME;

	static const double FOCUS_TOTAL_TIME;

	static const double THRESH_HOLD_TOP_VIEW;

	static const ps::ExtMath::vector BALL_PLACING_OFFSET;
	
	static const cocos2d::Vec3 LOOKAT_2D_UP;

	static int const OVERVIEW_VIEWER_P_CANDIDATES_LENGHT = 2;

	static double const OVERVIEW_VIEWER_P_CANDIDATES[OVERVIEW_VIEWER_P_CANDIDATES_LENGHT];

	static const int MOVING_MILI_VELOCITY = 7;
};


#endif // !InteractionManager_H_


