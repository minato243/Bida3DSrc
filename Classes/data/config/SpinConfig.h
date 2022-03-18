#ifndef SpinConfig_h_
#define SpinConfig_h_

#include <json/document.h>
#include <string.h>
#include <data/DataStruct.h>

struct WheelItemConfig
{
	int cellId;
	long long gold;
};

class SpinConfig
{
public:
	SpinConfig();
	~SpinConfig();

	void loadConfig();

	const WheelItemConfig &getItemConfig(int cellId);

public:
	rapidjson::Document *data;
	std::vector<WheelItemConfig> wheelItems;
};

#endif
