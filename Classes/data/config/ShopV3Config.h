#ifndef ShopV3Config_h_
#define ShopV3Config_h_

#include <json/document.h>
#include <string.h>
#include <data/DataStruct.h>


struct ShopItemConfig
{
	int id;
	int type;
	std::string shopCategory;
	int value;
	int cost;
	std::string typeCost;
	int discount;
};

class ShopConfig
{
public:
	ShopConfig();
	~ShopConfig();

	void loadConfig();

	const std::vector<ShopItemConfig> & getGoldItemList();

	const std::vector<ShopItemConfig> & getCashItemList();
public:
	rapidjson::Document *data;

	std::vector<ShopItemConfig> goldItemList;
	std::vector<ShopItemConfig> cashItemList;

};
#endif