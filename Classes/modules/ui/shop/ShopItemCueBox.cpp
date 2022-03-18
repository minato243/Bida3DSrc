#include "ShopItemCueBox.h"
#include "../../../data/Resource.h"
#include "../../../core/utils/StringUtility.h"
#include "../../../data/config/ConfigMgr.h"
#include "../gacha/CueGachaRollingList.h"
#include "../GuiMgr.h"
#include "ShopPopupConfirm.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

ShopItemCueBox::ShopItemCueBox()
{

}

ShopItemCueBox::~ShopItemCueBox()
{

}

bool ShopItemCueBox::init()
{
	if (!BaseLayer::init("ShopItem"))
	{
		return false;
	}

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::ShopItemCueBox);
	this->initWithBinaryFile(filePath);

	return true;
}

void ShopItemCueBox::initGUI()
{
	autoInitComponent(_layout);
	_bg = _nodeMap["bg"];

	_ndValue = _nodeMap["ndValue"];
	_ndCost = _nodeMap["ndCost"];
	_ndCostOld = _nodeMap["ndCostOld"];
	_imgBoxNameBox = (ImageView*)_nodeMap["imgBoxNameBox"];

	_lbNdValue = (Text*)getControl("lb", _ndValue);
	_sprNdValue = (Sprite*)getControl("spr", _ndValue);

	_lbNdCost = (Text*)getControl("lb", _ndCost);
	_sprNdCost = (Sprite*)getControl("spr", _ndCost);

	_lbNdCostOld = (Text*)getControl("lb", _ndCostOld);
	_sprNdCostOld = (Sprite*)getControl("spr", _ndCostOld);

	_lbBoxName = (Text*)_nodeMap["lbBoxName"];
	_btnImgItem = (Button *)_nodeMap["btnImgItem"];
	_btnImgItem->setTag(TAG::btnImgItem);
	_btnImgItem->addTouchEventListener(CC_CALLBACK_2(BaseLayer::onTouchEventHandler, this));

	_bgVisible = (Widget *)_nodeMap["bgVisible"];
}

void ShopItemCueBox::loadData(ShopItemData *data)
{
	_data = data;
	if (data->category == ShopCategory::CUE)
		loadDataCueBox(data);
	else if (data->category == ShopCategory::LIMITED)
		loadDataHotDeal(data);
}

cocos2d::Size ShopItemCueBox::getOriginalSize()
{
	return _bg->getContentSize();
}

void ShopItemCueBox::loadDataCueBox(ShopItemData *data)
{
	ShopCueBoxData *cueBox = (ShopCueBoxData*)data;
	_lbNdCost->setString(StringUtility::formatNumberSymbol(cueBox->cost));
	_ndCostOld->setVisible(false);
	_lbBoxName->setString(cueBox->name);
}

void ShopItemCueBox::loadDataHotDeal(ShopItemData *data)
{
	HotDealData *hotDeal = (HotDealData *)data;
	auto newCost = data->cost;

	if (hotDeal->isSold) {
		_bgVisible->setVisible(true);
		_btnImgItem->setTouchEnabled(false);
	}
	else {
		_bgVisible->setVisible(false);
		_btnImgItem->setTouchEnabled(true);
	}

	if (hotDeal->discount > 0) {
		newCost = hotDeal->cost*(100 - hotDeal->discount) / 100;
		_ndCostOld->setVisible(true);
		_lbNdCostOld->setString(StringUtility::formatNumberSymbol(hotDeal->cost));
	}
	else {
		_ndCostOld->setVisible(false);

		_sprNdCostOld->setTexture("res/GUI/GlobalPopup/res/Gold.png");
		_lbNdCostOld->setTextColor(Color4B(247, 198, 8, 255));
		_sprNdCostOld->setScale(0.7);

		_sprNdCost->setTexture("res/GUI/GlobalPopup/res/Gold.png");
		_sprNdCost->setScale(0.7);
		_lbNdCost->setTextColor(Color4B(247, 198, 8, 255));
	}

	_ndValue->setVisible(false);
	_lbNdCost->setString(StringUtility::formatNumberSymbol(newCost));

	auto name = hotDeal->hotDealType;
	if (name.find("boxId") != -1)
	{
		int i = hotDeal->hotDealType.find("_");
		auto index = hotDeal->hotDealType.substr(i + 1);
		name = configMgr->cueConfig->getCueBoxName(atoi(index.c_str()) - 1);
	}
	_lbBoxName->setString(name);
}

void ShopItemCueBox::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	switch (id)
	{
	case TAG::btnImgItem:
	{
		auto gui = (CueGachaRollingList*)guiMgr->getGui(Popup::GACHA_ROLLING);
		if (_data->category == ShopCategory::CUE)
		{
			gui->show();
			gui->loadDataCueBox(_data);
		}
		else if (_data->category == ShopCategory::LIMITED)
		{
			auto hotDeal = (HotDealData*)_data;
			auto type = hotDeal->hotDealType.substr(0, 5);
			if (type.compare("boxId"))
			{
				gui->show();
				gui->loadDataCueBoxDeal(hotDeal);
			}
			else showPopupConfirm();
		}
		else showPopupConfirm();
		break;
	}

	default:
		break;
	}
}

void ShopItemCueBox::showPopupConfirm()
{
	auto gui = guiMgr->getGui(Popup::POPUP_CONFIRM);
	auto popup = (ShopPopupConfirm*)gui;
	popup->show();
	popup->loadData(_data);
}

