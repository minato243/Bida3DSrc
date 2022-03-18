#ifndef CueRangeConfig_h_
#define CueRangeConfig_h_

#include <json/document.h>
class CueRangeConfig
{
public:
	CueRangeConfig();
	~CueRangeConfig();

	void loadConfig();

	int getRandomCueConfig(int type, int star);

public:
	rapidjson::Document *data;
};
#endif

