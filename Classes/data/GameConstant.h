#ifndef GLOBALVAR_H_
#define GLOBALVAR_H_

#include <cocos2d.h>
#include <string>

namespace gv {
	extern const double ALMOST_REALTIME_UPDATE_DELTA;
	extern bool ENABLE_SHOW_DEBUG_NOTIFY;
	extern cocos2d::Color3B ANNOUCEMENT_TEXT_COLOR;
	extern bool ENABLE_GD_PTB_TEST;
};

#define NUM_BALL 15
#define CUSTOM_CALLBACK std::function<void()>
#define cto_string(__type__) std::to_string(__type__).c_str()

#define Infinity DBL_MAX
#define NUM_CUES_FUSION 5
#define TAG_COMMON 1001
#define FAKE_CLIENT_RESULT false
#define NUM_POCKET 6
#define DEFAULT_CUE_BALL_ID 0


class GameConstant {
public:
	GameConstant() {};

public:
	const static int TURN_TIME = 35;
	const static int SHOOT_TIME = 35;
	const static int CUE_IN_BAG = 3;
};

extern const std::string SHOP_CATEGORY_GOLD; 
extern const std::string SHOP_CATEGORY_CASH;
extern const std::string SHOP_CATEGORY_CUE;

enum class ShopCategory {
	NONE = -1,
	CUE = 0,
	GOLD = 1,
	CASH = 2,
	LIMITED = 3
};

enum class ShopItemType {
	NONE = -1,
	CUE = 0,
	GOLD = 1,
	CASH = 2
};

enum ShopTabId 
{
	CUE = 0,
	GOLD = 1,
	CASH = 2,
	HOT_DEALS = 3
};

enum CueType
{
	NORMAL	= 1, 
	BREAK	= 2,
	JUMP	= 3, 
	MASSE	= 4
};

enum class SelectPocketState {
	CANNOT_SELECT = 0,
	SELECTING = 1,
	AFTER = 2
};

enum class PlaceCueBallState {
	CANNOT_PLACE = 0,
	PLACING = 1,
	AFTER_PLACE = 2
};

enum GameMode
{
	NO_MODE = 0,
	QUICK_MODE = 1,
	STREAK_MODE = 2,
	RT2_MODE = 3,
	RT3_MODE = 4,
	FRIEND_MODE = 5,
	PRACTISE_MODE = 6
};
const GameMode RT1 = QUICK_MODE;

enum Rule
{
	BALL_8_RULE = 8,
	BALL_9_RULE = 9
};


#endif // !GLOBALVAR_H_