#include "ShopPopupConfirm.h"
#include "data/Resource.h"
#include "core/utils/StringUtility.h"
#include "../GuiMgr.h"
#include "../../core/shop/ShopMgr.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

ShopPopupConfirm::ShopPopupConfirm()
{

}

ShopPopupConfirm::~ShopPopupConfirm()
{

}

bool ShopPopupConfirm::init()
{
	if (!BaseLayer::init("ShopPopupConfirm"))
	{
		return false;
	}

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::ShopPopupConfirm);
	this->initWithBinaryFile(filePath);

	this->_keep = true;
	return true;
}

void ShopPopupConfirm::initGUI()
{
	initTag();
	autoInitComponent(_layout);

	_bg = _nodeMap["bg"];
	_ndCost = _nodeMap["ndCost"];
	_ndCostOld = _nodeMap["ndCostOld"];
	_ndValue = _nodeMap["ndValue"];

	_lbNdCost = (Text *)getControl("lb", _ndCost);
	_sprNdCost = (Sprite *)getControl("spr", _ndCost);

	_lbNdCostOld = (Text *)getControl("lb", _ndCostOld);
	_sprNdCostOld = (Sprite *)getControl("spr", _ndCostOld);

	_lbNdValue = (Text *)getControl("lb", _ndValue);
	_sprNdValue = (Sprite *)getControl("spr", _ndValue);

	_text = (Text*)getControl("text", _bg);
}

void ShopPopupConfirm::initTag()
{
	_tagMap.insert(pair<string, int>("btnCancel", TAG::btnCancel));
	_tagMap.insert(pair<string, int>("btnConfirm", TAG::btnConfirm));
}

cocos2d::Size ShopPopupConfirm::getOriginalSize()
{
	return _bg->getContentSize();
}

void ShopPopupConfirm::loadData(ShopItemData * data)
{
	_data = data;
	if (data->value)
	{
		_ndValue->setVisible(true);
		_lbNdValue->setString(StringUtility::formatNumberSymbol(data->value));
		if (data->typeCost.compare("cash") == 0)
		{
			_sprNdValue->setTexture("res/GUI/GlobalPopup/res/Gold.png");
			_sprNdValue->setScale(0.7);
			_lbNdValue->setTextColor(Color4B(255, 176, 39, 255));
		}
		else
		{
			_sprNdValue->setTexture("res/GUI/GlobalPopup/res/Coupon_Normal.png");
			_sprNdValue->setScale(0.3);
			_lbNdValue->setTextColor(Color4B(32, 238, 152, 255));
		}
	}
	else _ndValue->setVisible(false);

	if (data->cost)
	{
		auto newCost = (data->cost*(100 - data->discount) / 100);
		_lbNdCost->setString(StringUtility::formatNumberSymbol(newCost));
		if (data->typeCost.compare("gold") == 0)
		{
			_sprNdCost->setVisible(true);
			_sprNdCost->setTexture("res/GUI/GlobalPopup/res/Gold.png");
			_sprNdCost->setScale(0.7);
			_lbNdCost->setTextColor(Color4B(255, 176, 39, 255));
		}
		else if (data->typeCost.compare("dollar") == 0)
		{
			_sprNdCost->setVisible(false);
			_lbNdCost->setTextColor(Color4B(32, 238, 152, 255));
			_lbNdCost->setString("$ "+StringUtility::formatNumberSymbol(newCost));
		}
		else
		{
			_sprNdCost->setVisible(true);
			_sprNdCost->setTexture("res/GUI/GlobalPopup/res/Coupon_Normal.png");
			_sprNdCost->setScale(0.3);
			_lbNdCost->setTextColor(Color4B(32, 238, 152, 255));
		}
	}
}

void ShopPopupConfirm::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	switch (id)
	{
	case TAG::btnCancel:
	{
		guiMgr->getGui(Popup::SHOP_UI);
		hide();
		break;
	}
	case TAG::btnConfirm:
	{
		processConfirm();
		break;
	}
	default:
		break;
	}
}

void ShopPopupConfirm::processConfirm()
{
	hide();

	if (_data->category == ShopCategory::GOLD)
	{
		shopMgr->buyGold(_data->itemId);
	}
	else if (_data->category == ShopCategory::CUE)
	{
		ShopCueBoxData *cueBox = (ShopCueBoxData*)_data;
		shopMgr->buyCueBox(cueBox->boxId, cueBox->typeCost);
	}
	else if (_data->category == ShopCategory::LIMITED)
	{
		HotDealData *hotDeal = (HotDealData*)_data;
		shopMgr->buyHotDeal(hotDeal->hotDealIdx, hotDeal->typeCost);
	}
	guiMgr->getGui(Popup::SHOP_UI);
}

