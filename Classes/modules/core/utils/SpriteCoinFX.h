#ifndef SpriteCoinFX_h_
#define SpritecoinFX_h_

#include<cocos2d.h>

class SpriteGoldCoin
	:public cocos2d::Sprite
{
public:
	SpriteGoldCoin();
	~SpriteGoldCoin();
	
	static SpriteGoldCoin *create();

	void onEnter();

	void onExit();

	void initAnimation();

private:
	cocos2d::Animation *_anim;
public:
	static const std::string NAME_ANIMATION_COIN;
	static const float TIME_ANIMATION_COIN;
	static const int NUM_SPRITE_FRAME;
};

#endif
