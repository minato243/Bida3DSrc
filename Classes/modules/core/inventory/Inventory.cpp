#include "Inventory.h"
#include "InventoryMgr.h"
#include <cocos2d.h>
#include <data/GameConstant.h>
#include <core/GameMgr.h>
#include "ItemConstant.h"

using namespace std;
USING_NS_CC;


//ITEM INVENTORY ----------------------------
SubInventoryItem::SubInventoryItem()
{

}

SubInventoryItem::~SubInventoryItem()
{

}

void SubInventoryItem::setOrCreateItemQuantity(int id, int quantity)
{
	if (_items.find(id) == _items.end())
	{
		addItem(id, quantity);
	}
	else
	{
		auto item = _items.find(id)->second;
		item->setQuantity(quantity);
	}

	inventoryMgr->onItemChange(id);
}

ItemInfo * SubInventoryItem::getItemById(int id)
{
	if (_items.find(id) != _items.end())
	{
		return  _items[id];
	}
	return nullptr;
}

ItemInfo * SubInventoryItem::addItem(int id, int quantity)
{
	ItemInfo *item = NULL;
	if (_items.find(id) == _items.end()) {
		item = new ItemInfo(id, quantity);
		_items.insert(pair<int, ItemInfo*>(id, item));
	}
	else {
		item = _items[id];
		item->_quantity += quantity;
	}

	inventoryMgr->onItemChange(id);
	return item;
}

ItemInfo * SubInventoryItem::deleteItem(int itemId)
{
	ItemInfo *item = _items.find(itemId)->second;
	CC_SAFE_DELETE(item);
	return item;
}

ItemInfo * SubInventoryItem::getItemInfo(int itemId)
{
	return _items.find(itemId)->second;
}

std::vector<ItemInfo *> SubInventoryItem::getItemArray(int type)
{
	std::vector<ItemInfo *> ans;
	for (auto ittr = _items.begin(); ittr != _items.end(); ittr++)
	{
		auto item = ittr->second;
		if(item->_type == type || type == -1)
			ans.push_back(ittr->second);
	}

	return ans;
}

std::vector<ItemInfo*> SubInventoryItem::getNonZeroItemsArray()
{
	std::vector<ItemInfo *> ans;
	for (auto ittr = _items.begin(); ittr != _items.end(); ittr++)
	{
		auto item = ittr->second;
		if (item->_quantity >0)
			ans.push_back(ittr->second);
	}

	return ans;
}

void SubInventoryItem::clear()
{
	for (auto ittr = _items.begin(); ittr != _items.end(); ittr++)
	{
		CC_SAFE_DELETE(ittr->second);
	}
}

//CUE INVENTORY------------------
SubInventoryCue::SubInventoryCue()
{

}

SubInventoryCue::~SubInventoryCue()
{

}

CueInfo * SubInventoryCue::getItemById(int id)
{
	if (_items.find(id) != _items.end())
	{
		return  _items[id];
	}
	return nullptr;
}

CueInfo* SubInventoryCue::addItem(int id, int quantity)
{
	auto item = new CueInfo();
	item->_id = id;
	_items.insert(pair<int, CueInfo*>(id, item));

	inventoryMgr->onItemChange(id);
	return item;
}

CueInfo * SubInventoryCue::addCue(const CueInfoData & data)
{
	auto item = new CueInfo(data);
	item->_id = data.id;
	_items.insert(pair<int, CueInfo*>(data.id, item));

	inventoryMgr->onItemChange(data.id);
	return item;
}

void SubInventoryCue::deleteItem(int itemId)
{
	CueInfo *item = _items.find(itemId)->second;
	item->remove();
	CC_SAFE_DELETE(item);
}

CueInfo * SubInventoryCue::getItemInfo(int itemId)
{
	return _items.find(itemId)->second;
}

std::vector<CueInfo *> SubInventoryCue::getItemArray(int type)
{
	std::vector<CueInfo *> ans;
	for (auto ittr = _items.begin(); ittr != _items.end(); ittr++)
	{
		auto item = ittr->second;
		if (item->_type == type || type == -1)
			ans.push_back(ittr->second);
	}

	return ans;
}

void SubInventoryCue::clear()
{
	for (auto ittr = _items.begin(); ittr != _items.end(); ittr++)
	{
		CC_SAFE_DELETE(ittr->second);
	}
	_items.clear();
}


//CueBag------------------
CueBag::CueBag()
{
	_cues.clear();
	_usingCue = -1;
}

CueBag::~CueBag()
{

}

void CueBag::clear()
{
	_cues.clear();
	_usingCue = -1;
}

CueInfo* CueBag::putCueToBag(CueInfo *cue)
{
	auto cueCapacity = GameConstant::CUE_IN_BAG;

	cue->putToBag();
	if (_cues.size() < cueCapacity) {
		_cues.push_back(cue);
	}
	else {
		for (auto i = 0; i < cueCapacity; i++) {
			// cue already in bag
			if (_cues[i] == cue) return cue;
		}
		_cues.insert(_cues.begin(), cue);
		for (auto i = cueCapacity; i < _cues.size(); i++) {
			_cues[i]->takeFromBag();
		}
		_cues.erase(_cues.begin()+cueCapacity);
	}
	return cue;
}

	bool CueBag::takeCueFromBag(CueInfo *cue)
	{
		for (auto i = 0; i < _cues.size(); i++) {
			if (_cues[i] == cue) {
				_cues.erase(_cues.begin() +i);
				cue->takeFromBag();

				if (_usingCue >= _cues.size())
					_usingCue = 0;
				return true;
			}
		}
		return false;
	}

	CueInfo * CueBag::getUsingCue()
	{
		if (_usingCue >= 0 && _usingCue < _cues.size())
			return _cues[_usingCue];

		// Using cue hasn't set yet
		if (_cues.size() > 0) {
			_usingCue = 0;
			useCue(_cues[0]);
			return _cues[0];
		}
		return nullptr;
	}

	void CueBag::useCue(CueInfo *cueData)
	{
		auto usingCue = getUsingCue();
		if (usingCue)
			usingCue->stopUse();
		for (int i = 0; i < _cues.size(); i++)
		{
			if (_cues[i] == cueData)
			{
				_usingCue = i;
				cueData->use();
				gameMgr->_ebpCtrl->onPlayerSwitchCue(cueData->_id);
			}
		}
	}

	void CueBag::resetOnNewMatch()
	{
		_usingCue = -1;
	}

	CueInfo * CueBag::getCueInfo(int id)
	{
		for (auto i = 0; i < _cues.size(); i++) {
			if (_cues[i] && _cues[i]->_id == id) {
				return _cues[i];
			}
		}
		return getUsingCue();
	}

	CueInfo * CueBag::getCueInfoByIdx(int idx)
	{
		idx = MIN(MAX(idx, 0), _cues.size() - 1);
		return _cues[idx];
	}

	int CueBag::getCueBagTotalScore()
	{
		auto ret = 0;
		for (auto i = 0; i < _cues.size(); i++) {
			ret += _cues[i]->_generalPoint;
		}
		return ret;
	}

	std::vector<CueInfo*> CueBag::getCueArray()
	{
		return _cues;
	}

Inventory::Inventory()
{
	_cueBag = new CueBag();
	_item = new SubInventoryItem();
	_cue = new SubInventoryCue();
}

Inventory::~Inventory()
{
	CC_SAFE_DELETE(_cue);
	CC_SAFE_DELETE(_cueBag);
	CC_SAFE_DELETE(_item);
}

CueInfo* Inventory::getCueInfo(int cueId) {
	return _cue->getItemInfo(cueId);
}

CueBag* Inventory::getCueBag() {
	return _cueBag;
}

CueInfo* Inventory::addCue(int id, int quantity) {
	return _cue->addItem(id, quantity);
}

CueInfo* Inventory::addCue(const CueInfoData & data) {
	return _cue->addCue(data);
}

void Inventory::deleteCue(int cueId) {
	auto cue = _cue->getItemInfo(cueId);
	_cueBag->takeCueFromBag(cue);
	_cue->deleteItem(cueId);
}

std::vector<CueInfo*> Inventory::getCueArray(int type) {
	return _cue->getItemArray(type);
}

std::vector<CueInfo*> Inventory::getCuesBagArray() {
	return _cueBag->getCueArray();
}

std::vector<ItemInfo*> Inventory::getItemArray() {
	return _item->getItemArray();
}

std::vector<ItemInfo*> Inventory::getNonZeroItemsArray() {
	auto array = _item->getNonZeroItemsArray();
	auto gold = new ItemInfo(ItemConstant::GOLD, gameMgr->_userInfo->getGold());
	auto cash = new ItemInfo(ItemConstant::CASH, gameMgr->_userInfo->getCash());
	array.push_back(gold);
	array.push_back(cash);

	sortByPriority(array);
	return array;
}

void Inventory::sortByPriority(std::vector<ItemInfo*>& itemList)
{
	int i, key, j;
	int n = itemList.size();
	for (i = 1; i < n; i++)
	{
		auto tmp = itemList[i];
		key = ItemConstant::getItemPriority(itemList[i]->_id);
		j = i - 1;

		/* Move elements of arr[0..i-1], that are
		greater than key, to one position ahead
		of their current position */
		while (j >= 0 && ItemConstant::getItemPriority(itemList[j]->_id) > key)
		{
			itemList[j + 1] = itemList[j];
			j = j - 1;
		}
		itemList[j + 1] = tmp;
	}
}


CueInfo * Inventory::getCueById(int id) {
	return _cue->getItemById(id);
}

ItemInfo * Inventory::getItemById(int id) {
	return _item->getItemById(id);
}

int Inventory::getItemQuantity(int id) {
	if (id == ItemConstant::GOLD) {
		return gameMgr->_userInfo->getGold();
	}
	if (id == ItemConstant::CASH) {
		return gameMgr->_userInfo->getCash();
	}
	auto item = _item->getItemById(id);
	if (!item) return 0;
	return item->_quantity;
}

int Inventory::getCueCoupon() {
	return getItemQuantity(ItemConstant::COUPON);
}

ItemInfo* Inventory::addItem(int id, int quantity) {
	auto ret = _item->addItem(id, quantity);
	return ret;
}

bool Inventory::reduceItem(int id, int quantity)
{
	ItemInfo *item = _item->getItemById(id);
	if (item == NULL) 
		return false;
	item->_quantity -= quantity;
	if (item->_quantity < 0) {
		item->_quantity = 0;
		return false;
	}

	return true;
}

int Inventory::getDust(int dustType) {
	auto dustItem = _item->getItemInfo(dustType);
	if (!dustItem) return 0;
	return dustItem->_quantity;
}

void Inventory::reduceItemQuantity(int id, int quantity) {
	auto curQuantity = getItemQuantity(id);
	setItemQuantity(id, curQuantity - quantity);
}

void Inventory::setItemQuantity(int type, int quantity) {
	if (type == ItemConstant::GOLD) {
		gameMgr->_userInfo->setGold(quantity);
		return;
	}
	if (type == ItemConstant::CASH) {
		gameMgr->_userInfo->setCash(quantity);
	}
	return _item->setOrCreateItemQuantity(type, quantity);
}

bool Inventory::checkItemsQuantity(const std::vector<ItemInfoData> & itemsList) {
	for (auto i = 0; i < itemsList.size(); i++) {
		auto type = itemsList[i].type;
		auto num = itemsList[i].num;
		if (getItemQuantity(type) < num) return false;
	}
	return true;
}

bool Inventory::checkItemQuantity(int type, int num) {
	return getItemQuantity(type) >= num;
}

// convenient function of setItemQuantity for dust
void Inventory::setDust(int dustType, int quantity) {
	_item->setOrCreateItemQuantity(dustType, quantity);
}

void Inventory::useDust(int dustType, int quantity) {
	auto remain = getDust(dustType);
	if (remain < quantity) {
		CCLOG("USE dust invalid", dustType);
		return;
	}
	setDust(dustType, remain - quantity);
}

void Inventory::clearInventory() {
	_item->clear();
	_cue->clear();
	_cueBag->clear();
}

int Inventory::getTypeFromConfig(std::string type)
{
	if (type.compare("gold") == 0)
		return ItemConstant::GOLD;

	return ItemConstant::GOLD;
}

