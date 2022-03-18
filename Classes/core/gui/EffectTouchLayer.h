#ifndef EffectTouchLayer_h__
#define EffectTouchLayer_h__

#include "cocos2d.h"

class EffectTouchLayer:
	public cocos2d::Layer{
public:
	struct Paper {
		bool isRunning;
		cocos2d::Sprite* sprite;
	};
public:
	CREATE_FUNC(EffectTouchLayer);
	EffectTouchLayer();
	~EffectTouchLayer();
	static EffectTouchLayer* getInstance();
public:
	bool init();
	void onExit();
	void effectTouch(cocos2d::Vec2 pos);
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
	void startEffect();
	void startEffectVip();
	void stopEffect();
	void effectPaperBirthday(float dt);
	void effectPaperVip(float dt);
public:
	cocos2d::Sprite* iconTouch;
	int sttIcon;
	std::vector<cocos2d::Sprite*> arrayDeco;
	cocos2d::EventListenerTouchOneByOne* _listener;
	std::vector<Paper*> paperList;
	
	static EffectTouchLayer* _instance;
	static const int TAG;


};
#endif // EffectTouchLayer_h__
