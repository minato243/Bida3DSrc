
#include "GameClient.h"
#include "core/PacketsFactory.h"
#include "LobbyHandler.h"
#include "engine/Handler.h"
#include "core/gui/SceneMgr.h"
#include "../CheckLogic.h"
#include "CmdDefine.h"
#include "login/LoginMgr.h"
#include "core/GameMgr.h"
#include "core/utils/LanguageMgr.h"
#include "core/Config.h"
#include "core/utils/JNI.h"
#include "LobbyPackets.h"
#include "core/GameData.h"
#include "login/UserInfo.h"
#include "core/GameSound.h"
#include "LobbyHandler.h"
#include "../ChanelConfig.h"
USING_NS_CC;
LobbyHandler::LobbyHandler()
{
}

LobbyHandler::~LobbyHandler()
{

}

void LobbyHandler::onFinishConnect(bool isSuccess)
{
	if (isSuccess) {
		timeHandShake	 = cocos2d::utils::getTimeInMilliseconds();
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

void LobbyHandler::onDisconnected()
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

	if (CheckLogic::checkInBoard()) {
		GameClient::connectLai = true;
	}
	else
		GameClient::connectLai = false;
}

void LobbyHandler::onReceived(fr::InPacket* packet)
{
	int cmd = packet->getCmdId();
	int controllerID = packet->getControllerId();
	if (cmd != 50 && cmd != 20001)
		cocos2d::log(" ON RECEIVED PACKET   CMD: %d CONTROLLER ID: %d ERRO.R: %d", cmd, packet->getControllerId(), packet->getError());
	if (controllerID == 0) {
		switch (cmd) {
		case CMD::HAND_SHAKE: {
			requestLogin(loginMgr->getSessionKey(), gameMgr->getDeviceID(), gameMgr->getPlatform(), false);
			break;
		}
		case CMD::CMD_PINGPONG: {
			GameClient::getInstance()->receivePingPong();
			break;
		}
		case CMD::CMD_ANOTHER_COM: {
			if (packet->getError() == 3) {
				GameClient::getInstance()->connectState = ConnectState::DISCONNECTED;

				GameClient::getInstance()->disconnect();
				sceneMgr->clearLoading();

				GameClient::destroyInstance();

				sceneMgr->showOkCancelDialog(LocalizedString::to("DISCONNECT_LOGIN"), [](int btnID) {
					bool checkPortal = false;
					if (btnID != 0) {
						checkPortal = true;
						loginMgr->onLogout();
					}

					loginMgr->backToLoginScene();
				});
			}
			break;
		}
		}
		return;
	}

	switch (cmd) {
		case CMD::CMD_LOGIN: {
			if (packet->getError() == 0) {
				cocos2d::log("_____________________LOGIN SUCCESSFUL_____________________");

				requestMobile(JNI::getDeviceModel(), JNI::getOsVersion(), gameMgr->getPlatform(), gameMgr->getDeviceID(), gameMgr->getInstallDate());
				loginMgr->onLoginSuccess();
			}
			else {
				log("_____________________LOGIN FAIL____________________");

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
		case CMD::CMD_LOGIN_FAIL: {
			log("__________________RETRY RECONNECT OTHER SERVER____________________");

			if (!Config::ENABLE_CHEAT) {
				std::string ip = packet->getString();
				int port = packet->getInt();
				loginMgr->saveServerIP(ip, port);
			}

			GameClient::getInstance()->connectState = ConnectState::DISCONNECTED;
			GameClient::getInstance()->disconnect();
			GameClient::destroyInstance();

			Director::getInstance()->getScheduler()->schedule([](float dt) {
				GameClient::getInstance()->connect();
			}, GameClient::getInstance(), 0, 0, 1, false, "connect_server");
			break;
		}
		case CMD::CMD_LOG_MOBILE: {
			std::vector<bool> payments;
			packet->getBools(payments);

			gameMgr->updatePayments(false, payments);

			requestGetConfig();
			break;
		}
		case CMD::CMD_GET_CONFIG: {

			MsgChannelConfig config;
			config.channelConfig = packet->getString();
			config.vipConfig = packet->getString();
			config.vipBenefit = packet->getString();
			config.vipEmotion = packet->getString();
			config.vipOld = packet->getString();
			config.friendConfig = packet->getString();
			config.playingGiftConfig = packet->getString();
			config.guestConfig = packet->getString();

			gamedata->setChannelConfig(config);
			gamedata->gameConfig->setVipConfig(config.vipConfig);
			gamedata->gameConfig->setVipBenefit(config.vipBenefit);
			gamedata->gameConfig->setVipEmotion(config.vipEmotion);
			gamedata->gameConfig->setOldVipConfig(config.vipOld);
			gamedata->gameConfig->setFriendConfig(config.friendConfig);
			//gamedata.gameConfig.setSurveyConfig(config.surveyConfig);

			requestUserInfo();

			gamedata->selectedChanel = -1;
			break;
		}
		case CMD::CMD_CONFIG_SHOP: {
			MsgShopConfig shopConfig;
			shopConfig.type = packet->getByte();
			char typeShopBonus = packet->getByte();
			if (typeShopBonus == 1) {
				shopConfig.isShopBonus = true;
				shopConfig.isShopBonusG = false;
			}
			else if (typeShopBonus == 2) {
				shopConfig.isShopBonus = false;
				shopConfig.isShopBonusG = true;
			}
			else if (typeShopBonus == 3) {
				shopConfig.isShopBonus = true;
				shopConfig.isShopBonusG = true;
			}
			else {
				shopConfig.isShopBonus = false;
				shopConfig.isShopBonusG = false;
			}

			shopConfig.stringConfigGold = packet->getString();
			shopConfig.stringConfigG = packet->getString();

			shopConfig.paymentConfig = packet->getString();

			gamedata->saveConfigPayment(shopConfig.paymentConfig);
			gamedata->saveConfigShop(shopConfig);
			break;
		}
		case CMD::CMD_GET_USER_INFO: {

			MsgUserInfo info;
			info.uId = packet->getInt();
			info.userName = packet->getString();
			info.zName = packet->getString();
			info.avatar = packet->getString();

			info.gold = packet->getDouble();
			info.zMoney = packet->getDouble();
			info.levelScore = packet->getInt();

			info.winCount = packet->getInt();
			info.lostCount = packet->getInt();

			userInfo->setUserInfo(info);
			gamedata->setUserInfo(info);


			if (!GameClient::holding) {
				int cId = gamedata->gameConfig->getCurrentChanel() + 0;
				if (cId < 0) 
					cId = 0;
				requestSelectChannel(cId);
			}


			JNI::sendLoginGSN(StringUtils::format("%d",userInfo->getUserID()),
				loginMgr->getCurrentSocialName(),
				userInfo->getOpenID(),
				userInfo->getUserName());


			GameClient::getInstance()->connectState = ConnectState::CONNECTED;
			GameClient::getInstance()->startPingPong();


			GameData::IS_FIRST_LOGIN_AFTER_OPEN_APP = false;
			
			break;
		}
		case CMD::CMD_SELECT_CHANEL: {
			int channelID = packet->getByte();

			if (GameClient::holding) {
				requestJoinRoom(channelID, GameClient::holdingRoom);
				GameClient::holding = false;
			}

			if (packet->getError() == 0) {
				gamedata->selectedChanel = channelID;
				sceneMgr->updateCurrentGUI();
			}
			else {
			}
			break;
		}
		case CMD::CMD_UPDATE_BUYGOLD: {
			MsgUpdateBuyGold msg;
			msg.nBuyGold = packet->getInt();
			msg.sizeMission = packet->getInt();
			for (int i = 0; i < msg.sizeMission; i++) {
				std::vector<int> value;
				int type = packet->getInt();
				packet->getInts(value);
				std::vector<char> isFirst;
				packet->getBytes(isFirst);
				msg.arrayTypeFirst.push_back(type);
				msg.arrayIsFirst.push_back(isFirst);
				msg.arrayValue.push_back(value);
			}

			gamedata->gameConfig->arrayTypeFirst = msg.arrayTypeFirst;
			gamedata->gameConfig->arrayValueShop = msg.arrayValue;
			gamedata->gameConfig->arrayIsFirstShop = msg.arrayIsFirst;

			gamedata->gameConfig->buyGoldCount = msg.nBuyGold;
			sceneMgr->updateCurrentGUI();

			BaseLayer* gui = (BaseLayer*)sceneMgr->getRunningScene()->getMainLayer();
			if (gui) {
				gui->onUpdateGUI();
			}
			break;
		}
		default: {
			//GameClient::getInstance()->gameHandler->onReceived(packet);
			break;
		}
	}
}

void LobbyHandler::send(fr::OutPacket* msg)
{
	GameClient::getInstance()->sendPacket(msg);
}

void LobbyHandler::requestHandshake()
{
	fr::OutPacket* packet = PacketsFactory::getOutPacket(CMD::HAND_SHAKE, 0);
	send(packet);
}

void LobbyHandler::requestLogin(const std::string sessionkey, const std::string deviceId, int platform, bool isLoginGuest)
{
	fr::OutPacket* packet = PacketsFactory::getOutPacket(CMD::CMD_LOGIN);
	packet->putString(sessionkey);
	packet->putString(deviceId);
	packet->putInt(0);//jsversion
	packet->putInt(platform);
	packet->putByte(0);
	send(packet);
}


void LobbyHandler::requestMobile(std::string deviceName, std::string osVersion, int platformId, std::string deviceID, std::string installDate)
{
	fr::OutPacket* packet = PacketsFactory::getOutPacket(CMD::CMD_LOG_MOBILE);
	packet->putString(deviceID);
	packet->putString(deviceName);
	packet->putString(osVersion);
	packet->putByte(platformId);
	packet->putString(gamedata->appVersion);
	packet->putString(installDate);
	packet->putShort(0);
	packet->putString(JNI::getGameResource());
	packet->putString(gameMgr->getCountryID());
	send(packet);
}

void LobbyHandler::requestGetConfig()
{
	fr::OutPacket* packet = PacketsFactory::getOutPacket(CMD::CMD_GET_CONFIG);
	send(packet);
}

void LobbyHandler::requestUserInfo()
{
	fr::OutPacket* packet = PacketsFactory::getOutPacket(CMD::CMD_GET_USER_INFO);
	send(packet);
}

void LobbyHandler::requestSelectChannel(int channelID)
{
	fr::OutPacket* packet = PacketsFactory::getOutPacket(CMD::CMD_SELECT_CHANEL);
	packet->putByte(channelID);
	send(packet);
}

void LobbyHandler::requestUpdateInfo()
{
	fr::OutPacket* packet = PacketsFactory::getOutPacket(CMD::UPDATE_GAME_INFO);
	send(packet);
}

void LobbyHandler::requestJoinRoom(int channelID, int id)
{
	fr::OutPacket* packet = PacketsFactory::getOutPacket(CMD::JOIN_ROOM);
	packet->putInt(channelID);
	packet->putInt(id);
	send(packet);
}

void LobbyHandler::requestFinishTutorial(int step)
{
	fr::OutPacket* packet = PacketsFactory::getOutPacket(CMD::CMD_TUTORIAL);
	packet->putByte(step);
	send(packet);
}
void LobbyHandler::requestQuickPlay(int index)
{
	fr::OutPacket* packet = PacketsFactory::getOutPacket(CMD::CMD_QUICK_PLAY);
	packet->putInt(index);
	send(packet);
}

void LobbyHandler::requestGetSupportBean()
{
	fr::OutPacket* packet = PacketsFactory::getOutPacket(CMD::SUPPORT_BEAN);
	send(packet);
}

void LobbyHandler::requestGiftInfo()
{
	fr::OutPacket* packet = PacketsFactory::getOutPacket(CMD::GET_DAILY_GIFT);
	send(packet);
}

void LobbyHandler::requestPingpong()
{
	fr::OutPacket* packet = PacketsFactory::getOutPacket(CMD::CMD_PINGPONG);
	send(packet);
}

void LobbyHandler::requestTutorialSuccess()
{
	fr::OutPacket* packet = PacketsFactory::getOutPacket(CMD::CMD_TUTORIAL);
	send(packet);
}

void LobbyHandler::requestChatTotal(char typeMsg, int toId, std::string msg, int emoticonIndex)
{
	fr::OutPacket* packet = PacketsFactory::getOutPacket(CMD::CMD_SEND_MESSAGE);
	packet->putByte(typeMsg);
	packet->putInt(toId);
	packet->putString(msg);
	packet->putInt(emoticonIndex);
	send(packet);
}

void LobbyHandler::requestPurchaseIAPApple(std::string receiptData)
{
	fr::OutPacket* packet = PacketsFactory::getOutPacket(CMD::CMD_PURCHASE_IAP_APPLE);
	packet->putString(receiptData);
	send(packet);
}

void LobbyHandler::requestConfigShop(char type, int version)
{
	fr::OutPacket* packet = PacketsFactory::getOutPacket(CMD::CMD_CONFIG_SHOP);
	packet->putByte(type);
	packet->putInt(version);
	send(packet);
}

