#include "Facebook.h"
#include "json/document.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "plugin/protocols/include/PluginManager.h"
#include "plugin/protocols/include/ProtocolUser.h"
using namespace cocos2d::plugin;
ProtocolUser* pluginFBUser = nullptr;
#endif

void fr::Facebook::init()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (pluginFBUser)
		return;
    pluginFBUser = dynamic_cast<ProtocolUser*>(PluginManager::getInstance()->loadPlugin("UserFacebook"));
#endif
}

void fr::Facebook::login(const FacebookLoginCallback& callback)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (pluginFBUser->isLoggedIn()) {
        ProtocolUser::ProtocolUserCallback cb = [callback](int result, std::string& response) {
            fr::Facebook::_requestLogin(callback);
        };
        pluginFBUser->logout(cb);
	}
	else {
		_requestLogin(callback);
	}
#endif
}

void fr::Facebook::logout()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (pluginFBUser && pluginFBUser->isLoggedIn()) {
        pluginFBUser->logout();
	}
#endif
}

void fr::Facebook::_requestLogin(const FacebookLoginCallback& callback)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ProtocolUser::ProtocolUserCallback cb = [callback](int result, std::string& response) {
        if (result == UserActionResultCode::kLogoutSucceed) {
            return;
        }
        if (result == UserActionResultCode::kLoginSucceed) {
            rapidjson::Document doc;
            doc.Parse<0>(response.c_str());
            if (!doc.HasParseError()) {
                if (doc.HasMember("accessToken") && doc["accessToken"].IsString())
                {
                    std::string token = doc["accessToken"].GetString();
                    callback(true, token);
                    return;
                }
            }
            std::string msg = "Parse response Error!";
            callback(false, msg);
        }
        else {
            std::string msg = response;
            callback(false, msg);
        }
    };
    
	pluginFBUser->login(cb);
#endif
}

