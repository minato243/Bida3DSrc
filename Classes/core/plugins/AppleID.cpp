#include "AppleID.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "plugin/protocols/include/PluginManager.h"
#include "plugin/protocols/include/ProtocolUser.h"
using namespace cocos2d::plugin;
ProtocolUser* pluginAppleID = nullptr;
#endif

void fr::AppleID::init()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (pluginAppleID)
		return;
	pluginAppleID = dynamic_cast<ProtocolUser*>(PluginManager::getInstance()->loadPlugin("AppleID"));
#endif
}

void fr::AppleID::login(const AppleIDLoginCallback& callback)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	ProtocolUser::ProtocolUserCallback cb = [callback](int result, std::string& response) {
		if (result == UserActionResultCode::kLogoutSucceed) {
			return;
		}
		if (result == UserActionResultCode::kLoginSucceed) {
			callback(true, response);
		}
		else {
			std::string msg = response;
			callback(false, msg);
		}
	};

	pluginAppleID->login(cb);
#endif
}

void fr::AppleID::logout()
{

}
