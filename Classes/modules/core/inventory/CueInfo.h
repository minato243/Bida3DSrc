#ifndef CueInfo_h_
#define CueInfo_h_

#include <string>
#include "../../../data/DataStruct.h"
#include <data/config/CueConfig.h>

struct TrajectoryLengthData
{
	double trajectA;
	double trajectB;
	double trajectC;
	double trajectE;

	TrajectoryLengthData(double trajectA, double trajectB, double trajectC, double trajectE);
	TrajectoryLengthData();
};

class CueInfo
{
public:
	CueInfo();
	~CueInfo();

	CueInfo(const CueInfoData & data);

	void initWithConfig(int configId);

	void initWithData(const CueInfoData & data);

	void initWithServerData(const CueInfoData & data);

	bool isCue();

	bool isDisappeared();

	void use();

	void stopUse();

	void putToBag();

	bool isUsing();

	bool isInBag();

	bool takeFromBag();

	void onShoot(double force);

	int getBatteryPercentage();

	int getMaxBattery();

	bool isFull();

	void charge();

	void upgrade_deprecated();

	void swap(CueInfo & dest);

	double getForce();

	double getAngular();

	std::string getAngleString();

	double* getAngle();

	void remove();

	TrajectoryLengthData getWorldTrajectoryLengths();

	double getDustPercent();

	void upgradeLevel();

	ModelPathData getModelPath();

	int getDustNeed();

	static int getRandomCueConfig(int type, int star);

	static std::string getRandomCueName();

	static CueConfigData getCueConfig(int type, int star, int level);

	static AccuracyConfigData getCueAccuracyConfig(int type, int star);

	static ModelPathData getCueModel(int configId, int type, int star, int level = 1);

protected:
private:
	
public:
	int _id;
	int _configId;
	int _tier;
	int _batteryDrainRate = 0;
	int _dustInCue = 0;
	int _level = 0;

	int _star;
	int _type;
	int _effectId;
	int _spriteId;

	std::string _alias = ""; // "nickname" for the cue
	int _scored = 0; // Total ball pocket by this cue
	bool _isInBag = false;
	bool _isUsing = false;

	int _aim;
	double _accuracy;
	int _generalPoint;

	double _angular;
	double _force;
	double _tip_e;
	double _tip_coef;
	double _angle[2];
	std::string _name;
	bool _removed;
};
#endif