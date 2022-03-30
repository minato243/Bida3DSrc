#include "ChannelMgr.h"
#include "../../../data/config/ConfigMgr.h"
#include "../inventory/ItemConstant.h"
#include <cocos2d.h>
#include "../inventory/Inventory.h"
#include "data/GameConstant.h"
#include "../inventory/InventoryMgr.h"
#include "data/config/GloveConfig.h"

using namespace std;

ChannelMgr::ChannelMgr()
{

}

ChannelMgr::~ChannelMgr()
{

}

ChannelMgr * ChannelMgr::getInstance()
{
	if (_intstance == NULL)
	{
		_intstance = new ChannelMgr();
	}
	return _intstance;
}

void ChannelMgr::destroyInstance()
{
	CC_SAFE_DELETE(_intstance);
}

std::vector<ItemInfoData> ChannelMgr::getListItemWin(GameMode gameMode, int channelId)
{
	vector<ItemInfoData> r;
	long long prize = configMgr->channelConfig->getChannelPrize(gameMode, channelId);
	ItemInfoData item = ItemInfoData(ItemConstant::GOLD, prize);
	r.push_back(item);

	return r;
}

std::vector<ItemInfoData> ChannelMgr::getListItemFee(GameMode gameMode, int channelId)
{
	vector<ItemInfoData> r;
	ChannelConfigData data = configMgr->channelConfig->getChannelConfig(gameMode, channelId);
	ItemInfoData item = ItemInfoData(ItemConstant::GOLD, data.entryFee);
	r.push_back(item);

	return r;
}

ItemInfoData * ChannelMgr::getListFee(int gameMode, int channelId)
{
	auto item = new ItemInfoData();
	auto channelConfig = configMgr->channelConfig->getChannelConfig(gameMode, channelId);
	item->num = channelConfig.entryFee;
	item->type = Inventory::getTypeFromConfig(channelConfig.entryFeeUnit);
	return item;
}

bool ChannelMgr::joinChannel(GameMode gameMode, int channelId)
{
	auto channelConfig = configMgr->channelConfig->getChannelConfig(gameMode, channelId);

	int type = Inventory::getTypeFromConfig(channelConfig.entryFeeUnit);
	long long quantity= inventoryMgr->getInventory()->getItemQuantity(type);

	return channelConfig.entryFee <= quantity;
}

MatchingGloveConfigData ChannelMgr::getGloveMatchingConfigByCurrentGlove(int glove)
{
	int currentGlove = inventoryMgr->getInventory()->getItemQuantity(ItemConstant::GLOVE);
	MatchingGloveConfigData data = configMgr->gloveConfig->getMatchingGloveConfig(currentGlove);

	return data;
}

int ChannelMgr::getGloveFeeByCurrentGlove()
{
	int currentGlove = inventoryMgr->getInventory()->getItemQuantity(ItemConstant::GLOVE);
	MatchingGloveConfigData data = configMgr->gloveConfig->getMatchingGloveConfig(currentGlove);

	return data.require;
}

int ChannelMgr::getGlovePrizeByCurrentGlove()
{
	int currentGlove = inventoryMgr->getInventory()->getItemQuantity(ItemConstant::GLOVE);
	MatchingGloveConfigData data = configMgr->gloveConfig->getMatchingGloveConfig(currentGlove);

	return data.prize;
}

ChannelMgr * ChannelMgr::_intstance = NULL;

