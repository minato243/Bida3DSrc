#include "OfferBaseUI.h"
#include "../../../data/Resource.h"
#include "OfferStarterUI.h"
#include "OfferOnlyCashUI.h"
#include "OfferOnlyGoldUI.h"
#include "OfferCumulativeUI.h"
#include "../elements/QuickCustomTableView.h"
#include "../../core/offer/OfferMgr.h"
#include "OfferNodeItem.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

OfferBaseUI::OfferBaseUI()
{
	_keep = true;
}

OfferBaseUI::~OfferBaseUI()
{

}

bool OfferBaseUI::init()
{
	if (!UIBaseLayer::init())
		return false;

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::OfferBaseUI);
	this->initWithBinaryFile(filePath);

	initListOffer();
	return true;
}

void OfferBaseUI::initGUI()
{
	initTag();
	autoInitComponent(_layout);

	_pnOffer = (Widget *)_nodeMap["pnOffer"];
	_imgLight = (ImageView *)_nodeMap["imgLight"];
	_imgLightGround = (ImageView *)_nodeMap["imgLightGround"];
	_pnListOffer = (Widget *)_nodeMap["pnListOffer"];

}

void OfferBaseUI::initTag()
{
	_tagMap["btnClose"] = btnClose;
}

void OfferBaseUI::show()
{
	UIBaseLayer::show();
	runAnimation();

	loadListOffer();
}

void OfferBaseUI::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	switch (id)
	{
	case btnClose: 
	{
		hide();
		break;
	}
	default:
		break;
	}
}

void OfferBaseUI::initTabOffer(OfferType offerId)
{
	if (_listTabOffer.find(offerId) != _listTabOffer.end() && _listTabOffer[offerId] != NULL)
		return;
	cocos2d::Node *tabOffer = NULL;
	switch (offerId)
	{
	case OfferType::STARTER:
		tabOffer = OfferStarterUI::create();
		break;
	case OfferType::ONLY_CASH:
		tabOffer = OfferOnlyCashUI::create();
		break;
	case OfferType::ONLY_GOLD:
		tabOffer = OfferOnlyGoldUI::create();
		break;
	case OfferType::CUMULATIVE:
		tabOffer = OfferCumulativeUI::create();
		break;
	default:
		break;
	}
	((BaseOfferTypeUI *)tabOffer)->loadData();
	_listTabOffer[offerId] = tabOffer;
	_pnOffer->addChild(tabOffer);

	if (offerId == OfferType::CUMULATIVE)
		((OfferCumulativeUI *)tabOffer)->loadProgressBar();
}

void OfferBaseUI::runAnimation()
{
	_imgLight->stopAllActions();
	_imgLightGround->stopAllActions();

	auto action = RepeatForever::create(
		Sequence::create(
			RotateTo::create(3.5, 25),
			DelayTime::create(0.1),
			RotateTo::create(3.5, -20),
			DelayTime::create(0.1),
			NULL
		)
	);
	_imgLight->runAction(action);

	auto action2 = RepeatForever::create(
		Sequence::create(
			ScaleTo::create(1, 1.02),
			ScaleTo::create(1,1),
			NULL
		)
	);
	_imgLightGround->runAction(action2);
}

void OfferBaseUI::initListOffer()
{
	auto size = _pnListOffer->getContentSize();
	auto listBtnOffer = QuickCustomTableView<BaseOffer, OfferNodeItem >::create(size, extension::ScrollView::Direction::VERTICAL, -1, 1);
	_pnListOffer->addChild(listBtnOffer);
	listBtnOffer->_button->setTouchEnabled(false);
	listBtnOffer->_tbView->setTouchEnabled(false);

	_listBtnOffer = listBtnOffer;
}

void OfferBaseUI::switchOffer(OfferType offerId)
{
	if (_curTab == offerId)
		return;
	initTabOffer(offerId);

	for (auto ittr = _listTabOffer.begin(); ittr != _listTabOffer.end(); ittr++)
	{
		if (ittr->second != NULL)
		{
			ittr->second->setVisible(ittr->first == offerId);
		}
	}

	changeStatusListOffer(offerId);
	_curTab = offerId;
}

void OfferBaseUI::loadListOffer()
{
	auto listOffer = offerMgr->_listOffer;

	auto listBtnOffer = (QuickCustomTableView<BaseOffer, OfferNodeItem >*) _listBtnOffer;
	listBtnOffer->reloadData(listOffer);

	if (listOffer.size() > 0)
	{

		_curTab = listOffer[0]->type;
		initTabOffer(_curTab);
		changeStatusListOffer(_curTab);
	}
}

void OfferBaseUI::changeStatusListOffer(OfferType offerId)
{
	auto listBtnOffer = (QuickCustomTableView<BaseOffer, OfferNodeItem > *) _listBtnOffer;
	auto n = listBtnOffer->numberOfCellsInTableView(NULL);
	for (auto i = 0; i < n; i++)
	{
		auto cell = listBtnOffer->getCellAtIndex(i);
		if (cell)
		{
			auto item = cell->getChildByTag(TAG_COMMON);
			auto offerNode = dynamic_cast<OfferNodeItem*>(item);
			offerNode->setStatusSelect(offerNode->_type == offerId);
		}
	}
}

