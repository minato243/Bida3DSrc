#include "OfferStarterUI.h"
#include "../../../data/Resource.h"
#include "../../core/offer/OfferMgr.h"
#include "../../core/offer/OfferEntity.h"
#include "../../../core/utils/StringUtility.h"
#include "../../core/inventory/ItemConstant.h"
#include "../../../core/utils/LanguageMgr.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

OfferStarterUI::OfferStarterUI()
{

}

OfferStarterUI::~OfferStarterUI()
{

}

bool OfferStarterUI::init()
{
	if (!UIBaseLayer::init())
		return false;

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::OfferStarterUI);
	this->initWithBinaryFile(filePath);

	hideTooltip();
	return true;
}

void OfferStarterUI::initGUI()
{
	initTag();
	autoInitComponent(_layout);

	_bg = _nodeMap["bg"];
	_btnBuy = (Button *)_nodeMap["btnBuy"];
	_lbCost = (Text *)_nodeMap["lbCost"];
	_timeRemain = (Text *)_nodeMap["timeRemain"];

	_bgTooltip = (ImageView *)_nodeMap["bgTooltip"];
	_lbTooltip = (Text *)_nodeMap["lbTooltip"];
	_pnGift = (Widget *)_nodeMap["pnGift"];
}

void OfferStarterUI::initTag()
{
	_tagMap["btnBuy"] = btnBuy;
	_tagMap["btnGift_0"] = btnGift_0;
	_tagMap["btnGift_1"] = btnGift_1;
	_tagMap["btnGift_2"] = btnGift_2;
	_tagMap["btnGift_3"] = btnGift_3;
	_tagMap["btnGift_4"] = btnGift_4;
	_tagMap["btnGift_5"] = btnGift_5;
	_tagMap["btnGift_6"] = btnGift_6;
}

void OfferStarterUI::onButtonTouched(cocos2d::ui::Button* button, int id)
{
	switch (id)
	{
	case btnGift_0:
	case btnGift_1:
	case btnGift_2:
	case btnGift_3:
	case btnGift_4:
	case btnGift_5:
	case btnGift_6:
		showTooltip(id);
	}
}

void OfferStarterUI::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	hideTooltip();
	switch (id)
	{
	case btnBuy:
	{
		offerMgr->sendBuyOffer(OfferType::STARTER);
		break;
	}
	default:
		break;
	}
}

void OfferStarterUI::loadData()
{
	auto data = offerMgr->getOfferById(OfferType::STARTER);
	auto offerStarter = dynamic_cast<OfferStarter *>(data);
	if (offerStarter)
	{
		_data = offerStarter;
		setEnabledButtonBuy(true);
		loadListGift(offerStarter->itemList);
		char text[20];
		sprintf(text, "$%.2f", data->cost);
		_lbCost->setString(text);
	}
}

void OfferStarterUI::setEnabledButtonBuy(bool isEnable)
{
	_btnBuy->setTouchEnabled(isEnable);
	_btnBuy->setBright(isEnable);
}

void OfferStarterUI::loadListGift(const std::vector<OfferItemData> & listGift)
{
	auto numGift = MIN(listGift.size(), 7);
	for (auto i = 0; i < numGift; i++)
	{
		char text[20];
		sprintf(text, "nodeGift_%d", i);
		auto node = getControl(text, _bg);
		loadNodeGift(node, listGift.at(i));
	}
}

void OfferStarterUI::loadNodeGift(cocos2d::Node *node, const OfferItemData & gift)
{
	auto imgSelect = (ImageView *)getControl("imgSelect", node);
	auto lbNum = (Text *)getControl("lbNum", node);
	auto imgItem = (ImageView *)getControl("imgItem", node);

	imgSelect->setVisible(false);
	lbNum->setString(StringUtility::formatNumberSymbol(gift.amount));
	std::string iconStr = ItemConstant::getItemIcon(gift.itemID);
	imgItem->loadTexture(iconStr, TextureResType::PLIST);
}

void OfferStarterUI::update(float dt)
{
	BaseOffer *baseOffer = offerMgr->getOfferById(OfferType::STARTER);
	OfferStarter *starterOffer = dynamic_cast<OfferStarter *>(baseOffer);
	if (starterOffer)
	{
		long long remainTime = starterOffer->timeRemain;
		if (remainTime <= 0)
		{
			remainTime = 0;
			setEnabledButtonBuy(false);
		}
		auto timeStr = StringUtility::getShortRemainTimeString(remainTime);
		_timeRemain->setString(timeStr);
	}
}

void OfferStarterUI::onEnter()
{
	UIBaseLayer::onEnter();
	scheduleUpdate();
}

void OfferStarterUI::onExit()
{
	unscheduleUpdate();
	UIBaseLayer::onExit();
}

void OfferStarterUI::showTooltip(int idx)
{
	char text[20];
	sprintf(text, "nodeGift_%d", idx);
	auto node = getControl(text, _pnGift);

	getControl("imgSelect", node)->setVisible(true);
	_bgTooltip->setVisible(true);
	char itemName[20];
	sprintf(itemName, "text_name_item_%d", _data->itemList[idx].itemID);
	_lbTooltip->setString(languageMgr->localizedText(itemName));

	auto pos = node->getPosition();
	pos.y += node->getContentSize().height / 2 + 5;
	_bgTooltip->setPosition(pos);
}

void OfferStarterUI::hideTooltip()
{
	_bgTooltip->setVisible(false);
	for (auto i = 0; i < 7; i++)
	{
		char text[20];
		sprintf(text, "nodeGift_%d", i);
		auto node = getControl(text, _bg);
		getControl("imgSelect", node)->setVisible(false);
	}
}

