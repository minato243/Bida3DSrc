#include "InventoryOutMsg.h"

using namespace pk;

CmdAddCueToBag::CmdAddCueToBag()
	:BaseOutMsg(PacketID::ADD_CUE_TO_BAG)
{

}

CmdAddCueToBag::~CmdAddCueToBag()
{

}

void CmdAddCueToBag::packData(int id)
{
	putInt(id);
}

CmdRmCueFromBag::CmdRmCueFromBag()
	:BaseOutMsg(PacketID::RM_CUE_FROM_BAG)
{

}

CmdRmCueFromBag::~CmdRmCueFromBag()
{

}

void CmdRmCueFromBag::packData(int id)
{
	putInt(id);
}
