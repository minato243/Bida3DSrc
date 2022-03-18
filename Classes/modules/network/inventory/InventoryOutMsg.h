#ifndef InventoryOutMsg_h_
#define InventoryOutMsg_h_

#include <BaseFramework.h>
#include "../base/BasePacket.h"
#include <data/DataStruct.h>

class CmdAddCueToBag
	: public BaseOutMsg
{
public:
	CmdAddCueToBag();
	~CmdAddCueToBag();

	void packData(int id);
};


class CmdRmCueFromBag
	: public BaseOutMsg
{
public:
	CmdRmCueFromBag();
	~CmdRmCueFromBag();

	void packData(int id);
};

#endif
