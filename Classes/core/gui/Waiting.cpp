#include "Waiting.h"

USING_NS_CC;
using namespace cocos2d::ui;

Waiting::Waiting()
{

}

Waiting::~Waiting()
{

}

bool Waiting::init()
{
	Layer::init();

	_layerColor = nullptr;
	_message = "";
	_fog = true;
	return true;
}

void Waiting::timeout(float time)
{
	runAction(Sequence::create(DelayTime::create(time), RemoveSelf::create(), nullptr));
}

void Waiting::onEnter()
{
	Layer::onEnter();

	_layerColor = LayerColor::create(Color4B::BLACK);
	_layerColor->runAction(FadeTo::create(0.25, 150));
	addChild(_layerColor);

	Size size = Director::getInstance()->getVisibleSize();
	float scale = size.width / 800;

	scale = (scale > 1) ? 1 : scale;

	_sprite = Sprite::create("common/circlewait.png");
	addChild(_sprite);
	_sprite->runAction(RepeatForever::create(RotateBy::create(1.2f, 360)));
	_sprite->setPosition(size.width / 2, size.height / 2);
	_sprite->setVisible(true);

	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event) {
		return true;
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void Waiting::remove()
{

}

