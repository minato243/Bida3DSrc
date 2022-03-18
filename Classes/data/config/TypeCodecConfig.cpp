#include "TypeCodecConfig.h"
#include "ConfigMgr.h"
#include <cocos2d.h>

using namespace std;

TypeCodecConfig::TypeCodecConfig()
{
	loadConfig();
}

TypeCodecConfig::~TypeCodecConfig()
{
	CC_SAFE_DELETE(data);
}

void TypeCodecConfig::loadConfig()
{
	char jsonPath[] = "res/config/TypeCodec.json";
	data = configMgr->loadJsonFile(jsonPath);

	for (rapidjson::Value::ConstMemberIterator itr = (*data).MemberBegin(); itr != (*data).MemberEnd(); itr++) {
		string name = itr->name.GetString();
		int itemId = itr->value.GetInt();
		items.insert(pair<string, int>(name, itemId));
	}
}
