#include "ConfigMgr.h"
#include <cocos2d.h>

using namespace std;
USING_NS_CC;

ConfigMgr::ConfigMgr()
{
	typeCodecConfig = nullptr;
}

ConfigMgr::~ConfigMgr()
{
	CC_SAFE_DELETE(typeCodecConfig);
}

void ConfigMgr::loadConfig()
{
	typeCodecConfig = new TypeCodecConfig();
	cueConfig		= new CueConfig();
	cueRangeConfig	= new CueRangeConfig();
	shopConfig		= new ShopConfig();
	channelConfig	= new ChannelConfig();
	offerConfig		= new OfferConfig();
	spinConfig		= new SpinConfig();
}

rapidjson::Document * ConfigMgr::loadJsonFile(const char * pzName)
{
	rapidjson::Document* document = new rapidjson::Document();
	string key = pzName;
	string data = UserDefault::sharedUserDefault()->getStringForKey(
		key.c_str(), "");
	if (data.compare("") == 0) {
		string fullPath = FileUtils::sharedFileUtils()->fullPathForFilename(pzName);
		long	_size;
		unsigned char	*_pFileContent = (unsigned char*)FileUtils::sharedFileUtils()->getFileData(fullPath, "r", &_size);
		if (!_pFileContent) return NULL;

		string str = string((char*)_pFileContent, _size);

		document->Parse<0>(str.c_str());
		delete _pFileContent;
		return document;
	}
	else {
		document->Parse<0>(data.c_str());
		return document;
	}
}

ConfigMgr * ConfigMgr::_instance = nullptr;
ConfigMgr * ConfigMgr::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new ConfigMgr();
		_instance->loadConfig();
	}
	return _instance;
}
