#ifndef TestScene_h_
#define TestScene_h_

#include "../../core/gui/BaseScene.h"
#include "../core-implement/Table.h"
#include "../core-implement/Ball.h"
#include "../camera-view/GameCamera.h"
#include "../core/ps/TrajectoryBuilder.h"
#include "../core/ps/internal/CueAngle.h"
#include "../../../cocos2d/effekseer/effekseer/Effekseer.h"
#include "../ui/ingame/IngameBaseUI.h"
#include "../ui/lobby/LobbyUI.h"
#include "../ui/lobby/FindMatchLayer.h"
#include "UITestLayer.h"

class TestScene
	:public BaseScene
{
public:
	Table* _table;
	Ball* _cueBall;
	Cue* _cue;
	GameCamera* _camera;
	TrajectoryBuilder* _trajectoryBuilder;
	CueAngle *_cueAngle;

	efk::EffectManager *_efkManager;
	efk::Effect *_lobby3D;
	efk::EffectEmitter *_lobbyEfx;

	IngameBaseUI *_ingameUI;
	LobbyUI *_lobbyUI;
	FindMatchLayer *_findMatchUI;

public:

	TestScene();
	~TestScene();

	static cocos2d::Scene* createScene();

	CREATE_FUNC(TestScene);
	virtual bool init();
	void initBall();
	void initTable();
	void initCue();
	void initCamera();
	void initUI();
	void initTrajectoryBuilder();
	void initEfkManager();
	void initLobby();
	void initCueAngle();
	void initListener();

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	void touchEvent(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);

	void onMouseScroll(cocos2d::Event *event);

	void rotationUpdate(float dt);
	void cueRotationUpdate(float dt);

	void onEnter();
	IngameBaseUI *getIngameUI();

	void updateTableDirection(double force);

private:
	UITestLayer* ui;

	int rotateCount;
	int cueRotateCount;
	cocos2d::Vec3 rotateSign;
	cocos2d::Vec3 cueSign;
	double yaw = 0;
	double pitch = 0;
	static cocos2d::Vec3 ROTATE_SIGN[250];
};
#endif
