#ifndef BaseConfig_h_
#define BaseConfig_h_

#include <json/document.h>

class BaseConfig
{
public:
	BaseConfig();
	~BaseConfig();

	virtual void loadConfig()=0;

public:
	rapidjson::Document *data;
};
#endif
