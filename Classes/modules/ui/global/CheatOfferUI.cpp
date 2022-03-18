#include "CheatOfferUI.h"
#include "../../../data/Resource.h"
#include "../../core/offer/OfferMgr.h"
#include "../../core/offer/OfferEntity.h"
#include "../../core/minigame/spin/SpinMgr.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

CheatOfferUI::CheatOfferUI()
{

}

CheatOfferUI::~CheatOfferUI()
{

}

bool CheatOfferUI::init()
{
	if (!UIBaseLayer::init("CheatItemUI"))
	{
		return false;
	}

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::CheatOffer);
	this->initWithBinaryFile(filePath);

	this->_keep = true;

	return true;
}

void CheatOfferUI::initGUI()
{
	initTag();
	autoInitComponent(_layout);

	_tfLevel = (TextField*)_nodeMap["tfLevel"];
	_tfPoint = (TextField*)_nodeMap["tfPoint"];
}

void CheatOfferUI::initTag()
{
	_tagMap["btnOnlyCashOffer"] = btnOnlyCashOffer;
	_tagMap["btnOnlyGoldOffer"] = btnOnlyGoldOffer;
	_tagMap["btnStarterOffer"] = btnStarterOffer;
	_tagMap["btnOfferCumulative"] = btnOfferCumulative;
	_tagMap["btnSpin"] = btnSpin;
}

void CheatOfferUI::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	switch (id)
	{
	case btnOnlyCashOffer:
		offerMgr->sendCheatOffer(OfferType::ONLY_CASH);
		break;
	case btnOnlyGoldOffer:
		offerMgr->sendCheatOffer(OfferType::ONLY_GOLD);
		break;
	case btnStarterOffer:
		offerMgr->sendCheatOffer(OfferType::STARTER);
		break;
	case btnOfferCumulative:
		cheateOfferCumulative();
		break;
	case btnSpin:
		spinMgr->sendCheatSpin();

	default:
		break;
	}
}

void CheatOfferUI::cheateOfferCumulative()
{
	string levelStr = _tfLevel->getString();
	int level = atoi(levelStr.c_str());

	string pointStr = _tfPoint->getString();
	int point = atoi(pointStr.c_str());
	offerMgr->sendCheatOfferCumulative(level, point);
}
