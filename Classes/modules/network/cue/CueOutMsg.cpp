#include "CueOutMsg.h"
#include "../../../data/GameConstant.h"

using namespace pk;

CmdCreateCue::CmdCreateCue()
	:BaseOutMsg(PacketID::CREATE_CUE)
{

}

CmdCreateCue::~CmdCreateCue()
{

}

void CmdCreateCue::packData(int type)
{
	putByte(type);
}


CmdChangeCueName::CmdChangeCueName()
	:BaseOutMsg(PacketID::CHANGE_EXTRA_CUE_NAME)
{

}

CmdChangeCueName::~CmdChangeCueName()
{

}

void CmdChangeCueName::packData(int cueId, std::string cueName)
{
	putInt(cueId);
	putString(cueName);
}

CmdChargeCueBattery::CmdChargeCueBattery()
	:BaseOutMsg(PacketID::RESET_CUE_BATTERY)
{

}

CmdChargeCueBattery::~CmdChargeCueBattery()
{

}

void CmdChargeCueBattery::packData(int cueId)
{
	putInt(cueId);
}

CmdDeleteCue::CmdDeleteCue()
	:BaseOutMsg(PacketID::DELETE_CUE)
{

}

CmdDeleteCue::~CmdDeleteCue()
{

}

void CmdDeleteCue::packData(int cueId)
{
	putInt(cueId);
}

CmdAccumulateDust::CmdAccumulateDust()
	:BaseOutMsg(PacketID::ACCUMULATE_DUST)
{

}

CmdAccumulateDust::~CmdAccumulateDust()
{

}

void CmdAccumulateDust::packData(int cueId, int dust)
{
	putInt(cueId);
	putInt(dust);
}

CmdBuyCueBox::CmdBuyCueBox()
	:BaseOutMsg(PacketID::BUY_CUE_BOX)
{

}

CmdBuyCueBox::~CmdBuyCueBox()
{

}

void CmdBuyCueBox::packData(int type, int quantity)
{
	putInt(type);
	putInt(quantity);
}

CmdCheatSetInventoryItem::CmdCheatSetInventoryItem()
	:BaseOutMsg(PacketID::CHEAT_SET_INVENTORY_ITEM)
{

}

CmdCheatSetInventoryItem::~CmdCheatSetInventoryItem()
{

}

void CmdCheatSetInventoryItem::packData(int type, int quantity)
{
	putInt(type);
	putInt(quantity);
}

CmdSendUpgradeCue::CmdSendUpgradeCue()
	:BaseOutMsg(PacketID::UPGRADE_CUE)
{

}

CmdSendUpgradeCue::~CmdSendUpgradeCue()
{

}

void CmdSendUpgradeCue::packData(int id)
{
	putInt(id);
}

CmdSendWorkshopUpgradeCue::CmdSendWorkshopUpgradeCue()
	:BaseOutMsg(PacketID::WORKSHOP_UPGRADE_CUE)
{

}

CmdSendWorkshopUpgradeCue::~CmdSendWorkshopUpgradeCue()
{

}

void CmdSendWorkshopUpgradeCue::packData(int cueId, const std::vector<MatData> & mats)
{
	putInt(cueId);
	putInt(mats.size());
	for (int i = 0; i < mats.size(); i++) {
		putInt(mats[i].id);
		putInt(mats[i].quantity);
	}
}

CmdSendWorkshopFusionCues::CmdSendWorkshopFusionCues()
	:BaseOutMsg(PacketID::WORKSHOP_FUSION_CUES)
{

}

CmdSendWorkshopFusionCues::~CmdSendWorkshopFusionCues()
{

}

void CmdSendWorkshopFusionCues::packData(const int * ids)
{
	for (int i = 0; i < NUM_CUES_FUSION; i++)
	{
		putInt(ids[i]);
	}
}

CmdSendWorkshopSwapCues::CmdSendWorkshopSwapCues()
	:BaseOutMsg(PacketID::WORKSHOP_SWAP_CUES)
{

}

CmdSendWorkshopSwapCues::~CmdSendWorkshopSwapCues()
{

}

void CmdSendWorkshopSwapCues::packData(int src, int dst)
{
	putInt(src);
	putInt(dst);
}

CmdSetAutoChargeCue::CmdSetAutoChargeCue()
	:BaseOutMsg(PacketID::SET_CUE_AUTO_CHARGE)
{

}

CmdSetAutoChargeCue::~CmdSetAutoChargeCue()
{

}

void CmdSetAutoChargeCue::packData(int id, bool val)
{
	putInt(id);
	putByte(val ? 1:0);
}
