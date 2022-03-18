#ifndef CueInPacket_h_
#define CueInPacket_h_

#include "../base/BasePacket.h"
#include <data/DataStruct.h>

class CreateCueInPacket
	:public BaseInPacket
{
public:
	CreateCueInPacket(fr::InPacket *pk);
	~CreateCueInPacket();

	void readData();

public:
	CueInfoData cue;
};

class ResetCueBatteryInPacket
	:public BaseInPacket
{
public:
	ResetCueBatteryInPacket(fr::InPacket *pk);
	~ResetCueBatteryInPacket();

	void readData();

	std::string getStatus();

public:
};

class ChangeCueNameInPacket
	:public BaseInPacket
{
public:
	ChangeCueNameInPacket(fr::InPacket *pk);
	~ChangeCueNameInPacket();

	void readData();

	std::string getStatus();

public:
	int id;
	std::string name;
};


class DeleteCueInPacket
	:public BaseInPacket
{
public:
	DeleteCueInPacket(fr::InPacket *pk);
	~DeleteCueInPacket();

	void readData();

	std::string getStatus();

public:
	int itemId;
	int newQuantity;
};

class AccumulateDustInPacket
	:public BaseInPacket
{
public:
	AccumulateDustInPacket(fr::InPacket *pk);
	~AccumulateDustInPacket();

	void readData();

	std::string getStatus();

public:
	int cueId;
	int newDustInCue;
	int generalPoint;
};

class BuyCueBoxInPacket
	:public BaseInPacket
{
public:
	BuyCueBoxInPacket(fr::InPacket *pk);
	~BuyCueBoxInPacket();

	void readData();

	std::string getStatus();
public:
	int type;
	int cueIdRes;
};

class UpgradeCueInPacket
	:public BaseInPacket
{
public:
	UpgradeCueInPacket(fr::InPacket *pk);
	~UpgradeCueInPacket();

	void readData();

	std::string getStatus();

public:
};

class WorkshopUpgradeCueInPacket
	:public BaseInPacket
{
public:
	WorkshopUpgradeCueInPacket(fr::InPacket *pk);
	~WorkshopUpgradeCueInPacket();

	void readData();

	bool isReduceMaterial();

	std::string getStatus();
public:
};

class FusionCueInPacket
	:public BaseInPacket
{
public:
	FusionCueInPacket(fr::InPacket *pk);
	~FusionCueInPacket();

	void readData();

	std::string getStatus();
public:
	int id;
	int type;
	int star;
	int level;
	int configId;
	int generalPoint;
	int aim;
	int accuracy;
};

class WorkshopSwapCueInPacket
	:public BaseInPacket
{
public:
	WorkshopSwapCueInPacket(fr::InPacket *pk);
	~WorkshopSwapCueInPacket();

	void readData();

	std::string getStatus();

public:
};

class SetCueAutoChargeInPacket
	:public BaseInPacket
{
public:
	SetCueAutoChargeInPacket(fr::InPacket *pk);
	~SetCueAutoChargeInPacket();

	void readData();

	std::string getStatus();
public:
	int cueId;
	bool on;
};

#endif
