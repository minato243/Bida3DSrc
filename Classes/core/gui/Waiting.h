#ifndef Waiting_h__
#define Waiting_h__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"

class Waiting:
	public cocos2d::Layer {
public:
	CREATE_FUNC(Waiting);
	Waiting();
	~Waiting();
	bool init();
public:
	void timeout(float time);
	void onEnter();
	void remove();
public:
	cocos2d::LayerColor* _layerColor;
	std::string _message;
	bool _fog;
	cocos2d::Sprite* _sprite;
};
#endif // Waiting_h__
