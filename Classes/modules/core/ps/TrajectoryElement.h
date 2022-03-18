#ifndef TRAJECTORYELEMENT_H_
#define TRAJECTORYELEMENT_H_

#include "internal/ExtensionMath.hpp"
#include <modules/core/ps/internal/BallBodyV2.h>
#include <modules/core/ps/internal/PhysicsCue.hpp>
#include <data/DataStruct.h>


using namespace ps;
using namespace ps::ExtMath;

class TrajectoryElement
{
public:
	double startTime;
	ExtMath::vector position;
	vector lineVel;
	vector lineAccel;
	bool isFinished;
	BallBody *collidedBall;
	vector collidedCushion;
	double duration;
	double endTime;


	TrajectoryElement();
	TrajectoryElement(double startTime, vector position, vector lineVel, vector lineAccel);
	~TrajectoryElement();
	void init(double startTime, vector position, vector lineVel, vector lineAccel);
	vector calc(double time);
	double calcX(double time);
	double calcY(double time);
	vector calcVelocity(double time);
	MotionData calcAll(double time);
	void checkCollisions(BallBody* cueBall, std::vector<BallBody*> balls);
	virtual double calcDuration();
};

class BasicTrajElem : public TrajectoryElement{
public:
	BasicTrajElem(double startTime, ps::ExtMath::vector position, ps::ExtMath::vector direction);
	~BasicTrajElem();

	double calcDuration();

	void checkCollisions(ps::PhysicsCue *cue, std::vector<ps::BallBody *> balls);
};

class FlyTrajElem : public TrajectoryElement{
public:
	FlyTrajElem(double startTime, vector position, vector lineVel);
	double calcDuration();
};

class PureTrajElem : public TrajectoryElement {
public:
	double lineVelLen;

	PureTrajElem(double startTime, vector position, vector velocity);
	PureTrajElem(double startTime, vector position, vector lineVel, vector lineAccel);

	double calcDuration();
};

class NonPureTrajElem : public TrajectoryElement {
public:
	vector vc;
	double vcLen;

	NonPureTrajElem(double startTime, vector position, vector velocity, vector vc, double vcLength);

	double calcDuration();
};

#endif // !TRAJECTORYELEMENT_H_

