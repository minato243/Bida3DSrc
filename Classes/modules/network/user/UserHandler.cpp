#include "UserHandler.h"
#include <lobby/network/GameClient.h>
#include "engine/Handler.h"
#include "core/gui/SceneMgr.h"
#include "../PacketIDDefine.h"
#include <core/PacketsFactory.h>
#include <login/LoginMgr.h>
#include <core/GameMgr.h>
#include <core/plugins/PlatformWrapper.h>
#include "core/utils/JNI.h"
#include "core/utils/LanguageMgr.h"
#include "../inventory/InventoryHandler.h"

using namespace pk;

UserHandler::UserHandler()
{
	_listenerID = 0;
	timeHandShake = 0;
}

UserHandler::~UserHandler()
{
}

void UserHandler::onFinishConnect(bool isSuccess)
{
	if (isSuccess) {
		timeHandShake = cocos2d::utils::getTimeInMilliseconds();
		requestHandshake();
		GameClient::getInstance()->connectState = ConnectState::CONNECTING;
		GameClient::connectCount = 0;
		GameClient::retryCount = 0;
	}
	else {
		GameClient::getInstance()->connectState = ConnectState::DISCONNECTED;
		GameClient::connectFailedHandle();
	}
}

void UserHandler::onDisconnected()
{
	GameClient::getInstance()->connectServer = false;
	if (GameClient::getInstance()->connectState == ConnectState::CONNECTED) {
		GameClient::getInstance()->connectState = ConnectState::DISCONNECTED;
		HandlerManager::getInstance()->forceRemoveHandler("pingpong");
		HandlerManager::getInstance()->forceRemoveHandler("received_pingpong");
		sceneMgr->clearLoading();
		GameClient::destroyInstance();
		GameClient::disconnectHandle();
	}
	else if (GameClient::getInstance()->connectState == ConnectState::CONNECTING) {
		GameClient::getInstance()->connectState = ConnectState::DISCONNECTED;
		HandlerManager::getInstance()->forceRemoveHandler("login");
		sceneMgr->clearLoading();
		GameClient::destroyInstance();
		GameClient::connectFailedHandle();

	}

	//if (CheckLogic::checkInBoard()) {
	//	GameClient::connectLai = true;
	//}
	//else
	//	GameClient::connectLai = false;
}

void UserHandler::onReceived(fr::InPacket * pk)
{

	int cmd = pk->getCmdId();
	int controllerID = pk->getControllerId();
	if (cmd != 50 && cmd != 20001)
		cocos2d::log(" ON RECEIVED PACKET   CMD: %d CONTROLLER ID: %d ERRO.R: %d", cmd, pk->getControllerId(), pk->getError());
	if (controllerID == 0) {
		switch (cmd) {
		case PacketID::HAND_SHAKE: {
			requestLogin(loginMgr->getSessionKey(), gameMgr->getDeviceID(), gameMgr->getPlatform(), false);
			break;
		}
		}
		return;
	}

	auto packet = pk;
	switch (cmd) {
	case PacketID::USER_LOGIN: {
		if (packet->getError() == 0) {
			cocos2d::log("_____________________LOGIN SUCCESSFUL_____________________");
			getUserInfo();
			loginMgr->onLoginSuccess();
		}
		else {
			cocos2d::log("_____________________LOGIN FAIL____________________");

			sceneMgr->clearLoading();
			loginMgr->onLogout();
			sceneMgr->showOkCancelDialog(LocalizedString::to("LOGIN_FAILED"), [](int btnID) {
				if (btnID != 0) {
					loginMgr->onLogout();
				}
				loginMgr->backToLoginScene();
				});

			sceneMgr->clearLoading();
		}
		break;
	}
	case PacketID::GET_USER_INFO:
	{
		auto msg = new UserInfoInPacket(pk);
		msg->readData();
		gameMgr->updateUserInfo(msg);

		inventoryHandler->getInventoryInfo();

		CC_SAFE_DELETE(msg);
		break;
	}

	case PacketID::UPDATE_HISTORY:
	{
		auto msg = new UpdateHistoryMsg(pk);
		msg->readData();
		if (msg->error == 0)
		{
			gameMgr->_userInfo->updateHistory(msg->history);
		}
		CC_SAFE_DELETE(msg);
		break;
	}
	}
}

void UserHandler::getUserInfo()
{
	fr::OutPacket* packet = PacketsFactory::getOutPacket(PacketID::GET_USER_INFO);
	send(packet);
}

void UserHandler::send(fr::OutPacket * msg)
{
	GameClient::getInstance()->sendPacket(msg);
}

void UserHandler::requestHandshake()
{
	fr::OutPacket* packet = PacketsFactory::getOutPacket(PacketID::HAND_SHAKE, 0);
	send(packet);
}

void UserHandler::requestLogin(const std::string sessionkey, const std::string deviceId, int platform, bool isLoginGuest)
{
	fr::OutPacket* packet = PacketsFactory::getOutPacket(PacketID::USER_LOGIN);

	packet->putString(fr::platformWrapper::getClientVersion());
	packet->putString(fr::platformWrapper::getOsName());
	packet->putString(fr::platformWrapper::getOSVersion());
	packet->putString(fr::platformWrapper::getDeviceModel());
	packet->putString(fr::platformWrapper::getConnectionStatusName());
	packet->putString("");//fr::platformWrapper::getDownloadSource()
	packet->putString("");//fr::platformWrapper::getThirdPartySource()
	packet->putString(deviceId);
	packet->putString(sessionkey);
	packet->putInt(fr::platformWrapper::getVersionCode()); //game version
	packet->putString(fr::platformWrapper::getPackageName());
	packet->putString("");
	packet->putString("VN");
	packet->putString("");

	send(packet);
}
