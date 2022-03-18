#include "CueRangeConfig.h"
#include "ConfigMgr.h"
#include <cocos2d.h>
#include <string>

using namespace std;

CueRangeConfig::CueRangeConfig()
{
	loadConfig();
}

CueRangeConfig::~CueRangeConfig()
{
	CC_SAFE_DELETE(data);
}

void CueRangeConfig::loadConfig()
{
	char jsonPath[] = "res/config/TypeCodec.json";
	data = configMgr->loadJsonFile(jsonPath);
}

int CueRangeConfig::getRandomCueConfig(int type, int star)
{
	int limit = (*data)[to_string(type).c_str()][to_string(star).c_str()].GetInt();
	return (rand() % limit) + 1;
}

