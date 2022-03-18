#ifndef ItemConstant_h_
#define ItemConstant_h_

#include <string>
#include <cocos2d.h>

#define itemConstant ItemConstant::getInstance()

class ItemConstant
{
public:
	enum class CueType
	{
		SONG_TOAN = 1,
		BOC_PHA = 2,
		CAU_BONG = 3,
		XOAN_OC = 4,
	};

	ItemConstant();
	~ItemConstant();

	void initItemPriority();

	static int getItemPriority(int itemId);

	void generateItemMap();
	
	static std::string getItemCodeFromId(int id);

	static int getIdFromItemCode(std::string itemCode);

	static const std::string & getItemIcon(int itemId);

	static cocos2d::Color3B getItemFontColor();

	static ItemConstant* getInstance();

private:
	static ItemConstant *_instance;
	std::map<std::string, int> itemMap;
	std::map<int, std::string> reverseItemMap;

public:	
	static const int GOLD = 1;
	static const int CUE_BOX_BRONZE = 2;
	static const int CUE_BOX_SILVER = 3;
	static const int CUE_BOX_GOLD = 4;
	static const int COUPON = 5;

	static const int DUST_SONG_TOAN = 6;
	static const int DUST_BOC_PHA = 7;
	static const int DUST_CAU_BONG = 8;
	static const int DUST_XOAN_OC = 9;

	static const int CUE_BOX_SONG_TOAN = 10;
	static const int CUE_BOX_BOC_PHA = 11;
	static const int CUE_BOX_CAU_BONG = 12;
	static const int CUE_BOX_XOAN_OC = 13;

	static const int CHALK = 14;
	static const int GLOVE = 15;

	static const int CHALLENGE_ITEM_SONG_TOAN = 16;
	static const int CHALLENGE_ITEM_BOC_PHA = 17;
	static const int CHALLENGE_ITEM_CAU_BONG = 18;
	static const int CHALLENGE_ITEM_XOAN_OC = 19;

	static const int CUE_UPGRADE_MAT_1 = 20;
	static const int CUE_UPGRADE_MAT_2 = 21;
	static const int CUE_UPGRADE_MAT_3 = 22;
	static const int CUE_UPGRADE_MAT_4 = 23;
	static const int CUE_UPGRADE_MAT_5 = 24;

	static const int CASH = 50;
	static const int END_GAME_CARD = 51;

	static const int MAX_ITEM = 100;

	int itemPriority[MAX_ITEM];

    static const std::string ICON_CHALK;
	static const std::string ICON_CASH;
	static const std::string ICON_GOLD;
	static const std::string ICON_GLOVE;
	static const std::string ICON_COUPON;
	static const std::string ICON_CUE_UPGRADE_MAT_1;
	static const std::string ICON_CUE_UPGRADE_MAT_2;
	static const std::string ICON_CUE_UPGRADE_MAT_3;
	static const std::string ICON_CUE_UPGRADE_MAT_4;
	static const std::string ICON_CUE_UPGRADE_MAT_5;
	static const std::string ICON_END_GAME_CARD;

};
#endif // !ItemConstant_h_



