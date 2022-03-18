#include "DataStruct.h"

using namespace ps::ExtMath;

RectData::RectData(ps::ExtMath::vector topLeft, ps::ExtMath::vector topRight, ps::ExtMath::vector botLeft, ps::ExtMath::vector botRight)
{
	this->topLeft = topLeft;
	this->topRight = topLeft;
	this->botLeft = topLeft;
	this->botRight = topLeft;
}


//Eclipse Data-------------------

void ElipseData::updateValues()
{
	a2 = a * a;
	b2 = b * b;
	bpa = b / a;
	a2b2 = a2 * b2;
}

ElipseBoundingData::ElipseBoundingData(vector top, vector bot, vector left, vector right)
{
	this->top = top;
	this->bot = bot;
	this->left = left;
	this->right = right;
}

TrajectoryReflectData::TrajectoryReflectData(ps::ExtMath::vector s1, ps::ExtMath::vector e1, ps::ExtMath::vector s2, ps::ExtMath::vector e2, int ballId)
{
	seg1 = segment(s1, e1);
	seg2 = segment(s2, e2);
	this->ballId = ballId;
}

TrajectoryReflectData::TrajectoryReflectData(ps::ExtMath::vector s1, ps::ExtMath::vector e1)
{
	seg1 = segment(s1, e1);
}

CamCoord::CamCoord(ps::ExtMath::vector position, ps::ExtMath::vector lookAt)
{
	this->position = position;
	this->lookAt = lookAt;
}

void ShootResultData::setData(int userId, int phrase, int nextTurnPlayerId, bool isPlacingCueBall, const std::vector<PocketBallData> & ballsPocketed, const std::vector<EnableBallData> & ballsEnable)
{
	this->userId = userId;
	this->nextTurnPlayerId = nextTurnPlayerId;
	this->phrase = phrase;
	this->isPlacingCueBall = isPlacingCueBall;

	this->ballsPocketed.clear();
	for (int i = 0; i < ballsPocketed.size(); i++)
	{
		this->ballsPocketed.push_back(ballsPocketed[i]);
	}

	this->ballsEnable.clear();
	for (int i = 0; i < ballsEnable.size(); i++)
	{
		this->ballsEnable.push_back(ballsEnable[i]);
	}
}

CueInfoData::CueInfoData()
{
	id = 0;
	type = 0;
	star = 0;
	level = 0;
	battery = 0;
	configId = 0;
	aim = 0;
	accuracy = 0;
	point = 0;
	wonChallenges = 0;
	autoCharge = false;
	name = "";
	owned = false;

	force = 0;
	tip_u = 0;
	angle[0] = 0;
	angle[1] = 90;
	angular = 0;
	tier = 0;
	charge_fee = 0;
}

CueInfoData::~CueInfoData()
{

}
