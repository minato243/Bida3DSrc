#ifndef ShopMgr_h_
#define ShopMgr_h_

#include <data/DataStruct.h>
#include "../../../data/GameConstant.h"
#include "../../network/payment/PaymentInPacket.h"
#include "../../../data/config/ShopV3Config.h"
#include "../../../data/config/CueConfig.h"

#define shopMgr ShopMgr::getInstance()

class ShopMgr
{
public:
	ShopMgr();
	~ShopMgr();

	void init();

	void initCueBoxData(const std::vector<CueBoxConfigData*> &data);

	void initWithData(ShopConfig *data);

	void syncServerData(UpdateShopDataInPacket *data);

	std::vector<ShopItemData*> getListHotDeals();

	void dispatchDataChange();

	void setItemHotDealIsSold(int hotDealIdx);

	const std::vector<ShopItemData*> & getShopData(ShopCategory cateogry);

	void buyGold(int id);

	void buyCueBox(int boxId, const std::string & typeCost);

	void onResponeBuyCueBox(BuyCueBoxByCashInPacket *msg);

	void buyHotDeal(int hotDealidx, const std::string &typeCost);

	void onResponeBuyGold(BuyGoldByCashInPacket *msg);

	const std::vector<CueRateInBox*> &getBoxData(int boxId);
private:
	static ShopMgr *_instance;

	std::vector<HotDealData*> _listHotDeals;
	std::vector<ShopItemData*> _listHotDealShop;
	std::map<ShopCategory, std::vector<ShopItemData*>> _tabsData;
	std::map<int, ShopItemData*> _itemsById;

public:
	static ShopMgr *getInstance();
};
#endif