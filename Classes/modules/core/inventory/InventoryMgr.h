#ifndef InventoryMgr_h_
#define InventoryMgr_h_

#include "Inventory.h"
#include "../../network/inventory/InventoryInPacket.h"

#define inventoryMgr InventoryMgr::getInstance()
class InventoryMgr
{
public:
	InventoryMgr();
	~InventoryMgr();

	void onItemChange(int id);

	Inventory *getInventory();

	void onReceiveNewCue(CueInfoData newCue);

	void onReceiveCueName(int cueInfo);
	
	void addOnItemChangeCallback(int itemId, int cb);
	
	void refreshInventoryGUIs();
	
	void onReceiveInventoryInfo(UpdateInventoryInfo *data);
	
	void checkOwning10IdenticalCue(int listCue);
	
	void updateCueAutoRecharge(int cueId, bool on);
	
	void revertChargeBatteryCue();
	
	void chargeBatteryCue(int id);
	
	void getCueUpgradeItems(int type);
	
	bool isCueBagFull();
	
	void canUpgradeCue(int cueOrCueId);
	
	void upgradeCue(int cueOrCueId);
	
	void setCueAutoRecharge(int id, int val);
	
	void addCueToBag(int id);
	
	void removeCueFromBag(int id);
	
	void resetUsingCue();
	
	void sendUseCueInMatch(int cueId);
	
	void useCueInMatch(int cueId);
	
	void useCueInMatchByIndex(double idx);
	
	std::vector<CueInfoData *> getCueCollectionData(int type);
	
	void sort(int mode);
	
	void getGroupOfCue(int cue);
	
	void setInventoryViewMgr(int view);
	
	void sendDeleteCue(int cueId);
	
	void getDustTypeFromCueType(int type);
	
	void sendAccumulateDust(int cueId, int dust);
	
	void deleteCue(int type, int newDustQuantity);
	
	void accumulateDust(int cueId, int dustInCue, int point);
	
	void sendCheatInventoryItem(int itemId, int quantity);
	
	void addItems(int itemList);
	
	void reduceItems(int itemList);
	
	void reduceItem(int itemId, int amount);
	
	void addItemQuantity(int itemId, int addAmount);
	
	void useNextCue();
	
	void clearInventory();
	
	CueInfoData convertCueIdToComponent(int cueId);

private:
	static InventoryMgr *_instance;
	Inventory *_inventory;

public:
	static InventoryMgr *getInstance();
};


#endif

