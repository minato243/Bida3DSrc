#ifndef CueOutMsg_h_
#define CueOutMsg_h_

#include <BaseFramework.h>
#include "../base/BasePacket.h"
#include <data/DataStruct.h>

class CmdCreateCue
	: public BaseOutMsg
{
public:
	CmdCreateCue();
	~CmdCreateCue();

	void packData(int type);
};

class CmdChangeCueName
	: public BaseOutMsg
{
public:
	CmdChangeCueName();
	~CmdChangeCueName();

	void packData(int cueId, std::string cueName);
};

class CmdChargeCueBattery
	: public BaseOutMsg
{
public:
	CmdChargeCueBattery();
	~CmdChargeCueBattery();

	void packData(int cueId);
};

class CmdDeleteCue
	: public BaseOutMsg
{
public:
	CmdDeleteCue();
	~CmdDeleteCue();

	void packData(int cueId);
};

class CmdAccumulateDust
	: public BaseOutMsg
{
public:
	CmdAccumulateDust();
	~CmdAccumulateDust();

	void packData(int cueId, int dust);
};

class CmdBuyCueBox
	: public BaseOutMsg
{
public:
	CmdBuyCueBox();
	~CmdBuyCueBox();

	void packData(int type, int quantity);
};

class CmdCheatSetInventoryItem
	: public BaseOutMsg
{
public:
	CmdCheatSetInventoryItem();
	~CmdCheatSetInventoryItem();

	void packData(int type, int quantity);
};

class CmdSendUpgradeCue
	: public BaseOutMsg
{
public:
	CmdSendUpgradeCue();
	~CmdSendUpgradeCue();

	void packData(int id);
};

class CmdSendWorkshopUpgradeCue
	: public BaseOutMsg
{
public:
	CmdSendWorkshopUpgradeCue();
	~CmdSendWorkshopUpgradeCue();

	void packData(int id, const std::vector<MatData> & mats);
};

class CmdSendWorkshopFusionCues
	: public BaseOutMsg
{
public:
	CmdSendWorkshopFusionCues();
	~CmdSendWorkshopFusionCues();

	void packData(const int* ids);
};


class CmdSendWorkshopSwapCues
	: public BaseOutMsg
{
public:
	CmdSendWorkshopSwapCues();
	~CmdSendWorkshopSwapCues();

	void packData(int src, int dst);
};

class CmdSetAutoChargeCue
	: public BaseOutMsg
{
public:
	CmdSetAutoChargeCue();
	~CmdSetAutoChargeCue();

	void packData(int id, bool val);
};

#endif
