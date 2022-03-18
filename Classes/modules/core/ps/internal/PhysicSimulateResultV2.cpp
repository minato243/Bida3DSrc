#include "PhysicSimulateResultV2.h"
using namespace ps;

ps::PhysicSimulateResult::TimeStampResult::TimeStampResult(const double & timestamp)
	:timestamp(timestamp)
{

}

ps::PhysicSimulateResult::BallIDPair::BallIDPair(const double & timestamp, const int & id_1, const int & id_2, const vector & v1, const vector & v2)
	:TimeStampResult(timestamp), id_1(id_1), id_2(id_2), vector_1(v1), vector_2(v2)
{

}


ps::PhysicSimulateResult::BallCushionPair::BallCushionPair(const double & timestamp, const int & ballId, const int & cushion)
	:TimeStampResult(timestamp), ballId(ballId), cushionId(cushion)
{

}

ps::PhysicSimulateResult::FallenBallPair::FallenBallPair(const double timestamp, const int ballId, const int pocketId)
	:TimeStampResult(timestamp), ballId(ballId), pocketId(pocketId)
{

}

ps::PhysicSimulateResult::BallOverBall::BallOverBall(double const & timestamp, const int & underBallId, const int & overBallId)
	:TimeStampResult(timestamp), underBallId(underBallId), overBallId(overBallId)
{

}

ps::PhysicSimulateResult::FlyingCueBall::FlyingCueBall(double startTime, double endTime, double highestZ)
	:startTime(startTime), endTime(endTime), highestZ(highestZ)
{

}

ps::PhysicSimulateResult::PhysicSimulateResult()
{
	valid = false;
	masseCueBall = NULL;
}

ps::PhysicSimulateResult::~PhysicSimulateResult()
{
	reset();
	CC_SAFE_DELETE(masseCueBall);
}

void ps::PhysicSimulateResult::reset()
{
	ballCollisionIdPairs.clear();
	cushionCollisions.clear();
	floorCollisionIds.clear();
	fallenBalls.clear();
	outsideTableBallIds.clear();
	ballOverBallPairs.clear();
	flyingCueBalls.clear();
	valid = false;
}

void ps::PhysicSimulateResult::setMasseCueBall(MasseCueBall *masse)
{
	masseCueBall = masse;
}
