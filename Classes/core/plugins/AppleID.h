#ifndef AppleID_h__
#define AppleID_h__

#include "BaseFramework.h"
#include <vector>
#include <string>

NS_FR_BEGIN
typedef std::function<void(bool, std::string&)> AppleIDLoginCallback;
class AppleID
{
public:
	static void init();
	static void login(const AppleIDLoginCallback& callback);
	static void logout();
};
NS_FR_END


#endif // AppleID_h__
