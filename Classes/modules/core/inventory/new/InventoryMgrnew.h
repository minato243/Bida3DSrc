#ifndef 	_INVENTORYMANAGERMODULE_H_
#define 	_INVENTORYMANAGERMODULE_H_


#include <cocos2d.h>

class InventoryManagerModule{
public:

	void const InventoryMgr ();
	void onReceiveNewCue (int newCue);
	void onReceiveCueName (int cueInfo);
	void onItemChange (int itemId);
	void addOnItemChangeCallback (int itemId, int cb);
	void refreshInventoryGUIs ();
	void getInventory ();
	void onReceiveInventoryInfo (int data);
	void checkOwning10IdenticalCue (int listCue);
	void updateCueAutoRecharge (int cueId, int on);
	void revertChargeBatteryCue ();
	void chargeBatteryCue (int id);
	void getCueUpgradeItems (int type);
	void isCueBagFull ();
	void canUpgradeCue (int cueOrCueId);
	void upgradeCue (int cueOrCueId);
	void (int function(resolve, int reject);
	void setCueAutoRecharge (int id, int val);
	void addCueToBag (int id);
	void removeCueFromBag (int id);
	void resetUsingCue ();
	void sendUseCueInMatch (int cueId);
	void useCueInMatch (int cueId);
	void useCueInMatchByIndex (double idx);
	void getCueCollectionData (int type);
	void sort (int mode);
	void getGroupOfCue (int cue);
	void setInventoryViewMgr (int view);
	void sendDeleteCue (int cueId);
	void getDustTypeFromCueType (int type);
	void sendAccumulateDust (int cueId, int dust);
	void deleteCue (int type, int newDustQuantity);
	void accumulateDust (int cueId, int dustInCue, int point);
	void sendCheatInventoryItem (int itemId, int quantity);
	void addItems (int itemList);
	void reduceItems (int itemList);
	void reduceItem (int itemId, int amount);
	void addItemQuantity (int itemId, int addAmount);
	void useNextCue ();
	void clearInventory ();
	void convertCueIdToComponent (int cueId);


private:
	int _onItemChangeCallbacks;
	int _onItemChangeCallbacks[itemId];
	int chargedCue;
	int getInventory();
	int view;
};
#endif