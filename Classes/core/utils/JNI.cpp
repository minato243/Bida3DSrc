#include "JNI.h"
#include "cocos2d.h"
#include "../GameData.h"
#include "../plugins/PlatformWrapper.h"
#include "../GameMgr.h"
#include "../plugins/AppleIAP.h"
USING_NS_CC;

std::string JNI::getRefer()
{
	std::string refer = fr::platformWrapper::getRefer();
	return refer;
}

std::string JNI::getDeviceID()
{
	std::string id = fr::platformWrapper::getDeviceID();
	return id;
}

std::string JNI::getDeviceModel()
{
	std::string id = fr::platformWrapper::getDeviceModel();
	return id;
}

std::string JNI::getOsVersion()
{
	std::string id = fr::platformWrapper::getOSVersion();
	return id;
}

bool JNI::getNetworkStatus()
{
	bool status = (fr::platformWrapper::getConnectionStatus() != fr::CONNECTION_STATUS::NO_NETWORK);
	return status;
}

void JNI::openFanpage(std::string pageId)
{
	std::string fbWebUrl = cocos2d::StringUtils::format("https://www.facebook.com/%s", pageId.c_str());
	std::string fbAppUrl = "fb://";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	fbAppUrl += "page/";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	fbAppUrl += "profile/";
#endif
	fbAppUrl += pageId;

	if (!Application::getInstance()->openURL(fbAppUrl)) {
		Application::getInstance()->openURL(fbWebUrl);
	}
}

void JNI::openURL(std::string url)
{
	Application::getInstance()->openURL(url);
}

void JNI::sendLoginGSN(std::string acountID, std::string acountType, std::string openID, std::string zName)
{
	fr::platformWrapper::trackLoginGSN(acountID, acountType, openID, zName);
}

std::string JNI::getVersionString()
{
	std::string ret = StringUtils::format("v%s.%s.%d.%s.0", 
		gameMgr->isGlobal() ? "1" : "0", 
		fr::platformWrapper::getVersionName().c_str(),
		fr::platformWrapper::getVersionCode(),
		gamedata->appVersion.c_str()
		);
	return ret;
}

int JNI::getVersionCode()
{
	int version = fr::platformWrapper::getVersionCode();
	return version;
}

std::string JNI::getNetworkOperator()
{
	return fr::platformWrapper::getNetworkOperator();
}

std::string JNI::getGameResource()
{
	std::string ret = "normal";
	return ret;
}

bool JNI::isEmulator()
{
	return false;
}

void JNI::openIAP()
{
	fr::appleIAP::init();
}

void JNI::purchaseItem(std::vector<std::string> itemIds)
{
	fr::appleIAP::requestPayProduct(itemIds);
}

void JNI::purchaseItemSuccess(std::vector<std::string> itemIds)
{
	fr::appleIAP::finishTransactions(itemIds);
}

