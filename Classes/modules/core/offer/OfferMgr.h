#ifndef OfferMgr_h_
#define OfferMgr_h_

#include "OfferEntity.h"

class OfferMgr 
{
public:
	OfferMgr();
	~OfferMgr();

	static OfferMgr *getInstance();

	void sendBuyOffer(OfferType offerId);

	void sendCheatOffer(OfferType offerId);

	void sendCheatOfferCumulative(int level, int point);

	BaseOffer *getOfferById(OfferType offerID);

	void addOfferOnlyGold(int packId, long long remainTime);

	void addOfferOnlyCash(int packId, long long remainTime);

	void addOfferStarter(long long remainTime);

	void addOfferData(OfferType offerId, int packId, long long remainTime);

	void updateTime(float dt);

	void addOfferCumulative(int level, int point);

	void openChestOffer(int level);

	void onResponeOpenChestOffer();

	void onResponePurchase(OfferType offerId);

	void removeOfferById(OfferType offerId);
private:
	static OfferMgr *_instance;

public:
	std::vector<BaseOffer*> _listOffer;
};

#define offerMgr OfferMgr::getInstance()

#endif