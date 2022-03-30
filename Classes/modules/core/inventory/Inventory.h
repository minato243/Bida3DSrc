#ifndef Inventory_h_
#define Inventory_h_

#include <map>
#include <vector>
#include "ItemInfo.h"
#include "CueInfo.h"

enum class InventoryType
{
	ITEM,
	CUE
};

class SubInventoryItem
{
public:
	SubInventoryItem();
	~SubInventoryItem();

	void setOrCreateItemQuantity(int id, int quantity);

	ItemInfo *getItemById(int id);

	ItemInfo * addItem(int id, int quantity);

	ItemInfo * deleteItem(int itemId);

	ItemInfo *getItemInfo(int itemId);

	std::vector<ItemInfo *> getItemArray(int type = -1);

	std::vector<ItemInfo *> getNonZeroItemsArray();

	void clear();

private:
	std::map<int, ItemInfo*> _items;
};

class SubInventoryCue
{
public:
	SubInventoryCue();
	~SubInventoryCue();

	CueInfo * getItemById(int id);

	CueInfo * addItem(int id, int quantity);

	CueInfo * addCue(const CueInfoData &data);

	void deleteItem(int itemId);

	CueInfo *getItemInfo(int itemId);

	std::vector<CueInfo *> getItemArray(int type = -1);

	void clear();
private:
	std::map<int, CueInfo*> _items;
};

class CueBag
{
public:
	CueBag();
	~CueBag();

	void clear();

	CueInfo *putCueToBag(CueInfo *cue);

	bool takeCueFromBag(CueInfo *cue);

	CueInfo *getUsingCue();

	void useCue(CueInfo *cue);

	void resetOnNewMatch();

	CueInfo *getCueInfo(int id);

	CueInfo *getCueInfoByIdx(int idx);

	int getCueBagTotalScore();

	std::vector<CueInfo*> getCueArray();
private:

	std::vector<CueInfo*> _cues;
	int _usingCue;
};

class Inventory
{
public:
	Inventory();
	~Inventory();

	CueInfo* getCueInfo(int cueId);

	CueBag* getCueBag();

	CueInfo* addCue(int id, int quantity);

	CueInfo* addCue(const CueInfoData & data);

	void deleteCue(int cueId);

	std::vector<CueInfo*> getCueArray(int type = -1);

	std::vector<CueInfo*> getCuesBagArray();

	std::vector<ItemInfo*> getItemArray();

	std::vector<ItemInfo*> getNonZeroItemsArray();

	void sortByPriority(std::vector<ItemInfo*> &itemList);

	CueInfo * getCueById(int id);

	ItemInfo * getItemById(int id);

	int getItemQuantity(int id);

	int getCueCoupon();

	ItemInfo * addItem(int id, int quantity);

	bool reduceItem(int id, int quantity);

	int getDust(int dustType);

	void reduceItemQuantity(int id, int quantity);

	void setItemQuantity(int type, int quantity);

	bool checkItemsQuantity(const std::vector<ItemInfoData> & itemsList);

	bool checkItemQuantity(int type, int num);

	void setDust(int dustType, int quantity);

	void useDust(int dustType, int quantity);

	void clearInventory();

public:
	SubInventoryItem *_item;
	SubInventoryCue *_cue;
	CueBag *_cueBag;

public:
	static int getTypeFromConfig(std::string type);
};


#endif


