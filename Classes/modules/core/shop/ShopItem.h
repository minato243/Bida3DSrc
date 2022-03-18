#ifndef ShopItem_h_
#define ShopItem_h_

#include <data/DataStruct.h>

class ShopItem
{
public:
	ShopItem();
	~ShopItem();

	void initWithData(ItemInfoData data);

	void updateServerData(ItemInfoData data);

public:
	int type;
	int itemId;
	int cost;
	int typeCost;
	int discount;
	int category;
	int value;
};
#endif