#include "SpriteCoinFX.h"
#include "../../../../cocos2d/cocos/2d/CCAnimationCache.h"

USING_NS_CC;

SpriteGoldCoin::SpriteGoldCoin()
:Sprite(),_anim(NULL)
{

}

SpriteGoldCoin::~SpriteGoldCoin()
{

}

SpriteGoldCoin * SpriteGoldCoin::create()
{
	SpriteGoldCoin *sp = new SpriteGoldCoin();
	sp->initWithSpriteFrameName("coin0.png");

	sp->initAnimation();
	sp->autorelease();
	return sp;
}

void SpriteGoldCoin::onEnter()
{
	Sprite::onEnter();

	setScale(0.5);
	Animate *animate = Animate::create(_anim);
	runAction(RepeatForever::create(animate));
}

void SpriteGoldCoin::onExit()
{
	stopAllActions();
	Sprite::onExit();
}

void SpriteGoldCoin::initAnimation()
{
	Animation *anim = AnimationCache::getInstance()->getAnimation(NAME_ANIMATION_COIN);
	Vector<SpriteFrame *> spriteFrames;
	if (!anim) {
		
		for (auto i = 0; i < NUM_SPRITE_FRAME; i++) {
			SpriteFrame *sf = SpriteFrameCache::getInstance()->getSpriteFrameByName(NAME_ANIMATION_COIN+std::to_string(i)+".png");
			spriteFrames.pushBack(sf);
		}
		anim = Animation::createWithSpriteFrames(spriteFrames, TIME_ANIMATION_COIN);
		AnimationCache::getInstance()->addAnimation(anim, NAME_ANIMATION_COIN);
	}
	_anim = anim;
}

const std::string SpriteGoldCoin::NAME_ANIMATION_COIN = "coin";

const float SpriteGoldCoin::TIME_ANIMATION_COIN = 0.05f;

const int SpriteGoldCoin::NUM_SPRITE_FRAME = 6;

