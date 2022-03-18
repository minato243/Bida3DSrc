#include "ShopUI.h"
#include "../../../data/Resource.h"
#include "../../camera-view/GameCamera.h"
#include "../../../core/GameMgr.h"
#include "../../../core/utils/StringUtility.h"
#include "ShopTabItem.h"
#include "core/gui/SceneMgr.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;


ShopUI::ShopUI()
{

}

ShopUI::~ShopUI()
{

}

bool ShopUI::init()
{
	if (!BaseLayer::init("ShopUI"))
	{
		return false;
	}

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::ShopGUIv3);
	this->initWithBinaryFile(filePath);

	this->_keep = true;
	//this->setCameraMask((int)GameCamera::FLAG_UI, true);
	return true;
}

void ShopUI::initGUI()
{
	initTag();
	this->autoInitComponent(_layout);
	_bg = _nodeMap["bg"];
	_container = (Widget*)_nodeMap["container"];
	_lbGold = (cocos2d::ui::Text *) _nodeMap["lbGold"];
	_lbCC = (cocos2d::ui::Text *) _nodeMap["lbCC"];

	this->initTabs();
	this->initData();
	this->initSubscribers();
	this->refresh();
	this->initTimerScroll();
}

void ShopUI::initTag()
{
	_tagMap.insert(pair<string, int>("btnBack", TAG::btnBack));
	_tagMap.insert(pair<string, int>("btnTab0", TAG::btnTab0));
	_tagMap.insert(pair<string, int>("btnTab1", TAG::btnTab1));
	_tagMap.insert(pair<string, int>("btnTab2", TAG::btnTab2));
	_tagMap.insert(pair<string, int>("btnTab3", TAG::btnTab3));
}

void ShopUI::initTimerScroll()
{
	schedule(schedule_selector(ShopUI::updateScroll), 0.2f);
}

void ShopUI::updateScroll(float dt)
{
	auto offset = convertPosToOffset(_scrollView->getInnerContainerPosition().y);
	if (_curOffset != offset) {
		_curOffset = offset;
		showTabSelected(_curOffset);
	}
}

int ShopUI::convertPosToOffset(float posY)
{
	auto pos = abs(posY);
	auto n = numTab;
	for (auto i = 0; i < n; i++) {
		if (pos < this->_listHeight[i]) {
			return n - i - 1;
		}
	}
}

void ShopUI::onSceneReload()
{
	this->refresh();
	this->switchTab(0, true);
	this->initInterval();
}

void ShopUI::reloadGUIHotDeals()
{
	this->switchTab(0, true);
	ShopTabLimited *tabLimited = (ShopTabLimited *)_listTabs[0];
	tabLimited->reloadHotDeals();
}

void ShopUI::initInterval()
{
	this->initTimerScroll();
	ShopTabLimited *tabLimited = (ShopTabLimited *)_listTabs[0];
	tabLimited->initTimer();
}

void ShopUI::initData()
{
	_listBtnTab[0]->loadTextures(SELECT_IMG_BUTTON[0], "", "");
	_listTabs.clear();
	_listTabs.push_back(ShopTabLimited::create());
	ShopCategory tabsCategory[] = {ShopCategory::LIMITED, ShopCategory::CUE, ShopCategory::GOLD, ShopCategory::CASH};
	for (auto i = 1; i < 4; i++)
		_listTabs.push_back(ShopTabItem::create(tabsCategory[i]));
	auto totalHeight = 0;
	for (auto i = 0; i < 4; i++) {
		totalHeight += this->_listTabs[i]->getOriginalSize().height;
	}

	auto container = _container;
	auto size = container->getContentSize();
	auto scrollView = ScrollView::create();
	scrollView->setDirection(ScrollView::Direction::VERTICAL);
	scrollView->setTouchEnabled(true);
	scrollView->setBounceEnabled(true);
	scrollView->setContentSize(size);
	scrollView->setInnerContainerSize(Size(size.width, totalHeight + 30));
	container->addChild(scrollView);
	this->_scrollView = scrollView;

	auto h = 0;
	this->_totalHeight = totalHeight;
	for (auto i = 3; i >= 0; i--) {
		_listTabs[i]->setPosition(Vec2(0, h));
		h += _listTabs[i]->getOriginalSize().height + 10;
		_listHeight.push_back(h);
		scrollView->addChild(_listTabs[i]);
	}
}

void ShopUI::initTabs()
{
	_btnTab0 = (cocos2d::ui::Button *) _nodeMap["btnTab0"];
	_btnTab1 = (cocos2d::ui::Button *) _nodeMap["btnTab1"];
	_btnTab2 = (cocos2d::ui::Button *) _nodeMap["btnTab2"];
	_btnTab3 = (cocos2d::ui::Button *) _nodeMap["btnTab3"];

	this->_listBtnTab[0] = _btnTab0;
	this->_listBtnTab[1] = _btnTab1;
	this->_listBtnTab[2] = _btnTab2;
	this->_listBtnTab[3] = _btnTab3;
}

void ShopUI::initSubscribers()
{

}

void ShopUI::refresh()
{
	this->setGoldValue(gameMgr->_userInfo->getGold());
	this->setCashValue(gameMgr->_userInfo->getCash());
	ShopTabItem *tabItem = (ShopTabItem *)_listTabs[1];
	tabItem->resetListData();
}

void ShopUI::setGoldValue(int gold)
{
	_lbGold->setString(StringUtility::formatNumberSymbol(gold));
}

void ShopUI::setCashValue(int cash)
{
	_lbCC->setString(StringUtility::formatNumberSymbol(cash));
}

void ShopUI::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	switch (id) {
	case TAG::btnBack:
		this->onRequestClose();
		break;
	case TAG::btnTab0:
		this->switchTab(0);
		break;
	case TAG::btnTab1:
		this->switchTab(1);
		break;
	case TAG::btnTab2:
		this->switchTab(2);
		break;
	case TAG::btnTab3:
		this->switchTab(3);
		break;
	default:
		return;
	}
}

void ShopUI::switchTab(int id, bool isInstant /*= false*/)
{
	auto time = 1;
	if (isInstant) {
		time = 0;
		this->showTabSelected(id);
	}
	this->_scrollView->scrollToPercentVertical(100 * (1 - (this->_listHeight[3 - id] - 160 * id) / this->_totalHeight), time, 1);
	ShopTabItem *tabItem = (ShopTabItem*)_listTabs[1];
	if (tabItem->_checkSelect)
		tabItem->hidePanelSelection();
}

void ShopUI::showTabSelected(int id)
{
	for (auto i = 0; i < 4; i++) {
		if (i == id) {
			_listBtnTab[i]->getChildByName("pnSelect")->setVisible(true);
			_listBtnTab[i]->loadTextures(SELECT_IMG_BUTTON[i], "", "");
		}
		else {
			_listBtnTab[i]->getChildByName("pnSelect")->setVisible(false);
			_listBtnTab[i]->loadTextures(TAB_IMG_BUTTON[i], "", "");
		}
	}
}

void ShopUI::onRequestClose()
{
	sceneMgr->loadSceneByTag(SceneTag::MAIN_SCENE);
}

std::string ShopUI::SELECT_IMG_BUTTON[ShopUI::numTab] =
{
	"GUI/BidaGUI/cocosstudio/Shop/res/BLACKMARKET/Hot_01.png",
	"GUI/BidaGUI/cocosstudio/Shop/res/BLACKMARKET/Box_01.png",
	"GUI/BidaGUI/cocosstudio/Shop/res/BLACKMARKET/Gold_01.png",
	"GUI/BidaGUI/cocosstudio/Shop/res/BLACKMARKET/Dollar_01.png"
};

std::string ShopUI::TAB_IMG_BUTTON[ShopUI::numTab] = 
{
	"GUI/BidaGUI/cocosstudio/Shop/res/BLACKMARKET/Hot_02.png",
	"GUI/BidaGUI/cocosstudio/Shop/res/BLACKMARKET/Box_02.png",
	"GUI/BidaGUI/cocosstudio/Shop/res/BLACKMARKET/Gold_02.png",
	"GUI/BidaGUI/cocosstudio/Shop/res/BLACKMARKET/Dollar_02.png"
};

