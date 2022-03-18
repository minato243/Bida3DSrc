#include "OfferCumulativeUI.h"
#include "../../../data/Resource.h"
#include "../../core/offer/OfferMgr.h"
#include "../../../core/utils/StringUtility.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

OfferCumulativeUI::OfferCumulativeUI()
{
	_curPoint = 0;
	_curPercent = 0;
	_maxPercent = 0;
	_require = 0;
}

OfferCumulativeUI::~OfferCumulativeUI()
{

}

bool OfferCumulativeUI::init()
{
	if (!UIBaseLayer::init())
		return false;

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::OfferCumulativeUI);
	this->initWithBinaryFile(filePath);
	return true;
}

void OfferCumulativeUI::initGUI()
{
	initTag();
	autoInitComponent(_layout);

	_btnOpen = (Button *)_nodeMap["btnOpen"];
	_progressBar = (LoadingBar *) _nodeMap["progressBar"];
	_lbProgress = (Text *) _nodeMap["lbProgress"];
}

void OfferCumulativeUI::initTag()
{
	_tagMap["btnOpen"] = btnOpen;
}

void OfferCumulativeUI::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	switch (id)
	{
	case btnOpen:
	{
		offerMgr->openChestOffer(_level);
		break;
	}
	default:
		break;
	}
}

void OfferCumulativeUI::loadData()
{
	auto data = offerMgr->getOfferById(OfferType::CUMULATIVE);
	auto cumulOffer = dynamic_cast<OfferCumulativeData *>(data);
	if (cumulOffer)
	{
		_level = cumulOffer->level;
		_point = cumulOffer->point;
		_require = cumulOffer->require;

		_maxPercent = MIN(100, floor(_point * 100/ _require));
		_maxPoint = MIN(_require, _point);

		loadGift(cumulOffer);
	}
}

void OfferCumulativeUI::loadGift(OfferCumulativeData *data)
{
	for (auto i = 0; i < data->listGift.size(); i++)
	{
		char text[20];
		sprintf(text, "nodeGift_%d", i);
		auto node = getControl(text, _bg);
		auto lbNum = (Text *)getControl("lbNum", node);
		lbNum->setString(StringUtility::formatNumberSymbol(data->listGift[i].num));
	}
}

void OfferCumulativeUI::show()
{
	UIBaseLayer::show();
	loadData();
	loadProgressBar();
}

void OfferCumulativeUI::loadProgressBar()
{
	_curPercent = 0;
	_curPoint = 0;

	setEnabledButton(false);
	schedule(schedule_selector(OfferCumulativeUI::updateProgressBar), 0.02f);
}

void OfferCumulativeUI::updateProgressBar(float dt)
{
	_curPercent += 1;
	_curPoint = (int)(_curPercent *_require / 100);
	if (_curPercent > _maxPercent)
	{
		_curPercent = _maxPercent;
		_curPoint = _point;
	}
	_progressBar->setPercent(_curPercent);

	char text[20];
	sprintf(text, "%d/%d USD", _curPoint, _require);
	_lbProgress->setString(text);

	if (_curPercent >= _maxPercent)
	{
		unschedule(schedule_selector(OfferCumulativeUI::updateProgressBar));
		if(_curPercent >= 100)
			setEnabledButton(true);
	}
}

void OfferCumulativeUI::setEnabledButton(bool isEnable)
{
	_btnOpen->setTouchEnabled(isEnable);
	_btnOpen->setBright(isEnable);
}
