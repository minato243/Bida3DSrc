#ifndef GAMESCENE_H_
#define GAMESCENE_H_

#include <cocos2d.h>
#include "ui/CocosGUI.h"
#include <modules/core-implement/Table.h>
#include <modules/core-implement/Ball.h>
#include <modules/core-implement/Cue.h>
#include <modules/camera-view/SphericalCamera.h>
#include "UITestLayer.h"
#include <modules/core/ps/TrajectoryBuilder.h>
#include <modules/ui/ingame/IngameUI.h>
#include <modules/camera-view/GameCamera.h>
#include <modules/ui/lobby/LobbyUI.h>
#include <modules/ui/lobby/FindMatchLayer.h>
#include <modules/ui/ingame/IngameBaseUI.h>
#include <modules/ui/ingame/IngameEightBallPoolUI.h>
#include <effekseer/effekseer/Effekseer.h>
#include "core/gui/BaseScene.h"
#include "../core/ps/internal/CueAngle.h"


class GameScene
	: public BaseScene
{
public:
	Table* _table;
	Ball* _cueBall;
	Cue* _cue;
	GameCamera* _camera;
	TrajectoryBuilder* _trajectoryBuilder;
	CueAngle *_cueAngle;

public:
	GameScene();
	~GameScene();

	static cocos2d::Scene* createScene();

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

	void toLobby();

	void npcView();

	void initListener();
	void onMouseScroll(cocos2d::Event *event);
	CREATE_FUNC(GameScene);

	void touchEvent(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void clickEvent(cocos2d::Ref* pSender);
	void resetEvent(cocos2d::Ref* pSender);
	void up(cocos2d::Ref* pSender);
	void down(cocos2d::Ref* pSender);
	void left(cocos2d::Ref* pSender);
	void right(cocos2d::Ref* pSender);
	void doShoot();
	void addButton(const char *text, cocos2d::Vec2 position, cocos2d::Size size, cocos2d::ui::Widget::ccWidgetClickCallback callback);
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void rotationUpdate(float dt);
	void cueRotationUpdate(float dt);
	void updateTableDirection(double force);
	void updateCueOffset(ps::ExtMath::vector offset);

	void onEnter();
	
	void update(float dt);

	void updateEfkManager(float dt);

	void hideLobbyGUI();

	void showFindMatchLayer(GameMode mode);

	void showIngameGUI();

	IngameBaseUI *getIngameUI();

	FindMatchLayer *getFindMatchUI();

private:
	int rotateCount;
	int cueRotateCount;
	cocos2d::Vec3 rotateSign;
	cocos2d::Vec3 cueSign;
	double yaw = 0;
	double pitch = 0;
	UITestLayer* ui;

	efk::EffectManager *_efkManager;
	efk::Effect *_lobby3D;
	efk::EffectEmitter *_lobbyEfx;

	bool _inLobby;
	cocos2d::Vec3 _lobbyCameraPos;
	cocos2d::Vec3 _lobbyCameraRotation;

public:
	IngameBaseUI *_ingameUI;
	LobbyUI *_lobbyUI;
	FindMatchLayer *_findMatchUI;

	static cocos2d::Vec3 ROTATE_SIGN[250];
	static const double MAX_POWER;
};

#endif // !GAMESCENE_H_
