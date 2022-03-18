#ifndef CueConfig_h_
#define CueConfig_h_

#include <json/document.h>
#include <string.h>
#include <data/DataStruct.h>

struct CueConfigData
{
	int type;
	int star;
	int configId;
	double tip_u;
	double angle[2];
	double force;
	double accuracy;
	int aim;
};

struct AccuracyConfigData
{
	double accuracy[2];
	int trajectory[2];
};

struct CueModelConfigData
{
	int type;
	int star;
	int configId;
	std::string model;
	std::string texture;
	std::string mrao;
};

struct CueRateInBox
{
	int cueId;
	int rate;
	int type;
};
struct CueBoxConfigData
{
	int boxId;
	std::string name;
	int price;

	std::vector<CueRateInBox> listCue;
};

class CueConfig 
{
public:
	CueConfig();
	~CueConfig();

	void loadConfig();

	CueConfigData getCueConfigData(int type, int star, int level);

	AccuracyConfigData getAccuracyConfigData(int type, int star);

	ModelPathData getModelPath(int type, int tier, int configId);

	std::vector<CueInfoData *> getCueConfigWithType(int type);

	std::vector<CueBoxConfigData *> getCueBoxConfigList();
	
	std::string getCueBoxName(int boxIdx);

public:
	rapidjson::Document *data;
	rapidjson::Document *cueUpgradeData;
	rapidjson::Document *cueModelData;
	rapidjson::Document *physicsCueData;
	rapidjson::Document *cueBoxData;
	rapidjson::Document *cueChargeBatteryData;
	rapidjson::Document *cueWorkshopData;
	rapidjson::Document *cueRangeData;
};
#endif // !CueConfig_h_

