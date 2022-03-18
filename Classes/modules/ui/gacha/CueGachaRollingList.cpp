#include "CueGachaRollingList.h"
#include "../../../data/Resource.h"
#include "../GuiMgr.h"
#include "../shop/ShopPopupConfirm.h"
#include "../../core/shop/ShopMgr.h"
#include "../../../data/config/CueConfig.h"
#include "CueGachaElementUI.h"
#include "../elements/QuickCustomTableView.h"
#include "../../../core/utils/StringUtility.h"
#include "../../../core/utils/LanguageMgr.h"
#include "../../../core/GameMgr.h"
#include "../shop/ShopUI.h"

USING_NS_CC;
using namespace std;
using namespace cocos2d::ui;

CueGachaRollingList::CueGachaRollingList()
{
	_locked = false;
	_rolled = false;
	_lastData = NULL;
	_rollingUI = NULL;
}

CueGachaRollingList::~CueGachaRollingList()
{

}

bool CueGachaRollingList::init()
{
	if (!BaseLayer::init("CueGachaRollingList"))
	{
		return false;
	}

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::CueGachaRollingListUI);
	this->initWithBinaryFile(filePath);

	this->_keep = true;
	setCameraMask((int)GameCamera::FLAG_UI);
	
	refresh();
	return true;
}

void CueGachaRollingList::initGUI()
{
	initTag();
	autoInitComponent(_layout);

	_bg = _nodeMap["bg"];
	_lbBoxName = (cocos2d::ui::Text *)_nodeMap["lbBoxName"];
	_btnStart = (Button*)_nodeMap["btnStart"];
	_lbBtnStart = (Text*)getControl("lb", _btnStart);
	_imgCCBtnStart = (ImageView*)getControl("imgCC", _btnStart);

	_pnList = (cocos2d::ui::Widget *)_nodeMap["pnList"];

	_lbGold = (cocos2d::ui::Text *)_nodeMap["lbGold"];
	_lbCC = (cocos2d::ui::Text *)_nodeMap["lbCC"];

	auto list = QuickCustomTableView<CueRateInBox, CueGachaElementUI>::create(
		_pnList->getContentSize(), 
		extension::ScrollView::Direction::VERTICAL, 
		-1, 1);
	_pnList->addChild(list);
	auto boundingBox = _pnList->getBoundingBox();
	auto frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
	auto winSize = Director::getInstance()->getWinSize();
	auto frameScale = frameSize.width / winSize.width *  Director::getInstance()->getOpenGLView()->getFrameZoomFactor();

	boundingBox.size.width *= frameScale;
	boundingBox.size.height *= frameScale;
	boundingBox.origin.x += _pnList->getParent()->getPositionX();
	boundingBox.origin.y += _pnList->getParent()->getPositionY();

	_boudingBoxItem = boundingBox;
	list->setInitItemCallback([this](CueGachaElementUI *item)
		{
			item->setContainerRect(_boudingBoxItem);
		}
	);
	_list = list;
}

void CueGachaRollingList::initTag()
{
	_tagMap.insert(pair<string, int>("btnBack", btnBack));
	_tagMap.insert(pair<string, int>("btnStart", btnStart));
}

void CueGachaRollingList::loadDataCueBox(ShopItemData *data)
{
	auto cueBox = (ShopCueBoxData*) data;
	_data = data;
	_boxesData = shopMgr->getBoxData(cueBox->boxId);
	_list->reloadData(_boxesData);
	
	_lbBtnStart->setString(StringUtility::standartNumber(cueBox->cost));
	_lbBtnStart->setTextColor(Color4B(32, 238, 152, 255));
	_lbBoxName->setString(languageMgr->localizedText(cueBox->name));
	_imgCCBtnStart->loadTexture("Icon_Coupon.png", TextureResType::PLIST);
}

void CueGachaRollingList::loadDataCueBoxDeal(HotDealData *data)
{
	_data = data;
	_preTab = ShopUI::OFFER_TAB;
	auto arr = StringUtility::split(data->hotDealType, '_');
	auto boxId = atoi(arr[1].c_str());
	auto cueBox = (ShopCueBoxData*)shopMgr->getShopData(ShopCategory::CUE)[boxId - 1];

	_boxesData = cueBox->listCue;
	_list->reloadData(_boxesData);

	auto newCost = data->cost * (100 - data->discount) / 100;
	_lbBtnStart->setString(StringUtility::standartNumber(newCost));
	_lbBoxName->setString(languageMgr->localizedText(cueBox->name));

	if (data->typeCost.compare("gold") == 0)
	{
		_imgCCBtnStart->loadTexture("res/GUI/GlobalPopup/res/Gold.png");
		_lbBtnStart->setTextColor(Color4B(255, 176, 39, 255));
	}
	else
	{
		_imgCCBtnStart->loadTexture("Icon_Coupon.png", TextureResType::PLIST);
		_lbBtnStart->setTextColor(Color4B(32, 238, 152, 255));
	}
}

void CueGachaRollingList::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	switch (id)
	{
	case TAG::btnBack:
	{
		hide();
		auto shop = guiMgr->getGui(Popup::SHOP_UI);
		shop->show();
		break;
	}

	case TAG::btnStart:
	{
		auto popupConfirm = (ShopPopupConfirm*)guiMgr->getGui(Popup::POPUP_CONFIRM);
		popupConfirm->show();
		popupConfirm->loadData(_data);
		break;
	}
	default:
		break;
	}
}

void CueGachaRollingList::refresh()
{
	setGoldValue(gameMgr->_userInfo->getGold());

	setGoldValue(gameMgr->_userInfo->getCash());
}

void CueGachaRollingList::setGoldValue(int gold)
{
	_lbGold->setString(StringUtility::standartNumber(gold));
}

void CueGachaRollingList::setCashValue(int cash)
{
	_lbCC->setString(StringUtility::standartNumber(cash));
}

void CueGachaRollingList::buyCueBox(int cueItemIdx)
{
	_locked = true;
	onOpenBoxSuccess(cueItemIdx);
}

void CueGachaRollingList::onOpenBoxSuccess(int cueItemIdx)
{
	_locked = false;
	_rolled = true;

	auto ui = getTrueRollingUI(cueItemIdx);
	ui->setVisible(true);
	_bg->setVisible(false);
	ui->onOpenBoxSuccess(_lastData);
}

CueGachaRolling* CueGachaRollingList::getTrueRollingUI(int cueItemIdx)
{
	if (!_rollingUI)
	{
		_rollingUI = CueGachaRolling::create();
		addChild(_rollingUI);
	}
	_rollingUI->setRandomData(_boxesData, cueItemIdx);

	return _rollingUI;
}

