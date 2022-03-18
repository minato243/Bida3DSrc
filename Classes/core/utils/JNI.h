#ifndef JNI_h__
#define JNI_h__
#include <string>
#include <vector>

class JNI {
public:
	static std::string getRefer();
	static std::string getDeviceID();
	static std::string getDeviceModel();
	static std::string getOsVersion();
	static bool getNetworkStatus();
	static void openFanpage(std::string pageId);
	static void openURL(std::string url);
	static void sendLoginGSN(std::string acountID, std::string acountType, std::string openID, std::string zName);
	static std::string getVersionString();
	static int getVersionCode();
	static std::string getNetworkOperator();
	static std::string getGameResource();
	static bool isEmulator();
	static void openIAP();
	static void purchaseItem(std::vector<std::string> itemIds);
	static void purchaseItemSuccess(std::vector<std::string> itemIds);
};
#endif // JNI_h__
