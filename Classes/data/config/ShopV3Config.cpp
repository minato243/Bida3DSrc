#include "ShopV3Config.h"
#include "ConfigMgr.h"

using namespace rapidjson;

ShopConfig::ShopConfig()
{
	loadConfig();
}

ShopConfig::~ShopConfig()
{
	CC_SAFE_DELETE(data);
}

void ShopConfig::loadConfig()
{
	data = configMgr->loadJsonFile("res/config/ShopV3Config.json");

	const Value& goldList = (*data)["GOLD"];
	for (SizeType i = 0; i < goldList.Size(); i++)
	{
		const Value& config = goldList[i];
		ShopItemConfig item;
		item.id = config["id"].GetInt();
		item.type = config["type"].GetInt();
		item.shopCategory = config["shopCategory"].GetString();
		item.value = config["value"].GetInt();
		item.cost = config["cost"].GetInt();
		item.typeCost = config["typeCost"].GetString();
		item.discount = config["discount"].GetInt();

		goldItemList.push_back(item);
	}

	const Value& cashList = (*data)["CASH"];
	for (SizeType i = 0; i < cashList.Size(); i++)
	{
		const Value& config = cashList[i];
		ShopItemConfig item;
		item.id = config["id"].GetInt();
		item.type = config["type"].GetInt();
		item.shopCategory = config["shopCategory"].GetString();
		item.value = config["value"].GetInt();
		item.cost = config["cost"].GetInt();
		item.typeCost = config["typeCost"].GetString();
		item.discount = config["discount"].GetInt();

		cashItemList.push_back(item);
	}
}

const std::vector<ShopItemConfig> & ShopConfig::getGoldItemList()
{
	return goldItemList;
}

const std::vector<ShopItemConfig> & ShopConfig::getCashItemList()
{
	return cashItemList;
}

