#include "Google.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "plugin/protocols/include/PluginManager.h"
#include "plugin/protocols/include/ProtocolUser.h"
using namespace cocos2d::plugin;
ProtocolUser* pluginGGUser = nullptr;
#endif
NS_FR_BEGIN
void fr::Google::init()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (pluginGGUser)
		return;
	TUserDeveloperInfo pInfo;
    pInfo["social"] = "gg";
    pluginGGUser = dynamic_cast<ProtocolUser*>(PluginManager::getInstance()->loadPlugin("UserGoogle"));
    pluginGGUser->configDeveloperInfo(pInfo);
#endif
}

void fr::Google::login(const GoogleLoginCallback& callback)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (pluginGGUser->isLoggedIn()) {
        ProtocolUser::ProtocolUserCallback cb = [callback](int result, std::string& response) {
            fr::Google::_requestLogin(callback);
        };
        pluginGGUser->logout(cb);
		
	}
	else {
		_requestLogin(callback);
	}
#endif
}

void fr::Google::logout()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (pluginGGUser && pluginGGUser->isLoggedIn()) {
        pluginGGUser->logout();
	}
#endif
}

void fr::Google::_requestLogin(const GoogleLoginCallback& callback)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ProtocolUser::ProtocolUserCallback cb = [callback](int result, std::string& response) {
        if (result == UserActionResultCode::kLogoutSucceed) {
            return;
        }
        if (result == UserActionResultCode::kLoginSucceed) {
            std::string token = response;
            callback(true, token);
        }
        else {
            std::string msg = response;
            callback(false, msg);
        }
    };
    
    pluginGGUser->login(cb);
#endif
}
NS_FR_END
