#ifndef MatchBallSuggestion_h_
#define MatchBallSuggestion_h_

#include "../../core-implement/Ball.h"
#include "data/GameConstant.h"

class MatchBallSuggestion
{
public:
	MatchBallSuggestion();
	~MatchBallSuggestion();

	void getNotCoveredBalls(Ball *cueBall, std::vector<Ball*> & validBalls, std::vector<Ball *> & allBalls);

	void getBallsInRange(Ball *cueBall, std::vector<Ball *> & validBalls, double trajectLength);

	void getBallsNearPocket(std::vector<Ball *> & validBall, std::map<Ball*, int> & nearestPockets);

	Ball* getBallSmallestAngle(Ball *cueBall, std::vector<Ball *> &validBalls, const std::map<Ball*, int> & nearestPockets);

	Ball* suggestBall(std::vector<int> & validBallIds, std::vector<Ball *> & balls, Ball *cueBall, double trajectLength);

	void removeInvalidBall(std::vector<Ball *> & balls, Ball *invalid);

	Ball *getReturnValue(std::vector<Ball*> & validBall, std::vector<Ball *> & tmp);

public:
	static const double BALL_NEAR_POCKET_THRESHOLD;
	static ps::ExtMath::vector POCKET_POS_ROUGH[NUM_POCKET];
};
#endif // !MatchBallSuggestion_H)

