#ifndef InventoryHandler_h_
#define InventoryHandler_h_

#include "BaseFramework.h"
#include <data/DataStruct.h>

#include "../base/BaseHandler.h"
#include "InventoryPacketBuilder.h"

#define inventoryHandler InventoryHandler::getInstance()

class InventoryHandler
	:BaseHandler
{
public:
	InventoryHandler();
	~InventoryHandler();

	bool onReceived(fr::InPacket* pk);

	void requestAddCueToBag(int id);

	void requestRemoveCueFromBag(int id);

	void requestAutoChargeCue(int id, bool value);

	void getInventoryInfo();

private:
	static InventoryHandler *_instance;
public:
	static InventoryHandler *getInstance();
};

#endif
