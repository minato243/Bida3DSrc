#include "ShopTabItem.h"
#include "../../../data/Resource.h"
#include "../../core/shop/ShopMgr.h"
#include "../elements/QuickCustomTableView.h"
#include "ShopItem.h"
#include "ShopItemCueBox.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

ShopTabItem::ShopTabItem()
{

}

ShopTabItem::~ShopTabItem()
{
	CC_SAFE_DELETE(_listItem);
}

bool ShopTabItem::init()
{
	if (!BaseLayer::init("ShopTabItem"))
	{
		return false;
	}

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::ShopTabItem);
	this->initWithBinaryFile(filePath);

	initListener();
	return true;
}

ShopTabItem * ShopTabItem::create(ShopCategory category)
{
	ShopTabItem *tab = new ShopTabItem();
	tab->init();
	tab->autorelease();

	tab->setSizeTabLayer(category);
	tab->loadData(category);

	return tab;
}

void ShopTabItem::initGUI()
{
	initTag();
	autoInitComponent(_layout);
	_bg = _nodeMap["bg"];
	_ndTop = _nodeMap["ndTop"];
	_tfSearch = (cocos2d::ui::TextField *) _nodeMap["tfSearch"];
	_nd_selection = (cocos2d::ui::Widget *)_nodeMap["nd_selection"];
	_imgArrow = (cocos2d::ui::ImageView *) _nodeMap["imgArrow"];
	_boxSearch = (cocos2d::ui::ImageView *)_nodeMap["boxSearch"];
	_box_sort = (cocos2d::ui::Button *)_nodeMap["box_sort"];
	_lbTextBoxSort = (cocos2d::ui::Text *) getControl("lbText", _box_sort);
	_title = (cocos2d::ui::Text *)_nodeMap["title"];
	_pnContent = (cocos2d::ui::Widget *) _nodeMap["pnContent"];

	_box_sort->setTag(TAG::box_sort);
	_box_sort->addTouchEventListener(CC_CALLBACK_2(BaseLayer::onTouchEventHandler, this));
}

void ShopTabItem::initTag()
{
	_tagMap.insert(pair<string, int>("btn_price_ascending", btn_price_ascending));
	_tagMap.insert(pair<string, int>("btn_price_descending", btn_price_descending));
	_tagMap.insert(pair<string, int>("btn_prefer_break", btn_prefer_break));
	_tagMap.insert(pair<string, int>("btn_prefer_jump", btn_prefer_jump));
	_tagMap.insert(pair<string, int>("btn_prefer_masses", btn_prefer_masses));
}

void ShopTabItem::initListener()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(false);
	listener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event) {
		return true;
	};
	listener->onTouchEnded = [this](cocos2d::Touch* touch, cocos2d::Event* event) {
		auto location = touch->getLocation();
		auto pos = _nd_selection->getPosition();
		auto size = _nd_selection->getContentSize();
		auto rect = Rect(pos.x - size.width/2, pos.y - size.height/2, pos.x + size.height, pos.y + size.height);

		if (!rect.containsPoint(location))
		{
			hidePanelSelection();
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, _nd_selection);
}

void ShopTabItem::loadData(ShopCategory category)
{
	_checkSelect = false;
	_nd_selection->setVisible(false);
	if (category != ShopCategory::CUE)
	{
		_boxSearch->setVisible(false);
		_box_sort->setVisible(false);
	}

	std::string tabName = "";

	switch (category)
	{
	case ShopCategory::CUE:
		tabName = "Cue Boxes";
		break;
	case ShopCategory::GOLD:
		tabName = "Gold";
		break;
	case ShopCategory::CASH:
		tabName = "Cash";
		break;
	}
	_title->setString(tabName);
	
	if (category == ShopCategory::CUE)
	{
		auto listItem = new QuickCustomTableView<ShopItemData, ShopItemCueBox>();
		listItem->init(_pnContent->getContentSize(), extension::ScrollView::Direction::VERTICAL, -1, 3);
		_pnContent->addChild(listItem);
		listItem->reloadData(_listData);
		listItem->_button->setTouchEnabled(false);
		listItem->_tbView->setTouchEnabled(false);
		_listItem = listItem;
	}
	else
	{
		auto listItem = new QuickCustomTableView<ShopItemData, ShopItem>();
		listItem->init(_pnContent->getContentSize(), extension::ScrollView::Direction::VERTICAL, -1, 3);
		_pnContent->addChild(listItem);
		listItem->reloadData(_listData);
		listItem->_button->setTouchEnabled(false);
		listItem->_tbView->setTouchEnabled(false);
		_listItem = listItem;
	}
}

void ShopTabItem::setSizeTabLayer(ShopCategory category)
{
	auto tbViewTmp = new QuickCustomTableView<ShopItemData, ShopItem>();
	tbViewTmp->init(_pnContent->getContentSize(), extension::ScrollView::Direction::VERTICAL, -1, 3);
	auto hItem = tbViewTmp->tableCellSizeForIndex().height;

	_listData = shopMgr->getShopData(category);
	auto numItem = _listData.size();
	auto addNum = (numItem + 2) / 3;
	auto bgSize = _bg->getContentSize();
	_bg->setContentSize(Size(bgSize.width, (addNum + 0.3) * hItem));
	_pnContent->setSize(Size(bgSize.width, addNum * hItem));

	_ndTop->setPosition(Vec2(0, _bg->getContentSize().height));
}

cocos2d::Size ShopTabItem::getOriginalSize()
{
	return _bg->getContentSize();
}

void ShopTabItem::scaleToSize(cocos2d::Size size)
{
	_bg->setScale(size.width / _bg->getContentSize().width);
}

void ShopTabItem::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	switch (id) {
	case TAG::btn_price_descending:
		sortListByCost(false);
		updateGui("Price Descending");
		break;
	case TAG::btn_price_ascending:
		sortListByCost(true);
		updateGui("Price Ascending");
		break;
	case TAG::btn_prefer_break:
		sortListByTypeBox("Break");
		updateGui("Prefer Break");
		break;
	case TAG::btn_prefer_jump:
		sortListByTypeBox("Jump");
		updateGui("Prefer Jump");
		break;
	case TAG::btn_prefer_masses:
		sortListByTypeBox("Masses");
		updateGui("Prefer Masses");
		break;
	case TAG::box_sort:
	{
		_checkSelect = !_checkSelect;
		_nd_selection->setVisible(_checkSelect);
		_imgArrow->setRotation(_checkSelect?180:0);
		break;
	}
	default:
		return;
	}
}

void ShopTabItem::sortListByCost(bool asc)
{
	if (asc) sort(_listData.begin(), _listData.end(), CC_CALLBACK_2(ShopTabItem::compairCost, this));
	else sort(_listData.begin(), _listData.end(), CC_CALLBACK_2(ShopTabItem::compairCostDec, this));
	auto listItem = (QuickCustomTableView<ShopItemData, ShopItem> *) _listItem;
	listItem->reloadData(_listData);
}

bool ShopTabItem::compairCost(ShopItemData* i1, ShopItemData *i2)
{
	return i1->cost < i2->cost;
}

bool ShopTabItem::compairCostDec(ShopItemData* i1, ShopItemData *i2)
{
	return i1->cost > i2->cost;
}

void ShopTabItem::sortListByTypeBox(std::string typeBox)
{
	auto filterList = vector<ShopItemData*>();
	for (auto i : _listData)
	{
		auto cueBox = (ShopCueBoxData*)i;

		if (cueBox->name.find(typeBox) != -1)
		{
			filterList.push_back(i);
		}
	}
	
	auto listItem = (QuickCustomTableView<ShopItemData, ShopItem> *) _listItem;
	listItem->reloadData(filterList);
}

void ShopTabItem::searchBoxByName(const std::string & name)
{
	auto filterList = vector<ShopItemData*>();
	for (auto i : _listData)
	{
		auto cueBox = (ShopCueBoxData*)i;

		if (cueBox->name.find(name) != -1)
		{
			filterList.push_back(i);
		}
	}

	auto listItem = (QuickCustomTableView<ShopItemData, ShopItem> *) _listItem;
	listItem->reloadData(filterList);
}

void ShopTabItem::resetListData()
{
	_tfSearch->setString("Search..");
	auto listItem = (QuickCustomTableView<ShopItemData, ShopItem> *) _listItem;
	listItem->reloadData(_listData);
}

void ShopTabItem::updateGui(std::string title)
{
	_checkSelect = false;
	_nd_selection->setVisible(false);
	_lbTextBoxSort->setString(title);
	_imgArrow->setRotation(_checkSelect?180:0);
	_tfSearch->setString("Search..");
}

void ShopTabItem::hidePanelSelection()
{
	_checkSelect = false;
	_nd_selection->setVisible(false);
	_imgArrow->setRotation(_checkSelect?180:0);
}

