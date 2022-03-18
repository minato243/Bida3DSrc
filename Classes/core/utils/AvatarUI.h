#ifndef AvatarUI_h__
#define AvatarUI_h__
#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "Avatar.h"

class AvatarUI:
	public cocos2d::Node{
public:
	AvatarUI();
	~AvatarUI();
	static AvatarUI* create(std::string defaultImg, std::string maskAvatar);
	bool init(std::string defaultImg, std::string maskAvatar);
public:
	void setImage(std::string path);
	void setDefaultImage();
	void setMaskSprite(std::string maskAvatar);
	void asyncExecuteWithUrl(std::string path, std::string url);
	void asyncExecute();
public:
	std::string _defaultImg;
	std::string _urlImg;
	std::string _pathImg;
	Avatar* avatar;
	int _downloading;
	cocos2d::ClippingNode* clipping;
	cocos2d::ui::ImageView* image;

};
#endif // AvatarUI_h__
