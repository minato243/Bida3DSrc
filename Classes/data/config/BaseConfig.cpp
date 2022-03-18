#include "BaseConfig.h"
#include <cocos2d.h>

BaseConfig::BaseConfig()
{
	loadConfig();
}

BaseConfig::~BaseConfig()
{
	CC_SAFE_DELETE(data);
}
