#include "OfferNodeItem.h"
#include "../../../data/Resource.h"
#include "../GuiMgr.h"
#include "OfferBaseUI.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

OfferNodeItem::OfferNodeItem()
{

}

OfferNodeItem::~OfferNodeItem()
{

}

bool OfferNodeItem::init()
{
	if (!UIBaseLayer::init())
		return false;

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::OfferNodeItem);
	this->initWithBinaryFile(filePath);
	return true;
}

void OfferNodeItem::initGUI()
{
	_tagMap["btn"] = btn;
	autoInitComponent(_layout);
	_bg = _nodeMap["bg"];
	_lb = (Text *)_nodeMap["lb"];
	_imgButton = (ImageView *)_nodeMap["imgButton"];
	_iconButton = (ImageView *)_nodeMap["iconButton"];
}

void OfferNodeItem::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	switch (id)
	{
	case btn:
	{
		switchOffer();
		break;
	}
	default:
		break;
	}
}

void OfferNodeItem::loadData(BaseOffer *data)
{
	_type = data->type;

	string name = "";
	switch (_type)
	{
	case OfferType::ONLY_GOLD:
		name = "Gold";
		break;
	case OfferType::ONLY_CASH:
		name = "Cash";
		break;
	case OfferType::STARTER:
		name = "Starter";
		break;
	case OfferType::CUMULATIVE:
		name = "Vip";
		break;
	}

	_lb->setString(name);

}

cocos2d::Size OfferNodeItem::getOriginalSize()
{
	return _bg->getContentSize();

}

void OfferNodeItem::switchOffer()
{
	auto ui = guiMgr->getGui(Popup::OFFER_BASE_UI);
	auto offerUI = dynamic_cast<OfferBaseUI*>(ui);

	if (offerUI && offerUI->isRunning())
	{
		offerUI->switchOffer(_type);
	}
}

void OfferNodeItem::setStatusSelect(bool isSelect)
{
	string img = isSelect ? "Tach_Offer Starter/01_BT_02_tab1b.png" : "Tach_Offer Starter/01_BT_02_tab1a.png";
	_imgButton->loadTexture(img, TextureResType::PLIST);

	string iconNameOn = "";
	string iconNameOff = "";
	switch (_type)
	{
	case OfferType::STARTER:
		iconNameOn = "Tach_Offer Starter/01_IC_Of1a.png";
		iconNameOff = "Tach_Offer Starter/01_IC_Of1b.png";
		break;

	case OfferType::ONLY_GOLD:
		iconNameOn = "OF_03_IC_Of3a.png";
		iconNameOff = "OF_03_IC_Of3b.png";
		break;

	case OfferType::ONLY_CASH:
		iconNameOn = "OF_04_IC_Of4a.png";
		iconNameOff = "OF_04_IC_Of4b.png";
		break;

	case OfferType::CUMULATIVE:
		iconNameOn = "02_IC_Of1a.png";
		iconNameOff = "02_IC_Of1b.png";
		break;
	}
	string iconName = isSelect ? iconNameOn : iconNameOff;
	_iconButton->loadTexture(iconName, TextureResType::PLIST);
}

