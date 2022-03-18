#include "CueConfig.h"
#include "ConfigMgr.h"
#include <string>
#include <cocos2d.h>
#include <data/GameConstant.h>

using namespace std;
using namespace rapidjson;

CueConfig::CueConfig()
{
	loadConfig();
}

CueConfig::~CueConfig()
{
	CC_SAFE_DELETE(data);
	CC_SAFE_DELETE(cueUpgradeData);
}

void CueConfig::loadConfig()
{
	data			= configMgr->loadJsonFile("res/config/CueConfig.json");
	cueUpgradeData	= configMgr->loadJsonFile("res/config/CueUpgradeConfig.json");
	cueModelData	= configMgr->loadJsonFile("res/config/CueModelConfig.json");
	physicsCueData	= configMgr->loadJsonFile("res/config/PhysicsCueConfig.json");
	cueBoxData		= configMgr->loadJsonFile("res/config/CueBoxConfig.json");
	cueChargeBatteryData	= configMgr->loadJsonFile("res/config/CueChargeBatteryConfig.json");
	cueWorkshopData			= configMgr->loadJsonFile("res/config/WorkshopUpgrade.json");
	cueRangeData			= configMgr->loadJsonFile("res/config/CueConfigRange.json");
}

CueConfigData CueConfig::getCueConfigData(int type, int star, int level)
{
	rapidjson::Value &config = (*data)["StarLevel"][to_string(type).c_str()][to_string(star).c_str()][to_string(level).c_str()];
	CueConfigData configData;
	configData.tip_u = config["tip_u"].GetDouble();
	configData.angle[0] = config["angle"][0].GetInt();
	configData.angle[1] = config["angle"][1].GetInt();
	configData.force = config["force"].GetDouble();
	configData.accuracy = config["accuracy"].GetDouble();
	configData.aim = config["aim"].GetInt();
	return configData;
}

AccuracyConfigData CueConfig::getAccuracyConfigData(int type, int star)
{
	rapidjson::Value &config = (*data)["Accuracy"][to_string(type).c_str()][to_string(star).c_str()];
	AccuracyConfigData configData;

	configData.accuracy[0] = config["accuracy"][0].GetDouble();
	configData.accuracy[1] = config["accuracy"][1].GetDouble();
	configData.trajectory[0] = config["trajectory"][0].GetDouble();
	configData.trajectory[1] = config["trajectory"][1].GetDouble();
	return configData;
}

ModelPathData CueConfig::getModelPath(int type, int tier, int configId)
{
	rapidjson::Value &config = (*cueModelData)[cto_string(type)][cto_string(tier)][cto_string(configId)];
	ModelPathData modelPath;
	//modelPath.type = config["type"].GetInt();
	//modelPath.star = config["star"].GetInt();
	//modelPath.configId = config["configId"].GetInt();
	modelPath.model = config["model"].GetString();
	modelPath.tex = config["texture"].GetString();
	modelPath.mrao = config["mrao"].GetString();

	return modelPath;
}

std::vector<CueInfoData*> CueConfig::getCueConfigWithType(int type)
{
	std::vector<CueInfoData *> ret;
	for (int star = 0; star < 5; star++)
	{
		int tier = star + 1;
		rapidjson::Value &config = (*cueModelData)[cto_string(type)][cto_string(tier)];
		for (rapidjson::Value::ConstMemberIterator itr = (config).MemberBegin(); itr != (config).MemberEnd(); itr++) {
			CueInfoData *data = new CueInfoData();
			data->type = type;
			data->star = tier;
			data->configId = atoi(itr->name.GetString());
			data->owned = false;
			
			ret.push_back(data);
		}
	}

	return ret;
}

std::vector<CueBoxConfigData *> CueConfig::getCueBoxConfigList()
{
	std::vector<CueBoxConfigData *> ret;

	const Value& listBox = (*cueBoxData)["listBox"];
	for (SizeType i = 0; i < listBox.Size(); i++)
	{
		const Value& config = listBox[i];
		CueBoxConfigData *item = new CueBoxConfigData();
		item->boxId = config["boxId"].GetInt();
		item->name = config["name"].GetString();
		item->price = config["price"].GetInt();

		for (SizeType i = 0; i < config["listCue"].Size(); i++)
		{
			CueRateInBox cue;
			cue.cueId = config["listCue"][i]["cueId"].GetInt();
			cue.rate = config["listCue"][i]["rate"].GetInt();
			item->listCue.push_back(cue);
		}
		ret.push_back(item);
	}

	return ret;
}

std::string CueConfig::getCueBoxName(int boxIdx)
{
	auto ret = (*cueBoxData)["listBox"][boxIdx]["name"].GetString();
	return ret;
}

