#include "BaseScene.h"
#include "SceneMgr.h"
#include "EffectTouchLayer.h"
USING_NS_CC;

BaseScene::BaseScene()
{

}

BaseScene::~BaseScene()
{

}

BaseScene* BaseScene::createWithLayer(cocos2d::Node* layer)
{
	BaseScene* scene = new BaseScene();
	scene->autorelease();
	scene->addChild(layer);
	return scene;
}

BaseScene* BaseScene::makeScene(cocos2d::Node* layer)
{
	BaseScene* scene = new BaseScene();
	scene->addChild(layer);
	scene->autorelease();
	return scene;
}

bool BaseScene::init()
{
	Scene::init();
	this->scheduleUpdate();
	return true;
}

void BaseScene::onEnter()
{
	Scene::onEnter();
}

void BaseScene::addChild(cocos2d::Node* child, int tag, int order)
{
	Scene::addChild(child);
	child->setTag(tag);
	child->setLocalZOrder(order);
	setContentSize(Director::getInstance()->getVisibleSize());
	setAnchorPoint(Vec2(0.5, 0.5));

}

cocos2d::Node* BaseScene::getMainLayer()
{
	return getChildByTag(BaseScene::TAG_LAYER);
}

cocos2d::Node* BaseScene::getLayerGUI()
{
	return getChildByTag(BaseScene::TAG_GUI);
}

void BaseScene::update(float dt)
{
	//sceneMgr->updateScene(dt);
}

const int BaseScene::TAG_LAYER = 101;

const int BaseScene::TAG_GUI = 102;

