#include "ShopMgr.h"
#include "../../../data/config/ConfigMgr.h"
#include "../../network/payment/PaymentHandler.h"
#include "../../ui/GuiMgr.h"
#include "../../ui/shop/ShopPopupResult.h"
#include "core/GameMgr.h"
#include "ShopItemData.h"
#include "../../ui/gacha/CueGachaRollingList.h"

using namespace std;

ShopMgr::ShopMgr()
{
	init();
}

ShopMgr::~ShopMgr()
{

}

void ShopMgr::init()
{
	initWithData(configMgr->shopConfig);
	initCueBoxData(configMgr->cueConfig->getCueBoxConfigList());
}

void ShopMgr::initCueBoxData(const std::vector<CueBoxConfigData*> &cueBoxList)
{
	for (int i = 0; i < cueBoxList.size(); i++)
	{
		auto item = new ShopCueBoxData();
		item->initWithData(*cueBoxList[i]);
		if (item->category != ShopCategory::NONE)
		{
			if (_tabsData.find(item->category) == _tabsData.end())
			{
				std::vector<ShopItemData*> itemList;
				_tabsData.insert(pair<ShopCategory, std::vector<ShopItemData*>>(item->category, itemList));
			}
			_tabsData[item->category].push_back(item);
		}

		_itemsById.insert(pair<int, ShopItemData*>(item->itemId, item));
	}
}

void ShopMgr::initWithData(ShopConfig *data)
{
	std::vector<ShopItemConfig> goldList = data->getGoldItemList();
	for (int i = 0; i < goldList.size(); i++)
	{
		auto item = new ShopItemData();
		item->initWithData(goldList[i]);
		if (item->category != ShopCategory::NONE)
		{
			if (_tabsData.find(item->category) == _tabsData.end())
			{
				std::vector<ShopItemData*> itemList;
				_tabsData.insert(pair<ShopCategory, std::vector<ShopItemData*>>(item->category, itemList));
			}
			_tabsData[item->category].push_back(item);
		}

		_itemsById.insert(pair<int, ShopItemData*>(item->itemId, item));
	}

	std::vector<ShopItemConfig> cashList = data->getCashItemList();
	for (int i = 0; i < cashList.size(); i++)
	{
		auto item = new ShopItemData();
		item->initWithData(cashList[i]);
		if (item->category != ShopCategory::NONE)
		{
			if (_tabsData.find(item->category) == _tabsData.end())
			{
				std::vector<ShopItemData*> itemList;
				_tabsData.insert(pair<ShopCategory, std::vector<ShopItemData*>>(item->category, itemList));
			}
			_tabsData[item->category].push_back(item);
		}

		_itemsById.insert(pair<int, ShopItemData*>(item->itemId, item));
	}
}

void ShopMgr::syncServerData(UpdateShopDataInPacket *data)
{
	deleteVector(_listHotDeals);
	for (int i = 0; i < data->listHotDeals.size(); i++)
	{
		HotDealData *item = new HotDealData(data->listHotDeals[i]);
		_listHotDeals.push_back(item);
		_listHotDealShop.push_back(item);
	}
}

std::vector<ShopItemData*> ShopMgr::getListHotDeals()
{
	return _listHotDealShop;
}

const std::vector<ShopItemData*> & ShopMgr::getShopData(ShopCategory category)
{
	return _tabsData[category];
}

void ShopMgr::buyGold(int id)
{
	paymentHandler->buyGoldByCash(id);
}

void ShopMgr::buyCueBox(int boxId, const std::string & typeCost)
{
	paymentHandler->buyCueBoxByCash(boxId);
}

void ShopMgr::onResponeBuyCueBox(BuyCueBoxByCashInPacket *msg)
{
	gameMgr->_userInfo->setCash(msg->newCash);
	guiMgr->refreshGui(Popup::SHOP_UI);
	guiMgr->refreshGui(Popup::GACHA_ROLLING);

	auto gui = (CueGachaRollingList*)guiMgr->getGui(Popup::GACHA_ROLLING);
	gui->buyCueBox(msg->cueItemIdx);
}

void ShopMgr::buyHotDeal(int hotDealidx, const std::string &typeCost)
{
	paymentHandler->buyHotDeal(hotDealidx);
}

void ShopMgr::onResponeBuyGold(BuyGoldByCashInPacket *msg)
{
	auto popup = (ShopPopupResult*)guiMgr->getGui(Popup::POPUP_RESULT);
	if (msg->error == 0)
	{
		gameMgr->_userInfo->setGold(msg->newGold);
		gameMgr->_userInfo->setCash(msg->newCash);
		guiMgr->refreshGui(Popup::SHOP_UI);
		popup->noticeSuccess();
	}
	else
	{
		popup->noticeError("CASH");
	}
	popup->show();
}

const std::vector<CueRateInBox*> & ShopMgr::getBoxData(int boxId)
{
	auto cueBox = (ShopCueBoxData*)_tabsData[ShopCategory::CUE][boxId - 1];
	return cueBox->listCue;
}

ShopMgr * ShopMgr::_instance = nullptr;

ShopMgr * ShopMgr::getInstance()
{
	if (!_instance)
	{
		_instance = new ShopMgr();
	}
	return _instance;
}

