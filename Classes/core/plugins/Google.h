#ifndef Google_h__
#define Google_h__

#include "BaseFramework.h"
#include <vector>
#include <string>

NS_FR_BEGIN
typedef std::function<void(bool, std::string&)> GoogleLoginCallback;
class Google
{
public:
	static void init();
	static void login(const GoogleLoginCallback& callback);
	static void logout();
	static void _requestLogin(const GoogleLoginCallback& callback);
};
NS_FR_END
#endif // Google_h__
