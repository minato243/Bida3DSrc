#include "Player.h"

Player::Player()
	: _ballType (BallType::NOT_DETERMINED)
{
}

Player::~Player()
{
	cues.clear();
}

bool Player::isBallTypeValid(BallType type)
{
	if (_ballType == NOT_DETERMINED)
		return true;
	return type == _ballType;
}

BallType Player::getBallType()
{
	return _ballType;
}

void Player::setBallType(BallType type)
{
	_ballType = type;
}

void Player::bet(int gold)
{
	this->gold -= gold;
}

CueInfo & Player::getCueInfo(int cueId)
{
	for (int i = 0; i < cues.size(); i++)
	{
		auto cue = cues[i];
		if (cue._id == cueId)
			return cues[i];
	}
	
	return cues[0];
}
