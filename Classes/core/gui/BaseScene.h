#ifndef BaseScene_h__
#define BaseScene_h__

#include "cocos2d.h"

class BaseScene:
	public cocos2d::Scene
{
public:
	BaseScene();
	~BaseScene();
	static BaseScene* createWithLayer(cocos2d::Node* layer);
	static BaseScene* makeScene(cocos2d::Node* layer);
	bool init();
	void onEnter();
public:
	void addChild(cocos2d::Node* child, int tag = 101, int order = 101);
	cocos2d::Node* getMainLayer();
	cocos2d::Node* getLayerGUI();
	void update(float dt);
public:
	static const int TAG_LAYER;
	static const int TAG_GUI;
};

#endif // BaseScene_h__
