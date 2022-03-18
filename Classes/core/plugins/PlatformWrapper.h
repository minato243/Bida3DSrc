#ifndef PlatformWrapper_h__
#define PlatformWrapper_h__
#include "BaseFramework.h"
NS_FR_BEGIN
enum SIM_STATE {
	UNKNOWN = 0,
	ABSENT = 1,
	PIN_REQUIRED = 2,
	PUK_REQUIRED = 3,
	NETWORK_LOCKED = 4,
	READY = 5
};

enum CONNECTION_STATUS {
	NO_NETWORK = 0,
	CELULAR_DATA = 1,
	WIFI = 2
};

class platformWrapper
{
public:
	static void init();
	static std::string getPhoneNumber();
	static std::string getMailAccount();
	static std::string getDeviceModel();
	static int getVersionCode();
	static std::string getOSVersion();
	static int getConnectionStatus();
	static bool hasNetwork();
	static std::string getConnectionStatusName();
	static std::string getOsName();
	static std::string getClientVersion();
	static std::string getExternalDataPath();
	static std::string getDeviceID();
	static std::string getAAID();
	static void trackLoginGSN(std::string acountID, std::string acountType, std::string openID, std::string zName);
	static std::string getNetworkOperator();
	static std::string getPackageName();
	static bool isAndroid();
	static bool isIOs();
	static std::string getVersionName();
	static std::string getGameVersion();
	static std::string getRefer();
	static std::string callStringFunc(std::string functionName);
	
};
NS_FR_END
#endif // PlatformWrapper_h__
