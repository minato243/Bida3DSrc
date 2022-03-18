#include "InventoryMgr.h"

{
    void InventoryManagerModule::const InventoryMgr () {
        // constructor
        this->_onItemChangeCallbacks = {};
    }

    auto proto = InventoryMgr.prototype;

    void InventoryManagerModule::onReceiveNewCue (int newCue){
        auto cue = this->getInventory().addCue(newCue);
        gv.GuiMgr.refreshGui(Popup.INVENTORY_UI);
        gv.GuiMgr.refreshGui(Popup.COMPETITION_BAG_UI);

        //No more gacha box
        auto gachaGUI = gv.GuiMgr.getGui(Popup.GACHA_ROLLING, CueGachaRollingList);
        if(gachaGUI.isShow){
            gachaGUI.lastData = {cueInfo: cue};
        }
        //gv.cueGachaMgr.onOpenBoxSuccess(newCue);

        // gv.gameMgr.loadSceneByTag(Scenes.INSPECT_SCENE).loadData(cue, true);
    }

    void InventoryManagerModule::onReceiveCueName (int cueInfo){
        auto cue = this->getInventory().getCueById(cueInfo.id);
        if(cue){
            cue.alias = cueInfo.name;
            gv.GuiMgr.refreshGui(Popup.INVENTORY_UI);
            gv.GuiMgr.refreshGui(Popup.COMPETITION_BAG_UI);
            gv.GuiMgr.refreshGui(Popup.CUE_INSPECT_UI);
        }
    }

    void InventoryManagerModule::onItemChange (int itemId){
        if(this->_onItemChangeCallbacks[itemId]){
            auto callbacks = this->_onItemChangeCallbacks[itemId];
            for(auto i=0;i<callbacks.length;i++){
                callbacks[i] && callbacks[i]();
            }
        }
    }

    void InventoryManagerModule::addOnItemChangeCallback (int itemId, int cb){
        if(this->_onItemChangeCallbacks[itemId]){
            this->_onItemChangeCallbacks[itemId].push_back(cb);
        }
        else{
            this->_onItemChangeCallbacks[itemId] = [cb];
        }
    }

    void InventoryManagerModule::refreshInventoryGUIs (){
        gv.GuiMgr.refreshGui(Popup.INVENTORY_UI);
        gv.GuiMgr.refreshGui(Popup.WORKSHOP_UI);
        gv.GuiMgr.refreshGui(Popup.CHANNEL_SELECT);
    }

    void InventoryManagerModule::getInventory (){
        return gv.user.inventory;
    }

    void InventoryManagerModule::onReceiveInventoryInfo (int data){
        for(auto i=0;i<data.cues.length;i++){
            this->getInventory().addCue(data.cues[i]);
        }
        for(auto i=0;i<data.bag.length;i++){
            this->getInventory().getCueBag().putCueToBag(this->getInventory().getCueById(data.bag[i]));
        }

        // Sync items
        for(auto i=0;i<data.items.length;i++){
            auto item = this->getInventory().setItemQuantity(data.items[i].id, data.items[i].quantity);
        }

        // const boxId = [];
        auto boxId = [ItemConstant.CUE_BOX_SONG_TOAN, ItemConstant.CUE_BOX_BOC_PHA, ItemConstant.CUE_BOX_CAU_BONG, ItemConstant.CUE_BOX_XOAN_OC];

        for(auto i=0;i<boxId.length;i++){
            gv.cueGachaMgr.onReceiveInventoryData(boxId[i], this->getInventory().getItemById(boxId[i]));
        }

        // Sync cue box buy time
        if(data.cueBoxesBuyTimes){
            for(auto i=0;i<data.cueBoxesBuyTimes.length;i++){
                auto d = data.cueBoxesBuyTimes[i];
                gv.cueGachaMgr.setCueBoxOpenTime(d.type, d.time);
            }
        }

        gv.gameMgr.lobbyUI.updateUserInfo();

        if(!gv.achievementMgr.isAchievementUnlocked(ACHIEVEMENT_ID.OWNING_10_IDENTICAL_CUE)) {
            this->checkOwning10IdenticalCue(data.cues);
        }
        // utils.log("INVENTORY", this.getInventory());
    }

    void InventoryManagerModule::checkOwning10IdenticalCue (int listCue) {
        auto listCountByConfigId = [0,0,0,0,0,0,0,0,0,0,0];
        auto maxCount = 0;
        for(auto it in listCue) {
            auto configId = listCue[it].configId;
            listCountByConfigId[configId]++;
            if(listCountByConfigId[configId] > maxCount) maxCount = listCountByConfigId[configId];
        }
        if(maxCount >= 0)
            gv.achievementHandler.sendUnlockAchievement(ACHIEVEMENT_ID.OWNING_10_IDENTICAL_CUE);
    }

    void InventoryManagerModule::updateCueAutoRecharge (int cueId, int on){
        auto cue = this->getInventory().getCueById(cueId);
        cue.autoCharge = on;

        gv.GuiMgr.refreshGui(Popup.INVENTORY_UI);
        gv.GuiMgr.refreshGui(Popup.CUE_INSPECT_UI);
    }

    void InventoryManagerModule::revertChargeBatteryCue (){
        auto config = Config.CueChargeBatteryConfig;

        auto cue = this->getInventory().getCueById(this->chargedCue.id);
        cue.battery = this->chargedCue.battery;

        gv.inventoryMgr.getInventory().getItemById(ItemConstant.CHALK).quantity += config[cue.type][cue.tier];

        gv.GuiMgr.refreshGui(Popup.LOBBY_UI);
    }

    void InventoryManagerModule::chargeBatteryCue (int id){
        // gv.connector.sendChargeCue(id);
        gv.cueHandler.requestChargeCue(id)
        .then(response => {
            if (result.error == 0) {
                this->chargeBatteryCue(null, true);
            }
        });

        auto config = Config.CueChargeBatteryConfig;
        auto cue = this->getInventory().getCueById(id);
        if(cue)
            cue.charge();
        this->chargedCue = {id: id, battery: cue.battery};
        utils.log("CHARGE CUE", id, cue);
        gv.inventoryMgr.getInventory().getItemById(ItemConstant.CHALK).quantity -= config[cue.type][cue.tier];
        gv.GuiMgr.refreshGui(Popup.LOBBY_UI);
    }

    void InventoryManagerModule::getCueUpgradeItems (int type){
        auto itemCode;
        switch(type){
            case 1:
                itemCode = ItemConstant.CHALLENGE_ITEM_SONG_TOAN;
                break;
            case 2:
                itemCode = ItemConstant.CHALLENGE_ITEM_BOC_PHA;
                break;
            case 3:
                itemCode = ItemConstant.CHALLENGE_ITEM_CAU_BONG;
                break;
            case 4:
                itemCode = ItemConstant.CHALLENGE_ITEM_XOAN_OC;
                break;
            default:
throw "Wrong cue type";
        }
        return itemCode;
    }

    void InventoryManagerModule::isCueBagFull (){
        // utils.log("cue bag", this.getInventory().getCuesBagArray());
        return this->getInventory().getCuesBagArray().length == gv.GameConst.CUE_IN_BAG;
    }

    void InventoryManagerModule::canUpgradeCue (int cueOrCueId){
        auto cue = cueOrCueId;
        if(utils.isNumber(cueOrCueId)){
            cue = this->getInventory().getCueById(cueOrCueId);
        }
        auto type = cue.type;
        auto config = Config.CueUpgradeConfig[type];
        return this->getInventory().getItemQuantity(this->getCueUpgradeItems(type)) >= config.requirePoint[cue.tier]
            && this->getInventory().getItemQuantity(ItemConstant.GOLD) > config.requireGold[cue.tier];
    }

    void InventoryManagerModule::upgradeCue (int cueOrCueId){
        if(!this->canUpgradeCue(cueOrCueId))
            void InventoryManagerModule::(int function(resolve, int reject) {
                resolve({val: false, detail: "don't have enough materials"});
            });

        auto cue = cueOrCueId;
        if(utils.isNumber(cueOrCueId)){
            cue = this->getInventory().getCueById(cueOrCueId);
        }

        return gv.cueHandler.requestUpgradeCue(cue.id).then(data => {
            if(data.error != 0){
                utils.log("upgrade cue error:", data.getStatus());
                return new Promise((resolve) => resolve({val: false, detail: data.getStatus()}));
            }
            else{
                utils.log("upgrade cue success");
                this->addItems([
                    {
                        type: this->getCueUpgradeItems(cue.type),
                        num: -Config.CueUpgradeConfig[cue.type].requirePoint[cue.tier]
                    }
                    {
                        type: ItemConstant.GOLD,
                        num: -Config.CueUpgradeConfig[cue.type].requireGold[cue.tier]
                    }]);
                cue.upgrade();
                this->view.reloadInventory();
                return new Promise((resolve) => resolve({val: true, detail: null}));
            }
        });
    }

    void InventoryManagerModule::setCueAutoRecharge (int id, int val){
        auto cue = this->getInventory().getCueById(id);
        if(cue){
            cue.autoCharge = val;
            gv.GuiMgr.refreshGui(Popup.INVENTORY_UI);
            gv.GuiMgr.refreshGui(Popup.COMPETITION_BAG_UI);
            gv.GuiMgr.refreshGui(Popup.CUE_INSPECT_UI);
        }
    }

    void InventoryManagerModule::addCueToBag (int id){
        this->getInventory().getCueBag().putCueToBag(this->getInventory().getCueById(id));
        gv.GuiMgr.refreshGui(Popup.INVENTORY_UI);
        gv.GuiMgr.refreshGui(Popup.COMPETITION_BAG_UI);
        gv.GuiMgr.refreshGui(Popup.CHANNEL_SELECT);
    }

    void InventoryManagerModule::removeCueFromBag (int id){
        this->getInventory().getCueBag().takeCueFromBag(this->getInventory().getCueById(id));
        gv.GuiMgr.refreshGui(Popup.INVENTORY_UI);
        gv.GuiMgr.refreshGui(Popup.COMPETITION_BAG_UI);
        gv.GuiMgr.refreshGui(Popup.CHANNEL_SELECT);
    }

    void InventoryManagerModule::resetUsingCue (){
        auto cueBag = this->getInventory().getCueBag();
        auto usingCue = cueBag.getUsingCue();
        usingCue.stopUse();
    }

    void InventoryManagerModule::sendUseCueInMatch (int cueId){
        utils.log("Send use cue in match", cueId);
        // if(!DEBUG_PLAY_ALONE){
            gv.connector.sendUseCueInMatch(cueId);
        // }
        // else{
        //     this.useCueInMatch(cueId);
        // }
    }

    void InventoryManagerModule::useCueInMatch (int cueId){
        auto cueBag = this->getInventory().getCueBag();
        cueBag.useCue(cueBag.getCueInfo(cueId))
    }

    void InventoryManagerModule::useCueInMatchByIndex (double idx){
        idx = idx || 0;
        auto cueBag = this->getInventory().getCueBag();
        cueBag.useCue(cueBag.getCueInfoByIdx(idx));
    }

    void InventoryManagerModule::getCueCollectionData (int type){
        auto cues = this->getInventory().getCuesArray();
        auto ret = [];

        auto config = Config.CueModel;
        auto map = {};

        for(auto star = 1;star <=5; star++){
            map[star] = {};
            for(auto configId in config[type][star]){
                auto obj = {configId: configId, type: type, star: star, level: 1, owned: false};
                ret.push_back(obj);
                map[star][configId] = obj;
            }
        }

        for(auto i=0;i<cues.length;i++){
            auto cue = cues[i];
            if(cue.type != type) continue;
            auto star = cue.tier;
            auto configId = cue.configId;
            map[star][configId].owned = true;
        }

        return ret;
    }

	auto SortModes = {};
	InventoryMgr.SortModes = SortModes;

	SortModes.BY_QUALITY = 0;
	SortModes.BY_GROUP = 1;

	void InventoryManagerModule::sort (int mode) {
		auto cloneCues = Array.from(this->_cues);
		switch (mode) {
			case SortModes.BY_QUALITY:
				cloneCues.sort((c1, c2) => (c2.quality() - c1.quality()));
				break;
			case SortModes.BY_GROUP: {
				auto groups = cloneCues.map(cue => this->getGroupOfCue(cue));
				cloneCues = cloneCues
					.map((cue, index) => ({ cue: cue, group: groups[index] }))
					.sort((d1, d2) => (d2.group - d1.group))
					.map(data => data.cue);
				break;
			}
			default:
				cc.log("Invalid filter mode");
		}
		return cloneCues;
	}

	void InventoryManagerModule::getGroupOfCue (int cue) {
		// TODO: Return group of cue here
		return 0;
	}

	void InventoryManagerModule::setInventoryViewMgr (int view) {
		this->view = view;
	}

	void InventoryManagerModule::sendDeleteCue (int cueId){
	    if(this->getInventory().getCueById(cueId).isInBag()){
	        this->removeCueFromBag(cueId);
        }
	    // gv.connector.sendDeleteCue(cueId);

        gv.cueHandler.requestDeleteCue(cueId)
        .then(result => {
            if (result.error == 0) {
                this->deleteCue(result.itemId, result.newQuantity);
            }
        });

        this->getInventory().deleteCue(cueId);

        gv.GuiMgr.refreshGui(Popup.INVENTORY_UI);
        gv.GuiMgr.refreshGui(Popup.COMPETITION_BAG_UI);
    }

    void InventoryManagerModule::getDustTypeFromCueType (int type){
	    auto dustType;
        switch(type){
            case ItemConstant.CueType.SONG_TOAN: dustType = ItemConstant.DUST_SONG_TOAN; break;
            case ItemConstant.CueType.BOC_PHA:   dustType = ItemConstant.DUST_BOC_PHA; break;
            case ItemConstant.CueType.CAU_BONG:  dustType = ItemConstant.DUST_CAU_BONG; break;
            case ItemConstant.CueType.XOAN_OC:   dustType = ItemConstant.DUST_XOAN_OC; break;
        }
        return dustType;
    }

    void InventoryManagerModule::sendAccumulateDust (int cueId, int dust){
	    auto type = this->getInventory().getCueInfo(cueId).type;
        auto dustType = this->getDustTypeFromCueType(type);

	    auto dustRemain = this->getInventory().getDust(dustType);
	    if(dustRemain < dust){
	        utils.log("Error: Not enough dust. Remain:", dustRemain);
	        return;
        }

	    this->getInventory().useDust(dustType, dust);
	    // gv.connector.sendAccumulateDust(cueId, dust);
        gv.cueHandler.requestAccumulateDust(cueId, dust)
        .then(response => {
            if (response.error == 0) {
                this->accumulateDust(response.cueId, response.newDustInCue, response.generalPoint);
            }
        });
    }

    void InventoryManagerModule::deleteCue (int type, int newDustQuantity){
        // const type = this.getInventory().getCueInfo(cueId).type;
        this->getInventory().setDust(type, newDustQuantity);
        this->view.getUpgradeAndSalvageView().onReceiveCueSalvageSuccess();
    }

    void InventoryManagerModule::accumulateDust (int cueId, int dustInCue, int point){
	    auto cue = this->getInventory().getCueInfo(cueId);
	    cue.dust = dustInCue;
	    cue.generalPoint = point;

	    this->view.getUpgradeAndSalvageView().onReceiveCueUpgradeSuccess();
    }

    void InventoryManagerModule::sendCheatInventoryItem (int itemId, int quantity){
	    this->getInventory().setItemQuantity(itemId, quantity);
	    // gv.connector.sendCheatInventoryItem(itemId, quantity);
        gv.cueHandler.requestCheatInventoryItem(itemId, quantity);
    }

    void InventoryManagerModule::addItems (int itemList){
        for(auto i=0;i<itemList.length;i++){
            auto item = itemList[i];
            // if(item.type === ItemConstant.GOLD){
            //     gv.user.gold += item.num;
            // }
            // else{
                this->addItemQuantity(item.type, item.num);
            // }
        }
    }

    void InventoryManagerModule::reduceItems (int itemList){
	    auto ret = true;
        for(auto i=0;i<itemList.length;i++){
            auto item = itemList[i];
            ret = ret && this->reduceItem(item.type, item.num);
            if(!ret) return false;
        }
        return ret;
    }

    void InventoryManagerModule::reduceItem (int itemId, int amount){
	    if(this->getInventory().getItemQuantity(itemId) < amount)
	        return false;
	    this->addItemQuantity(itemId, -amount);
	    return true;
    }

    void InventoryManagerModule::addItemQuantity (int itemId, int addAmount){
	    utils.log("add item quantity", itemId, addAmount)
	    if(itemId == ItemConstant.GOLD){
	        utils.log("add gold", addAmount);
	        gv.user.gold += addAmount;
	        return;
        }
        if(itemId == ItemConstant.CASH){
            gv.user.cash += addAmount;
            return;
        }
	    auto item = this->getInventory().getItemById(itemId);
	    if(item){
	        this->getInventory().setItemQuantity(itemId, item.quantity + addAmount);
        }
	    else{
            this->getInventory().setItemQuantity(itemId, addAmount);
        }
    }

    void InventoryManagerModule::useNextCue () {
        auto cues = this->getInventory().getCuesBagArray();

        utils.log("USE NEXT CUE PLS", cues.length, this->getInventory()._cueBag._usingCue, gv.gameMgr.cue.getCueInfo());
        for (auto i = 0; i < cues.length; i++) {
            utils.log("cue", cues[i].id, cues[i]._isUsing)
            if (cues[i].isUsing()) {
                utils.log("Send use cue in match", i, cues[i].id );
                this->sendUseCueInMatch(cues[(i + 1) % cues.length].id);
                break;
            }
        }
    }

    void InventoryManagerModule::clearInventory (){
	    this->getInventory().clearInventory();
	    this->refreshInventoryGUIs();
    }

    void InventoryManagerModule::convertCueIdToComponent (int cueId) {
	    auto obj = {};
	    obj.type = parseInt(cueId/1000);
	    obj.star = parseInt((cueId%1000)/100);
	    obj.configId = cueId%100;
	    return obj;
    }

    return InventoryMgr;
}
