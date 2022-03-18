#include "GameData.h"
#include "lobby/ChanelConfig.h"
#include "lobby/module/chat/ChatConfig.h"
#include "gui/SceneMgr.h"
#include "json/document.h"
#include "GameMgr.h"

GameConfig::GameConfig()
{
	versionShopGold = 0;
}

GameConfig::~GameConfig()
{

}

void GameConfig::setChanelConfig(MsgChannelConfig config)
{
	ChanelConfig::instance()->setConfig(config.channelConfig);
	setGiftOnlineConfig(config.playingGiftConfig);
	setGuestConfig(config.guestConfig);
}

int GameConfig::getCurrentChanel()
{
	return 0;
}

void GameConfig::setNewShopGoldConfig(std::string stringConfig)
{

	rapidjson::Document json;
	json.Parse<0>(stringConfig.c_str());
	// Print error
	if (json.HasParseError()) {
		size_t offset = json.GetErrorOffset();
		if (offset > 0)
			offset--;
		std::string errorSnippet = stringConfig.substr(offset, 10);
		CCLOG("GameConfig::setNewShopGoldConfig File parse error %d at <%s>\n", json.GetParseError(), errorSnippet.c_str());
	}

	versionShopGold = json["version"].GetInt();
	timeStartBonus = json["bonusStartDate"].GetString();
	timeEndBonus = json["bonusEndDate"].GetString();

	std::string keyChannel = "iosChannels";


	arrayShopGoldConfig.clear();
	const rapidjson::Value& channels = json[keyChannel.c_str()];
	for (rapidjson::SizeType i = 0; i < channels.Size(); ++i)
	{
		const rapidjson::Value& channel = channels[i];
		ChannelIAP channelIAP;
		channelIAP.id = channel["id"].GetInt();
		channelIAP.name = channel["name"].GetString();
		if (channelIAP.name.compare("IAP") != 0)
		{
			continue;
		}
		channelIAP.type = channel["type"].GetInt();
		channelIAP.priority = channel["priority"].GetInt();

		const rapidjson::Value& isMaintaineds = channel["isMaintained"];
		for (rapidjson::SizeType j = 0; j < isMaintaineds.Size(); ++j)
		{
			channelIAP.isMaintained.push_back(isMaintaineds[j].GetInt());
		}
		channelIAP.isShopBonus = channel["isShopBonus"].GetBool();

		const rapidjson::Value& packages = channel["packages"];
		for (rapidjson::SizeType j = 0; j < packages.Size(); ++j)
		{
			const rapidjson::Value& package = packages[j];
			ChannelIAPPackage pkg;
			pkg.id = package["id"].GetInt();
			pkg.value = package["value"].GetInt();
			pkg.coin = package["coin"].GetInt();
			pkg.br = package["br"].GetFloat();
			pkg.usd = package["usd"].GetFloat();
			pkg.requirePack = package["requirePack"].GetInt();
			pkg.maxBuyTime = package["maxBuyTime"].GetInt();
			pkg.timeCountDown = package["timeCountDown"].GetInt();
			const rapidjson::Value& vipBonus = package["vipBonus"];
			for (rapidjson::SizeType k = 0; k < vipBonus.Size(); ++k)
			{
				pkg.vipBonus.push_back(vipBonus[k].GetInt());
			}

			pkg.hourBonus = package["hourBonus"].GetInt();
			pkg.gold = package["gold"].GetInt64();
			pkg.shopBonus = package["shopBonus"].GetInt();
			pkg.firstBonus = package["firstBonus"].GetInt();
			pkg.gStar = package["gStar"].GetInt();
			pkg.portalAndroidId = package["portalAndroidId"].GetString();

			const rapidjson::Value& packIds = package["packIds"];
			for (rapidjson::SizeType k = 0; k < packIds.Size(); ++k)
			{
				pkg.packIds.push_back(packIds[k].GetString());
			}


			channelIAP.packages.push_back(pkg);
			channelIAP.numPackage = channelIAP.packages.size();
		}
		if (gamedata->payments[channelIAP.id]) {
			arrayShopGoldConfig.push_back(channelIAP);
		}
	}
}

void GameConfig::setNewShopGConfig(std::string stringConfig)
{

}

int GameConfig::getIsFirstGold(long long value, int id)
{
	int idType = -1;
	for (int i = 0; i < arrayTypeFirst.size(); i++) {
		if (arrayTypeFirst[i] == id) {
			idType = i;
			break;
		}
	}
	if (idType == -1) 
		return 0;
	for (int i = 0; i < arrayValueShop[idType].size(); i++) {
		if (arrayValueShop[idType][i] == value)
			return arrayIsFirstShop[idType][i];
	}
	return 0;
}

ChannelIAP GameConfig::getShopGoldById(int id)
{
	if (arrayShopGoldConfig.empty()) 
		gamedata->parseShopConfig();

	for (int i = 0; i < arrayShopGoldConfig.size(); i++) {
		if (arrayShopGoldConfig[i].id == id)
			return arrayShopGoldConfig[i];
	}
	ChannelIAP iap;
	iap.id = -1;
	return iap;
}

long long GameConfig::convertGStar(long long gold)
{
	return 0;
}

void GameConfig::setVipConfig(std::string configVip)
{

}

void GameConfig::setVipBenefit(std::string vipBenefit)
{

}

void GameConfig::setVipEmotion(std::string vipEmotion)
{
	chatConfig->setChatConfig(vipEmotion);
}

void GameConfig::setOldVipConfig(std::string vipOld)
{

}

void GameConfig::setFriendConfig(std::string friendConfig)
{

}

void GameConfig::setGiftOnlineConfig(std::string giftOnlineConfig)
{

}

void GameConfig::setGuestConfig(std::string guestConfig)
{

}

GameData::GameData()
{
	gameConfig = nullptr;
	userData = {};

	source = "";
	forum = "https://www.facebook.com/zingplaybrasil";
	isAppSupport = 1;
	support = "vng.cs.td.hotro";
	supporturl = "http://hotro.zing.vn";
	supportphone = "";
	autologin = 3;
	payments.clear();

	storeUrl = "";

	voteAppEnable = false;

	// shop + quest
	isShopBonus = false;
	isShopIAPBonus = false;
	giftIndex = -1;

	// suport
	numSupport = -1;
	timeSupport = -1;
	showSupportTime = false;

	// player info in game
	selectedChanel = -1;
	jackpot = 0;

	// app version
	appVersion = "";

	// setting
	sound = true;
	music = true;
	soundhuman = false;
	soundhumansusun = false;
	vibrate = true;
	acceptInvite = true;
	acceptFriend = true;
	assistance = false;
	completeTooltip = false;
	loggedFacebook = false;
	wasShowPopupSurvey = false;
	logged = false;
	timeSendGetListGiftCode = 0;
	// dem so van choi trong mot session, de show Rate Game
	countGame = 0;

	//language
	language = "EN";

	// country
	canRateApp = false;
	countGame = 0;
	countryID = "";
	waitReconnectBoard = false;
	cmdShopConfig.type = -1;
}

GameData::~GameData()
{

}

GameData* GameData::getInstance()
{
	if (!_instance) {
		_instance = new GameData();
	}
	return _instance;
}

int GameData::getTimeLimitLeft(int idx)
{
	return 0;
}

int GameData::getTimeLimitWebviewLeft(int idx)
{
	return 0;
}

std::string GameData::getTimeLimitString(int idx)
{
	return "";
}

std::string GameData::getDisplayName()
{
	return "";
}

long long GameData::getUserGold()
{
	return 0;
}

long long GameData::getUserId()
{
	return 0;
}

std::string GameData::getUserAvatar()
{
	return "";
}

void GameData::setUserInfo(MsgUserInfo info)
{
	userData.avatar = info.avatar;
	userData.displayName = info.userName;
	userData.zName = info.zName;

	userData.name = userData.displayName;
	userData.bean = info.gold;
	userData.coin = info.zMoney;
	userData.levelScore = info.levelScore;
	userData.winCount = info.winCount;
	userData.lostCount = info.lostCount;
	userData.star = 0;
	userData.uID = info.uId;
	//userData.listGiftCode = [];

	isShopBonus = false;
	isShopIAPBonus = false;
	userData.openID = openID;
	
	parseShopConfig();
}

void GameData::updateMoney(MsgUpdateBean update)
{
	if (update.uID == userData.uID) {
		userData.bean = update.bean;
		userData.coin = update.coin;
		userData.winCount = update.winCount;
		userData.lostCount = update.lostCount;
		userData.levelScore = update.levelScore;
	}
}

void GameData::saveConfigPayment(std::string paymentConfig)
{
}

void GameData::saveConfigShop(MsgShopConfig configShop)
{
	cmdShopConfig = configShop;
}

void GameData::parseShopConfig()
{
	if (!cmdShopConfig.type) return;

	//cc.log("PACKEG " + JSON.stringify(cmdGetConfigShop));
	if (cmdShopConfig.type == 1 || cmdShopConfig.type == 3) {
		gamedata->gameConfig->setNewShopGoldConfig(cmdShopConfig.stringConfigGold);
		sceneMgr->updateCurrentGUI();
	}
	else {
		throw "";
	}
}

void GameData::setChannelConfig(MsgChannelConfig config)
{
	gameConfig = new GameConfig();
	gameConfig->setChanelConfig(config);
}

void GameData::getEventBonusTicket()
{

}

GameData* GameData::_instance = nullptr;

bool GameData::IS_RECEIVE_START_UP = true;

bool GameData::IS_RECEIVE_SHOW_TUT_PACKAGE = true;
bool GameData::IS_FIRST_LOGIN_AFTER_OPEN_APP = false;
