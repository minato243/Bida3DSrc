#include "CueCollectionUI.h"
#include "data/Resource.h"
#include "modules/core/inventory/InventoryMgr.h"
#include "modules/camera-view/GameCamera.h"
#include "base/UIFactory.h"
#include "modules/scene/GameScene.h"
#include "core/GameMgr.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;
USING_NS_CC_EXT;

CueCollectionUI::CueCollectionUI()
{
	_keep = true;
	_curTab = 0;
	_tableView = nullptr;
}

CueCollectionUI::~CueCollectionUI()
{
}

bool CueCollectionUI::init()
{
	if (!BaseLayer::init("CueCollectionUI"))
	{
		return false;
	}

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::CueCollectionUI);
	this->initWithBinaryFile(filePath);

	this->_keep = true;
	this->initCamera();
	this->retain();
	return true;
}

void CueCollectionUI::initGUI()
{
	_tagMap.insert(std::pair<string, int>("btnTabMasse", TAG::btnTabMasse));
	_tagMap.insert(std::pair<string, int>("btnTabBreak", TAG::btnTabBreak));
	_tagMap.insert(std::pair<string, int>("btnTabJump", TAG::btnTabJump));
	_tagMap.insert(std::pair<string, int>("btnBack", TAG::btnBack));
	autoInitComponent(_layout);
	getNodeInMap("fake")->setVisible(false);

	_imgBg = (ImageView*)getNodeInMap("imgBg");
	_imgBg->setCameraMask((int)GameCamera::FLAG_BG_LOWEST, false);

	_pnDockCount = (Widget *)getNodeInMap("pnDockCount");
	_imgType = (ImageView*)_pnDockCount->getChildByName("imgPanel")->getChildByName("imgType");
	_imgType->ignoreContentAdaptWithSize(true);
	_lbCount = (cocos2d::ui::Text *) getNodeInMap("lbCount");

	_pnContent = (Widget *)getNodeInMap("pnContent");

	_imgTabBar = (ImageView*)getNodeInMap("imgTabBar");
	_btnTabJump = (Button*)getNodeInMap("btnTabJump");
	_btnTabBreak = (Button*)getNodeInMap("btnTabBreak");
	_btnTabMasse = (Button*)getNodeInMap("btnTabMasse");

	this->initList();
	this->initTabs();
}

void CueCollectionUI::show()
{
	BaseLayer::show();
	this->refresh();

	//MainLayer.instance.setVisible(false);
	//if (gv.GuiMgr.isCached(Popup.LOBBY_UI)) {
	//	gv.GuiMgr.getGui(Popup.LOBBY_UI, LobbyUILayer).setVisible(false);
	//}
}

void CueCollectionUI::hide()
{
	BaseLayer::hide();

	//MainLayer.instance.setVisible(true);
	//if (gv.GuiMgr.isCached(Popup.LOBBY_UI)) {
	//	gv.GuiMgr.getGui(Popup.LOBBY_UI, LobbyUILayer).setVisible(true);
	//}
	auto scene = (GameScene*)gameMgr->_scene;
	scene->_lobbyUI->setVisible(true);
}

void CueCollectionUI::refresh()
{
	this->updateTab();

}

void CueCollectionUI::updateTab()
{
	if (this->_curTab < 0) return;
	int types[] = {3,4,2,1};
	auto type = types[this->_curTab];
	std::string iconName[] = {
		"Icon_Active_02.png",
		"Icon_Active_02.png",
		"Icon_Active_03.png",
		"Icon_Active_04.png"
	};

	auto typeData = inventoryMgr->getCueCollectionData(type);
	int numOwned = 0;
	for (int i = 0; i < typeData.size(); i++)
	{
		if (typeData[i]->owned == true)
			numOwned++;
	}

	auto img = (ImageView*)_pnDockCount->getChildByName("imgPanel")->getChildByName("imgType");
	auto countString = to_string(numOwned) + "/" + std::to_string(typeData.size());
	img->loadTexture(iconName[type-1], Widget::TextureResType::PLIST);
	_lbCount->setString(countString);

	auto scale = 40 / img->getContentSize().width;
	img->setScale(scale);
}

void CueCollectionUI::initCamera()
{
	auto winSize = Director::getInstance()->getVisibleSize();
	this->_cam = Camera::createOrthographic(
		winSize.width,
		winSize.height,
		-10,
		50
	);
	this->addChild(this->_cam);
	this->_cam->setDepth(3);
	this->_cam->setPosition3D(Vec3(0, 0, 10));
	this->_cam->lookAt(Vec3(0, 0, 0));
	this->_cam->setCameraFlag(GameCamera::FLAG_3D_ABOVE_UI);

	this->_cam = Camera::createOrthographic(
		winSize.width,
		winSize.height,
		-10,
		50
	);
	this->addChild(this->_cam);
	this->_cam->setDepth(4);
	this->_cam->setPosition3D(Vec3(0, 0, 10));
	this->_cam->lookAt(Vec3(0, 0, 0));
	this->_cam->setCameraFlag(GameCamera::FLAG_UI_HIGHEST);

	this->_cam = Camera::createOrthographic(
		winSize.width,
		winSize.height,
		-10,
		50
	);

	this->addChild(this->_cam);
	this->_cam->setDepth(5);
	this->_cam->setPosition3D(Vec3(0, 0, 10));
	this->_cam->lookAt(Vec3(0, 0, 0));
	this->_cam->setCameraFlag(GameCamera::FLAG_UI_ALWAYS_TOP);
}

void CueCollectionUI::initTabs()
{
	this->_curTab = 1;
	this->_tabActivePosX = _btnTabMasse->getPositionX();
	this->_tabInactivePosX = _btnTabBreak->getPositionX();
	_btnTab.insert(_btnTab.begin(), { _btnTabJump, _btnTabMasse, _btnTabBreak });
	_iconNameBtnTabs[0] = "3";
	_iconNameBtnTabs[1] = "4";
	_iconNameBtnTabs[2] = "2";
	for (int i = 0; i < _btnTab.size(); i++)
	{
		auto btn = _btnTab[i];
		((ImageView*)btn->getChildByName("img"))->ignoreContentAdaptWithSize(true);
	}
	getNodeInMap("imgTabActive0")->setVisible(false);
	getNodeInMap("imgTabActive1")->setVisible(false);
	getNodeInMap("imgTabActive2")->setVisible(false);
	this->switchTab(0);
}

void CueCollectionUI::switchTab(int tabIdx)
{
	if (this->_curTab == tabIdx) return;
	auto oldTab = this->_curTab;
	this->_curTab = tabIdx;

	auto oldIconName = _iconNameBtnTabs[oldTab];
	auto newIconName = _iconNameBtnTabs[tabIdx];
	((ImageView*)this->_btnTab[oldTab]->getChildByName("img"))->loadTexture("Icon_Inactive_0" + oldIconName + ".png", Widget::TextureResType::PLIST);
	((ImageView*)this->_btnTab[tabIdx]->getChildByName("img"))->loadTexture("Icon_Inactive_0" + newIconName + ".png", Widget::TextureResType::PLIST);

	_imgTabBar->getChildByName("imgTabActive" + to_string(oldTab))->setVisible(false);
	_imgTabBar->getChildByName("imgTabActive" + to_string(tabIdx))->setVisible(true);

	this->reloadTableViewData(false);
	this->updateTab();
}

int CueCollectionUI::getTypeByTabIndex(int idx)
{
	int types[] = {3,4,2,1};
	return types[idx];
}

void CueCollectionUI::reloadTableViewData(bool keep)
{
	_dataList = inventoryMgr->getCueCollectionData(getTypeByTabIndex(_curTab));
	_tableView->reloadData();
}

void CueCollectionUI::initList()
{
	auto panel = _pnContent;
	auto sideBarSize = _imgTabBar->getContentSize();
	auto scaleFactor = UIFactory::getScaleFactor();
	auto scaleWidth = scaleFactor.width;
	auto scaleHeight = scaleFactor.height;
	sideBarSize.width *= scaleWidth;
	sideBarSize.height *= scaleHeight;

	TableView *tableView = TableView::create(this, panel->getContentSize());
	tableView->setDirection(extension::ScrollView::Direction::HORIZONTAL);
	panel->addChild(tableView);
	_dataList = inventoryMgr->getCueCollectionData(getTypeByTabIndex(_curTab));
	tableView->reloadData();
	
	_tableView = tableView;
}

void CueCollectionUI::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	switch (id) {
	case TAG::btnTabMasse:
		this->switchTab(1);
		break;
	case TAG::btnTabBreak:
		this->switchTab(2);
		break;
	case TAG::btnTabJump:
		this->switchTab(0);
		break;
	case TAG::btnBack:
		this->hide();
		break;
	}
}

cocos2d::extension::TableViewCell* CueCollectionUI::tableCellAtIndex(TableView *table, ssize_t idx)
{
	auto cell = table->dequeueCell();
	CueCollectionCueItem *item;
	if (!cell)
	{
		cell = TableViewCell::create();
		auto cellSize = tableCellSizeForIndex(table, idx);
		item = new CueCollectionCueItem();
		item->init();
		cell->addChild(item);
		item->setPosition(Vec2(cellSize.width/2, cellSize.height/2));
		item->setTag(1001);
	}
	else
	{
		item = (CueCollectionCueItem*)cell->getChildByTag(1001);
		
	}
	item->setIdx(idx);
	item->loadData(_dataList[idx]);

	return cell;
}

ssize_t CueCollectionUI::numberOfCellsInTableView(TableView *table)
{
	return _dataList.size();
}

void CueCollectionUI::tableCellTouched(TableView* table, TableViewCell* cell)
{
	
}

cocos2d::Size CueCollectionUI::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	return Size(200, 500);
}

void CueCollectionUI::setVisible(bool visible)
{
	Node::setVisible(visible);
	if(_tableView)
		_tableView->setVisible(visible);
}

void CueCollectionUI::onEnter()
{
	BaseLayer::onEnter();
	_tableView->onEnter();
}

const std::string CueCollectionUI::className = "CueCollectionUI";

