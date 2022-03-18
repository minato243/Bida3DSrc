#include "ItemConstant.h"
#include "data/config/ConfigMgr.h"

using namespace std;

ItemConstant::ItemConstant()
{
	initItemPriority();
}

ItemConstant::~ItemConstant()
{
}

void ItemConstant::initItemPriority()
{
	for (int i = 0; i < MAX_ITEM; i++)
	{
		itemPriority[i] = 0;
	}
	itemPriority[ItemConstant::CASH] = 4;
	itemPriority[ItemConstant::GOLD] = 3;
	itemPriority[ItemConstant::CHALK] = 6;
	itemPriority[ItemConstant::GLOVE] = 5;
	itemPriority[ItemConstant::CHALLENGE_ITEM_SONG_TOAN] = 7;
	itemPriority[ItemConstant::CHALLENGE_ITEM_BOC_PHA] = 8;
	itemPriority[ItemConstant::CHALLENGE_ITEM_CAU_BONG] = 9;
	itemPriority[ItemConstant::CHALLENGE_ITEM_XOAN_OC] = 10;
	itemPriority[ItemConstant::CUE_UPGRADE_MAT_5] = 11;
	itemPriority[ItemConstant::CUE_UPGRADE_MAT_4] = 12;
	itemPriority[ItemConstant::CUE_UPGRADE_MAT_3] = 13;
	itemPriority[ItemConstant::CUE_UPGRADE_MAT_2] = 14;
	itemPriority[ItemConstant::CUE_UPGRADE_MAT_1] = 15;
}

int ItemConstant::getItemPriority(int itemId)
{
	int priority = itemConstant->itemPriority[itemId];
	if (priority == 0)
		return 100;
	return itemConstant->itemPriority[itemId];
}

void ItemConstant::generateItemMap()
{
	auto items = configMgr->typeCodecConfig->items;
	for (auto item = items.begin(); item != items.end(); item++)
	{
		itemMap.insert(pair<string, int>(item->first, item->second));
		reverseItemMap.insert(pair<int, string>(item->second, item->first));
	}
}

string ItemConstant::getItemCodeFromId(int id)
{
	if (itemConstant->reverseItemMap.find(id) == itemConstant->reverseItemMap.end())
		return "";
	return itemConstant->reverseItemMap.find(id)->second;
}

int ItemConstant::getIdFromItemCode(std::string itemCode)
{
	if(itemConstant->itemMap.find(itemCode) == itemConstant->itemMap.end())
		return 0;
	return itemConstant->itemMap.find(itemCode)->second;
}

const std::string & ItemConstant::getItemIcon(int id)
{
	switch (id) {
		case ItemConstant::CHALK: return ICON_CHALK;
		case ItemConstant::CASH: return ICON_CASH;
		case ItemConstant::GOLD: return ICON_GOLD;
		case ItemConstant::GLOVE: return ICON_GLOVE;
		case ItemConstant::COUPON: return ICON_COUPON;
		case ItemConstant::CUE_UPGRADE_MAT_1: return ICON_CUE_UPGRADE_MAT_1;
		case ItemConstant::CUE_UPGRADE_MAT_2: return ICON_CUE_UPGRADE_MAT_2;
		case ItemConstant::CUE_UPGRADE_MAT_3: return ICON_CUE_UPGRADE_MAT_3;
		case ItemConstant::CUE_UPGRADE_MAT_4: return ICON_CUE_UPGRADE_MAT_4;
		case ItemConstant::CUE_UPGRADE_MAT_5: return ICON_CUE_UPGRADE_MAT_5;
		case ItemConstant::END_GAME_CARD: return ICON_END_GAME_CARD;
	}
	return ICON_GOLD;
}

cocos2d::Color3B ItemConstant::getItemFontColor()
{
	return cocos2d::Color3B();
}

ItemConstant* ItemConstant::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new ItemConstant();
	}
	return _instance;
}

ItemConstant *ItemConstant::_instance = nullptr;

const std::string ItemConstant::ICON_CHALK = "Bleu_Icon.png";
const std::string ItemConstant::ICON_CASH = "Coupon_Normal.png";
const std::string ItemConstant::ICON_GOLD = "Gold.png";
const std::string ItemConstant::ICON_GLOVE = "Gloves_Icon.png";
const std::string ItemConstant::ICON_COUPON = "Coupon_Normal.png";
const std::string ItemConstant::ICON_CUE_UPGRADE_MAT_1 = "workshop_Green_icon.png";
const std::string ItemConstant::ICON_CUE_UPGRADE_MAT_2 = "workshop_Blue_icon.png";
const std::string ItemConstant::ICON_CUE_UPGRADE_MAT_3 = "workshop_Red_icon.png";
const std::string ItemConstant::ICON_CUE_UPGRADE_MAT_4 = "workshop_Purple_icon.png";
const std::string ItemConstant::ICON_CUE_UPGRADE_MAT_5 = "workshop_Yellow.png";
const std::string ItemConstant::ICON_END_GAME_CARD = "GameModeV2.Item_Card.png";
