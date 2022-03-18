#include "InventoryMgr.h"
#include "data/config/ConfigMgr.h"
#include "data/GameConstant.h"
#include "ItemConstant.h"
#include "../../ui/GuiMgr.h"
#include "../../network/game/GameHandler.h"

InventoryMgr::InventoryMgr()
{
	_inventory = new Inventory();
}

InventoryMgr::~InventoryMgr()
{
	CC_SAFE_DELETE(_inventory);
}

void InventoryMgr::onItemChange(int id)
{
	//TODO
}

Inventory * InventoryMgr::getInventory()
{
	return _inventory;
}

void InventoryMgr::onReceiveNewCue(CueInfoData newCue)
{
	auto cue = _inventory->addCue(newCue);
	//gv.GuiMgr.refreshGui(Popup.INVENTORY_UI);
	//gv.GuiMgr.refreshGui(Popup.COMPETITION_BAG_UI);

	////No more gacha box
	//auto gachaGUI = gv.GuiMgr.getGui(Popup.GACHA_ROLLING, CueGachaRollingList);
	//if (gachaGUI.isShow) {
	//	gachaGUI.lastData = { cueInfo: cue };
	//}
}

void InventoryMgr::onReceiveInventoryInfo(UpdateInventoryInfo *data)
{
	for (int i = 0; i < data->cues.size(); i++)
	{
		_inventory->addCue(data->cues[i]);
	}

	for (int i = 0; i < data->bag.size(); i++)
	{
		_inventory->getCueBag()->putCueToBag(_inventory->getCueById(data->bag[i]));
	}

	for (int i = 0; i < data->items.size(); i++)
	{
		_inventory->setItemQuantity(data->items[i].type, data->items[i].num);
	}

	int boxId[] = { ItemConstant::CUE_BOX_SONG_TOAN, ItemConstant::CUE_BOX_BOC_PHA,
	ItemConstant::CUE_BOX_CAU_BONG, ItemConstant::CUE_BOX_XOAN_OC};

	for (int i = 0; i < 4; i++)
	{

	}
}

void InventoryMgr::updateCueAutoRecharge(int cueId, bool on)
{
	auto cue = _inventory->getCueById(cueId);

	guiMgr->refreshGui(Popup::INVENTORY_UI);
	guiMgr->refreshGui(Popup::CUE_INSPECT_UI);
}

bool InventoryMgr::isCueBagFull()
{
	bool ret = _inventory->getCuesBagArray().size() == GameConstant::CUE_IN_BAG;
	return ret;
}

void InventoryMgr::addCueToBag(int id)
{
	_inventory->getCueBag()->putCueToBag(_inventory->getCueById(id));
	guiMgr->refreshGui(Popup::INVENTORY_UI);
	guiMgr->refreshGui(Popup::COMPETITION_BAG_UI);
	guiMgr->refreshGui(Popup::CHANNEL_SELECT);
}

void InventoryMgr::removeCueFromBag(int id)
{
	_inventory->getCueBag()->takeCueFromBag(_inventory->getCueById(id));
	guiMgr->refreshGui(Popup::INVENTORY_UI);
	//guiMgr->refreshGui(Popup::COMPETITION_BAG_UI);
	//guiMgr->refreshGui(Popup::CHANNEL_SELECT);
}

void InventoryMgr::sendUseCueInMatch(int cueId)
{
	gameHandler->sendSwitchCue(cueId);
}

void InventoryMgr::useCueInMatch(int cueId)
{
	auto cueBag = _inventory->getCueBag();
	auto cueInfo = cueBag->getCueInfo(cueId);
	cueBag->useCue(cueInfo);
}

std::vector<CueInfoData *> InventoryMgr::getCueCollectionData(int type)
{
	auto cues = getInventory()->getCueArray();
	std::vector<CueInfoData *> ret = configMgr->cueConfig->getCueConfigWithType(type);

	for (auto i = 0; i < cues.size(); i++) {
		auto cue = cues[i];
		if (cue->_type != type) continue;
		auto star = cue->_tier;
		auto configId = cue->_configId;
		
		for (int j = 0; j < ret.size(); j++)
		{
			if (ret[j]->star == star && ret[j]->configId == configId)
			{
				ret[j]->owned = true;
			}
		}
	}

	return ret;
}

void InventoryMgr::useNextCue()
{
	auto cues = _inventory->getCuesBagArray();
	for (int i = 0; i < cues.size(); i++) 
	{
		auto cue = cues[i];
		if (cue->_isUsing)
		{
			int nextCueId = cues[(i + 1) % cues.size()]->_id;
			sendUseCueInMatch(nextCueId);
			break;
		}
	}
}

CueInfoData InventoryMgr::convertCueIdToComponent(int cueId)
{
	CueInfoData obj;
	obj.type = cueId / 1000;
	obj.star = (cueId % 1000) / 100;
	obj.configId = cueId % 100;
	return obj;
}

InventoryMgr * InventoryMgr::_instance = nullptr;
InventoryMgr * InventoryMgr::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new InventoryMgr();
	}
	return _instance;
}
