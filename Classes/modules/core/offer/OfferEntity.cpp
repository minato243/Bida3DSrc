#include "OfferEntity.h"
#include "../../../data/config/ConfigMgr.h"

BaseOffer::BaseOffer()
{
	cost = 0;
	num = 0;
	timeRemain = 0;
}

BaseOffer::~BaseOffer()
{

}


void BaseOffer::update(float dt)
{
	timeRemain -= dt;
	if (timeRemain < 0) timeRemain = 0;
}

//OfferOnlyGold
OfferOnlyGold::OfferOnlyGold()
	:BaseOffer()
{
	type = OfferType::ONLY_GOLD;
	name = "GET BACK TO THE GAME";
}

OfferOnlyGold::OfferOnlyGold(int packId, long long remainTime)
{
	type = OfferType::ONLY_GOLD;
	name = "GET BACK TO THE GAME";
	this->timeRemain = remainTime;
	auto goldConfig = configMgr->offerConfig->getGoldOfferConfig(packId);
	this->cost = goldConfig.price;
	this->num = goldConfig.gold;
}

OfferOnlyGold::~OfferOnlyGold()
{

}

//OfferOnlyCash
OfferOnlyCash::OfferOnlyCash()
	:BaseOffer()
{
	type = OfferType::ONLY_CASH;
	name = "PREMIUM 100 PERCENT TOP-UP";
}

OfferOnlyCash::OfferOnlyCash(int packId, long long remainTime)
{
	type = OfferType::ONLY_CASH;
	name = "PREMIUM 100 PERCENT TOP-UP";
	this->timeRemain = remainTime;

	auto cashConfig = configMgr->offerConfig->getCashOfferConfig(packId);
	this->cost = cashConfig.price;
	this->num = cashConfig.cash;
}

OfferOnlyCash::~OfferOnlyCash()
{

}

//OfferCumulativeData
OfferCumulativeData::OfferCumulativeData()
	:BaseOffer()
{
	type = OfferType::CUMULATIVE;
	name = "Cumulative chest";

	level = 0;
	point = 0;
}

OfferCumulativeData::OfferCumulativeData(int level, int point)
	:BaseOffer()
{
	type = OfferType::CUMULATIVE;
	name = "Cumulative chest";
	initWithData(level, point);
}

OfferCumulativeData::~OfferCumulativeData()
{

}

void OfferCumulativeData::initWithData(int level, int point)
{
	this->level = level;
	this->point = point;

	auto cumulConfig = configMgr->offerConfig->getCumulativeOfferConfig(level);
	this->require = cumulConfig->require;
	for (auto i = 0; i < cumulConfig->listGift.size(); i++)
	{
		this->listGift.push_back(cumulConfig->listGift.at(i));
	}
	CC_SAFE_DELETE(cumulConfig);
}

//OfferStarter
OfferStarter::OfferStarter()
	:BaseOffer()
{
	type = OfferType::STARTER;
	init();
}

OfferStarter::OfferStarter(long long remainTime)
	:BaseOffer()
{
	type = OfferType::STARTER;
	this->timeRemain = remainTime;
	init();
}

OfferStarter::~OfferStarter()
{
	itemList.clear();
}

void OfferStarter::init()
{
	auto config = configMgr->offerConfig->getStarterOfferData();
	cost = config->cost;
	name = config->name;

	for (auto i = 0; i < config->itemList.size(); i++)
	{
		auto item = config->itemList.at(i);
		itemList.push_back(item);
	}
}
