#include "OfferOnlyGoldUI.h"
#include "../../../data/Resource.h"
#include "../../core/offer/OfferMgr.h"
#include "../../../core/utils/StringUtility.h"

USING_NS_CC;
using namespace cocos2d::ui;

OfferOnlyGoldUI::OfferOnlyGoldUI()
{
	_keep = true;
}

OfferOnlyGoldUI::~OfferOnlyGoldUI()
{

}

bool OfferOnlyGoldUI::init()
{
	if (!UIBaseLayer::init())
	{
		return false;
	}

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::OfferOnlyGoldUI);
	this->initWithBinaryFile(filePath);
	return true;
}

void OfferOnlyGoldUI::initGUI()
{
	initTag();
	autoInitComponent(_layout);

	_lbValue = (cocos2d::ui::Text *) _nodeMap["lbValue"];
	_timeRemain = (cocos2d::ui::Text *) _nodeMap["timeRemain"];
	_lbCost = (cocos2d::ui::Text *) _nodeMap["lbCost"];

	_btnBuy = (cocos2d::ui::Button *) _nodeMap["btnBuy"];
}

void OfferOnlyGoldUI::initTag()
{
	_tagMap["btnBuy"] = btnBuy;
}

void OfferOnlyGoldUI::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	switch (id)
	{
	case btnBuy:
	{
		offerMgr->sendBuyOffer(OfferType::ONLY_GOLD);
		break;
	}
	}
}

void OfferOnlyGoldUI::loadData()
{
	setEnabledButtonBuy(true);
	BaseOffer *baseOffer = offerMgr->getOfferById(OfferType::ONLY_GOLD);
	OfferOnlyGold *goldOffer = dynamic_cast<OfferOnlyGold *>(baseOffer);
	if (goldOffer)
	{
		auto valueStr = StringUtility::standartNumber(goldOffer->num);
		_lbValue->setString(valueStr);

		char costStr[10];
		sprintf(costStr, "%2.f", goldOffer->cost);
		_lbCost->setString(costStr);
	}
}

void OfferOnlyGoldUI::update(float dt)
{
	BaseOffer *baseOffer = offerMgr->getOfferById(OfferType::ONLY_GOLD);
	OfferOnlyGold *goldOffer = dynamic_cast<OfferOnlyGold *>(baseOffer);
	if (goldOffer)
	{
		long long remainTime = goldOffer->timeRemain;
		if (remainTime <= 0)
		{
			remainTime = 0;
			setEnabledButtonBuy(false);
		}
		auto timeStr = StringUtility::getRemainTimeString(remainTime);
		_timeRemain->setString(timeStr);
	}
	
}

void OfferOnlyGoldUI::setEnabledButtonBuy(bool isEnable)
{
	_btnBuy->setTouchEnabled(isEnable);
	_btnBuy->setBright(isEnable);
}

void OfferOnlyGoldUI::show()
{
	UIBaseLayer::show();
	loadData();
}

