#ifndef ShopItemData_h_
#define ShopItemData_h_

#include <data/DataStruct.h>
#include "../../../data/GameConstant.h"
#include "../../../data/config/ShopV3Config.h"
#include "../../../data/config/CueConfig.h"

class ShopItemData
{
public:
	int type;
	int itemId;
	int cost;
	std::string typeCost;
	int discount;
	std::vector<std::string> items;
	ShopCategory category;
	int value;

	void initWithData(const ShopItemConfig & data)
	{
		this->type = data.type;
		this->itemId = data.id;
		this->cost = data.cost;
		this->typeCost = data.typeCost;
		this->discount = 0;
		// Each item has it own prop

		this->category = getCategory(data.shopCategory);
		this->value = data.value;
	}

	static ShopCategory getCategory(const std::string & data)
	{
		ShopCategory category;
		if (data.compare(SHOP_CATEGORY_GOLD) == 0)
			return ShopCategory::GOLD;
		if (data.compare(SHOP_CATEGORY_CASH) == 0)
			return ShopCategory::CASH;
		if (data.compare(SHOP_CATEGORY_CUE) == 0)
			return ShopCategory::CUE;
		if (data.compare(SHOP_CATEGORY_GOLD) == 0)
			return ShopCategory::LIMITED;

		return ShopCategory::NONE;
	}
};

class ShopCueBoxData :
	public ShopItemData
{
public:
	int boxId;
	std::string name;
	std::vector<CueRateInBox*> listCue;

public:
	~ShopCueBoxData();
	void initWithData(const CueBoxConfigData & data)
	{
		this->boxId = data.boxId;
		this->name = data.name;
		this->cost = data.price;
		this->category = ShopCategory::CUE;
		this->listCue.clear();
		for (int i = 0; i < data.listCue.size(); i++) {
			CueRateInBox *obj = new CueRateInBox();
			obj->cueId = data.listCue[i].cueId;
			obj->rate = data.listCue[i].rate;
			obj->type = obj->cueId / 1000;
			this->listCue.push_back(obj);
		}
	}
};

class HotDealData
	:public ShopItemData
{
public:
	std::string hotDealType;
	int rate;
	int cost;
	int discount;
	bool isSold;
	int category;
	int hotDealIdx;
	std::string typeCost;

public:
	HotDealData() {};
	HotDealData(const HotDealData & src)
	{
		this->type = src.type;
		this->rate = src.rate;
		this->cost = src.cost;
		this->discount = src.discount;
		this->isSold = src.isSold;
		this->category = src.category;
		this->hotDealIdx = src.hotDealIdx;
		this->typeCost = src.typeCost;
	}

};

#endif