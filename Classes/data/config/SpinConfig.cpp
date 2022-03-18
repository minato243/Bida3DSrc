#include "SpinConfig.h"
#include "ConfigMgr.h"

using namespace std;
using namespace rapidjson;

SpinConfig::SpinConfig()
{
	loadConfig();
}

SpinConfig::~SpinConfig()
{

}

void SpinConfig::loadConfig()
{
	data = configMgr->loadJsonFile("res/config/SpinConfig.json");
	
	Value &itemList = (*data)["wheel"];
	for (auto i = 0; i < itemList.Size(); i++)
	{
		WheelItemConfig item;
		item.gold = itemList[i]["gold"].GetInt64();
		item.cellId = itemList[i]["cellId"].GetInt();
		wheelItems.push_back(item);
	}
}

const WheelItemConfig & SpinConfig::getItemConfig(int cellId)
{
	return wheelItems[cellId];
}

