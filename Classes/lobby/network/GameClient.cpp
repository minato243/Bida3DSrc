#include "GameClient.h"
#include "core/Config.h"
#include "login/LoginMgr.h"
#include "engine/Handler.h"
#include "core/gui/SceneMgr.h"
#include "core/utils/LanguageMgr.h"
#include "core/PacketsFactory.h"
#include "../CheckLogic.h"
#include "login/LoginScene.h"
#include "core/utils/JNI.h"
#include "modules/network/user/UserHandler.h"
#include "modules/ui/GuiMgr.h"

GameClient::GameClient() {
	_clientID = cocos2d::utils::getTimeInMilliseconds();

	userHandler = new UserHandler();

	_tcpClient = fr::GsnClient::create();

	_tcpClient->setFinishConnectListener([this](bool isSuccess) {
		userHandler->onFinishConnect(isSuccess);
	});
	_tcpClient->setDisconnectListener([this]() {
		userHandler->onDisconnected();
	});

	_tcpClient->setReceiveDataListener([&](int msgId, fr::sPackage* pkg) {
		cocos2d::log("cmd: %d", msgId);
		fr::InPacket* inPacket = PacketsFactory::getInPacket(pkg);
		userHandler->onReceived(inPacket);
		for (auto handler : _handlerList)
		{
			bool solved = handler->onReceived(inPacket);
			if (solved)
				break;
		}
	});
	connectState = ConnectState::DISCONNECTED;

	connectServer = false;
}
GameClient::~GameClient() {

}

GameClient* GameClient::getInstance()
{
	if (!_instance)
	{
		_instance = new GameClient();
	}
	return _instance;
}

void GameClient::destroyInstance()
{
	
	if (_instance) {
		_instance->destroy();
		delete _instance;
		_instance = nullptr;
	}
}

fr::GsnClient* GameClient::getNetwork()
{
	return _tcpClient;
}

void GameClient::connect(std::string server, int port)
{
	timeSendLatency = 50;
	connectState = ConnectState::CONNECTING;
	connectServer = true;

	// setup server
	std::string sLog = "";
	if (server.empty()) {
		if (Config::ENABLE_CHEAT) {
			//TODO:cheat
			/*sLog = "PRIVATE";

			server = CheatCenter.SERVER_IP;
			port = CheatCenter.SERVER_PORT;*/
		}
		else {
			sLog = "LIVE";

			server = loginMgr->getServerIP();
			port = loginMgr->getServerPort();
		}
	}


	// connect
	cocos2d::log("______CONNECT SERVER %s, %s, %d", sLog.c_str(), server.c_str(), port);
	_tcpClient->connect(server.c_str(), port);

	connectState = ConnectState::CONNECTING;
	connectServer = true;

	timeoutConnectHandler->startCountDown();
}

void GameClient::sendPacket(fr::OutPacket* msg)
{
	if (_tcpClient && connectServer)
		_tcpClient->send(msg);
}

void GameClient::disconnect()
{
	_tcpClient->disconnect();
	_tcpClient->setFinishConnectListener(nullptr);
	_tcpClient->setDisconnectListener(nullptr);
	_tcpClient->setReceiveDataListener(nullptr);

	connectServer = false;
	timeoutConnectHandler->clearCountDown();
}

void GameClient::retryConnectInGame()
{
	HandlerManager::getInstance()->forceRemoveHandler("pingpong");
	HandlerManager::getInstance()->forceRemoveHandler("received_pingpong");
	sceneMgr->clearLoading();

	GameClient::destroyInstance();
	GameClient::disconnectHandle();
}

void GameClient::retryManualConnect()
{
	HandlerManager::getInstance()->forceRemoveHandler("pingpong");
	HandlerManager::getInstance()->forceRemoveHandler("received_pingpong");
	sceneMgr->clearLoading();
	GameClient::destroyInstance();
	HandlerManager::getInstance()->addHandler("login_count", [this](std::string str, cocos2d::Ref* ref) {

	});
	HandlerManager::getInstance()->getHandler("login_count")->setTimeOut(GameClient::TIME_OUT_RETRY, true);

}

void GameClient::startPingPong()
{
	pingPongHandler->startPing();
	timeoutConnectHandler->clearCountDown();
}

void GameClient::receivePingPong()
{
	pingPongHandler->receivePing();
}

void GameClient::destroy()
{
	_clientID = 0;
	delete _tcpClient;
	_tcpClient = nullptr;
}

void GameClient::addHandler(BaseHandler *handler)
{
	bool exist = false;
	for (int i = 0; i < _handlerList.size(); i++) 
	{
		if (_handlerList[i] == handler)
		{
			exist = true;
			break;
		}
	}
	if (!exist)
		_handlerList.push_back(handler);
}

void GameClient::disconnectHandle()
{
	if (!CheckLogic::checkInBoard()) {
		sceneMgr->showOkCancelDialog(LocalizedString::to("_CONNECTFAILED_"), [](int btnId) {
			bool checkPortal = false;
			if (btnId == 0) {
				// no action
			}
			else {
				loginMgr->onLogout();
				checkPortal = true;
			}

			loginMgr->backToLoginScene();
		});
	}
	else {
		//CheckLogic::showNotifyNetworkSlow(true);
		GameClient::processRetryConnect();
	}
}

void GameClient::connectFailedHandle()
{
	GameClient::getInstance()->disconnect();
	GameClient::destroyInstance();

	// notify
	if (!CheckLogic::checkInBoard()) {
		sceneMgr->clearLoading();
		loginMgr->onLogout();

		guiMgr->showOkCancelPopup(LocalizedString::to("CONFIRM_CONNECT"), [](int btnID) {
			if (btnID == MessagePoup::TAG::btnPositive){
				loginMgr->backToLoginScene();
			}
			else {
				loginMgr->onLogout();
			}
		});
	}
	else {
		GameClient::processRetryConnect();
	}
}

void GameClient::processRetryConnect()
{
	if (GameClient::connectCount >= GameClient::MAX_RETRY_CONNECT) {
		GameClient::showNetworkFail();
	}
	else {
		HandlerManager::getInstance()->addHandler("login_count", [](std::string str, cocos2d::Ref* ref) {
			GameClient::getInstance()->connect();
			GameClient::connectCount++;
		});
		HandlerManager::getInstance()->getHandler("login_count")->setTimeOut(GameClient::TIME_OUT_RETRY, true);
	}
}

void GameClient::showNetworkFail()
{
	bool isNetwork = JNI::getNetworkStatus();

	if (GameClient::retryCount >= GameClient::MAX_NETWORK_UNSTABLE) {
		isNetwork = false;
		GameClient::retryCount = 0;
	}
	else {
		GameClient::retryCount++;
	}

	std::string msg = "";
	if (isNetwork) {
		msg = LocalizedString::to("DISCONNECT_UNKNOWN");
	}
	else {
		msg = LocalizedString::to("CHECK_NETWORK");
	}

	sceneMgr->showOkCancelDialog(msg, [](int btnID) {
		if (btnID == 0) {
			GameClient::connectCount = 0;
			GameClient::getInstance()->connect();
		}
		else {
			loginMgr->onLogout();
			loginMgr->backToLoginScene();
		}
	});
}

GameClient* GameClient::_instance;

int GameClient::connectCount;

int GameClient::retryCount;

bool GameClient::reconnect;

bool GameClient::holding;

int GameClient::holdingRoom;

std::string GameClient::holdingPass;

bool GameClient::connectLai;

const int GameClient::MAX_RETRY_CONNECT = 3;

const int GameClient::MAX_NETWORK_UNSTABLE = 5;

const int GameClient::TIME_OUT_RETRY = 5;

const int GameClient::PINGPONG_TIMEOUT = 15;

const int GameClient::PINGPONG_INTERVAL = 5;

const int GameClient::NETWORK_SLOW_NOTIFY = 5;

const std::vector<float> GameClient::NETWORK_STATE_TIME_NOTIFY = { 0.1f, 1, 5, 10 };

PingPongHandler::PingPongHandler()
{
	timePingPong = 0;
	timeDelayPingPong = 0;
	timeRetryPingPong = 0;
	arStateTime = {};

	isRetryPingpong = false;
	isDelayPingpong = false;
	isPingpong = false;

	isNetworkSlow = false;

	isStopHandler = true;
}

PingPongHandler::~PingPongHandler()
{

}

PingPongHandler* PingPongHandler::getInstance()
{
	if(!_instance)
	{
		_instance = new PingPongHandler();
	}
	return _instance;
}

void PingPongHandler::startPing()
{
	isStopHandler = false;

	sendPing();

	timePingPong = 0;
	timeDelayPingPong = 0;
	isDelayPingpong = false;
	isPingpong = true;
	isNetworkSlow = false;

	arStateTime = GameClient::NETWORK_STATE_TIME_NOTIFY;

	//CheckLogic::showNotifyNetworkSlow(false);
	//CheckLogic::updateNetworkState();
	//CheckLogic::retryConnectInBoard();
}

void PingPongHandler::receivePing()
{
	timePingPong = 0;
	timeDelayPingPong = 0;
	timeRetryPingPong = 0;

	isRetryPingpong = false;
	isDelayPingpong = true;
	isPingpong = false;

	isNetworkSlow = false;

	//CheckLogic::showNotifyNetworkSlow(false);
	//CheckLogic::updateNetworkState();
}

void PingPongHandler::sendPing()
{
	timeRetryPingPong = 0;
	isRetryPingpong = true;

	GameClient::getInstance()->lobbyHandler->requestPingpong();
}

void PingPongHandler::checkNeedPing()
{
	if (sceneMgr->checkMainLayer<LoginScene>()) {
		isStopHandler = true;
	}
}

void PingPongHandler::autoDisconnect()
{
	isStopHandler = true;

	isPingpong = false;
	isDelayPingpong = false;
	timePingPong = 0;
	timeDelayPingPong = 0;

	//CheckLogic::updateNetworkState(0);

	sceneMgr->clearLoading();

	GameClient::getInstance()->disconnect();
	GameClient::destroyInstance();
	GameClient::disconnectHandle();

	if (CheckLogic::checkInBoard()) {
		GameClient::connectLai = true;
	}
	else {
		GameClient::connectLai = false;
	}
}

void PingPongHandler::updatePing(float dt)
{
	if (isStopHandler) return;

	// check state ping and check timeout ping
	if (isPingpong) {
		if (timePingPong < GameClient::PINGPONG_TIMEOUT) {
			timePingPong += dt;

			// network state wifi
			for (int i = 0, size = arStateTime.size(); i < size; i++) {
				if (timePingPong > arStateTime[i]) {
				//	CheckLogic::updateNetworkState(size - i - 1);
					arStateTime.erase(arStateTime.begin() + i);
					break;
				}
			}

			// network slowly
			if (timePingPong > GameClient::NETWORK_SLOW_NOTIFY && !isNetworkSlow) {
				isNetworkSlow = true;
				//CheckLogic::showNotifyNetworkSlow(true);
			}

			// pingpong timeout
			if (timePingPong >= GameClient::PINGPONG_TIMEOUT) {
				autoDisconnect();
			}
		}
	}

	// retry send ping if connect slow
	if (isRetryPingpong) {
		if (timeRetryPingPong < GameClient::PINGPONG_INTERVAL) {
			timeRetryPingPong += dt;
			if (timeRetryPingPong >= GameClient::PINGPONG_INTERVAL) {
				sendPing();
			}
		}
	}

	// delay to send ping
	if (isDelayPingpong && timeDelayPingPong < GameClient::PINGPONG_INTERVAL) {
		timeDelayPingPong += dt;

		if (timeDelayPingPong >= GameClient::PINGPONG_INTERVAL) {
			startPing();
		}
	}
}

PingPongHandler* PingPongHandler::_instance = nullptr;

TimeoutConnectHandler::TimeoutConnectHandler()
{
	timeCountDown = 0;
	isWaitConnect = false;
}

TimeoutConnectHandler::~TimeoutConnectHandler()
{
}

TimeoutConnectHandler* TimeoutConnectHandler::getInstance()
{
	if (!_inst) {
		_inst = new TimeoutConnectHandler();
	}
	return _inst;
}

void TimeoutConnectHandler::startCountDown()
{
	isWaitConnect = true;
	timeCountDown = Config::TIMEOUT_CONNECT_SERVER;
}

void TimeoutConnectHandler::clearCountDown()
{
	isWaitConnect = false;
	timeCountDown = 0;
}

void TimeoutConnectHandler::doTimeoutConnect()
{
	if (Config::TIMEOUT_CONNECT_SERVER == 0) return;

	cocos2d::log("##TimeoutConnect::onTimeout ");
	clearCountDown();
	GameClient::destroyInstance();

	sceneMgr->clearLoading();

	if (CheckLogic::checkInBoard()) {
		GameClient::processRetryConnect();
	}
	else {
		guiMgr->showOkCancelPopup(LocalizedString::to("CONFIRM_CONNECT"), [](int btnId) {
			bool checkPortal = false;
			if (btnId == 0) {
			}
			else {
				loginMgr->onLogout();
				checkPortal = true;
			}

			loginMgr->backToLoginScene();
		});
	}
}

void TimeoutConnectHandler::updateCountDown(float dt)
{
	if (Config::TIMEOUT_CONNECT_SERVER == 0) return;

	if (isWaitConnect) {
		if (timeCountDown > 0) {
			timeCountDown -= dt;
			if (timeCountDown <= 0) {
				doTimeoutConnect();
			}
		}
	}
}

TimeoutConnectHandler* TimeoutConnectHandler::_inst = nullptr;
