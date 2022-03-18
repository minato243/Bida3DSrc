#include "ChannelMgr.h"
#include "../../../data/config/ConfigMgr.h"
#include "../inventory/ItemConstant.h"
#include <cocos2d.h>
#include "../inventory/Inventory.h"
#include "data/GameConstant.h"
#include "../inventory/InventoryMgr.h"

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

std::vector<ItemInfoData> ChannelMgr::getListItemWin(int gameMode, int channelId)
{
	vector<ItemInfoData> r;
	long long prize = configMgr->channelConfig->getChannelPrize(gameMode, channelId);
	ItemInfoData item = ItemInfoData(ItemConstant::GOLD, prize);
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

	return channelConfig.entryFee < quantity;
}

ChannelMgr * ChannelMgr::_intstance = NULL;

