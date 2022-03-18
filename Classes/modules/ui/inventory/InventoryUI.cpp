#include "InventoryUI.h"
#include "../../../data/Resource.h"
#include "../../camera-view/GameCamera.h"
#include "../../scene/GameScene.h"
#include "../../../core/GameMgr.h"
#include "../elements/QuickCustomTableView.h"
#include "InventoryElementUI.h"
#include "../../../data/DataStruct.h"
#include "../../core/inventory/InventoryMgr.h"
#include "../../core/inventory/CueInfo.h"
#include "ModelOffset.h"
#include "InventoryItemElementUI.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

InventoryUI::InventoryUI()
{

}

InventoryUI::~InventoryUI()
{

}

bool InventoryUI::init()
{
	if (!BaseLayer::init("InventoryUI"))
	{
		return false;
	}

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::InventoryUIv2);
	this->initWithBinaryFile(filePath);

	this->_keep = true;

	return true;
}

void InventoryUI::initGUI()
{
	initTag();
	this->autoInitComponent(_layout);

	this->_drawNodeHeight = 300;
	getNodeInMap("fake")->setVisible(false);
	_bg = _nodeMap["bg"];
	_bg->setCameraMask((int)GameCamera::FLAG_BG_LOWEST, false);
	_btnTabBreak = (Button*)_nodeMap["btnTabBreak"];
	_btnTabMasse = (Button*)_nodeMap["btnTabMasse"];
	_btnTabJump  = (Button*)_nodeMap["btnTabJump"];
	_btnTabOther = (Button*)_nodeMap["btnTabOther"];
	
	_imgCueRackIconsBar = (ImageView *) _nodeMap["imgCueRackIconsBar"];
	_imgCueRack = (ImageView*)_nodeMap["imgCueRack"];

	this->initTabs();
	this->initCams();
	this->initCues();
	this->initRackDebugOffset();
}

void InventoryUI::initTag()
{
	_tagMap.insert(pair<string, int>("btnTabBreak", TAG::btnTabBreak));
	_tagMap.insert(pair<string, int>("btnTabJump", TAG::btnTabJump));
	_tagMap.insert(pair<string, int>("btnTabMasse", TAG::btnTabMasse));
	_tagMap.insert(pair<string, int>("btnTabOther", TAG::btnTabOther));
	_tagMap.insert(pair<string, int>("btnBack", TAG::btnBack));
	_tagMap.insert(pair<string, int>("btnCueActive1", TAG::btnCueActive1));
	_tagMap.insert(pair<string, int>("btnCueActive2", TAG::btnCueActive2));
	_tagMap.insert(pair<string, int>("btnCueActive3", TAG::btnCueActive3));
}

void InventoryUI::initTabs()
{
	auto frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
	auto winSize = Director::getInstance()->getWinSize();
	this->_curTab = -1;
	// one container for 4 cue tab
	auto container = _nodeMap["imgListBg"]->getChildByName("container");
	QuickCustomTableView<CueInfo, InventoryElementUI> *cueList = new QuickCustomTableView<CueInfo, InventoryElementUI>();
	cueList->init(container->getContentSize(), extension::ScrollView::Direction::VERTICAL, -1, 1);
	container->addChild(cueList);

	QuickCustomTableView <ItemInfo, InventoryItemElementUI> *itemList
		= new QuickCustomTableView<ItemInfo, InventoryItemElementUI>();
	itemList->init(container->getContentSize(), extension::ScrollView::Direction::VERTICAL, -1, 4);
	container->addChild(itemList);

	auto boundingBox = container->getBoundingBox();
	auto frameScale = frameSize.width / winSize.width * Director::getInstance()->getOpenGLView()->getFrameZoomFactor();

	boundingBox.size.width *= frameScale;
	boundingBox.size.height *= frameScale;
	boundingBox.origin.x += container->getParent()->getPositionX();
	boundingBox.origin.y += container->getParent()->getPositionY();

	auto callback = std::bind(&InventoryUI::onInitCueElement, this, std::placeholders::_1, boundingBox);
	cueList->setInitItemCallback(callback);
	itemList->setVisible(false);
	this->_cueList = cueList;
	this->_itemList = itemList;
	this->reloadTableViewData(false);
	// one container for the other tab

	this->_tabActivePosX = _btnTabBreak->getPositionX();
	this->_tabInactivePosX = _btnTabBreak->getPositionX();
	this->_btnTab.insert(_btnTab.begin(), { _btnTabBreak, _btnTabBreak, _btnTabJump, _btnTabMasse, _btnTabOther });
	_iconNameBtnTab[0] = "Yellow";
	_iconNameBtnTab[1] = "Red";
	_iconNameBtnTab[2] = "Purple";
	_iconNameBtnTab[3] = "Blue";
	_iconNameBtnTab[4] = "Other";
	for (auto button : _btnTab)
	{
		auto img = (ImageView*)button->getChildByName("img");
		img->ignoreContentAdaptWithSize(true);
	}
	this->switchTab(1);
}

void InventoryUI::initCams()
{
	auto winSize = Director::getInstance()->getWinSize();
	this->_cam = Camera::createOrthographic(
		winSize.width,
		winSize.height,
		-50,
		100
	);
	this->addChild(this->_cam);
	this->_cam->setDepth(3);
	this->_cam->setPosition3D(Vec3(0, 0, 50));
	this->_cam->lookAt(Vec3(0, 0, 0));
	this->_cam->setCameraFlag(GameCamera::FLAG_3D_ABOVE_UI);

	auto camUIHigher = Camera::createOrthographic(
		winSize.width,
		winSize.height,
		-1,
		1000
	);
	this->addChild(camUIHigher);
	camUIHigher->setDepth(4);
	camUIHigher->setPosition3D(Vec3(0, 0, 50));
	camUIHigher->lookAt(Vec3(0, 0, 0));
	camUIHigher->setCameraFlag(GameCamera::FLAG_UI_ALWAYS_TOP);
}

void InventoryUI::initCues()
{
	_imgCueRackIconsBar->setCameraMask((int)GameCamera::FLAG_UI_ALWAYS_TOP);
	_cues3d[0] = nullptr; _cues3d[1] = nullptr; _cues3d[2] = nullptr;
	for (auto i = 0; i < GameConstant::CUE_IN_BAG; i++) {
		map<string, Sprite3D*> cache;
		this->_cues3dCache.push_back(cache);
		ImageView *icon = (ImageView*) _imgCueRackIconsBar->getChildByName("btnCueActive" + to_string(i + 1))->getChildByName("icon");
		icon->ignoreContentAdaptWithSize(true);

		this->addCueModelToRack(i, Res::CUE_VANG_1, Res::CUE_TEXTURE_SET_1, Res::CUE_TEXTURE_SET_1_MRAO);
	}
}

void InventoryUI::initRackDebugOffset()
{

}

void InventoryUI::show()
{
	BaseLayer::show();
	refresh();
	auto scene = (GameScene*)gameMgr->_scene;
	scene->_lobbyUI->setVisible(false);
}

void InventoryUI::hide()
{
	BaseLayer::hide();
	auto scene = (GameScene*)gameMgr->_scene;
	scene->_lobbyUI->setVisible(true);
}

int InventoryUI::getTypeByTabIndex(int idx)
{
	int tabs[] = {1,2,3,4};
	return tabs[idx];
}

int InventoryUI::getTabIndexByType(int type)
{
	return type - 1;
}

void InventoryUI::refresh()
{
	reloadTableViewData(true);
	reloadCue3DView();
}

void InventoryUI::reloadTableViewData(bool keep)
{
	QuickCustomTableView<CueInfo, InventoryElementUI> *cueList = (QuickCustomTableView<CueInfo, InventoryElementUI> *) _cueList;
	auto minOffset = cueList->_tbView->minContainerOffset();
	int type = getTypeByTabIndex(_curTab);
	std::vector<CueInfo*> data = inventoryMgr->getInventory()->getCueArray(type);
	cueList->reloadData(data);

	QuickCustomTableView<ItemInfo, InventoryItemElementUI> *itemList
		= (QuickCustomTableView<ItemInfo, InventoryItemElementUI> *)_itemList;
	auto itemMinOffset = itemList->_tbView->minContainerOffset();
	Vec2 offset;
	if (keep) {
		offset = itemList->_tbView->getContentOffset();
	}
	std::vector<ItemInfo*> itemDataList = inventoryMgr->getInventory()->getNonZeroItemsArray();
	itemList->reloadData(itemDataList);
	if (keep && offset.y > itemMinOffset.y) {
		itemList->_tbView->setContentOffset(offset);
	}
}

void InventoryUI::reloadCue3DView()
{
	auto cuesBag = inventoryMgr->getInventory()->getCuesBagArray();
	auto count = cuesBag.size();
	for (int i = 0; i < GameConstant::CUE_IN_BAG; i++) {
		this->_cues3d[i]->setVisible(i < count);
		auto icon = (ImageView*)_imgCueRackIconsBar->getChildByName("btnCueActive" + to_string(i + 1))->getChildByName("icon");
		if (i < count) {
			// const state = this->_cues3d[i].state;
			// const color = InventoryElementUI.TYPE_FRAMES[cuesBag[i].type - 1].color;
			auto paths = cuesBag[i]->getModelPath();
			this->addCueModelToRack(i, paths.model, paths.tex, paths.mrao);
			string texture;
			switch (cuesBag[i]->_type) {
			case 1: texture = "inventory.v2_Yellow_Icon_02.png"; break;
			case 2: texture = "inventory.v2_Red_Icon_02.png"; break;
			case 3: texture = "inventory.v2_Purple_Icon_02.png"; break;
			case 4: texture = "inventory.v2_Blue_Icon_02.png"; break;
			}
			icon->setVisible(true);
			icon->loadTexture(texture, Widget::TextureResType::PLIST);
		}
		else {
			icon->setVisible(false);
		}
	}
}

void InventoryUI::addCueModelToRack(int index, const std::string & path, const std::string &texPath, const std::string &mraoPath)
{
	auto winSize = Director::getInstance()->getWinSize();
	if (this->_cues3d[index])
		this->_cues3d[index]->setVisible(false);
	if (_cues3dCache[index].find(path) == _cues3dCache[index].end() 
		|| !this->_cues3dCache[index][path]) 
	{
		auto rack = _imgCueRack;
		auto distance = 180 / GameConstant::CUE_IN_BAG;

		auto scale = path.find("Stick") >= 0 ? 2.0 : 1.5;
		scale *= winSize.width / 1136;
		auto cue = Sprite3D::create(path);
		_cues3dCache[index].insert(pair<string, Sprite3D*>(path, cue));
		cue->setCameraMask((int)GameCamera::FLAG_3D_ABOVE_UI);

		auto cueWidth = scale * modelOffset->getModelWidthByPath(path);
		auto rackSize = rack->getContentSize();
		cue->setPosition(rackSize.width / 2 + distance * (index - (GameConstant::CUE_IN_BAG*1.f / 2 - 0.5)), 20 + cueWidth);
		cue->setScale(scale);
		cue->setRotation3D(Vec3(270, 90, 0));
		rack->addChild(cue);
	}
	this->_cues3dCache[index][path]->setTexture(texPath);
	if (index == 0) this->dy = 0;
	this->_cues3d[index] = this->_cues3dCache[index][path];
	this->_cues3d[index]->setVisible(true);
}

void InventoryUI::selectActivatingCue(int cueIdx)
{
	auto cuesBag = inventoryMgr->getInventory()->getCuesBagArray();
	if (cueIdx >= cuesBag.size()) return;
	auto cue = cuesBag[cueIdx];
	auto type = cue->_type;
	switchTab(getTabIndexByType(type));
	QuickCustomTableView<CueInfo, InventoryElementUI> *cueList = (QuickCustomTableView<CueInfo, InventoryElementUI> *)_cueList;
	auto minOffset = cueList->_tbView->minContainerOffset().y;
	auto size = cueList->_size.height / cueList->_primaryDirCount;
	auto data = inventoryMgr->getInventory()->getCueArray(type);
	int idx = 0;
	for (int i = 0; i < data.size(); i++)
	{
		if (data[i] == cue)
		{
			idx = i;
		}
	}
	cueList->_tbView->setContentOffset(Vec2(0, minOffset + size * idx), true);
}

void InventoryUI::switchTab(int tabIdx)
{
	if (this->_curTab == tabIdx) return;
	auto oldTab = this->_curTab;
	this->_curTab = tabIdx;

	auto newIconName = _iconNameBtnTab[tabIdx];
	if (oldTab >= 0) {
		auto oldIconName = _iconNameBtnTab[oldTab];
		auto btn = _btnTab[oldTab];
		btn->setPositionX(this->_tabInactivePosX);
		btn->loadTextures("inventory.v2_Tab_02.png", "inventory.v2_Tab_02.png", "inventory.v2_Tab_02.png", Widget::TextureResType::PLIST);
		auto img = (ImageView*)btn->getChildByName("img");
		img->loadTexture("inventory.v2_" + oldIconName + "_Icon_01.png", Widget::TextureResType::PLIST);
		img->setPosition(Vec2(btn->getContentSize().width / 2, btn->getContentSize().height / 2));
	}

	auto curBtn = this->_btnTab[tabIdx];
	curBtn->setPositionX(this->_tabActivePosX);
	curBtn->loadTextures("inventory.v2_Tab_01.png", "inventory.v2_Tab_01.png", "inventory.v2_Tab_01.png", Widget::TextureResType::PLIST);
	auto img = (ImageView*)curBtn->getChildByName("img");
	img->loadTexture("inventory.v2_" + newIconName + "_Icon_02.png", Widget::TextureResType::PLIST);
	img->setPosition(Vec2(curBtn->getContentSize().width / 2, curBtn->getContentSize().height / 2));

	// utils.log("Switch to tab", tabIdx);
	if (tabIdx < 4) {
		this->_cueList->setVisible(true);
		this->_itemList->setVisible(false);
	}
	else {
		this->_cueList->setVisible(false);
		this->_itemList->setVisible(true);
	}
	this->reloadTableViewData(false);
}

void InventoryUI::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	switch (id) {
	case TAG::btnTabMasse: switchTab(getTabIndexByType(4)); break;
	case TAG::btnTabBreak: switchTab(getTabIndexByType(2)); break;
		//case TAG::btnTabNeutral: switchTab(getTabIndexByType(1)); break;
	case TAG::btnTabJump: switchTab(getTabIndexByType(3)); break;
	case TAG::btnTabOther: switchTab(4); break;
	case TAG::btnBack: hide(); break;
	case TAG::btnCueActive1: selectActivatingCue(0); break;
	case TAG::btnCueActive2: selectActivatingCue(1); break;
	case TAG::btnCueActive3: selectActivatingCue(2); break;
	}
}

void InventoryUI::onInitCueElement(InventoryElementUI *item, const cocos2d::Rect & rect)
{
	item->setContainerRect(rect);
}

