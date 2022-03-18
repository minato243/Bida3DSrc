#include "CueInPacket.h"

CreateCueInPacket::CreateCueInPacket(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

CreateCueInPacket::~CreateCueInPacket()
{

}

void CreateCueInPacket::readData()
{
	cue.id = _pk->getInt();
	cue.type = _pk->getByte();
	cue.star = _pk->getInt();
	cue.level = _pk->getInt();
	// cue.generalPoint = _pk->getInt();
	cue.battery = _pk->getInt();
	cue.configId = _pk->getInt();
	cue.aim = _pk->getInt();
	cue.accuracy = _pk->getDouble();
}

ResetCueBatteryInPacket::ResetCueBatteryInPacket(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

ResetCueBatteryInPacket::~ResetCueBatteryInPacket()
{

}

void ResetCueBatteryInPacket::readData()
{

}

std::string ResetCueBatteryInPacket::getStatus()
{
	switch (this->error) {
	case 0: return "Success";
	case 1: return "Cue not found";
	case 2: return "Fail";
	}
}

ChangeCueNameInPacket::ChangeCueNameInPacket(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

ChangeCueNameInPacket::~ChangeCueNameInPacket()
{

}

void ChangeCueNameInPacket::readData()
{
	if (this->error == 0)
	{
		id = _pk->getInt();
		name = _pk->getString();
	}
}

std::string ChangeCueNameInPacket::getStatus()
{
	switch (this->error) {
	case 0: return "Success";
	case 1: return "Cue not found";
	case 2: return "Invalid Cue";
	case 3: return "Fail";
	}
}

DeleteCueInPacket::DeleteCueInPacket(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

DeleteCueInPacket::~DeleteCueInPacket()
{

}

void DeleteCueInPacket::readData()
{
	if (this->error == 0) {
		itemId = _pk->getInt();
		newQuantity = _pk->getInt();
	}
}

std::string DeleteCueInPacket::getStatus()
{
	switch (this->error) {
	case 0: return "Success";
	case 1: return "Cue not found";
	case 2: return "FAIL";
	}
}

AccumulateDustInPacket::AccumulateDustInPacket(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

AccumulateDustInPacket::~AccumulateDustInPacket()
{

}

void AccumulateDustInPacket::readData()
{
	if (error == 0) {
		cueId = _pk->getInt();
		newDustInCue = _pk->getInt();
		generalPoint = _pk->getInt();
	}
}

std::string AccumulateDustInPacket::getStatus()
{
	switch (this->error) {
	case 0: return "Success";
	case 1: return "Cue not found";
	case 2: return "Not enough dust";
	case 3: return "Invalid general point";
	case 4: return "FAIL";
	}
}

BuyCueBoxInPacket::BuyCueBoxInPacket(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

BuyCueBoxInPacket::~BuyCueBoxInPacket()
{

}

void BuyCueBoxInPacket::readData()
{
	if (this->error == 0) {
		type = _pk->getInt();
		cueIdRes = _pk->getInt();
	}
}

std::string BuyCueBoxInPacket::getStatus()
{
	switch (this->error) {
	case 0: return "Success";
	case 1: return "Not enough coupon";
	case 2: return "Invalid box type";
	case 3: return "Fail";
	default: return "Not handled error";
	}
}

UpgradeCueInPacket::UpgradeCueInPacket(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

UpgradeCueInPacket::~UpgradeCueInPacket()
{

}

void UpgradeCueInPacket::readData()
{

}

std::string UpgradeCueInPacket::getStatus()
{
	switch (this->error) {
	case 0: return "Success";
	case 1: return "Invalid cue";
	case 2: return "Not enough gold";
	case 3: return "Not enough item";
	case 4: return "Cue already maxed";
	case 5: return "Failed";
	default: return "Undefined error";
	}
}

WorkshopUpgradeCueInPacket::WorkshopUpgradeCueInPacket(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

WorkshopUpgradeCueInPacket::~WorkshopUpgradeCueInPacket()
{

}

void WorkshopUpgradeCueInPacket::readData()
{

}

bool WorkshopUpgradeCueInPacket::isReduceMaterial()
{
	return error == 0 || error == 5;
}

std::string WorkshopUpgradeCueInPacket::getStatus()
{
	switch (this->error) {
	case 0: return "SUCCESS";
	case 1: return "INVALID CUE";
	case 2: return "CUE MAXED";
	case 3: return "INSUFFICIENT RESOURCE";
	case 4: return "INVALID MATERIAL";
	case 5: return "UPGRADE UNSUCCESS";
	case 6: return "FAILED";
	}
}

FusionCueInPacket::FusionCueInPacket(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

FusionCueInPacket::~FusionCueInPacket()
{

}

void FusionCueInPacket::readData()
{
	if (this->error == 0) {
		id = _pk->getInt();
		type = _pk->getInt();
		star = _pk->getInt();
		level = _pk->getInt();
		configId = _pk->getInt();
		generalPoint = _pk->getInt();
		aim = _pk->getInt();
		accuracy = _pk->getDouble();
	}
}

std::string FusionCueInPacket::getStatus()
{
	switch (this->error) {
	case 0: return "SUCCESS";
	case 1: return "INVALID CUE";
	case 2: return "INVALID CUE STAR";
	case 3: return "FAILED";
	}
}

SetCueAutoChargeInPacket::SetCueAutoChargeInPacket(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

SetCueAutoChargeInPacket::~SetCueAutoChargeInPacket()
{

}

void SetCueAutoChargeInPacket::readData()
{
	if (error == 0) {
		cueId = _pk->getInt();
		int autoCharge = _pk->getByte();
		on = (autoCharge == 1);
	}
}

std::string SetCueAutoChargeInPacket::getStatus()
{
	switch (error) {
	case 0: return "SUCCESS";
	case 1: return "INVALID CUE";
	case 2: return "CUE NOT EQUIPED";
	case 3: return "FAILED";
	}
}

WorkshopSwapCueInPacket::WorkshopSwapCueInPacket(fr::InPacket *pk)
	:BaseInPacket(pk)
{

}

WorkshopSwapCueInPacket::~WorkshopSwapCueInPacket()
{

}

void WorkshopSwapCueInPacket::readData()
{

}

std::string WorkshopSwapCueInPacket::getStatus()
{
	switch (error) {
	case 0: return "SUCCESS";
	case 1: return "INVALID CUE";
	case 2: return "NOT ENOUGH GOLD";
	case 3: return "NOT ENOUGH MAT";
	case 4: return "CUE TYPE INVALID";
	case 5: return "FAILED";
	}
}
