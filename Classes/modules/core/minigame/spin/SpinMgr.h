#ifndef SpinMgr_h_
#define SpinMgr_h_

#include "../../../../data/config/SpinConfig.h"

class SpinConstant
{
public:
	static const float POINTER_ANGLE;
	static const float	CELL_ANGLE;
	static const int N_TURNING_ROUND = 2;
	static const float BASE_ANGLE_SPEED;
	static const int GOLD_PER_COIN = 100;
	static const int N_REWARD_COIN = 10;
	static const int N_CELL = 14;
};

class SpinMgr
{
public:
	SpinMgr();
	~SpinMgr();

	WheelItemConfig getCellById(int cellId);

	int getNumberOfSpin();

	float getBisectorAngleByCellId(int cellId);

	void setNumberOfSpin(const int & nSpin);

	void sendSpinWheel();

	void onResponeSpinWheel(const int & cellId);

	void sendCheatSpin();
private:
	int _numberOfSpin;

private:
	static SpinMgr *_instance;

public:
	static SpinMgr *getInstance();
};

#define spinMgr SpinMgr::getInstance()

#endif
