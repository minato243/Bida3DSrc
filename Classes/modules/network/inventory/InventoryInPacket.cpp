#include "InventoryInPacket.h"

using namespace pk;
UpdateInventoryInfo::UpdateInventoryInfo(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

UpdateInventoryInfo::~UpdateInventoryInfo()
{

}

void UpdateInventoryInfo::readData()
{
	auto cueNum = _pk->getInt();
	cues.clear();
	for (auto i = 0; i < cueNum; i++) {
		CueInfoData cue;
		readCue(cue);
		cues.push_back(cue);
	}
	bag.clear();
	auto cueBagNum = _pk->getByte();
	for (auto i = 0; i < cueBagNum; i++) {
		bag.push_back(_pk->getInt());
	}
	items.clear();
	auto itemNum = _pk->getInt();
	for (auto i = 0; i < itemNum; i++) {
		ItemInfoData item;
		item.type = _pk->getInt();
		item.num = _pk->getInt();
		items.push_back(item);
	}
	/*cueBoxesBuyTimes.clear();
	auto cueBoxesType = _pk->getInt();
	for (auto i = 0; i < cueBoxesType; i++) {
		CueBoxData cueBoxData = {};
		cueBoxData.type = _pk->getInt();
		cueBoxData.time = _pk->getInt();
		cueBoxesBuyTimes.push_back(cueBoxData);
	}*/
	int cuePartSize = _pk->getInt();
	for (auto i = 0; i < cuePartSize; i++)
	{
		CuePart cuePart;
		getPart(cuePart);
		cueParts.push_back(cuePart);
	}
}

void UpdateInventoryInfo::readCue(CueInfoData & cue)
{
	cue.id = _pk->getInt();
	cue.configId = _pk->getInt();
	cue.type = _pk->getByte();
	cue.star = _pk->getInt();
	cue.maxStar = _pk->getInt();
	cue.level = _pk->getInt();
	cue.aim = _pk->getInt();
	cue.accuracy = _pk->getDouble();
	// cue.alias = _pk->getString();
	cue.point = _pk->getInt();
	//cue.battery = _pk->getInt();
	// cue.batteryDrainRate = _pk->getInt();
	// cue.generalPoint = _pk->getInt();
	// cue.dust = _pk->getInt();
	cue.wonChallenges = _pk->getInt();
	//cue.autoCharge = _pk->getByte() == 1;

	auto partCount = _pk->getInt();
	for (auto i = 0; i < partCount; i++)
	{
		auto pos = _pk->getShort();
		auto havePart = _pk->getByte() == 1;
		if (havePart) {
			CuePart part;
			getPart(part);
			//cue.parts[pos] = part;
		}
		else {
			//cue.parts[pos] = null;
		}
	}
}

void UpdateInventoryInfo::getPart(CuePart &cuePart)
{
	cuePart.id = _pk->getInt();
	cuePart.pos = _pk->getShort();
	cuePart.type = _pk->getShort();
	cuePart.rank = _pk->getShort();
}

AddCueToBagInPacket::AddCueToBagInPacket(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

AddCueToBagInPacket::~AddCueToBagInPacket()
{

}

void AddCueToBagInPacket::readData()
{
	id = _pk->getInt();
}

RmCueFromBagInPacket::RmCueFromBagInPacket(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

RmCueFromBagInPacket::~RmCueFromBagInPacket()
{

}

void RmCueFromBagInPacket::readData()
{
	id = _pk->getInt();
}
