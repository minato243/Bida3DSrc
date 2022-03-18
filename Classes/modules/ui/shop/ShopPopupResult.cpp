#include "ShopPopupResult.h"
#include "data/Resource.h"
#include "../GuiMgr.h"
#include "ShopUI.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

ShopPopupResult::ShopPopupResult()
{
	_keep = true;
}

ShopPopupResult::~ShopPopupResult()
{

}

bool ShopPopupResult::init()
{
	if (!BasePopupLayer::init("ShopPopupResult"))
	{
		return false;
	}

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::ShopPopupResult);
	this->initWithBinaryFile(filePath);

	this->_keep = true;
	return true;
}

void ShopPopupResult::initGUI()
{
	autoInitComponent(_layout);
	_ndError = _nodeMap["ndError"];
	_ndSuccess = _nodeMap["ndSuccess"];
	_btnBuy = (Button*)_nodeMap["btnBuy"];
}

void ShopPopupResult::initTag()
{
	_tagMap.insert(pair<string, int>("btnBuy", TAG::btnBuy));
}

void ShopPopupResult::noticeSuccess()
{
	_ndError->setVisible(false);
	_btnBuy->setVisible(false);
}

void ShopPopupResult::noticeError(std::string tag)
{
	_ndSuccess->setVisible(false);
	if (tag.compare(SHOP_CATEGORY_GOLD) == 0)
		_idx = 2;
	else if (tag.compare(SHOP_CATEGORY_CASH) == 0)
		_idx = 3;

	auto gui = guiMgr->getGui(Popup::GACHA_ROLLING);
	if (gui && gui->_isShow)
	{
		_btnBuy->setVisible(true);
	}
	else
	{
		_btnBuy->setVisible(false);
		ShopUI *shopUI = (ShopUI*)guiMgr->getGui(Popup::SHOP_UI);
		shopUI->switchTab(_idx);
	}
}

void ShopPopupResult::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	switch (id)
	{
	case TAG::btnBuy:
	{
		hide();
		auto shop = (ShopUI*)guiMgr->getGui(Popup::SHOP_UI);
		shop->show();
		shop->switchTab(_idx);
		
		auto gui = guiMgr->getGui(Popup::GACHA_ROLLING);
		gui->hide();
		break;
	}
	default:
		break;
	}
}

