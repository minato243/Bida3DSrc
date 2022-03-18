#ifndef Facebook_h__
#define Facebook_h__

#include "BaseFramework.h"
#include <vector>
#include <string>

NS_FR_BEGIN
typedef std::function<void(bool, std::string&)> FacebookLoginCallback;
class Facebook
{
public:
	static void init();
	static void login(const FacebookLoginCallback& callback);
	static void logout();
	static void _requestLogin(const FacebookLoginCallback& callback);
};
NS_FR_END

#endif // Facebook_h__
