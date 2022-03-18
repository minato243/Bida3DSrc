#include "InventoryItemElementUI.h"
#include "../../../data/Resource.h"
#include "../../../base/UIFactory.h"
#include "../../core/inventory/ItemConstant.h"
#include "../../../core/utils/LanguageMgr.h"
#include "../../../core/utils/StringUtility.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

InventoryItemElementUI::InventoryItemElementUI()
{

}

InventoryItemElementUI::~InventoryItemElementUI()
{

}

bool InventoryItemElementUI::init()
{
	if (!BaseLayer::init("InventoryItemElementUI"))
	{
		return false;
	}

	string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::InventoryItemElementUIv2_2);
	this->initWithBinaryFile(filePath);

	return true;
}

void InventoryItemElementUI::initGUI()
{
	initTag();
	this->autoInitComponent(_layout);
	_nodeMap["fake"]->setVisible(false);

	_bg = _nodeMap["bg"];
	_btnOpen = (Button *) _nodeMap["btnOpen"];
	_btnOpenMulti = (Button *)_nodeMap["btnOpenMulti"];
	_btnInfo = (Button *)_nodeMap["btnInfo"];

	_lbTitle = (Text*)_nodeMap["lbTitle"];
	_lbQuantity = (Text*)_nodeMap["lbQuantity"];

	_imgItem = (ImageView *)_nodeMap["imgItem"];
	_btnOpen->setVisible(false);
	_btnOpenMulti->setVisible(false);
	_btnInfo->setVisible(false);
	_imgItem->ignoreContentAdaptWithSize(true);
}

void InventoryItemElementUI::initTag()
{
	_tagMap.insert(pair<string, int>("btnInfo", TAG::btnInfo));
	_tagMap.insert(pair<string, int>("btnOpen", TAG::btnOpen));
	_tagMap.insert(pair<string, int>("btnOpenMulti", TAG::btnOpenMulti));
}

void InventoryItemElementUI::autoResize()
{
	auto size = _bg->getContentSize();
	UIFactory::scaleToSize(this->_bg, this->_size.width * 0.95, this->_size.height * 0.95);
}

cocos2d::Size InventoryItemElementUI::getOriginalSize()
{
	return this->_bg->getContentSize();
}

void InventoryItemElementUI::loadData(ItemInfo *data)
{
	this->_data = data;
	if (!data) return;
	string itemCode = to_string(data->_id);
	StringUtility::fillChar(itemCode, 4, '0');
	_lbTitle->setString(languageMgr->localizedText("text_item_" + itemCode));
	_lbQuantity->setString("x" + StringUtility::formatNumberSymbol(data->_quantity));
	string path = ItemConstant::getItemIcon(data->_id);
	_imgItem->loadTexture(path, Widget::TextureResType::PLIST);

	UIFactory::scaleToSize(_imgItem, 100, 100);
}

void InventoryItemElementUI::onButtonRelease(cocos2d::ui::Button* button, int id)
{

}

