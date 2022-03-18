#include "ShopTabLimited.h"
#include "../../../data/Resource.h"
#include "../../core/utils/TimeUtils.h"
#include "../../../core/utils/StringUtility.h"
#include "../elements/QuickCustomTableView.h"
#include "../../../data/DataStruct.h"
#include "ShopItem.h"
#include "ShopItemLimited.h"
#include "ShopItemCueBox.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

ShopTabLimited::ShopTabLimited()
{

}

ShopTabLimited::~ShopTabLimited()
{

}

bool ShopTabLimited::init()
{
	if (!BaseLayer::init("ShopTabLimited"))
	{
		return false;
	}

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::ShopTabLimited);
	this->initWithBinaryFile(filePath);

	this->setSizeTabLayer();
	this->loadData(0);
	return true;
}

void ShopTabLimited::initTimer()
{
	_remainTimeDeal = timeUtils->getRemainTimeEndDay();
	if (_remainTimeDeal < 1)
		_remainTimeDeal = 86400 - 1;
	_time_remain->setString(StringUtility::getRemainTimeString(_remainTimeDeal));

	schedule(schedule_selector(ShopTabLimited::updateRemainTime), 1);
}

void ShopTabLimited::updateRemainTime(float dt)
{
	_remainTimeDeal--;
	_time_remain->setString(StringUtility::getRemainTimeString(_remainTimeDeal));
}

void ShopTabLimited::initGUI()
{
	autoInitComponent(_layout);

	_bg = _nodeMap["bg"];
	_pnContent_0 = (cocos2d::ui::Widget *) _nodeMap["pnContent_0"];
	_pnContent_1 = (cocos2d::ui::Widget *) _nodeMap["pnContent_1"];
	_bgTop = (cocos2d::ui::Widget *) _nodeMap["bgTop"];
	_time_remain = (cocos2d::ui::Text *) _nodeMap["time_remain"];

	initTimer();
}

void ShopTabLimited::setSizeTabLayer()
{
	auto tbSize = _pnContent_1->getContentSize();
	auto tbViewTmp = new QuickCustomTableView<ShopItemData, ShopItem>();
	tbViewTmp->init(tbSize, extension::ScrollView::Direction::VERTICAL, -1, 3);
	vector<ShopItemData*> data = vector<ShopItemData*>();
	tbViewTmp->reloadData(data);
	auto hItem = tbViewTmp->tableCellSizeForIndex().height;

	auto deltaHeight = 2 * hItem - _pnContent_1->getContentSize().height;
	((Widget*)_bg)->setSize(Size(this->_bg->getContentSize().width, _bg->getContentSize().height + deltaHeight));
	_pnContent_1->setSize(Size(_pnContent_1->getContentSize().width, 2 * hItem));
	_bgTop->setPosition(Vec2(0, _bg->getContentSize().height));

	CC_SAFE_DELETE(tbViewTmp);
}

void ShopTabLimited::loadData(int idx)
{
	this->loadDataLimited();
	this->loadDataHotDeals();
}

void ShopTabLimited::loadDataLimited()
{
	auto container0 = _pnContent_0;

	auto listItemLimited = new QuickCustomTableView<int, ShopItemLimited>();
	listItemLimited->init(container0->getContentSize(), extension::ScrollView::Direction::HORIZONTAL, 6, 1);
	container0->addChild(listItemLimited);
	auto tmpData = vector<int*>();
	for (auto i = 0; i < 6; i++) {
		int  *a= new int(i);
		tmpData.push_back(a);
	}

	listItemLimited->reloadData(tmpData);
	_listItemLimited = listItemLimited;
	listItemLimited->_button->setTouchEnabled(false);
	listItemLimited->_tbView->setTouchEnabled(false);
}

void ShopTabLimited::loadDataHotDeals()
{
	auto container1 = _pnContent_1;
	auto listItem = new QuickCustomTableView<ShopItemData, ShopItemCueBox>();
	listItem->init(container1->getContentSize(), extension::ScrollView::Direction::VERTICAL, -1, 3);
	container1->addChild(listItem);

	listItem->reloadData(shopMgr->getListHotDeals());
	_listItem = listItem;
	listItem->_button->setTouchEnabled(false);
	listItem->_tbView->setTouchEnabled(false);
}

void ShopTabLimited::reloadHotDeals()
{
	//clearInterval(this->setRemainTimeDeal);
	this->initTimer();
	auto listItem = (QuickCustomTableView<ShopItemData, ShopItem> *)_listItem;
	listItem->reloadData(shopMgr->getListHotDeals());
}

void ShopTabLimited::updateDataHotDeals()
{
	auto listItem = (QuickCustomTableView<ShopItemData, ShopItem> *)_listItem;
	listItem->reloadData(shopMgr->getListHotDeals());
}

cocos2d::Size ShopTabLimited::getOriginalSize()
{
	return this->_bg->getContentSize();
}

void ShopTabLimited::scaleToSize(cocos2d::Size size)
{
	this->_bg->setScale(size.width / this->_bg->getContentSize().width);
}

