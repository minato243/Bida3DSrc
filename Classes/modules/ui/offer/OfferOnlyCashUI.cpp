#include "OfferOnlyCashUI.h"
#include "../../../data/Resource.h"
#include "../../core/offer/OfferEntity.h"
#include "../../core/offer/OfferMgr.h"
#include "../../../core/utils/StringUtility.h"

USING_NS_CC;
using namespace cocos2d::ui;
OfferOnlyCashUI::OfferOnlyCashUI()
{

}

OfferOnlyCashUI::~OfferOnlyCashUI()
{

}

bool OfferOnlyCashUI::init()
{
	if (!UIBaseLayer::init())
		return false;

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::OfferOnlyCashUI);
	this->initWithBinaryFile(filePath);
	return true;
}

void OfferOnlyCashUI::initGUI()
{
	initTag();
	autoInitComponent(_layout);

	_lbCost = (Text*)_nodeMap["lbCost"];
	_lbNewCash = (Text*)_nodeMap["lbNewCash"];
	_lbOldCash = (Text*)_nodeMap["lbOldCash"];

	_btnBuy = (Button *) _nodeMap["btnBuy"];
	_timeRemain = (Text *) _nodeMap["timeRemain"];
}

void OfferOnlyCashUI::initTag()
{
	_tagMap["btnBuy"] = btnBuy;
}

void OfferOnlyCashUI::onButtonRelease(Button* button, int id)
{
	switch (id)
	{
	case btnBuy:
		OfferMgr::getInstance()->sendBuyOffer(OfferType::ONLY_CASH);
		break;
	}
}

void OfferOnlyCashUI::loadData()
{
	setEnabledButtonBuy(true);
	BaseOffer *baseOffer = offerMgr->getOfferById(OfferType::ONLY_CASH);
	OfferOnlyCash *cashOffer = dynamic_cast<OfferOnlyCash *>(baseOffer);
	if (cashOffer)
	{
		auto valueStr = StringUtility::standartNumber(cashOffer->num);
		_lbNewCash->setString(valueStr);

		_lbOldCash->setString(StringUtility::standartNumber(cashOffer->num/2));

		char costStr[10];
		sprintf(costStr, "$ %.2f", cashOffer->cost);
		_lbCost->setString(costStr);
	}
}

void OfferOnlyCashUI::setEnabledButtonBuy(bool isEnable)
{
	_btnBuy->setTouchEnabled(isEnable);
	_btnBuy->setBright(isEnable);
}

void OfferOnlyCashUI::update(float dt)
{
	BaseOffer *baseOffer = offerMgr->getOfferById(OfferType::ONLY_CASH);
	OfferOnlyCash *cashOffer = dynamic_cast<OfferOnlyCash *>(baseOffer);
	if (cashOffer)
	{
		long long remainTime = cashOffer->timeRemain;
		if (remainTime <= 0)
		{
			remainTime = 0;
			setEnabledButtonBuy(false);
		}
		auto timeStr = StringUtility::getShortRemainTimeString(remainTime);
		_timeRemain->setString(timeStr);
	}
}

void OfferOnlyCashUI::onEnter()
{
	UIBaseLayer::onEnter();
	scheduleUpdate();
}

void OfferOnlyCashUI::onExit()
{
	unscheduleUpdate();
	UIBaseLayer::onExit();
}

