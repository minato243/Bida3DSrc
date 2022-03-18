#include "ResourceMgr.h"
#include "cocos2d.h"
#include "renderer/DBCCFactory.h"

USING_NS_CC;
using namespace dragonBones;

ResourceMgr::ResourceMgr()
{

}

ResourceMgr::~ResourceMgr()
{

}

ResourceMgr* ResourceMgr::getInstance()
{
	if (!_instance)
	{
		_instance = new ResourceMgr();
	}
	return _instance;
}

void ResourceMgr::init()
{
	preloadResource();
	preloadAnimation();
	preloadSound();
}

void ResourceMgr::preloadResource()
{

}

void ResourceMgr::preloadAnimation()
{
    /*
	DBCCFactory::getInstance()->loadDragonBonesData("Armatures/Emoticon/skeleton.xml","Emoticon");
	DBCCFactory::getInstance()->loadTextureAtlas("Armatures/Emoticon/texture.plist", "Emoticon");

	DBCCFactory::getInstance()->loadDragonBonesData("Armatures/SaoBay/skeleton.xml", "SaobayCard");
	DBCCFactory::getInstance()->loadTextureAtlas("Armatures/SaoBay/texture.plist", "SaobayCard");
	DBCCFactory::getInstance()->loadDragonBonesData("Armatures/tomorrowStamp/skeleton.xml", "tomorrowStamp");
	DBCCFactory::getInstance()->loadTextureAtlas("Armatures/tomorrowStamp/texture.plist", "tomorrowStamp");
	DBCCFactory::getInstance()->loadDragonBonesData("Armatures/InGame/hand_guide/skeleton.xml", "hand_guide");
	DBCCFactory::getInstance()->loadTextureAtlas("Armatures/InGame/hand_guide/texture.plist", "hand_guide");
     */
	// load sprite frame
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("common/gold.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("common/coin.plist");
}

void ResourceMgr::preloadSound()
{

}

ResourceMgr* ResourceMgr::_instance = nullptr;

