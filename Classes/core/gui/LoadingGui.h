#ifndef LoadingGui_h__
#define LoadingGui_h__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"

class LoadingGui:
	public cocos2d::Layer{
public:
	LoadingGui();
	~LoadingGui();
	static LoadingGui* create(std::string text = "", bool fog = true);
	bool init(std::string text = "", bool fog = true);
public:
	void timeout(float time);
	void onEnter();
	void remove();
public:
	cocos2d::LayerColor* _layerColor;
	cocos2d::ui::Text* _label;
	std::string _message;
	bool _fog;
	cocos2d::Sprite* commonbg;
	cocos2d::Sprite* commoncircel;

};
#endif // LoadingGui_h__
