#include "EffectTouchLayer.h"
#include "SceneMgr.h"

USING_NS_CC;

EffectTouchLayer::EffectTouchLayer()
{

}

EffectTouchLayer::~EffectTouchLayer()
{

}

EffectTouchLayer* EffectTouchLayer::getInstance()
{
	if (!_instance) {
		_instance = EffectTouchLayer::create();
		_instance->retain();
	}
	return _instance;
}

bool EffectTouchLayer::init()
{
	Layer::init();

	Size winSize = Director::getInstance()->getVisibleSize();
	setContentSize(winSize);

	iconTouch = Sprite::create();
	iconTouch->setVisible(false);
	addChild(iconTouch);
	sttIcon = 0;

	int NUM_DECO = 3;
	arrayDeco.clear();

	for (int i = 0; i < NUM_DECO; i++) {
		Sprite* deco = Sprite::create("Lobby/Common/touchDeco.png");
		deco->setBlendFunc(BlendFunc{ GL_DST_COLOR, GL_ONE });
		addChild(deco);
		Sprite* square = Sprite::create("Lobby/Common/touchSquare.png");
		square->setName("square");
		deco->addChild(square);
		square->setBlendFunc(BlendFunc{ GL_DST_COLOR, GL_ONE });
		deco->setCascadeOpacityEnabled(false);
		square->setPosition(Vec2(deco->getContentSize().width / 2, deco->getContentSize().height / 2));
		square->setScale(1.05, 1.05);
		arrayDeco.push_back(deco);
		deco->setVisible(false);
	}
	_listener = EventListenerTouchOneByOne::create();
	_listener->onTouchBegan = CC_CALLBACK_2(EffectTouchLayer::onTouchBegan, this);
	_listener->setSwallowTouches(false);

	_director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_listener, this);

	return true;
}

void EffectTouchLayer::onExit()
{
	Layer::onExit();

	iconTouch->setVisible(false);
	for (Sprite* deco: arrayDeco) 
		deco->setVisible(false);

}

void EffectTouchLayer::effectTouch(cocos2d::Vec2 pos)
{
	float EFF_TIME = 0.7f;
	iconTouch->initWithFile(StringUtils::format("Lobby/Common/touchIcon%d.png",sttIcon));
	sttIcon++;
	if (sttIcon >= 4) sttIcon = 0;
	iconTouch->setBlendFunc(BlendFunc{GL_DST_COLOR, GL_ONE});
	iconTouch->setPosition(pos);
	iconTouch->setRotation(CCRANDOM_0_1() * 360);
	iconTouch->setVisible(true);
	iconTouch->setScale(0);
	iconTouch->setOpacity(255);
	iconTouch->stopAllActions();
	iconTouch->runAction(Sequence::create(
		DelayTime::create(0.1f),
		Spawn::create(
			RotateBy::create(0.6f, 45),
			EaseSineOut::create(ScaleTo::create(0.6f, 1.5)),
			EaseSineIn::create(FadeOut::create(0.6f)),
			nullptr),
		nullptr
	));

	/////
	for (Sprite* deco:arrayDeco) {
		deco->stopAllActions();
		deco->setVisible(true);
		deco->setPosition(pos);
		deco->setRotation(CCRANDOM_0_1() * 90);
		deco->setOpacity(0);
		deco->setScale(0.2f);
		deco->getChildByName("square")->setOpacity(0);
		deco->getChildByName("square")->stopAllActions();
		//
		float angle = CCRANDOM_0_1() * M_PI * 2;
		float del = CCRANDOM_0_1() * 85 + 15;
		Vec2 pDelta = Vec2(del * cos(angle), del * sin(angle));
		float scale = 0.5f + CCRANDOM_0_1() * 0.7f;
		deco->runAction(Spawn::create(
			RotateBy::create(EFF_TIME, (CCRANDOM_0_1() - 0.5) * 360),
			EaseSineOut::create(MoveBy::create(EFF_TIME, pDelta)),
			Sequence::create(
				ScaleTo::create(0.2f, scale), 
				ScaleTo::create(0.5f, scale * 0.5),
				nullptr),
			nullptr
		));
		deco->runAction(Sequence::create(
			FadeIn::create(0.4f),
			FadeOut::create(0.1f),
			FadeTo::create(0.15f, 150),
			FadeOut::create(0.1f),
			nullptr
		));
		deco->getChildByName("square")->runAction(Sequence::create(
			FadeIn::create(0.25f),
			FadeOut::create(0.1f),
			FadeTo::create(0.2f, 150),
			FadeOut::create(0.15f),
			nullptr
		));
	}
}

bool EffectTouchLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
	Vec2 pos = touch->getLocation();
	this->effectTouch(pos);
	return true;
}

void EffectTouchLayer::startEffect()
{
	for (int i = 0; i < paperList.size(); i++) {
		paperList[i]->sprite->setVisible(false);
		paperList[i]->isRunning = false;
	}
	schedule(schedule_selector(EffectTouchLayer::effectPaperBirthday), 20);
}

void EffectTouchLayer::startEffectVip()
{
	for (int i = 0; i < paperList.size(); i++) {
		paperList[i]->sprite->setVisible(false);
		paperList[i]->isRunning = false;
	}
	schedule(schedule_selector(EffectTouchLayer::effectPaperVip), 1.0f);
}

void EffectTouchLayer::stopEffect()
{
	unschedule(schedule_selector(EffectTouchLayer::effectPaperBirthday));
	for (int i = 0; i < paperList.size(); i++) {
		paperList[i]->sprite->setVisible(false);
	}
}

void EffectTouchLayer::effectPaperBirthday(float dt)
{

}
void EffectTouchLayer::effectPaperVip(float dt)
{
	Node* scene = sceneMgr->getRunningScene()->getMainLayer();
	int NUM_PAPER = 60;
	std::vector<Color3B> COLOR_LIST = {
		Color3B(145, 87, 235),
			Color3B(126, 215, 210),
			Color3B(63, 122, 242),
			Color3B(204, 100, 153),
			Color3B(218, 197, 82),
			Color3B(52, 152, 80) };
	if (paperList.size() == 0) {
		for (int i = 0; i < NUM_PAPER; i++) {
			Paper* paper = new Paper();
			paper->sprite = Sprite::create("Lobby/Common/paper.png");
			paper->sprite->setAnchorPoint(Vec2(0.2, 0.2));
			paper->isRunning = false;
			addChild(paper->sprite, CCRANDOM_0_1() < 0.3 ? 1 : -1);
			paperList.push_back(paper);
			///
		}
	}
	///TODO
	//if (scene instanceof NewVipScene == false) {
	//}
	//else {
	Size size = getContentSize();
	Vec2 posDef = Vec2(CCRANDOM_0_1() * size.width, size.height + 150);
	float xMove = 5 - 10 * CCRANDOM_0_1();
	for (Paper* paper : paperList) {
		if (CCRANDOM_0_1() < 0.3 || paper->isRunning == true) continue;
		posDef = Vec2(CCRANDOM_0_1() * size.width, size.height + 150);
		xMove = posDef.x + 5 - CCRANDOM_0_1() * 10;
		paper->sprite->stopAllActions();
		paper->sprite->setVisible(false);
		paper->isRunning = true;
		paper->sprite->setColor(COLOR_LIST[(int)(CCRANDOM_0_1() * 10000000) % COLOR_LIST.size()]);
		paper->sprite->setScale(0.8f + CCRANDOM_0_1() * 1.2);
		paper->sprite->setRotation(CCRANDOM_0_1() * 360);
		paper->sprite->setPosition(posDef);
		float rateX = 0.1f + CCRANDOM_0_1()* 1.1f;
		float rateY = 0;
		if (rateX > 1.1f) {
			rateY += (rateX - 1.1f) * 2;
			rateX = 1.2f;
		}
		float time = 22;
		float delay = CCRANDOM_0_1() * 22;
		// if (delay > 3) {
		//     delay += (delay - 3) * 4;
		//     if (time < 3) time *= 2;
		// }
		paper->sprite->runAction(Sequence::create(
			DelayTime::create(delay), 
			Show::create(), 
			Spawn::create(
				EaseSineOut::create(MoveTo::create(time, Vec2(xMove, size.height * rateY - 100))),
				RotateBy::create(time * 1.5, (CCRANDOM_0_1() - 0.5) * 8000, (CCRANDOM_0_1() - 0.5) * 4000),
				nullptr
			), 
			Hide::create(), CallFunc::create([paper](){
				paper->isRunning = false;
			}),
			nullptr));
	}
}

EffectTouchLayer* EffectTouchLayer::_instance = nullptr;

const int EffectTouchLayer::TAG = 10000;
