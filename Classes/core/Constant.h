#ifndef Constant_h__
#define Constant_h__
#include <string>

static const int SOCIAL_FACEBOOK = 1;
static const int SOCIAL_GOOGLE = 2;
static const int SOCIAL_GUEST = 3;
static const int SOCIAL_APPLEID = 4;

const std::string CONSTANT_APP_VERSION_URL = "http://conquian-service.mto.zing.vn/versionV2.php";

const std::string CONSTANT_SMS_PRIVATE = "http://120.138.65.118/paymentIndo/all/out/testnew.php";

const std::string CONSTANT_IAP_PRIVATE = "http://120.138.65.118/paymentIndo/siete/out/BillingService.php";

const std::string CONSTANT_BRASIL_PORTAL_SERVICE_URL = "https://brazil-login.zingplay.com/?";
const std::string CONSTANT_GLOBAL_PORTAL_SERVICE_URL = "https://login-global.zingplay.com/?";

const std::string CONSTANT_HOT_NEW_API = "https://news-latam.service.zingplay.com/news?gameId=siete";;

const std::string CONSTANT_COUNTRY_DETECT_API = "https://sub.zingplay.com/mobile/openapi.php?&api=geoip2";;

const std::string CONSTANT_FACEBOOK_FANPAGE = "108712497689080";

const int CONSTANT_API_SERVICES_TIMEOUT = 5000;

const int CONSTANT_PLATFORM_IOS = 1;

const int CONSTANT_PLATFORM_ANDROID = 3;

const int CONSTANT_WALLET_LINE = 1;

const int CONSTANT_WALLET_TRUE = 2;

const int CONSTANT_WALLET_MPAY = 3;

const int CONSTANT_WIDTH = 1200;

const int CONSTANT_HEIGHT = 720;

static unsigned short cameraFlag = 2;
static unsigned short cameraDepth = 16;
#endif // Constant_h__
