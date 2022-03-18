#ifndef AppleIAP_h__
#define AppleIAP_h__
#include "BaseFramework.h"
#include <vector>
#include <string>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "plugin/protocols/include/ProtocolIAP.h"
class MyPurchaseResult : public cocos2d::plugin::PayResultListener
{
public:
	virtual void onPayResult(cocos2d::plugin::PayResultCode ret, const char* msg, cocos2d::plugin::TProductInfo info);
};

#endif
NS_FR_BEGIN
class appleIAP {
public:
	static void init();	 
	static void requestProducts();
	static void requestPayProduct(std::vector<std::string> ids);
	static std::string getProductPrice(std::vector<std::string> productKeys);
	static std::string getProductID(std::vector<std::string> ids);
	static std::string getProductCurrency(std::vector<std::string> ids);
	static void finishTransactions(std::vector<std::string> ids);
};
NS_FR_END
#endif // AppleIAP_h__
