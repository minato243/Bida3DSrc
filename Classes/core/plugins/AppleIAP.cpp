#include "AppleIAP.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "plugin/protocols/include/PluginManager.h"
using namespace cocos2d::plugin;
MyPurchaseResult* s_pRetListener;
ProtocolIAP* appleIAPPlugin = nullptr;
#endif
#include <map>
static std::map<std::string, std::string> mapIAP= {
	{"1A","siete.pack1"},
	{"2A","siete.pack2"},
	{"5A","siete.pack5"},
	{"10A","siete.pack10"},
	{"20A","siete.pack20"},
	{"50A","siete.pack50"}
};

NS_FR_BEGIN
void appleIAP::init()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (appleIAPPlugin)
		return;
	s_pRetListener = new MyPurchaseResult();
	TIAPDeveloperInfo pInfo;
	appleIAPPlugin = dynamic_cast<ProtocolIAP*>(PluginManager::getInstance()->loadPlugin("IOSIAP"));
	appleIAPPlugin->configDeveloperInfo(pInfo);
	appleIAPPlugin->setResultListener(s_pRetListener);
	requestProducts();
#endif
}

void fr::appleIAP::requestProducts()
{
	std::string list;
	std::string productId = "";
	for (std::map<std::string, std::string>::iterator iter = mapIAP.begin(); iter != mapIAP.end(); iter++) {
		productId = iter->second;
		list = cocos2d::StringUtils::format("%s%s,", list.c_str(), productId.c_str());
	}
	list = list.substr(0, list.size() - 1);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	PluginParam _list(list.c_str());
	appleIAPPlugin->callFuncWithParam("requestProducts", &_list, NULL);
#endif

}

std::string fr::appleIAP::getProductCurrency(std::vector<std::string> ids)
{
	std::string productId = getProductID(ids);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	PluginParam _productId(productId.c_str());
	return appleIAPPlugin->callStringFuncWithParam("getProductCurrencyById", &_productId, NULL);
#else
	return "";
#endif

}

std::string fr::appleIAP::getProductID(std::vector<std::string> ids)
{
	std::string productId = "";
	for (std::string s : ids) {
		std::map<std::string, std::string>::iterator itr = mapIAP.find(s);
		if (itr != mapIAP.end()) {
			productId = (itr->second).c_str();
		}
		if (productId.compare(s) == 0) 
			break;
	}
	cocos2d::log("appleIAP::getProductID available %s", productId.c_str());
	return productId;
}

std::string fr::appleIAP::getProductPrice(std::vector<std::string> productKeys)
{
	std::string productId = getProductID(productKeys);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	PluginParam _productId(productId.c_str());
	return appleIAPPlugin->callStringFuncWithParam("getProductLocalCurrencyById", &_productId, NULL);
#else
	return "";
#endif
}
void appleIAP::requestPayProduct(std::vector<std::string> ids)
{
	std::string productId = getProductID(ids);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	TProductInfo info;
    info["productId"] = productId;
    appleIAPPlugin->payForProduct(info);
#else
	//iapHandler->onIAPPurchase(1, "");
#endif

}
NS_FR_END


void fr::appleIAP::finishTransactions(std::vector<std::string> ids)
{
	//std::string productId = getProductID(ids);
    for(std::string productId:ids){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	PluginParam _productId(productId.c_str());
	appleIAPPlugin->callFuncWithParam("finishTransaction", &_productId, NULL);
#endif
    }
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void MyPurchaseResult::onPayResult(PayResultCode ret, const char* msg, cocos2d::plugin::TProductInfo info)
{
	//iapHandler->onIAPPurchase(ret, msg);
}
#endif

