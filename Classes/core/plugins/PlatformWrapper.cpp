#include "PlatformWrapper.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "plugin/protocols/include/PluginManager.h"
using namespace cocos2d::plugin;

PluginProtocol* nativeServicePlugin = nullptr;
#endif
#include "../utils/LanguageMgr.h"
USING_NS_CC;
NS_FR_BEGIN
void platformWrapper::init()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	nativeServicePlugin = PluginManager::getInstance()->loadPlugin("PlatformWrapper");
#endif
}

std::string platformWrapper::getMailAccount()
{
	return callStringFunc("getMailAccount");
}

std::string platformWrapper::getDeviceModel()
{
	return callStringFunc("getDeviceModel");
}


int platformWrapper::getVersionCode()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	return nativeServicePlugin->callIntFuncWithParam("getVersionCode", nullptr);
#endif

	return -1;
}

std::string platformWrapper::getOSVersion()
{
	return callStringFunc("getOSVersion");
}

int platformWrapper::getConnectionStatus()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	return nativeServicePlugin->callIntFuncWithParam("getConnectionStatus", nullptr);
#endif
	return -1;
}

bool platformWrapper::hasNetwork()
{
	if (getConnectionStatus() == CONNECTION_STATUS::NO_NETWORK) {
		return false;
	}
	else return true;
}

std::string platformWrapper::getConnectionStatusName()
{
	int connectionType = getConnectionStatus();
	switch (connectionType) {
	case 0:
		return "unknown";
	case 1:
		return "3g";
	case 2:
		return "wifi";
	}
	return "";
}

std::string platformWrapper::getOsName()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	return "Android";
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	return "IOS";
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	return "Win32";
#endif
}

std::string platformWrapper::getClientVersion()
{
	return callStringFunc("getAppVersion");
}

std::string platformWrapper::getExternalDataPath()
{
	std::string path = callStringFunc("getExternalDataPath");
	if (path.empty()) {
		path = FileUtils::getInstance()->getWritablePath();
	}
	return path;
}

std::string platformWrapper::getDeviceID()
{
	std::string deviceID = callStringFunc("getDeviceID");
	return deviceID;
}

std::string platformWrapper::getAAID()
{
	std::string aaid = callStringFunc("getAAID");
	return aaid;
}

void platformWrapper::trackLoginGSN(std::string acountID, std::string acountType, std::string openID, std::string zName)
{
	std::string str = "{";
	str += "\"accountId\" : \"" + acountID + "\",";
	str += "\"accountType\" : \"" + acountType + "\",";
	str += "\"openAccount\" : \"" + openID + "\",";
	str += "\"zingName\" : \"" + zName + "\"";
	str += "}";

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	PluginParam _list(str.c_str());
	nativeServicePlugin->callFuncWithParam("trackLoginGSN", &_list, NULL);
#endif
}


std::string platformWrapper::getNetworkOperator()
{
	return "";
}

std::string platformWrapper::getPackageName()
{
	std::string pkg = callStringFunc("getPackageName");
	if (pkg.empty()) {
		return LocalizedString::config("DEFAULT_PACKAGE_NAME");
	}
	else {
		return pkg;
	}
}

bool platformWrapper::isAndroid()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	return true;
#else
	return false;
#endif
}

bool platformWrapper::isIOs()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	return true;
#else
	return false;
#endif
}

std::string platformWrapper::getVersionName()
{
	return callStringFunc("getAppVersion");
}

std::string platformWrapper::getGameVersion()
{
	return callStringFunc("getGameVersion");
}

std::string platformWrapper::getPhoneNumber()
{
	return callStringFunc("getPhoneNumber");
}

std::string fr::platformWrapper::getRefer()
{
	return callStringFunc("getRefer");
}

std::string platformWrapper::callStringFunc(std::string functionName)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	return nativeServicePlugin->callStringFuncWithParam(functionName.c_str(), NULL);
#else
	return "";
#endif
}
NS_FR_END