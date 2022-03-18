#ifndef Config_h__
#define Config_h__
#include <string>
#include <vector>

class Config {
public:
	static int APP_VERSION_ANDROID;
	static int APP_VERSION_IOS;

	static std::string DEVICE_ID_W32;

	static bool ENABLE_NOTIFICATION;

	static std::string SERVER_PRIVATE;
	static int PORT;

	static std::string SERVER_PRIVATE2;
	static int PORT2;

	static std::string SERVER_LOCAL;
	static int PORT_LOCAL;

	static std::string SERVER_LIVE;
	static int PORT_LIVE;

	static bool DEV_LOCAL;
	static bool ENABLE_CHEAT;

	static bool PRELOAD_LAYER;

	static bool ENABLE_EVENT;
	static bool ENABLE_HOT_DEAL;

	static int TIMEOUT_CONNECT_SERVER;

	static bool ENABLE_IAP_LIMIT_TIME;           // bat IAP moi , han che thoi gian mua goi

	static bool ENABLE_TOOL_TIP;

	static int CHEAT_MAX_PLAYER;
	static int CHEAT_MAX_CARD;
	static float CARD_CHEAT_SCALE_DECK;
	static float CARD_CHEAT_SCALE_PLAYER;
	static int CARD_CHEAT_PLAYER_LINE;

	static int APP_VERSION_ANDROID_DEFAULT;
	static int APP_VERSION_IOS_DEFAULT;

	static std::vector<int> EMO_HAPPY;        // id emo happy, use for Chiu
	static std::vector<int> EMO_SAD;   // id emo sad
	static std::vector<int> EMO_WIN;// id emo win

	static int TOTAL_GAME_RATE_APP;
	static int TOTAL_GAME_RATE_APP_MIN;
	static int SESSION_GAME_RATE_APP;

	static std::string MCC_MEXICO;
	static std::string MCC_VIETNAM;

	static std::string COUNTRY_MX;   // mexico
	static std::string COUNTRY_GLOBAL;   // global

};
#endif // Config_h__
