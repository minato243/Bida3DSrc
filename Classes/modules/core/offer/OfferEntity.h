#ifndef OfferEntity_h_
#define OfferEntity_h_

#include <string>
#include "../../../data/config/OfferConfig.h"

enum class OfferType
{
	STARTER = 1,
	ONLY_CASH = 2,
	ONLY_GOLD = 3,
	CUMULATIVE = 4
};
class BaseOffer
{
public:
	BaseOffer();
	virtual ~BaseOffer();

	void update(float dt);

public:
	OfferType type;
	std::string name;
	float cost;
	long long num;
	double timeRemain;
};

class OfferStarter
	:public BaseOffer
{
public:
	OfferStarter();
	OfferStarter(long long remainTime);
	~OfferStarter();
	void init();

public:
	std::vector<OfferItemData> itemList;
};

class OfferOnlyGold
	:public BaseOffer
{
public:
	OfferOnlyGold();
	OfferOnlyGold(int packId, long long remainTime);
	~OfferOnlyGold();
};

class OfferOnlyCash
	:public BaseOffer
{
public:
	OfferOnlyCash();
	OfferOnlyCash(int packId, long long remainTime);
	~OfferOnlyCash();

};

class OfferCumulativeData
	:public BaseOffer
{
public:
	OfferCumulativeData();
	OfferCumulativeData(int level, int point);
	~OfferCumulativeData();

	void initWithData(int level, int point);

public:
	int level;
	int point;
	int require;
	std::vector<ItemInfoData> listGift;
};
#endif
