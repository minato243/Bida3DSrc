#include "LoginMgr.h"
#include "cocos2d.h"
#include "core/Constant.h"
#include "core/gui/SceneMgr.h"
#include "core/Config.h"
#include "LoginScene.h"
#include "core/GameMgr.h"
#include "core/utils/StorageUtil.h"
#include "lobby/gui/BroadcastMgr.h"
#include "core/utils/JNI.h"
#include "core/utils/LanguageMgr.h"
#include "lobby/network/GameClient.h"
#include "core/utils/StringUtility.h"
#include "core/plugins/PlatformWrapper.h"
#include "core/utils/Network.h"
#include "json/document.h"
#include "core/plugins/Google.h"
#include "core/GameData.h"
#include "core/plugins/Facebook.h"
#include "core/plugins/AppleID.h"
#include "modules/ui/inventory/cue_collection/CueCollectionUI.h"
#include "modules/scene/loading/LoadingSceneNew.h"
USING_NS_CC;

static const std::string SOCIAL_FACEBOOK_STR = "facebook";
static const std::string SOCIAL_GOOGLE_STR = "google";
static const std::string SOCIAL_GUEST_STR = "guest";
static const std::string SOCIAL_APPLEID_STR = "apple";

static const std::string SESSION_KEY_PREFIX = "+++";

// key
static const std::string KEY_LOGIN_AUTO_LOGIN = "login_auto_login";
static const std::string KEY_LOGIN_SESSION_KEY = "login_sesison_key";
static const std::string KEY_LOGIN_DEFAULT = "login_default_key";


static const std::string KEY_SAVE_IP_SERVER = "save_ip_server";
static const std::string KEY_SAVE_PORT_SERVER = "save_port_server";

static const std::string KEY_SAVE_GUEST_DEVICE_ID = "save_guest_device_id_final";

LoginMgr::LoginMgr()
{
	defaultLogin = -1;

	currentSocial = 0;
	accessToken = "";
	sessionKey = "";
	openID = "";

	ipServer = "";
	portServer = 0;
}

LoginMgr::~LoginMgr()
{

}

LoginMgr* LoginMgr::getInstance()
{
	if (!_instance) {
		_instance = new LoginMgr();
	}
	return _instance;
}

void LoginMgr::loadCache()
{

}

bool LoginMgr::checkCurrentSocial(int social)
{
	return currentSocial == social;
}

bool LoginMgr::checkNetwork()
{
 	if (!JNI::getNetworkStatus())
 	{
 		sceneMgr->showOKDialog(LocalizedString::to("_NONETWORK_"));
 		return false;
 	}
	return true;
}

bool LoginMgr::checkDeviceSupport(std::string social)
{

	if (Application::getInstance()->getTargetPlatform() != ApplicationProtocol::Platform::OS_WINDOWS) {
		return true;
	}

	ToastFloat::makeToast(ToastFloat::LONG, StringUtils::format("Login %s Device Not Support !",social.c_str()));
	return false;
}

void LoginMgr::setDefaultLogin(int dLogin)
{
	defaultLogin = -1;
	int isDef = StorageUtil::getInt(KEY_LOGIN_DEFAULT, 0);
	if (isDef == 0) {
		switch (dLogin) {
		case 0: {
			defaultLogin = SOCIAL_GOOGLE;
			break;
		}
		case 1: {
			defaultLogin = SOCIAL_GUEST;
			break;
		}
		case 2: {
			defaultLogin = SOCIAL_FACEBOOK;
			break;
		}
		}
	}
}

void LoginMgr::clearDefaultLogin()
{
	defaultLogin = -1;
	StorageUtil::setInt(KEY_LOGIN_DEFAULT, 1);
}


void LoginMgr::loginFacebook()
{
	if (!loginMgr->checkDeviceSupport("Facebook")) {
		//TODO:CHEAT
		/*
		if (Config.ENABLE_CHEAT) {
			setTimeout(function() {
				loginMgr.currentSocial = SOCIAL_FACEBOOK;
				this.onResponseAccessToken(true, TOKEN_FACEBOOK_TEST);
			}.bind(this), 1000);
		}
		*/
		return;
	}
	if (!loginMgr->checkNetwork()) return;

	sceneMgr->addLoading();

	loginMgr->currentSocial = SOCIAL_FACEBOOK;
	if (loginMgr->checkCacheSessionKey()) {
		loginMgr->sessionKey = loginMgr->getCacheSessionKey();
		loginMgr->startConnectServer();
	}
	else {
		fr::Facebook::login([](bool result, std::string msg) {
			loginMgr->onResponseAccessToken(result, msg);
		});
	}
}

void LoginMgr::loginGoogle()
{
	if (!loginMgr->checkDeviceSupport("Google")) {
		//TODO:CHEAT
		/*
		if (Config::ENABLE_CHEAT) {
			setTimeout(function() {
				loginMgr.currentSocial = SOCIAL_GOOGLE;
				this.onResponseAccessToken(true, TOKEN_GOOGLE_TEST);
			}.bind(this), 1000);
		}
		*/
		return;
	}
	if (!loginMgr->checkNetwork()) return;

	sceneMgr->addLoading();

	loginMgr->currentSocial = SOCIAL_GOOGLE;
	if (loginMgr->checkCacheSessionKey()) {
		loginMgr->sessionKey = loginMgr->getCacheSessionKey();
		loginMgr->startConnectServer();
	}
	else {
		fr::Google::login([](bool result, std::string msg) {
			loginMgr->onResponseAccessToken(result, msg);
		});
	}
}

void LoginMgr::loginAppleID()
{
	if (!loginMgr->checkDeviceSupport("AppleID")) {
		return;
	}
	if (!loginMgr->checkNetwork()) return;

	sceneMgr->addLoading();

	loginMgr->currentSocial = SOCIAL_APPLEID;
	if (loginMgr->checkCacheSessionKey()) {
		loginMgr->sessionKey = loginMgr->getCacheSessionKey();
		loginMgr->startConnectServer();
	}
	else {
		fr::AppleID::login([](bool result, std::string msg) {
			loginMgr->onResponseAccessToken(result, msg);
		});
	}
}

void LoginMgr::loginGuest()
{
	if (!loginMgr->checkNetwork()) return;

	sceneMgr->addLoading();

	loginMgr->currentSocial = SOCIAL_GUEST;
	if (loginMgr->checkCacheSessionKey()) {
		loginMgr->sessionKey = loginMgr->getCacheSessionKey();
		loginMgr->startConnectServer();
	}
	else {
		loginMgr->fetchSessionKeyGuest();
	}
}

void LoginMgr::loginDev(std::string uid)
{
	sceneMgr->initialLayer();
	sceneMgr->addLoading();

	loginMgr->currentSocial = 0;
	loginMgr->sessionKey = uid;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	loginMgr->startConnectServer(Config::SERVER_LOCAL, Config::PORT_LOCAL);
#else
	loginMgr->startConnectServer(Config::SERVER_PRIVATE, Config::PORT);
#endif
}

void LoginMgr::startConnectServer(std::string server, int port)
{
	sceneMgr->addLoading();

	GameClient::getInstance()->connect(server, port);
}

void LoginMgr::stopConnectServer()
{
	loginMgr->currentSocial = -1;
	loginMgr->sessionKey = "";
}

void LoginMgr::onLoginSuccess()
{
	broadcastMgr->onStart();
	loginMgr->saveCurrentLogin();
	loginMgr->clearDefaultLogin();
	gameMgr->startGameScene();
	//sceneMgr->openGUI(CueCollectionUI::className);
}

void LoginMgr::onLogout()
{
	currentSocial = -1;
	sessionKey = "";

	loginMgr->saveCurrentLogin();
}

void LoginMgr::backToLoginScene()
{

	Node* curScene = sceneMgr->getMainLayer();

	if (dynamic_cast<LoadingSceneNew *>(curScene))
	{
		auto loadingScene = dynamic_cast<LoadingSceneNew *>(curScene);
		loadingScene->processLogin();
	}
}

void LoginMgr::saveServerIP(std::string ip, int port)
{
	if (ip.empty())
	{
		return;
	}
	StorageUtil::setString(KEY_SAVE_IP_SERVER, ip);
	StorageUtil::setInt(KEY_SAVE_PORT_SERVER, port);
}

std::string LoginMgr::getServerIP()
{
	if (ipServer.empty()) 
		ipServer = Config::SERVER_LIVE;
	return ipServer;
}

int LoginMgr::getServerPort()
{
	if (!portServer) 
		portServer = Config::PORT_LIVE;
	return portServer;
}

void LoginMgr::onResponseAccessToken(bool res, std::string token)
{
	if (res) {
		loginMgr->accessToken = token;
		loginMgr->fetchSessionKey();
	}
	else {
		sceneMgr->clearLoading();
		Toast::makeToast(Toast::LONG, LocalizedString::to("_LOGIN_ERROR_"));
	}
}

void LoginMgr::onResponseSessionKey(bool res, std::string data)
{
	sceneMgr->clearLoading();

	//parse
	rapidjson::Document doc;
	doc.Parse<0>(data.c_str());
	if (!doc.HasParseError()) {
		if (doc.HasMember("error") && doc["error"].IsInt() && doc["error"].GetInt() == 0)
		{
			if (doc.HasMember("sessionKey") && doc["sessionKey"].IsString() )
			{
				std::string sessionKey = doc["sessionKey"].GetString();
				loginMgr->sessionKey = StringUtils::format("%s%s", SESSION_KEY_PREFIX.c_str(), sessionKey.c_str());
				loginMgr->openID = "";
				loginMgr->startConnectServer();
				return;
			}
		}
	}
	
	loginMgr->clearAutoLogin();
	sceneMgr->showOKDialog(LocalizedString::to("_LOGIN_ERROR_"));
}

std::string LoginMgr::getGuestID()
{
	std::string str = StorageUtil::getString(KEY_SAVE_GUEST_DEVICE_ID, "");

	if (Config::ENABLE_CHEAT) {
		str = gameMgr->getDeviceID();
	}

	if (str.empty()) {
		str = StringUtility::replaceAll(fr::platformWrapper::getAAID(), "-", "");
		if (str.empty()) {
			str = gameMgr->getDeviceID();
		}

		if (str.empty()) {
			str = StringUtility::md5(StringUtils::format("%ld",utils::getTimeInMilliseconds()));
		}

		StorageUtil::setString(KEY_SAVE_GUEST_DEVICE_ID, str);
	}
	return str;
}

std::string LoginMgr::getSessionKey()
{
	return loginMgr->sessionKey;
}

bool LoginMgr::checkCacheSessionKey()
{
	std::string ss = StorageUtil::getString(KEY_LOGIN_SESSION_KEY, "");
	return !ss.empty();
}

std::string LoginMgr::getCacheSessionKey()
{
	std::string ss = StorageUtil::getString(KEY_LOGIN_SESSION_KEY, "");
	if (!ss.empty()) 
		return ss;

	return "";
}

int LoginMgr::getCurrentSocial()
{
	return currentSocial;
}

std::string LoginMgr::getCurrentSocialName()
{
	switch (loginMgr->currentSocial) {
	case SOCIAL_FACEBOOK:
		return SOCIAL_FACEBOOK_STR;
	case SOCIAL_GOOGLE:
		return SOCIAL_GOOGLE_STR;
	case SOCIAL_GUEST:
		return SOCIAL_GUEST_STR;
	case SOCIAL_APPLEID:
		return SOCIAL_APPLEID_STR;
	}
	return "";
}

void LoginMgr::fetchSessionKey()
{
	sceneMgr->addLoading();

	std::string gameId = LocalizedString::config("GAME_LOGIN_ID");
	std::string secretKey = LocalizedString::config("GAME_LOGIN_SECRET_KEY");
	std::string _social = loginMgr->getCurrentSocialName();
	std::string _clientInfo = _social + gamedata->source;
	std::string _deviceId = gameMgr->getDeviceID();
    std::string url = CONSTANT_BRASIL_PORTAL_SERVICE_URL;
    std::string accessToken = StringUtility::encodeURIComponent(loginMgr->accessToken);
    if (currentSocial == SOCIAL_APPLEID) {
        gameId="321";
        url = CONSTANT_GLOBAL_PORTAL_SERVICE_URL;
    }
	std::string _dataMac = StringUtility::md5(_social + accessToken + _clientInfo + _deviceId + gameId + secretKey);
    
	std::string data = "service_name=getSessionKey";
	data += "&clientInfo=" + _clientInfo;
	data += "&gameId=" + gameId;
	data += "&deviceId=" + _deviceId;
	data += "&social=" + _social;
	data += "&accessToken=" + accessToken;
	data += "&mac=" + _dataMac;

	Network::addRequestPost(url, data, [this](bool isSuccess, std::string data) {
		onResponseSessionKey(isSuccess, data);
	});

}

void LoginMgr::fetchSessionKeyGuest()
{
	sceneMgr->addLoading();

	std::string gameId = LocalizedString::config("GAME_LOGIN_ID");
	std::string secretKey = LocalizedString::config("GAME_LOGIN_SECRET_KEY");
	std::string _deviceId = loginMgr->getGuestID();
	std::string _dataMac = StringUtility::md5(_deviceId + gameId + secretKey);

	std::string data = StringUtils::format("service_name=zacc_guest&gameId=%s&deviceId=%s&isPortal=0&mac=%s", gameId.c_str(), _deviceId.c_str(), _dataMac.c_str());
	cocos2d::log("LoginMgr::fetchSessionKeyGuest %s?%s",CONSTANT_BRASIL_PORTAL_SERVICE_URL.c_str(), data.c_str());
	Network::addRequestPost(CONSTANT_BRASIL_PORTAL_SERVICE_URL, data, [this](bool isSuccess, std::string data) {
		onResponseSessionKey(isSuccess, data);
	});
}

int LoginMgr::getDefaultLogin()
{
	return defaultLogin;
}

int LoginMgr::getCurrentLogin()
{
	return StorageUtil::getInt(KEY_LOGIN_AUTO_LOGIN, 0);
}

void LoginMgr::saveCurrentLogin()
{
	StorageUtil::setInt(KEY_LOGIN_AUTO_LOGIN, currentSocial);
	StorageUtil::setString(KEY_LOGIN_SESSION_KEY, sessionKey);
}

void LoginMgr::clearAutoLogin()
{
	currentSocial = -1;
	sessionKey = "";
}

LoginMgr* LoginMgr::_instance = nullptr;

