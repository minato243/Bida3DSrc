#include "CueCollectionUIDetail.h"
#include "data/Resource.h"
#include "../../GuiMgr.h"
#include "base/UIFactory.h"
#include "modules/core/inventory/InventoryMgr.h"
#include "CueCollectionCueItem.h"
#include "data/config/ConfigMgr.h"
#include "../CueInspectUI.h"
#include "../InventoryElementUI.h"
#include "core/utils/LanguageMgr.h"
#include "modules/camera-view/GameCamera.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace std;


CueCollectionUIDetail::CueCollectionUIDetail()
{
	_keep = true;
	_tag = Popup::Cue_COLLECTION_DETAIL;
}

CueCollectionUIDetail::~CueCollectionUIDetail()
{
}

bool CueCollectionUIDetail::init()
{
	if (!BaseLayer::init("CueCollectionUIDetail"))
	{
		return false;
	}

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::CueCollectionUIDetail);
	this->initWithBinaryFile(filePath);

	getNodeInMap("btnPlaytest")->setCameraMask((int)GameCamera::FLAG_UI_HIGHEST);
	getNodeInMap("imgPanelInfo")->setCameraMask((int)GameCamera::FLAG_UI_HIGHEST);

	initBar();
	initList();
	return true;
}

void CueCollectionUIDetail::initGUI()
{
	_tagMap.insert(pair<string, int>("btnBack", TAG::BTN_BACK));
	autoInitComponent(_layout);
	getNodeInMap("fake")->setVisible(false);

	_bg = (ImageView*)getControl("bg", _layout);
	_pnContent = (Widget*)getControl("pnContent", _bg);
}

void CueCollectionUIDetail::initBar()
{
	_pnForce = (Widget*)_nodeMap["pnForce"];
	_pnSpin = (Widget*)_nodeMap["pnSpin"];
	_pnAngle = (Widget*)_nodeMap["pnAngle"];
	_pnAim = (Widget*)_nodeMap["pnAim"];
	_pnAccuracy = (Widget*)_nodeMap["pnAccuracy"];

	_pnForceWrapper = new StatBarPanel(_pnForce);
	_pnForceWrapper->_stencil->setCameraMask((int)GameCamera::FLAG_UI_HIGHEST);
	_pnSpinWrapper = new StatBarPanel(_pnSpin);
	_pnSpinWrapper->_stencil->setCameraMask((int)GameCamera::FLAG_UI_HIGHEST);
	_pnAimWrapper = new StatBarPanel(_pnAim);
	_pnAimWrapper->_stencil->setCameraMask((int)GameCamera::FLAG_UI_HIGHEST);
	_pnAccuracyWrapper = new StatBarPanel(_pnAccuracy);
	_pnAccuracyWrapper->_stencil->setCameraMask((int)GameCamera::FLAG_UI_HIGHEST);

	_pnAngleWrapper = new StatBarPanelAngle(_pnAngle);
	_pnAngleWrapper->_stencil->setCameraMask((int)GameCamera::FLAG_UI_HIGHEST);

	_pnAccuracyWrapper->setPostfix("%");
}

void CueCollectionUIDetail::initList()
{
	auto panel = _pnContent;
	auto scaleFactor = UIFactory::getScaleFactor();
	auto scaleWidth = scaleFactor.width;
	auto scaleHeight = scaleFactor.height;

	_tableView = new CueCollectionTableView();
	auto size = panel->getContentSize();
	auto itemSize = CueCollectionCueItem::defaultSize;
	_tableView->init(size, size.width/itemSize.width, 1);
	_tableView->_mainUI = this;
	panel->addChild(_tableView);
}

void CueCollectionUIDetail::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	switch (id)
	{
	case TAG::BTN_BACK:
		hide();
		break;
	default:
		break;
	}
}

void CueCollectionUIDetail::show()
{
	BaseLayer::show();
	auto ccUI = guiMgr->getGui(Popup::CUE_COLLECTION_UI);
	if(ccUI)
		ccUI->setVisible(false);
}

void CueCollectionUIDetail::hide()
{
	BaseLayer::hide();
	auto ccUI = guiMgr->getGui(Popup::CUE_COLLECTION_UI);
	if(ccUI)
		ccUI->setVisible(true);
}

void CueCollectionUIDetail::loadData(int type, int idx)
{
	this->_type = type;
	this->_curIdx = idx;
	_dataList = inventoryMgr->getCueCollectionData(type);
	_nullPadding = 1;

	for (int i = 0; i < _nullPadding; i++) {
		_dataList.insert(_dataList.begin(), nullptr);
		_dataList.push_back(nullptr);
	}

	_tableView->reloadData(_dataList);
	this->loadCueInfo(_dataList[idx + _nullPadding]);
	this->_tableView->scrollToIndex(idx + _nullPadding);
	this->updateTab();
}

void CueCollectionUIDetail::loadCueInfo(CueInfoData *cue)
{

	if (cue == nullptr) {
		this->setName(-1);
		this->setStar(0);
		_pnForceWrapper->setDefaultValue();
		_pnSpinWrapper->setDefaultValue();
		_pnAngleWrapper->setDefaultValue();
		_pnAimWrapper->setDefaultValue();
		_pnAccuracyWrapper->setDefaultValue();
		return;
	}
	auto type = cue->type;
	auto star = cue->star;
	auto level = 10;

	auto config = CueInfo::getCueConfig(type, star, level);
	auto acc = CueInfo::getCueAccuracyConfig(type, star);
	auto force = config.force;
	auto angular = config.tip_u * 100;
	auto angle = config.angle;
	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	auto accuracy = r * (acc.accuracy[1] - acc.accuracy[0]) + acc.accuracy[0];
	auto aim = rand()%(acc.trajectory[1] - acc.trajectory[0]) + acc.trajectory[0];
	_pnForceWrapper->setInfo(floor(force), force / CueInspectUI::MAX_FORCE);
	_pnSpinWrapper->setInfo(floor(angular), angular / CueInspectUI::MAX_ANGULAR);
	_pnAngleWrapper->setInfo(floor(angle[0]), floor(angle[1]), (angle[1] - angle[0]) / CueInspectUI::MAX_ANGLE);
	_pnAimWrapper->setInfo(floor(aim), aim / CueInspectUI::MAX_AIM);
	_pnAccuracyWrapper->setInfo(floor(accuracy*100), accuracy);

	auto iconName = InventoryElementUI::TYPE_FRAMES[type - 1].icon;
	((Sprite*)getNodeInMap("imgTitleBar")->getChildByName("imgType"))->setSpriteFrame(iconName);
	((Sprite*)getNodeInMap("imgTitleBar")->getChildByName("imgType1"))->setSpriteFrame(iconName);

	this->setName(type);
	this->setStar(star);
}

void CueCollectionUIDetail::setName(int type)
{
	std::string name;
	switch (type) {
	case -1: name = "???"; break;
	case 1: name = languageMgr->localizedText("name_cue_song_toan"); break;
	case 2: name = languageMgr->localizedText("name_cue_boc_pha"); break;
	case 3: name = languageMgr->localizedText("name_cue_cau_bong"); break;
	case 4: name = languageMgr->localizedText("name_cue_xoan_oc"); break;
	}

	((Text*)getNodeInMap("lbTitle"))->setText(name);
}

void CueCollectionUIDetail::setStar(int star)
{
	for (int i = 1; i <= 5; i++) {
		if (i <= star) {
			((ImageView*)getNodeInMap("star_" + to_string(i)))->loadTexture("inven_Star_02.png", Widget::TextureResType::PLIST);
		}
		else {
			((ImageView*)getNodeInMap("star_" + to_string(i)))->loadTexture("inven_Star_01.png", Widget::TextureResType::PLIST);
		}
	}
}

void CueCollectionUIDetail::updateTab()
{
	auto type = this->_type;
	std::string iconName[] = {
		"Icon_Active_02.png",
		"Icon_Active_02.png",
		"Icon_Active_03.png",
		"Icon_Active_04.png"
	};

	auto typeData = this->_dataList;
	int numOwned = 0;
	for (int i = 0; i < typeData.size(); i++)
	{
		if (typeData[i] && typeData[i]->owned)
			numOwned++;
	}

	auto img = (ImageView*)getNodeInMap("imgPanel")->getChildByName("imgType");
	char text[20];
	sprintf(text, "%d/%d", numOwned, typeData.size() - _nullPadding *2);
	img->loadTexture(iconName[type-1], Widget::TextureResType::PLIST);
	((Text*)getNodeInMap("lbCount"))->setText(text);

	auto scale = 40 / img->getContentSize().width;
	img->setScale(scale);
}

void CueCollectionUIDetail::scrollToData(CueInfoData *data)
{
	auto collectionData = this->_dataList;
	int idx = -1;
	for (int i = 0; i < collectionData.size(); i++)
	{
		if (collectionData[i] && collectionData[i]->star == data->star && collectionData[i]->configId == data->configId)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0) {
		return;
	}

	_tableView->scrollToIndex(idx);
}

void CueCollectionUIDetail::setActiveItem(int idx)
{
	_curIdx = idx;
	loadCueInfo(_dataList[idx]);
}

//CueCollectionTableView------------------------------------

CueCollectionTableView::CueCollectionTableView()
{
	_tbView = nullptr;
	_isTouching = false;
	_mainUI = nullptr;
}

CueCollectionTableView::~CueCollectionTableView()
{
}

bool CueCollectionTableView::init(cocos2d::Size size, float primaryDirCount, float alternativeDirCount)
{
	_size = size;
	_primaryDirCount = primaryDirCount;
	Layer::init();
	_tbView = TableView::create(this, size);
	_tbView->setDelegate(this);
	_tbView->setDirection(extension::ScrollView::Direction::HORIZONTAL);

	_button = Button::create(Res::Base::img_btn_normal);
	_button->setSwallowTouches(false);
	_button->setPosition(Vec2(0, 0));
	_button->setAnchorPoint(Vec2(0, 0));
	_button->setZoomScale(0);
	_button->setScale(size.width / 46, size.height / 36);
	_button->addTouchEventListener(CC_CALLBACK_2(CueCollectionTableView::touchBtn, this));
	_button->setOpacity(0);
	
	addChild(_tbView);
	addChild(_button);

	scheduleUpdate();

	return true;
}

void CueCollectionTableView::touchBtn(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType type)
{
	Button *btn = (Button*)sender;
	switch (type) {
	case Widget::TouchEventType::BEGAN:
		_isTouching = true;
		_isScrollToNearestCell = false;
		break;
	case Widget::TouchEventType::MOVED:
	{
		break;
	}
		
	case Widget::TouchEventType::ENDED:
	case Widget::TouchEventType::CANCELED:
		_isTouching = false;
		break;
	}
	if (_isTouching)
	{
		_autoSnap = true;
	}
}

cocos2d::Size CueCollectionTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	return Size(200, 500);
}

cocos2d::extension::TableViewCell* CueCollectionTableView::tableCellAtIndex(TableView *table, ssize_t idx)
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
		item->setPosition(Vec2(cellSize.width / 2, cellSize.height / 2));
		item->setTag(1001);
		item->setModeDetail(true);
	}
	else
	{
		item = (CueCollectionCueItem*)cell->getChildByTag(1001);
	}
	item->loadData(_dataList[idx]);

	return cell;
}

ssize_t CueCollectionTableView::numberOfCellsInTableView(TableView *table)
{
	return _dataList.size();
}

void CueCollectionTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{
}

void CueCollectionTableView::setData(const std::vector<CueInfoData*> & data)
{
	_dataList.clear();
	_dataList.insert(_dataList.begin(), data.begin(), data.end());
}

void CueCollectionTableView::reloadData(const std::vector<CueInfoData*> & data)
{
	setData(data);
	_tbView->reloadData();
}

void CueCollectionTableView::scrollToIndex(int index)
{
	if (_dataList[index] == nullptr) {
		while (_dataList[index] == nullptr && index > _dataList.size() / 2) {
			index--;
		}
		while (_dataList[index] == nullptr && index < _dataList.size() / 2) {
			index++;
		}
		if (_dataList[index] == nullptr) return;
	}

	auto centerIdx = _primaryDirCount / 2;
	auto cellWidth = _size.width / _primaryDirCount;
	auto centerOffset = (centerIdx - 0.5) * cellWidth;

	auto curOffset = _tbView->getContentOffset();
	auto offset = index * cellWidth;
	_tbView->setContentOffset(Vec2(-offset + centerOffset, curOffset.y), true);

	_autoSnap = false;
}

void CueCollectionTableView::scrollViewDidScroll(cocos2d::extension:: ScrollView* view)
{
	CCLOG("scrollViewDidScroll");
	_scrolling = true;
	_scrollCurFrame = true;
	auto centerIdx = _primaryDirCount / 2;
	auto cellWidth = _size.width / _primaryDirCount;
	auto offsetX = _tbView->maxContainerOffset().x - _tbView->getContentOffset().x;
	auto centerCellIdx = (offsetX + cellWidth * centerIdx) / cellWidth;
	auto centerCellIdxFloor = (int)(centerCellIdx);

	auto cell = _tbView->cellAtIndex(centerCellIdxFloor);
	if (!cell)
		return;

	auto item = (CueCollectionCueItem*)cell->getChildByTag(TAG_COMMON);
	item->setActive(true);

	if (!_autoSnap)
		((CueCollectionUIDetail*)_mainUI)->setActiveItem(centerCellIdxFloor);

	auto nearCell = _tbView->cellAtIndex(centerCellIdxFloor + 1);
	if (nearCell)
	{
		auto nearItem = (CueCollectionCueItem*)nearCell->getChildByTag(TAG_COMMON);
		nearItem->setActive(false);
	}

	nearCell = _tbView->cellAtIndex(centerCellIdxFloor - 1);
	if (nearCell)
	{
		auto nearItem = (CueCollectionCueItem*)nearCell->getChildByTag(TAG_COMMON);
		nearItem->setActive(false);
	}	
}

void CueCollectionTableView::scrollToNearest()
{
	auto centerIdx = _primaryDirCount / 2;
	auto cellWidth = _size.width / _primaryDirCount;

	auto curOffset = _tbView->getContentOffset();
	auto offsetX = _tbView->maxContainerOffset().x - curOffset.x;
	auto centerCellIdx = (offsetX + cellWidth * centerIdx) / cellWidth;
	auto roundedIdx = floor(centerCellIdx);

	scrollToIndex(roundedIdx);
}

void CueCollectionTableView::onStopScrolling()
{
	if (!_isTouching && _autoSnap)
		scrollToNearest();
}

void CueCollectionTableView::update(float dt)
{
	if (_scrolling) {
		if (!_scrollLastFrame && !_scrollCurFrame) {
			_scrolling = false;
			onStopScrolling();
		}
		_scrollLastFrame = _scrollCurFrame;
		_scrollCurFrame = false;
	}
}
