#ifndef TypeCodecConfig_h_
#define TypeCodecConfig_h_

#include <map>
#include <json/document.h>

class TypeCodecConfig
{
public:
	TypeCodecConfig();
	~TypeCodecConfig();

	void loadConfig();

public:
	rapidjson::Document *data;
	std::map<std::string, int> items;
};

#endif // !TypeCodecConfig_h_



