#ifndef InventoryInPacket_h_
#define InventoryInPacket_h_

#include "../base/BasePacket.h"
#include <data/DataStruct.h>

class UpdateInventoryInfo
	:public BaseInPacket
{
public:
	UpdateInventoryInfo(fr::InPacket *pk);
	~UpdateInventoryInfo();

	void readData();

	void readCue(CueInfoData & cue);

	void getPart(CuePart &cuePart);

public:
	std::vector<CueInfoData> cues;
	std::vector<ItemInfoData> items;
	std::vector<int> bag;
	std::vector<CueBoxData> cueBoxesBuyTimes;
	std::vector<CuePart> cueParts;
};

class AddCueToBagInPacket
	:public BaseInPacket
{
public:
	AddCueToBagInPacket(fr::InPacket *pk);
	~AddCueToBagInPacket();

	void readData();

public:
	int id;
};

class RmCueFromBagInPacket
	:public BaseInPacket
{
public:
	RmCueFromBagInPacket(fr::InPacket *pk);
	~RmCueFromBagInPacket();

	void readData();

public:
	int id;
};


#endif