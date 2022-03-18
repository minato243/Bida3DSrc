#include "GameMgr.h"
#include "cocos2d.h"
#include "gui/SceneMgr.h"
#include "utils/LanguageMgr.h"
#include "ResourceMgr.h"
#include "login/LoginScene.h"
#include "lobby/gui/BroadcastMgr.h"
#include "login/LoadingScene.h"
#include "GameData.h"
#include "Config.h"
#include "utils/JNI.h"
#include "Constant.h"
#include "lobby/ChanelConfig.h"
#include "login/UserInfo.h"
#include "lobby/network/LobbyHandler.h"
#include "lobby/network/GameClient.h"
#include "login/LoginMgr.h"
#include "plugins/PlatformWrapper.h"
#include "plugins/Facebook.h"
#include "plugins/Google.h"
#include "plugins/AppleID.h"
#include "utils/StorageUtil.h"
#include "modules/scene/GameScene.h"
#include "modules/core-implement/SettingMgr.h"
#include <modules/core/ps/TrajectoryElement.h>
#include <modules/core/interaction/InteractionManager.h>
#include <modules/core/interaction/TouchManager.h>
#include <modules/core/utils/TimeUtils.h>
#include <modules/utils/SoundMgr.h>
#include <modules/scene/loading/LoadingSceneNew.h>
#include <data/config/ConfigMgr.h>
#include "modules/ui/inventory/cue_collection/CueCollectionUI.h"
#include "modules/network/inventory/InventoryHandler.h"
#include "modules/network/cue/CueHandler.h"
#include "modules/network/cheat/CheatHandler.h"
#include "modules/network/offer/OfferHandler.h"
#include "modules/core/offer/OfferMgr.h"
#include "modules/network/minigame/MiniGameHandler.h"
#include "modules/core/ps/static/StaticWorld.h"
#include "modules/core/ps/PhysicsFactory.h"
#include "modules/scene/TestScene.h"

#define DEBUG_PHYSICS false

USING_NS_CC;

GameMgr::GameMgr()
{
	nServicesAPI = 0;
	_userInfo = userInst;
	_startedShot = false;
	_table = nullptr;
	_scene = NULL;
}

GameMgr::~GameMgr()
{
	CC_SAFE_DELETE(_trajectoryBuilder);
	CC_SAFE_DELETE(_ebpCtrl);
}

GameMgr* GameMgr::getInstance()
{
	if (!_instance) {
		_instance = new GameMgr();
	}
	return _instance;
}

void GameMgr::start()
{
	languageMgr->init();
    initPlugin();
	loadCache();
	initCheat();
	initModule();
	addSearchPath();
	preloadGUI();

	configMgr->loadConfig();
	resourceMgr->init();
#if DEBUG_PHYSICS
	startTestScene();
#else 
	openLoading();
#endif

	//gameMgr->openLoading();
	// needed api request
	//gameMgr->requestCountryID();
	//gameMgr->requestCheckAppVersion();

	_enableGuildLine = true;
	_trajectoryBuilder = new TrajectoryBuilder();

	_ebpCtrl = new EightBallPollCtrl();

	
	//startGameScene();

}

void GameMgr::initCheat()
{

}

void GameMgr::initPlugin()
{
    fr::platformWrapper::init();
    fr::Facebook::init();
    fr::Google::init();
	fr::AppleID::init();
}

void GameMgr::initModule()
{
	inventoryHandler;
	cueHandler;
	gameHandler;
	matchHandler;
	cheatHandler;
	offerHandler;
	miniGameHandler;

	//PhysicsFactory *pf = new PhysicsFactory();
	//pf->test();
}

void GameMgr::addSearchPath()
{
	FileUtils::getInstance()->addSearchPath("common");
	FileUtils::getInstance()->addSearchPath("Board");
	FileUtils::getInstance()->addSearchPath("Lobby");
	FileUtils::getInstance()->addSearchPath("Armatures");
	FileUtils::getInstance()->addSearchPath("SieteYMedia");
	FileUtils::getInstance()->addSearchPath("res/GUI");
}

void GameMgr::preloadGUI()
{
	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("res/ui/announcer/Announcer.plist", "res/ui/announcer/Announcer.png");
	cache->addSpriteFramesWithFile("res/GUI/Game/res/Ingame_3.plist", "res/GUI/Game/res/Ingame_3.png");
	cache->addSpriteFramesWithFile("res/GUI/FindMatch/res/Duo_RT3.plist", "res/GUI/FindMatch/res/Duo_RT3.png");
	cache->addSpriteFramesWithFile("res/GUI/Shop/res/Shop_UI.plist", "res/GUI/Shop/res/Shop_UI.png");
	cache->addSpriteFramesWithFile("res/GUI/Resources/Endgame.plist", "res/GUI/Resources/Endgame.png");
	cache->addSpriteFramesWithFile("res/GUI/Resources/NumberIngame.plist", "res/GUI/Resources/NumberIngame.png");
	cache->addSpriteFramesWithFile("res/GUI/Resources/offer_chest.plist", "res/GUI/Resources/offer_chest.png");
	cache->addSpriteFramesWithFile("res/GUI/Workshop/res/Workshop.plist", "res/GUI/Workshop/res/Workshop.png");
	cache->addSpriteFramesWithFile("res/GUI/Resources/ItemIcons.plist", "res/GUI/Resources/ItemIcons.png");
	cache->addSpriteFramesWithFile("res/GUI/Resources/OpenCard.plist", "res/GUI/Resources/OpenCard.png");
	cache->addSpriteFramesWithFile("res/GUI/Resources/coin.plist", "res/GUI/Resources/coin.png");
	cache->addSpriteFramesWithFile("res/GUI/Resources/Channel.plist", "res/GUI/Resources/Channel.png");
	cache->addSpriteFramesWithFile("res/GUI/Resources/Profile.plist", "res/GUI/Resources/Profile.png");
	cache->addSpriteFramesWithFile("res/circles_efx.plist", "res/circles_efx.png");
	cache->addSpriteFramesWithFile("res/GUI/Resources/Localize_vi.plist", "res/GUI/Resources/Localize_vi.png");
}

void GameMgr::loadCache()
{
	loginMgr->loadCache();

	// APP VERSION
	std::string appVersion = StorageUtil::getString(LocalizedString::config("KEY_APP_VERSION"), "");
	if (!appVersion.empty()) {
		gamedata->appVersion = appVersion;
	}
	else {
		gamedata->appVersion = fr::platformWrapper::getGameVersion();

		if (gamedata->appVersion.empty()) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			gamedata->appVersion = Config::APP_VERSION_ANDROID;
#elseif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			gamedata->appVersion = Config::APP_VERSION_IOS;
#else
			gamedata->appVersion = "1";
#endif
				
		}
	}

	gamedata->sound = StorageUtil::getBool("sound", true);
	gamedata->music = StorageUtil::getBool("music", true);
	gamedata->soundhumansusun = StorageUtil::getBool("soundhumansusun", true);
	gamedata->acceptInvite = StorageUtil::getBool("invite", true);
	gamedata->acceptFriend = StorageUtil::getBool("friend", true);
	gamedata->loggedFacebook = StorageUtil::getBool("logged_facebook", true);
	gamedata->completeTooltip = StorageUtil::getBool("COMPLETE_TOOLTIP", true);
	gamedata->wasShowPopupSurvey = StorageUtil::getBool("SHOW_POPUP_SURVEY", true);
	gamedata->doneSurvey = StorageUtil::getBool("DONE_SURVEY", true);
	gamedata->logged = StorageUtil::getBool("LOGGED", true);
	gamedata->dataCacheService = StorageUtil::getString(LocalizedString::config("KEY_DATA_URL"), "");
}

void GameMgr::cleanUserData()
{
	//TODO:chat
	//chatMgr->clear();
}

void GameMgr::updatePayments(bool enablePayment, std::vector<bool> payments)
{
	gamedata->enablePayement = enablePayment;
	gamedata->payments = payments;

	for (int i = 0; i < gamedata->payments.size(); i++) {
		if (gamedata->payments[i]) {
			gamedata->enablePayement = true;
		}
	}

	if (Config::ENABLE_CHEAT) {
		//TODO: cheat
		/*if (CheatCenter.ENABLE_PAYMENT_IAP) {
			gamedata->payments[0] = true;
			cc.log("Payment::EnableCheatPaymentIAP");
		}

		if (CheatCenter.ENABLE_PAYMENT_DIRECT) {
			gamedata->payments[5] = true;
			gamedata->payments[7] = true;
			cc.log("Payment::EnableCheatPaymentDirect");
		}*/
	}

}

void GameMgr::requestCheckAppVersion()
{
	nServicesAPI++;

	/*if (!gameMgr.isPortal()) {
		if (gamedata->dataCacheService) {
			cc.log("GameMgr::requestCheckAppVersion from cache");
			responseCheckAppVersion(gamedata->dataCacheService);
			return;
		}
	}

	var packageName = fr.platformWrapper.getPackageName();
	var data = "game=" + LocalizedString.config(packageName);
	data += "&device=" + gameMgr.getDeviceID();
	data += "&version=" + gameMgr.getAppVersion();

	cc.log("gameMgr::checkAppVersion ", Constant.APP_VERSION_URL, data);
	httpRequest.addRequestPOST(Constant.APP_VERSION_URL, data,
		gameMgr.responseCheckAppVersion.bind(gameMgr),
		gameMgr.responseCheckAppVersion.bind(gameMgr),
		Constant.API_SERVICES_TIMEOUT);*/
	gameMgr->onCheckOpenLogin();
}

void GameMgr::requestCountryID()
{
	nServicesAPI++;
	gameMgr->onCheckOpenLogin();
}

void GameMgr::onEnterScene()
{
	//TODO
	///CheatCenter.openCheatPopup();

	PingPongHandler::getInstance()->checkNeedPing();

	broadcastMgr->reloadBroadcast();
}

void GameMgr::onUpdateScene(float dt)
{
	broadcastMgr->updateBroadcast(dt);
	PingPongHandler::getInstance()->updatePing(dt);
	TimeoutConnectHandler::getInstance()->updateCountDown(dt);
}

void GameMgr::onCheckOpenLogin()
{
	nServicesAPI--;

	if (nServicesAPI == 0) {
		gameMgr->openLogin();
	}
}

BaseLayer* GameMgr::openLoading()
{
	return sceneMgr->openScene(LoadingSceneNew::className);
}

BaseLayer* GameMgr::openLogin()
{
	return sceneMgr->openScene(LoginScene::className);
}

BaseLayer* GameMgr::openLobby()
{
	return nullptr;
}

bool GameMgr::openShop()
{
	return false;
}

bool GameMgr::checkSupportBean()
{
	if (GameData::IS_RECEIVE_START_UP == false)
		return false;

	bool ret = false;
	//TODO:vip
	//var vipLevel = newVipManager.getVipLevel();
	//var beanNeedSupport = newVipManager.getBeanNeedSupportConfig();
	//var minBean = beanNeedSupport[vipLevel];
	int vipLevel = 0;
	int minBean = 0;

	if ((userInfo->getGold() < ChanelConfig::instance()->chanelConfig[0].minGold || userInfo->getGold() < minBean) && GameData::IS_RECEIVE_SHOW_TUT_PACKAGE) {
		LobbyHandler::requestGetSupportBean();
		ret = true;
	}
	else {
		ret = false;
	}
	return ret;
}

bool GameMgr::checkEnablePayment()
{
	//TODO: implement
	/*
	for (int i = 0; i < gamedata->payments.size(); i++) {
		if (i == Payment::IDX_SHOP_G) {
			if (gamedata->payments[i]) {
				return true;
			}
		}
		else {
			if (gamedata->payments[i]) {
				return true;
			}
		}
	}
	*/
	return true;
}

std::string GameMgr::getInstallDate() {
	//TODO
	/*
	std::string strInstallDate = cc.sys.localStorage.getItem("game_install_date");
	cc.log("GameData::InstallDate: ", strInstallDate);
	if (strInstallDate) {
		return strInstallDate;
	}
	else {
		var today = new Date();
		var dd = today.getDate();
		var mm = today.getMonth() + 1; //January is 0!
		var yyyy = today.getFullYear();
		if (dd < 10) {
			dd = '0' + dd;
		}
		if (mm < 10) {
			mm = '0' + mm;
		}
		var strDate = dd + '-' + mm + '-' + yyyy;
		cc.sys.localStorage.setItem("game_install_date", strDate);
		cc.log("GameData::InstallDate: new Install", strDate);
		return strDate;
	}*/
	return "";
}


int GameMgr::getPlatform() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	return CONSTANT_PLATFORM_ANDROID;
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	return  CONSTANT_PLATFORM_IOS;
#endif
	return CONSTANT_PLATFORM_ANDROID;
}

std::string GameMgr::getDeviceID() {
	std::string ret = Config::DEVICE_ID_W32;

	if (Config::ENABLE_CHEAT) {
		//TODO:Cheat
		/*if (CheatCenter.IS_FAKE_UID) {
			cc.log("_____________DEVICE__ID_CHEAT : " + CheatCenter.DEVICE_ID_FAKE);
			return CheatCenter.DEVICE_ID_FAKE;
		}*/
	}
	ret = JNI::getDeviceID();
	if (ret.empty()) {
		log("_____________DEVICE__ID_DEFAULT : %s", Config::DEVICE_ID_W32.c_str());
		return Config::DEVICE_ID_W32;
	}
	log("_____________DEVICE__ID : %s", ret.c_str());
	return ret;
}


std::string GameMgr::getCountryID()
{
	//TODO:cheat

	//if (Config.ENABLE_CHEAT && CheatCenter.FAKE_COUNTRY_ID) {
	//	cc.log("gameMgr::getCountryID fake ", CheatCenter.FAKE_COUNTRY_ID);
	//	return CheatCenter.FAKE_COUNTRY_ID;
	//}

	return gamedata->countryID;
}

bool GameMgr::isGlobal()
{
	/*if (gameMgr->getCountryID().compare(Config::COUNTRY_MX)) {
		return false;
	}*/
	return true;
}

bool GameMgr::isCurPlayer(int pId)
{
	return pId == userInst->_uId;
}

float GameMgr::onChangePower(float percent)
{
	auto force = calcForce(percent);
	_force = force;
	updateTrajectory(force);
	_table->drawForce(_table->getCue()->getDirection(), percent);
	return force;
}

void GameMgr::shoot(float percent)
{
	//if (!_table.isCueBallPosValid() || !ebpCtrl._curMatch.canShoot()) return false;
	if (!gameMgr->_ebpCtrl->_startedMatch && !DEBUG_PHYSICS)
		return;
	auto force = calcForce(percent);
	_force = 0;
	_table->setGuildLineVisible(false);
	_table->setVisibleEffectForce(false);

	if (!DEBUG_PHYSICS) 
	{
		_ebpCtrl->getCurMatch()->onShoot(0, false);
		_ebpCtrl->onBeforeShoot();
		ShootData shootData;
		Cue * cue = _table->getCue();
		shootData.direction = cue->getDirection();
		shootData.offsetX = cue->getOffset().x;
		shootData.offsetY = cue->getOffset().y;
		shootData.force = force;

		gameHandler->sendShoot(shootData);
	}
	else shootByForce(force, false);
}

void GameMgr::shootByForce(double force, bool opponentShoot)
{
	auto direction = _table->getCue()->getDirection();
	auto directionPtr = &direction;
	auto offset = _table->getCue()->getOffset();
	this->_table->getWorld()->shoot(0, force, direction, offset);
	this->_table->setGuildLineVisible(false);
	auto cue = _table->getCue();
	cue->doShootAnimation(force, [&]() 
		{
			soundMgr->soundShoot(force);
			
			interactionMgr->setCueOffset(vector(0,0));

			auto isLag = _ebpCtrl->getCurMatch()->isLag();
			// this._playerShoot = this._playerShoot || !opponentShoot;
			_table->resume(opponentShoot && isLag);

			interactionMgr->startShoting();
		}
	);
}

double GameMgr::calcForce(float percent)
{
	return _table->getCue()->getForceMax() / 100 * percent;
}

void GameMgr::updateTrajectory(double force)
{
	if (force == 0) {
		_enableGuildLine = true;
		return updateBasicGuildLine();
	}
	_enableGuildLine = false;
	auto staticWorld = _table->getWorld()->getStaticWorld();
	TrajectoryLengthData lengths = getCurrentCueInfo()->getWorldTrajectoryLengths();

	auto resultVel = _table->getCue()->calcCueBallVelocities(force);
	StaticWorld::Velocities vels = StaticWorld::Velocities(resultVel.linearVelocity, resultVel.angularVelocity);
	
	std::vector<std::list<vector>> segments;
	segments.reserve(4);
	segments.clear();
	int ballIndex = -1;
	staticWorld->spawnMotionTrajectoryPoints(
		segments,
		ballIndex,
		vels,
		lengths.trajectA,
		lengths.trajectE
	);
	_table->drawTrajSegmentPoints(segments, true);

	auto firstSegment = segments[0];
	auto collidedPoint = firstSegment.back();

	if (ballIndex > -1 && segments.size() > 1) { // Collided with ball
		auto secondSegment = segments[1];
		interactionMgr->setGhostBallPosition(collidedPoint);
	}
	else {
		interactionMgr->setGhostBallPosition(collidedPoint);
	}
	//gameUI.getIngameUIs().forEach(e = > e.setInteractionBall(result.ballIndex));

}

void GameMgr::updateBasicGuildLine()
{
	if (!_enableGuildLine) return; // Avoid Lerp Overriding Update
	if (_table->getCue() == NULL) return;
	vector dir = _table->getCue()->getDirection();
	CueInfo * cueInfo = getCurrentCueInfo();
	if (cueInfo == NULL) return;
	auto lengths = cueInfo->getWorldTrajectoryLengths();
	auto staticWorld = _table->getWorld()->getStaticWorld();
	std::vector<segment> segments;
	std::vector<std::list<vector>> pointLists;
	int ballIndex = -1;
	staticWorld->buildBasicTrajectorySegments(segments, ballIndex, dir, lengths.trajectA, lengths.trajectE);
	staticWorld->spawnBasicTrajectoryPoints(pointLists, segments);
	_table->drawTrajSegmentPoints(pointLists, true);
	if (pointLists[0].size() > 0) {
		vector collidedPoint = pointLists[0].back();
		if (ballIndex > -1) {
			if (pointLists.size() == 1) {
				interactionMgr->setGhostBallPosition(collidedPoint);
			}
			else {
				std::list<vector>& secondSegment = pointLists[1];
				vector reflectPoint = secondSegment.back();
				interactionMgr->setGhostBallPosition(collidedPoint, vector::unit(
					vector::sub(reflectPoint, collidedPoint)
				));
			}
		}
		else {
			interactionMgr->setGhostBallPosition(collidedPoint);
		}
	}
	//TODO this.gameUI.getIngameUIs().forEach(e => e.setInteractionBall(result.ballIndex));
}

void GameMgr::setEnableGuildLine(bool enable)
{
	_enableGuildLine = enable;
}

void GameMgr::update(float dt)
{
	interactionMgr->update(dt);
	timeUtils->update(dt);
	offerMgr->updateTime(dt);
}

Ball * GameMgr::getSuggestedBall()
{
	auto curMatch = _ebpCtrl->getCurMatch();
	Ball *ball = curMatch->getSuggestedBall();

	return ball;
}

CueInfo * GameMgr::getCurrentCueInfo()
{
	return _table->getCue()->getCueInfo();
}

void GameMgr::addUserInfo(int userId, User * user)
{
	//TODO
}

void GameMgr::setBallMode2D(bool val)
{
	auto balls = _table->getAllBall();
	for (int i = 0; i < balls.size(); i ++) 
	{
		auto ball = balls[i];
		if (val) {
			ball->setMode2D();
		}
		else {
			ball->resetRoughness();
		}
	}

	if (val) {
		_table->getWhiteBall()->setMode2D();
	}
	else {
		_table->getWhiteBall()->resetRoughness();
	}
}

void GameMgr::onCallPocketEnded()
{
	auto cue = _table->getCue();
	cue->animateVisible(false, [this]() {
		setEnableGuildLine(true);
		_table->setGuildLineVisible(true);
		updateBasicGuildLine();
	});

}

void GameMgr::updateUserInfo(UserInfoInPacket *data)
{
	_userInfo->updateInfo(data);
	
	if (_scene->isRunning() && dynamic_cast<GameScene *>(_scene)) {
		auto gameScene = dynamic_cast<GameScene *>(_scene);
		auto lobbyUI = gameScene->_lobbyUI;
		lobbyUI->updateUserInfo(_userInfo);
	}

}

void GameMgr::startGameScene()
{
	auto gameScene = (GameScene*)GameScene::createScene();
	sceneMgr->showScene(gameScene, SceneTag::MAIN_SCENE);
	_scene = gameScene;
	_table = gameScene->_table;
	_ingameUI = gameScene->getIngameUI();
	_lobbyUI = gameScene->_lobbyUI;
	_camera = gameScene->_camera;
	_cueAngle = gameScene->_cueAngle;

	settingMgr->setCamera(_camera);
	initTouchManager();
	initInteraction();

}

void GameMgr::startTestScene()
{
	auto gameScene = (TestScene*)TestScene::createScene();
	sceneMgr->showScene(gameScene, SceneTag::MAIN_SCENE);
	_scene = gameScene;
	_table = gameScene->_table;
	_ingameUI = gameScene->getIngameUI();
	_camera = gameScene->_camera;
	_cueAngle = gameScene->_cueAngle;

	settingMgr->setCamera(_camera);
	initTouchManager();
}

void GameMgr::initInteraction()
{
	interactionMgr->setEntities(_camera, _table->getCue(), _table->getWhiteBall(), _ingameUI, _touchManager, _cueAngle);
	interactionMgr->enablePlayingMode();

	auto gameScene = dynamic_cast<GameScene *>(_scene);
	if(gameScene) gameScene->toLobby();
}

void GameMgr::initTouchManager()
{
	auto touchManager = TouchManager::create();
	_scene->addChild(touchManager);
	_touchManager = touchManager;
}

GameMgr* GameMgr::_instance = nullptr;
