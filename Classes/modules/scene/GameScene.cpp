#include "GameScene.h"
#include <data/Resource.h>
#include <modules/core-implement/Table.h>
#include <modules/core-implement/Ball.h>
#include <modules/core/ps/internal/PhysicsConstants.hpp>
#include <modules/core-implement/Cue.h>
#include <data/GameConstant.h>
#include <modules/core/utils/LerpUtils.h>
#include <modules/camera-view/SphericalCamera.h>
#include <cmath>
#include <modules/camera-view/utils/ThiccDrawNode3D.h>
#include <external/json/rapidjson.h>
#include <external/json/document.h>
#include <modules/core-implement/SettingMgr.h>
#include <modules/ui/ingame/IngameUI.h>
#include <core/GameMgr.h>
#include <modules/ui/GuiMgr.h>
#include <modules/ui/ingame/IngameBaseUI.h>
#include "../utils/SoundMgr.h"
#include "../ui/inventory/workshop/WorkshopUI.h"
#include "../ui/inventory/cue_collection/CueCollectionUI.h"
#include "../camera-view/GameCamera.h"
#include "../core/ps/internal/BallGenerator.hpp"

USING_NS_CC;
using namespace ui;
using namespace ps;

Vec3 GameScene::ROTATE_SIGN[250];
const double  GameScene::MAX_POWER = 500;

GameScene::GameScene()
{
	_findMatchUI	= NULL;
	_lobbyUI		= NULL;
	_ingameUI		= NULL;
}

GameScene::~GameScene()
{

}

cocos2d::Scene* GameScene::createScene()
{
    GameScene::ROTATE_SIGN[(int)EventKeyboard::KeyCode::KEY_A] = Vec3(1, 0, 0);
    GameScene::ROTATE_SIGN[(int)EventKeyboard::KeyCode::KEY_D] = Vec3(-1, 0, 0);
    GameScene::ROTATE_SIGN[(int)EventKeyboard::KeyCode::KEY_W] = Vec3(0, 1, 0);
    GameScene::ROTATE_SIGN[(int)EventKeyboard::KeyCode::KEY_S] = Vec3(0, -1, 0);
    GameScene::ROTATE_SIGN[(int)EventKeyboard::KeyCode::KEY_UP_ARROW] = Vec3(0, 1, 0);
    GameScene::ROTATE_SIGN[(int)EventKeyboard::KeyCode::KEY_DOWN_ARROW] = Vec3(0, -1, 0);
    GameScene::ROTATE_SIGN[(int)EventKeyboard::KeyCode::KEY_LEFT_ARROW] = Vec3(1, 0, 0);
    GameScene::ROTATE_SIGN[(int)EventKeyboard::KeyCode::KEY_RIGHT_ARROW] = Vec3(-1, 0, 0);
	return GameScene::create();
}

bool GameScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	settingMgr->initTextures();
	initEfkManager();
    this->initTable();
    this->initBall();
	_table->getWorld()->resetStaticWorldBallPosition();
    this->initCamera();
    this->initListener();
    this->initCue();
	this->initCueAngle();
    this->initUI();
	
	
	initLobby();

	toLobby();

	//this->addButton("Reset", Vec2(500, 200), Size(100, 54), CC_CALLBACK_1(GameScene::resetEvent, this));
	//this->addButton("NextTurn", Vec2(600, 200), Size(100, 54), CC_CALLBACK_1(GameScene::clickEvent, this));
    return true;
}

void GameScene::initTable() {
    auto table = Table::create();
    table->setCameraMask((int)GameCamera::FLAG_3D);
    this->addChild(table);
    this->_table = table;
	table->setEfkManager(_efkManager);

    //this->_table->initGuildLine();
}

void GameScene::initCue()
{
    auto cue = Cue::create();
    cue->setCameraMask((int)GameCamera::FLAG_3D);
	cue->setVisible(false);
    this->_table->addCue(cue); 
    this->_cue = cue;
}

void GameScene::initCamera()
{
    auto camera = GameCamera::create();
    //camera->setPosition3D(Vec3(0, 350, 120));
    this->addChild(camera);
    this->_camera = camera;
}

void GameScene::initUI()
{
	_lobbyUI = LobbyUI::create();
	this->addChild(_lobbyUI);
	_lobbyUI->getLocalZOrder();

	auto ingameBaseUi = IngameEightBallPoolUI::create();
	this->addChild(ingameBaseUi);
	ingameBaseUi->setVisible(false);
	_ingameUI = ingameBaseUi;
}

void GameScene::initTrajectoryBuilder()
{
}

void GameScene::initEfkManager()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	_efkManager = efk::EffectManager::create(visibleSize);
}

void GameScene::initLobby()
{
	auto efkManager = _efkManager;
	auto lobby = efk::Effect::create("res/3d/lobby_animate/lobby_allmesh_3_August_2021.efk", 0.2);
	_lobby3D = lobby;
	auto emitter = efk::EffectEmitter::create(efkManager);
	emitter->setEffect(lobby);
	emitter->setPlayOnEnter(true);
	emitter->play(0);
	emitter->setSpeed(1);
	emitter->setScale(29.5);
	emitter->setPosition3D(Vec3(-331, 17, -273));
	emitter->setCameraMask((int)GameCamera::FLAG_3D);
	_lobbyEfx = emitter;

	addChild(emitter);

	auto floor = Sprite3D::create("res/3d/floor/Bida_floor.c3b");
	floor->setCameraMask((int)GameCamera::FLAG_3D);
	floor->setScale(1);
	floor->setPosition3D(Vec3(0, 2.5, 0));
	addChild(floor);
}

void GameScene::initCueAngle()
{
	_cueAngle = new CueAngle();
	_cueAngle->setBalls(_table->getWorld()->allBalls());
	_cueAngle->setCueShape(0.79, 1.08, 83.66534004, abs(Cue::OFFSET_DISTANCE));
	_cueAngle->setCueOffset(_cue->getOffset());
	_cueAngle->setCueDirection(_cue->getDirection());
}

void GameScene::toLobby()
{
	//todo
	CCLOG("GameScene::toLobby");
	_lobbyEfx->setVisible(true);
	_inLobby = true;
	npcView();
	_lobbyUI->setVisible(true);
	//_lobbyUI->moveUIBack();
	//_lobbyUI._uiButtons.setInfoAndSponsorVisible(true);
	
	_ingameUI->setVisible(false);
	//_npc->playIdle1();
	_table->setGuildLineVisible(false);
	soundMgr->playBgLobby();
}

void GameScene::npcView()
{
	//_npc->setVisible(true);

	_lobbyCameraPos = Vec3(305.4342041015625, 51.43067169189453, 215.65988159179688);
	/*_npc->startAnimating();
	_npc->setPosition3D(Vec3(222,-94,250));
	_npc->setRotationQuat(Quaternion(0, 0.783326909627484, 0, 0.621609968270664));*/

	_camera->getCamera()->setPosition3D(_lobbyCameraPos);
	_camera->getCamera()->setRotation3D(Vec3(
		0.1420116424560547,
		50.97305679321289,
		5.93173183460749e-7
		));

	_lobbyCameraRotation.y = _camera->getCamera()->getRotation3D().y;
	_lobbyCameraRotation.x = _camera->getCamera()->getRotation3D().x;


	_camera->dispatchChangePosition(_camera->getCamera()->getPosition3D());
	//_npc->onChangeCameraPosition(_camera._camera.getPosition3D());

	_table->onChangeCameraPosition(_camera->getCamera()->getPosition3D());
}

void GameScene::initListener()
{
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseScroll = CC_CALLBACK_1(GameScene::onMouseScroll, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

void GameScene::onMouseScroll(cocos2d::Event * event)
{
	auto mouseEvent = static_cast<EventMouse*>(event);
	auto scrollY = mouseEvent->getScrollY();
	this->_camera->focus(scrollY * 10);
}

void GameScene::initBall() {
    auto cueBall = Ball::createBall(0);
    cueBall->setCameraMask((int)GameCamera::FLAG_3D);
    this->_table->addBall(cueBall, PhysicsConstants::CUE_BALL_POSITION);
    this->_cueBall = cueBall;

	auto ballPositions = (new BallGenerator())->generateDefault();
    for (int i = 0; i < NUM_BALL; i++) {
        auto ball = Ball::createBall(i + 1);
        ball->setNumber(i + 1);
        auto pos = ballPositions[i];
        ball->setCameraMask((int)GameCamera::FLAG_3D);
        this->_table->addBall(ball, pos);
    }
}

void GameScene::touchEvent(Ref* pSender, Widget::TouchEventType type)
{
    switch (type)
    {
    case Widget::TouchEventType::BEGAN:
    {
        CCLOG("BEGAN");
        auto pos = this->_camera->getPosition3D();
        pos.y += 1;
        this->_camera->setPosition3D(pos);
        break;
    }

    case Widget::TouchEventType::MOVED:
    {
        auto pos = this->_camera->getPosition3D();
        pos.y += 1;
        this->_camera->setPosition3D(pos);
        CCLOG("MOVED");
        break;
    }

    case Widget::TouchEventType::ENDED:
    {

        CCLOG("ENDED");
    }
    break;

    case Widget::TouchEventType::CANCELED:
        break;

    default:
        break;
    }
}

void GameScene::clickEvent(Ref* pSender)
{
	CCLOG("clickEvent");
    //this->_cue->doShootAnimation(300., callback);
	gameMgr->_ebpCtrl->getCurMatch()->changePlayer();
}

void GameScene::resetEvent(Ref* pSender)
{
    this->_table->reset();
    //this->updateTableDirection(this->ui->getPowerValue() * this->ui->getMaxPower() / 100);

}

void GameScene::up(cocos2d::Ref* pSender)
{
    auto pos = this->_camera->getPosition3D();
    pos.y += 1;
    this->_camera->setPosition3D(pos);
}

void GameScene::down(cocos2d::Ref* pSender)
{
    auto pos = this->_camera->getPosition3D();
    pos.y += -1;
    this->_camera->setPosition3D(pos);
}

void GameScene::left(cocos2d::Ref* pSender)
{
    auto pos = this->_camera->getPosition3D();
    pos.x -= 1;
    this->_camera->setPosition3D(pos);
}

void GameScene::right(cocos2d::Ref* pSender)
{
    auto pos = this->_camera->getPosition3D();
    pos.x += 1;
    this->_camera->setPosition3D(pos);
}

void GameScene::doShoot()
{
    CCLOG("doShoot");
    auto force = GameScene::MAX_POWER * this->ui->getPowerValue() /100;
    auto direction = this->_cue->getDirection();
    auto offset = this->_cue->getOffset();
    this->_table->getWorld()->shoot(0, 0, force, direction, offset);
    this->_table->setGuildLineVisible(false);
}

void GameScene::addButton(const char* text, cocos2d::Vec2 position, cocos2d::Size size, cocos2d::ui::Widget::ccWidgetClickCallback  callback)
{
    auto button = ui::Button::create("button_blue_1.png");
    auto rect = Rect(0, 0, 200, 80);
    button->setTitleText(text);
    this->addChild(button);
    button->setPosition(position);
    button->addClickEventListener(callback);
    button->setScale9Enabled(true);
    button->setCapInsets(rect);
    button->setContentSize(size);
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    CCLOG("onKeyPressed");
    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_A:
    case EventKeyboard::KeyCode::KEY_D:
    case EventKeyboard::KeyCode::KEY_W:
    case EventKeyboard::KeyCode::KEY_S:
        this->rotateCount = this->rotateCount + 1;
        this->rotateSign = GameScene::ROTATE_SIGN[(int)keyCode];
        this->schedule(CC_SCHEDULE_SELECTOR(GameScene::rotationUpdate));
        break;
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        this->cueRotateCount = this->cueRotateCount + 1;
        this->cueSign = GameScene::ROTATE_SIGN[(int)keyCode];
        this->schedule(CC_SCHEDULE_SELECTOR(GameScene::cueRotationUpdate));
        break;
        case EventKeyboard::KeyCode::KEY_SPACE:
            this->doShoot();
    }
}

void GameScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    CCLOG("onKeyReleased");
    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_A:
    case EventKeyboard::KeyCode::KEY_D:
    case EventKeyboard::KeyCode::KEY_W:
    case EventKeyboard::KeyCode::KEY_S:
        this->unschedule(CC_SCHEDULE_SELECTOR(GameScene::rotationUpdate));
        break;
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        this->unschedule(CC_SCHEDULE_SELECTOR(GameScene::cueRotationUpdate));
        break;
    }
}

void GameScene::rotationUpdate(float dt) {
    auto deltaPhi = this->rotateSign.x * dt;
    auto deltaTheta = this->rotateSign.y * dt;
    this->_camera->moveSpherical(deltaPhi, deltaTheta);
}


void GameScene::cueRotationUpdate(float dt) {
    auto speed = this->ui->getCueSpeedValue() / 50;
    this->yaw = this->yaw + dt * this->cueSign.x * speed;
    this->pitch = this->pitch + dt * this->cueSign.y * speed;
    this->pitch = std::min(0.499 * ExtMath::PI, std::max(0., this->pitch));
    auto angle = this->pitch * 180 / ExtMath::PI;
    angle = floor(angle * 100) / 100;
    char text[20];
    sprintf(text, "%.2f d", angle);
    CCLOG(text);
    this->ui->updateAngle(text);
    auto cosPitch = std::cos(this->pitch);
    this->_cue->updateDirection(ps::ExtMath::vector(
        cosPitch * std::cos(this->yaw),
        cosPitch * std::sin(this->yaw),
        -std::sin(this->pitch)
        ));
    this->updateTableDirection(this->ui->getPowerValue() * this->ui->getMaxPower() / 100);
}

void GameScene::updateTableDirection(double force)
{
    auto trajectories = this->_trajectoryBuilder->buildTrajectories(force, this->_cue, this->_cueBall->body(), *(this->_table->getWorld()->allBalls()));
    auto points = this->_trajectoryBuilder->buildPoints(trajectories);
    this->_table->drawTrajectory(trajectories, points);
    deleteVector(trajectories);
    points.clear();
}

void GameScene::updateCueOffset(ps::ExtMath::vector offset)
{
    auto radius = PhysicsConstants::MAX_CUE_OFFSET;
    _cue->setOffset(ps::ExtMath::vector(radius * offset.x, radius * offset.y));
}

void GameScene::onEnter()
{
	Scene::onEnter();
	settingMgr->autoSelectProfile();
	scheduleUpdate();

	_lobbyEfx->play(0);
	schedule(schedule_selector(GameScene::updateEfkManager), 1 / 60.f);

}

void GameScene::update(float dt)
{
	gameMgr->update(dt);
}

void GameScene::updateEfkManager(float dt)
{
	auto camera = _camera->getCamera();
	_efkManager->update();
	if (camera) {
		_efkManager->setCameraMatrix(camera->getViewMatrix());
		_efkManager->setProjectionMatrix(camera->getProjectionMatrix());
	}
}

void GameScene::hideLobbyGUI()
{
	_lobbyUI->setVisible(false);
}

void GameScene::showFindMatchLayer(GameMode mode)
{
	if (!_findMatchUI)
	{
		_findMatchUI = (FindMatchLayer*) guiMgr->getGui(Popup::FIND_MATCH_LAYER);
	}
	_findMatchUI->setInfo(mode);
	_findMatchUI->reset();
	_findMatchUI->show();

	hideLobbyGUI();
}

void GameScene::showIngameGUI()
{
	_ingameUI->setVisible(true);
	_findMatchUI->hide();
}

IngameBaseUI * GameScene::getIngameUI()
{
	return _ingameUI;
}

FindMatchLayer * GameScene::getFindMatchUI()
{
	return _findMatchUI;
}
