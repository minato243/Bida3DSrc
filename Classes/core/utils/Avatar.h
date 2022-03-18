#ifndef Avatar_h__
#define Avatar_h__
#include "cocos2d.h"
#include "BaseFramework.h"
class Avatar:
	public cocos2d::Node{
public:
	Avatar();
	~Avatar();
	static Avatar* create(std::string defaultImg, std::string url = "");
	bool init(std::string defaultImg, std::string url = "");
public:
	void updateAvatar(std::string url);
	void onFinishLoad(bool result);
	std::string getStorePath(std::string url);
public:
	cocos2d::Sprite* defaultAvatar;
	fr::AsyncSprite* avatar;
};
#endif // Avatar_h__
