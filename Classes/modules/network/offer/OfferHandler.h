#ifndef OfferHandler_h_
#define OfferHandler_h_

#include "../base/BaseHandler.h"

class OfferHandler
	:public BaseHandler
{
public:
	OfferHandler();
	~OfferHandler();

	static OfferHandler *getInstance();

	bool onReceived(fr::InPacket* pk);

	void sendPurchaseOffer(int offerId);

	void sendCheatOffer(int offerId);

	void sendOpenChestOffer(int level);

	void sendCheatOfferCumulative(int level, int point);

private:
	static OfferHandler *_instance;
};

#define offerHandler OfferHandler::getInstance()
#endif
