#include "ShopItemLimited.h"
#include "../../../data/Resource.h"

USING_NS_CC;

ShopItemLimited::ShopItemLimited()
{

}

ShopItemLimited::~ShopItemLimited()
{

}

bool ShopItemLimited::init()
{
	if (!BaseLayer::init("ShopTabLimited"))
	{
		return false;
	}

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::ShopItemLimitedV3);
	this->initWithBinaryFile(filePath);

	return true;
}

void ShopItemLimited::initGUI()
{
	autoInitComponent(_layout);
	_bg = _nodeMap["bg"];
	_img = (cocos2d::ui::ImageView *)_nodeMap["img"];
	_lbName = (cocos2d::ui::Text *) _nodeMap["lbName"];
}

void ShopItemLimited::loadData(int *data)
{

}

cocos2d::Size ShopItemLimited::getOriginalSize()
{
	return _bg->getContentSize();
}

