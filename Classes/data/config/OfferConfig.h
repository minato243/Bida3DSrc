#ifndef OfferConfig_h_
#define OfferConfig_h_

#include <json/document.h>
#include <string.h>
#include <data/DataStruct.h>

struct OfferItemData
{
	std::string name;
	int itemID;
	int amount;
	int itemType;
};

struct StarterOfferData
{
	float cost;
	std::string name;
	std::vector<OfferItemData> itemList;

	StarterOfferData();
	~StarterOfferData();
};

struct CumulativeOfferConfig
{
	int require;
	std::vector<ItemInfoData> listGift;
};

struct GoldOfferConfig
{
	long long gold;
	float price;
};

struct CashOfferConfig
{
	int cash;
	float price;
};

class OfferConfig
{
public:
	OfferConfig();
	~OfferConfig();

	void loadConfig();

	StarterOfferData *getStarterOfferData();

	CumulativeOfferConfig *getCumulativeOfferConfig(int level);

	GoldOfferConfig getGoldOfferConfig(int packId);

	CashOfferConfig getCashOfferConfig(int packId);
public:
	rapidjson::Document *data;
	StarterOfferData *starterOfferData;
	
};
#endif
