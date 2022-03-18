#include "OfferMgr.h"
#include "cocos2d.h"
#include "../../network/offer/OfferHandler.h"
#include "../../ui/GuiMgr.h"
#include "../../ui/offer/OfferBaseUI.h"

OfferMgr::OfferMgr()
{

}

OfferMgr::~OfferMgr()
{

}

OfferMgr * OfferMgr::getInstance()
{
	if (_instance == NULL)
	{
		_instance = new OfferMgr();
	}
	return _instance;
}

void OfferMgr::sendBuyOffer(OfferType offerId)
{
	offerHandler->sendPurchaseOffer((int)offerId);
}

void OfferMgr::sendCheatOffer(OfferType offerId)
{
	offerHandler->sendCheatOffer((int)offerId);
}


void OfferMgr::sendCheatOfferCumulative(int level, int point)
{
	offerHandler->sendCheatOfferCumulative(level, point);
}

void OfferMgr::addOfferData(OfferType offerId, int packId, long long remainTime)
{
	switch (offerId)
	{
	case OfferType::STARTER:
	{
		addOfferStarter(remainTime);
		break;
	}

	case OfferType::ONLY_GOLD:
	{
		addOfferOnlyGold(packId, remainTime);
		break;
	}

	case OfferType::ONLY_CASH:
	{
		addOfferOnlyCash(packId,remainTime);
		break;
	}
		
	}
}

void OfferMgr::updateTime(float dt)
{
	for (auto i = 0; i < _listOffer.size(); i++)
	{
		_listOffer[i]->update(dt);
	}
}

void OfferMgr::addOfferCumulative(int level, int point)
{
	auto cumulOffer = new OfferCumulativeData(level, point);
	_listOffer.push_back(cumulOffer);
}

void OfferMgr::openChestOffer(int level)
{
	offerHandler->sendOpenChestOffer(level);
}

void OfferMgr::onResponeOpenChestOffer()
{
	CCLOG("OfferMgr::onResponeOpenChestOffer TODO");
	/*let cumulativeOffer = this.getOfferByID(OFFER_CONSTANT.OFFER_CUMULATIVE_ID);
				this.updateItemInventory(cumulativeOffer.listGift);
				cc.log("cumulativeOffer.level : " + cumulativeOffer.level + ' ' + cumulativeOffer.point);
				cumulativeOffer.initWithData({
					"level": cumulativeOffer.level + 1,
					"point": cumulativeOffer.point - cumulativeOffer.require
				})

				gv.GuiMgr.getGui(Popup.OFFER_BASE_UI, OfferBaseUI).hide();
				gv.GuiMgr.getGui(Popup.OFFER_POPUP_OPEN_CHEST, PopupOpenChest).show();*/
	auto offer = getOfferById(OfferType::CUMULATIVE);
	auto cumulOffer = dynamic_cast<OfferCumulativeData *>(offer);
	cumulOffer->level += 1;
	cumulOffer->point = cumulOffer->point - cumulOffer->require;

	guiMgr->getGui(Popup::OFFER_BASE_UI)->hide();
	guiMgr->getGui(Popup::OFFER_POPUP_OPEN_CHEST)->show();
}

void OfferMgr::onResponePurchase(OfferType offerId)
{
	removeOfferById(offerId);
	guiMgr->notifyDebugText("Purchase Success");
	auto offerGui = dynamic_cast<OfferBaseUI*>(guiMgr->getGui(Popup::OFFER_BASE_UI));
	offerGui->loadListOffer();
	offerGui->hide();

}

void OfferMgr::removeOfferById(OfferType offerId)
{
	for (auto i = 0; i < _listOffer.size(); i++)
	{
		if (_listOffer[i]->type == offerId)
		{
			auto offer = _listOffer[i];
			CC_SAFE_DELETE(offer);
			_listOffer.erase(_listOffer.begin() +i);
			break;
		}
	}
}

BaseOffer * OfferMgr::getOfferById(OfferType offerID)
{
	for (auto i = 0; i < _listOffer.size(); i++)
	{
		if (_listOffer[i]->type == offerID)
		{
			return _listOffer[i];
		}
	}
	return NULL;
}

void OfferMgr::addOfferOnlyGold(int packId, long long remainTime)
{
	auto offerOnlyGold = new OfferOnlyGold(packId, remainTime);
	_listOffer.push_back(offerOnlyGold);
}

void OfferMgr::addOfferOnlyCash(int packId, long long remainTime)
{
	auto offerOnlyCash = new OfferOnlyCash(packId, remainTime);
	_listOffer.push_back(offerOnlyCash);
}

void OfferMgr::addOfferStarter(long long remainTime)
{
	auto offerStarter = new OfferStarter(remainTime);
	_listOffer.push_back(offerStarter);
}

OfferMgr * OfferMgr::_instance = NULL;

