#include "TestScene.h"
#include "../../../cocos2d/cocos/base/CCDirector.h"
#include "cocos2d.h"
#include "../core-implement/SettingMgr.h"
#include "../core/ps/internal/BallGenerator.hpp"
#include "../ui/lobby/LobbyUI.h"
#include "../ui/ingame/IngameEightBallPoolUI.h"
#include "../../core/GameMgr.h"

USING_NS_CC;
using namespace cocos2d::ui;

TestScene::TestScene()
{
	_ingameUI = NULL;
	_lobbyUI = NULL;
	_findMatchUI = NULL;
}

TestScene::~TestScene()
{

}

cocos2d::Scene* TestScene::createScene()
{
	TestScene::ROTATE_SIGN[(int)EventKeyboard::KeyCode::KEY_A] = Vec3(1, 0, 0);
	TestScene::ROTATE_SIGN[(int)EventKeyboard::KeyCode::KEY_D] = Vec3(-1, 0, 0);
	TestScene::ROTATE_SIGN[(int)EventKeyboard::KeyCode::KEY_W] = Vec3(0, 1, 0);
	TestScene::ROTATE_SIGN[(int)EventKeyboard::KeyCode::KEY_S] = Vec3(0, -1, 0);
	TestScene::ROTATE_SIGN[(int)EventKeyboard::KeyCode::KEY_UP_ARROW] = Vec3(0, 1, 0);
	TestScene::ROTATE_SIGN[(int)EventKeyboard::KeyCode::KEY_DOWN_ARROW] = Vec3(0, -1, 0);
	TestScene::ROTATE_SIGN[(int)EventKeyboard::KeyCode::KEY_LEFT_ARROW] = Vec3(1, 0, 0);
	TestScene::ROTATE_SIGN[(int)EventKeyboard::KeyCode::KEY_RIGHT_ARROW] = Vec3(-1, 0, 0);

	return TestScene::create();
}

bool TestScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	initCamera();
	if (settingMgr->_ballEnvTexture == NULL)
		settingMgr->initTextures();
	initEfkManager();
	initTable();
	initBall();

	initListener();
	initCue();
	initCueAngle();
	initUI();

	initLobby();
	//addButton("Reset", Vec2(500, 200), Size(100, 54), CC_CALLBACK_1(GameScene::resetEvent, this));
	//addButton("NextTurn", Vec2(600, 200), Size(100, 54), CC_CALLBACK_1(GameScene::clickEvent, this));
	return true;
}

void TestScene::initBall()
{
	auto cueBall = Ball::createBall(0);
	cueBall->setCameraMask((int)GameCamera::FLAG_3D);
	_table->addBall(cueBall, PhysicsConstants::CUE_BALL_POSITION);
	_cueBall = cueBall;

	auto ballPositions = (new BallGenerator())->generateDefault();
	for (int i = 0; i < NUM_BALL; i++) {
		auto ball = Ball::createBall(i + 1);
		ball->setNumber(i + 1);
		auto pos = ballPositions[i];
		ball->setCameraMask((int)GameCamera::FLAG_3D);
		_table->addBall(ball, pos);
	}
}

void TestScene::initTable()
{
	auto table = Table::create();
	table->setCameraMask((int)GameCamera::FLAG_3D);
	addChild(table);
	_table = table;
	table->setEfkManager(_efkManager);

	_table->initGuildLine();
}

void TestScene::initCue()
{
	auto cue = Cue::create();
	cue->setCameraMask((int)GameCamera::FLAG_3D);
	cue->setVisible(false);
	_table->addCue(cue);
	_cue = cue;
}

void TestScene::initCamera()
{
	auto camera = GameCamera::create();
	addChild(camera);
	_camera = camera;

}

void TestScene::initUI()
{
	_lobbyUI = LobbyUI::create();
	addChild(_lobbyUI);
	_lobbyUI->getLocalZOrder();
	_lobbyUI->setVisible(false);

	auto ingameBaseUi = IngameEightBallPoolUI::create();
	addChild(ingameBaseUi);
	ingameBaseUi->setVisible(true);
	_ingameUI = ingameBaseUi;
}

void TestScene::initTrajectoryBuilder()
{

}

void TestScene::initEfkManager()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	_efkManager = efk::EffectManager::create(visibleSize);
}

void TestScene::initLobby()
{

}

void TestScene::initCueAngle()
{
	_cueAngle = new CueAngle();
	_cueAngle->setBalls(_table->getWorld()->allBalls());
	_cueAngle->setCueShape(0.79, 1.08, 83.66534004, abs(Cue::OFFSET_DISTANCE));
	_cueAngle->setCueOffset(_cue->getOffset());
	_cueAngle->setCueDirection(_cue->getDirection());
}

void TestScene::initListener()
{
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(TestScene::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(TestScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseScroll = CC_CALLBACK_1(TestScene::onMouseScroll, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

void TestScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	CCLOG("onKeyPressed");
	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_A:
	case EventKeyboard::KeyCode::KEY_D:
	case EventKeyboard::KeyCode::KEY_W:
	case EventKeyboard::KeyCode::KEY_S:
		rotateCount = rotateCount + 1;
		rotateSign = TestScene::ROTATE_SIGN[(int)keyCode];
		schedule(CC_SCHEDULE_SELECTOR(TestScene::rotationUpdate));
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		cueRotateCount = cueRotateCount + 1;
		cueSign = TestScene::ROTATE_SIGN[(int)keyCode];
		schedule(CC_SCHEDULE_SELECTOR(TestScene::cueRotationUpdate));
		break;
	case EventKeyboard::KeyCode::KEY_SPACE:
		//doShoot();
		break;
	}
}

void TestScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	CCLOG("onKeyReleased");
	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_A:
	case EventKeyboard::KeyCode::KEY_D:
	case EventKeyboard::KeyCode::KEY_W:
	case EventKeyboard::KeyCode::KEY_S:
		unschedule(CC_SCHEDULE_SELECTOR(TestScene::rotationUpdate));
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		unschedule(CC_SCHEDULE_SELECTOR(TestScene::cueRotationUpdate));
		break;
	}
}

void TestScene::touchEvent(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
	{
		CCLOG("BEGAN");
		auto pos = _camera->getPosition3D();
		pos.y += 1;
		_camera->setPosition3D(pos);
		break;
	}

	case Widget::TouchEventType::MOVED:
	{
		auto pos = _camera->getPosition3D();
		pos.y += 1;
		_camera->setPosition3D(pos);
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

void TestScene::onMouseScroll(cocos2d::Event *event)
{
	auto mouseEvent = static_cast<EventMouse*>(event);
	auto scrollY = mouseEvent->getScrollY();
	_camera->focus(scrollY * 10);
}

void TestScene::rotationUpdate(float dt)
{
	auto deltaPhi = rotateSign.x * dt;
	auto deltaTheta = rotateSign.y * dt;
	_camera->moveSpherical(deltaPhi, deltaTheta);
}

void TestScene::cueRotationUpdate(float dt)
{
	auto speed = ui->getCueSpeedValue() / 50;
	yaw = yaw + dt * cueSign.x * speed;
	pitch = pitch + dt * cueSign.y * speed;
	pitch = std::min(0.499 * ExtMath::PI, std::max(0., pitch));
	auto angle = pitch * 180 / ExtMath::PI;
	angle = floor(angle * 100) / 100;
	char text[20];
	sprintf(text, "%.2f d", angle);
	CCLOG(text);
	ui->updateAngle(text);
	auto cosPitch = std::cos(pitch);
	_cue->updateDirection(ps::ExtMath::vector(
		cosPitch * std::cos(yaw),
		cosPitch * std::sin(yaw),
		-std::sin(pitch)
	));
	updateTableDirection(ui->getPowerValue() * ui->getMaxPower() / 100);
}

void TestScene::onEnter()
{
	Scene::onEnter();
	
	
	
	gameMgr->initInteraction();
}

IngameBaseUI * TestScene::getIngameUI()
{
	return _ingameUI;
}

void TestScene::updateTableDirection(double force)
{
	auto trajectories = _trajectoryBuilder->buildTrajectories(force, _cue, _cueBall->body(), *(_table->getWorld()->allBalls()));
	auto points = _trajectoryBuilder->buildPoints(trajectories);
	_table->drawTrajectory(trajectories, points);
	deleteVector(trajectories);
	points.clear();
}

cocos2d::Vec3 TestScene::ROTATE_SIGN[250];

