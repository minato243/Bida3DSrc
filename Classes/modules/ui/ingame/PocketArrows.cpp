#include "PocketArrows.h"
#include "data/Resource.h"

USING_NS_CC;

PocketArrows::PocketArrows()
{
	_passedTime = 0;
}

PocketArrows::~PocketArrows()
{

}

bool PocketArrows::init()
{
	if (!Node::init())
	{
		return false;
	}
	this->initArrows();

	return true;
}

void PocketArrows::initArrows()
{
	for (int i = 0; i < PocketArrows::ARROWS_COUNT; i++) {
		auto arrow = Sprite::create(Res::INGAME_ARROW_POCKET);
		arrow->setPositionY(i * 25);
		addChild(arrow);

		_arrows.push_back(arrow);
	}
}

void PocketArrows::update(float dt)
{
	_passedTime += dt;
	if (_passedTime > ANIM_TIME)
		_passedTime -= ANIM_TIME;

	for (int i = 0; i < PocketArrows::ARROWS_COUNT; i++) {
		auto arrow = _arrows[i];
		auto timeOffset = PocketArrows::ANIM_TIME * i / PocketArrows::ARROWS_COUNT;

		auto curArrowTime = _passedTime + timeOffset;
		if (curArrowTime > PocketArrows::ANIM_TIME) {
			curArrowTime -= PocketArrows::ANIM_TIME;
		}

		arrow->setOpacity(100 + 155 * (1 - (curArrowTime / PocketArrows::ANIM_TIME)));
	}
}

void PocketArrows::setVisible(bool isVisible)
{
	Node::setVisible(isVisible);
	if (isVisible)
	{
		_passedTime = 0;
		scheduleUpdate();
	}
	else
	{
		unscheduleUpdate();
	}
}

const float PocketArrows::ANIM_TIME = 1;

