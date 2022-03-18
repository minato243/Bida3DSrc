#include "OfferConfig.h"
#include "ConfigMgr.h"

using namespace std;
using namespace rapidjson;

OfferConfig::OfferConfig()
{
	loadConfig();
}

OfferConfig::~OfferConfig()
{

}

void OfferConfig::loadConfig()
{
	data = configMgr->loadJsonFile("res/config/OfferConfig.json");
	
	starterOfferData = new StarterOfferData();
	starterOfferData->cost = (*data)["starter"]["cost"].GetFloat();
	starterOfferData->name = (*data)["starter"]["name"].GetString();

	Value &itemList = (*data)["starter"]["itemList"];
	for (auto i = 0; i < (*data)["starter"]["itemList"].Size(); i++)
	{
		OfferItemData item;
		item.name = itemList[i]["name"].GetString();
		item.itemID = itemList[i]["itemID"].GetInt();
		item.amount = itemList[i]["amount"].GetInt();
		item.itemType = itemList[i]["itemType"].GetInt();

		starterOfferData->itemList.push_back(item);
	}
}

StarterOfferData * OfferConfig::getStarterOfferData()
{
	return starterOfferData;
}

CumulativeOfferConfig * OfferConfig::getCumulativeOfferConfig(int level)
{
	CumulativeOfferConfig *cumulOffer = new CumulativeOfferConfig();
	Value &config = (*data)["cumulative"][level - 1];
	cumulOffer->require = config["require"].GetInt();
	
	for (auto i = 0; i < config["listGift"].Size(); i++)
	{
		ItemInfoData item;
		item.type = config["listGift"][i]["itemID"].GetInt();
		item.num = config["listGift"][i]["num"].GetInt();
		cumulOffer->listGift.push_back(item);
	}

	return cumulOffer;
}

GoldOfferConfig OfferConfig::getGoldOfferConfig(int packId)
{
	Value &config = (*data)["onlyGold"][packId];
	GoldOfferConfig goldOffer;
	goldOffer.gold = config["gold"].GetInt64();
	goldOffer.price = config["price"].GetFloat();

	return goldOffer;
}

CashOfferConfig OfferConfig::getCashOfferConfig(int packId)
{
	Value &config = (*data)["onlyCash"][packId];
	CashOfferConfig cashOffer;
	cashOffer.cash = config["cash"].GetInt();
	cashOffer.price = config["price"].GetFloat();

	return cashOffer;
}

StarterOfferData::StarterOfferData()
{

}

StarterOfferData::~StarterOfferData()
{
	itemList.clear();
}
