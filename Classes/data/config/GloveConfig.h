#ifndef GloveConfig_h_
#define GloveConfig_h_

#include <string>
#include <json/document.h>
#include <vector>

class MatchingGloveConfigData
{
public:
	int min, max, require, prize;

};

class GloveConfig
{
public:
	GloveConfig();
	~GloveConfig();


	void loadConfig();

	MatchingGloveConfigData getMatchingGloveConfig(int glove);

private:
	rapidjson::Document *data;

	std::vector<MatchingGloveConfigData> matchingGloveConfigs;
};

#endif