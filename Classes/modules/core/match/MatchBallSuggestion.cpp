#include "MatchBallSuggestion.h"
#include "../../../data/GameConstant.h"
#include "../ps/internal/PhysicsConstants.hpp"

using namespace ps;
using namespace ps::ExtMath;

MatchBallSuggestion::MatchBallSuggestion()
{

}

MatchBallSuggestion::~MatchBallSuggestion()
{

}

void MatchBallSuggestion::getNotCoveredBalls(Ball *cueBall, std::vector<Ball*> & validBalls, std::vector<Ball *> & balls)
{
	vector cuePos = cueBall->body()->position();
	for (int i = validBalls.size() -1; i >= 0; i--)
	{
		Ball* curBall = validBalls[i];
		vector pos = curBall->body()->position();
		segment seg = segment(cuePos, pos);

		// check exist ball between curBall and cueBall
		bool valid = true;
		for (int j = 0; j < balls.size(); j++)
		{
			Ball *checkBall = balls[j];
			if (checkBall == curBall)
				continue;
			if (checkBall->_number == DEFAULT_CUE_BALL_ID)
				continue;

			vector pos2 = checkBall->body()->position();
			double distance = segment::distance(pos2, seg);
			if (distance < PhysicsConstants::BALL_RADIUS * 2)
			{
				valid = false;
				break;
			}
		}
		if (!valid) 
		{
			removeInvalidBall(validBalls, curBall);
		}
	}
}

void MatchBallSuggestion::getBallsInRange(Ball *cueBall, std::vector<Ball *> & validBalls, double trajectLength)
{
	vector cuePos = cueBall->body()->position();
	double rangeSquare = trajectLength * trajectLength;
	for (int i = validBalls.size() -1; i >=0; i--) 
	{
		vector pos = validBalls[i]->body()->position();
		double distance = vector::distanceSquare(cuePos, pos);

		if (distance > rangeSquare)
		{
			validBalls.erase(validBalls.begin() + i);
		}
	}
}

void MatchBallSuggestion::getBallsNearPocket(std::vector<Ball *> & validBall, std::map<Ball*, int> & nearestPockets)
{
	double limitSquare = BALL_NEAR_POCKET_THRESHOLD * BALL_NEAR_POCKET_THRESHOLD;

	for (int i = validBall.size() - 1; i >= 0; i--)
	{
		vector pos = validBall[i]->body()->position();
		
		bool inRange = false;
		for (int j = 0; j < NUM_POCKET; j++)
		{
			vector pocketPos = POCKET_POS_ROUGH[j];
			double distSquare = vector::distanceSquare(pos, pocketPos);
			if (distSquare < limitSquare)
			{
				inRange = true;
				nearestPockets.insert(std::pair<Ball*, int>(validBall[i], j));
				break;
			}
		}

		if (!inRange)
		{
			validBall.erase(validBall.begin() + i);
		}
	}
}

Ball* MatchBallSuggestion::getBallSmallestAngle(Ball *cueBall, std::vector<Ball *> &validBalls, const std::map<Ball*, int> & nearestPockets)
{
	double minCosAngle = -1;
	Ball *ret = NULL;
	vector cuePos = cueBall->body()->position();
	for (int i = 0; i < validBalls.size(); i++) {
		Ball* ball = validBalls[i];
		vector pos = ball->body()->position();
		int pocketIdx = nearestPockets.find(ball)->second;
		vector pocketPos = POCKET_POS_ROUGH[pocketIdx];

		vector pocketToBall = vector::sub(pos, pocketPos);
		vector ballToCueBall = vector::sub(cuePos, pos);

		vector desireCueBallPos = vector::add(pos, vector::multiply(PhysicsConstants::BALL_RADIUS *2, vector::unit(pocketToBall)));
		vector cueBallMove = vector::sub(desireCueBallPos, cuePos);
		vector straightMove = vector::sub(pos, cuePos);

		double cosAngle = vector::dot(cueBallMove, straightMove)/ vector::length(cueBallMove) * vector::length(straightMove);
		if (cosAngle > minCosAngle) 
		{
			minCosAngle = cosAngle;
			ret = ball;
		}
	}

	return ret;
}

Ball* MatchBallSuggestion::suggestBall(std::vector<int> & validBallIds, std::vector<Ball *> & balls, Ball *cueBall, double trajectLength)
{
	Ball *ret = NULL;
	std::vector<Ball *> validBalls;
	for (unsigned i = 0; i < balls.size(); i++)
	{
		Ball *ball = balls[i];
		for (unsigned j = 0; j < validBallIds.size(); j++)
		{
			if (ball->_number == validBallIds[j])
			{
				validBalls.push_back(ball);
				break;
			}
		}
	}

	std::vector<Ball*> tmp = balls;
	ret = getReturnValue(validBalls, tmp);
	if (ret != NULL)
		return ret;
	
	tmp = validBalls;
	getNotCoveredBalls(cueBall, validBalls, balls);
	ret = getReturnValue(validBalls, tmp);
	if (ret != NULL)
		return ret;

	// Convert "Design length" to "physics length"
	double physicsLength = trajectLength * PhysicsConstants::TABLE_WIDTH / 200;
	tmp = validBalls;
	getBallsInRange(cueBall, validBalls, physicsLength);
	ret = getReturnValue(validBalls, tmp);
	if (ret != NULL)
		return ret;

	// near pocket
	tmp = validBalls;
	std::map<Ball*, int> nearestPockets;
	getBallsNearPocket(validBalls, nearestPockets);
	
	ret = getReturnValue(validBalls, tmp);
	if (ret != NULL)
		return ret;

	return getBallSmallestAngle(cueBall, validBalls, nearestPockets);
}

void MatchBallSuggestion::removeInvalidBall(std::vector<Ball *> & balls, Ball *invalid)
{
	for (int i = balls.size() -1; i >=0; i--)
	{
		if (balls[i] == invalid)
		{
			balls.erase(balls.begin() + i);
			break;
		}
	}
}

Ball * MatchBallSuggestion::getReturnValue(std::vector<Ball*> & validBalls, std::vector<Ball *> & tmp)
{
	if (validBalls.size() == 1)
	{
		return validBalls[0];
	}
	else if (validBalls.size() == 0)
	{
		int r = rand() % tmp.size();
		return tmp[r];
	}

	return NULL;
}

const double MatchBallSuggestion::BALL_NEAR_POCKET_THRESHOLD = PhysicsConstants::TABLE_HEIGHT / 2 + 2 * PhysicsConstants::BALL_RADIUS;

vector MatchBallSuggestion::POCKET_POS_ROUGH[] = {
	vector(PhysicsConstants::TABLE_WIDTH / 2, PhysicsConstants::TABLE_HEIGHT / 2),
	vector(PhysicsConstants::TABLE_WIDTH / 2, -PhysicsConstants::TABLE_HEIGHT / 2),
	vector(-PhysicsConstants::TABLE_WIDTH / 2, PhysicsConstants::TABLE_HEIGHT / 2),
	vector(-PhysicsConstants::TABLE_WIDTH / 2, -PhysicsConstants::TABLE_HEIGHT / 2),
	vector(0, PhysicsConstants::TABLE_HEIGHT / 2),
	vector(0, -PhysicsConstants::TABLE_HEIGHT / 2)
};

