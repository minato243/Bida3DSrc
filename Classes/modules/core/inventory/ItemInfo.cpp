#include "ItemInfo.h"
#include "ItemConstant.h"

ItemInfo::ItemInfo()
{
}

ItemInfo::~ItemInfo()
{
}

ItemInfo::ItemInfo(int id, int quantity)
{
	_id = id;
	_quantity = quantity;
	_itemCode = ItemConstant::getItemCodeFromId(id);
}


void ItemInfo::initWithServerData()
{

}

std::string ItemInfo::getName()
{
	return "Lo co thu";
}

bool ItemInfo::isCue()
{
	return false;
}

void ItemInfo::setId(int id)
{
	_id = id;
	_itemCode = ItemConstant::getItemCodeFromId(id);
}

const int & ItemInfo::getId()
{
	return _id;
}

void ItemInfo::setQuantity(int quantity)
{
	_quantity = quantity;
}

int ItemInfo::getQuantity()
{
	return _quantity;
}

void ItemInfo::setItemCode(std::string itemCode)
{
	_itemCode = itemCode;
}

std::string ItemInfo::getItemCode()
{
	return _itemCode;
}

std::string ItemInfo::getIcon()
{
	return _icon;
}
