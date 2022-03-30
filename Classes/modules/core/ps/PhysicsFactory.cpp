#include "PhysicsFactory.h"
#include "static/StaticWorld.h"
#include "modules/core/ps/internal/PhysicsCue.hpp"
#include "internal/ExtensionMath.hpp"
#include "../../../data/GameConstant.h"
#include "internal/BallGenerator.hpp"
#include "static/StaticMotionController.h"
#include "internal/PhysicSimulateResultV2.h"
#include "../../../data/DataStruct.h"
#include "internal/PhysicsWorldV2.h"
#include "core/GameMgr.h"

using namespace ps;

PhysicsFactory::PhysicsFactory()
{

}

PhysicsFactory::~PhysicsFactory()
{

}

void PhysicsFactory::test()
{
	std::vector<vector> positions = {
	vector(-60.0000000000000, 0.0000000000000, 0.0000000000000),
	vector(60.0000000000000, 0.0000000000000, 0.0000000000000),
	vector(81.1310198523403, 12.2000000000000, 0.0000000000000),
	vector(65.2827549630851, -3.0500000000000, 0.0000000000000),
	vector(70.5655099261702, 6.1000000000000, 0.0000000000000),
	vector(75.8482648892552, 3.0500000000000, 0.0000000000000),
	vector(70.5655099261702, -6.1000000000000, 0.0000000000000),
	vector(75.8482648892552, 9.1500000000000, 0.0000000000000),
	vector(70.5655099261702, 0.0000000000000, 0.0000000000000),
	vector(75.8482648892552, -3.0500000000000, 0.0000000000000),
	vector(75.8482648892552, -9.1500000000000, 0.0000000000000),
	vector(65.2827549630851, 3.0500000000000, 0.0000000000000),
	vector(81.1310198523403, 6.1000000000000, 0.0000000000000),
	vector(81.1310198523403, 0.0000000000000, 0.0000000000000),
	vector(81.1310198523403, -6.1000000000000, 0.0000000000000),
	vector(81.1310198523403, -12.2000000000000, 0.0000000000000)
	};

	//StaticWorld::initCushionAndPockets();
	//StaticWorld *staticWorld = new StaticWorld(NUM_BALL + 1);
	ps::PhysicsCue::ResultVelocities velocities;


	velocities.linearVelocity = vector(683.8000391345270, -0.0000000000001, 20.6560168875189);
	velocities.angularVelocity = vector(-0.0000000000000, -0.0000000000000, 0.0000000000000);

	PhysicsWorldV2 *world = gameMgr->_table->getWorld();
	world->setBreak(false);
	world->setRandomSeed(1648168291232.);
	world->setCueRandomSeed(203662820.);

	//double force = 131.3937055489453;
	//vector offset = vector(0.000000000000000, 0.000000000000000, 0.000000000000000);
	//vector direction = vector(0.883000155384692, 0.464921069933980, - 0.064491273224001);
	//world->_cue->_attrs.tip_coef = 0.58;
	//world->_cue->setOffset(offset);
	//world->_cue->setDirection(direction);
	//world->_cue->calcCueBallVelocities(force);
	
	for (int i = 0; i < positions.size(); i++)
	{
		BallBody *ball = world->getBall(i);
		ball->setPosition(positions[i]);
		if (positions[i].z < 0) ball->disable();
	}

	world->shoot(0, velocities);
}

void PhysicsFactory::generatePhysicsWorld(PhysicsWorldV2 * world)
{
	PhysicsCue *cue = new PhysicsCue();
	cue->initDefaultPhysicsCue();
	BallBody *cueBall = new BallBody();
	cueBall->setId(0);
	cueBall->setPosition(PhysicsConstants::CUE_BALL_POSITION);
	world->addBall(cueBall, true);
	std::map<int, vector> positions;
	positions[0] = PhysicsConstants::CUE_BALL_POSITION;
	auto ballPositions = (new BallGenerator())->generateDefault();
	for (auto i = 0; i < ballPositions.size(); i++)
	{
		positions[i + 1] = ballPositions[i];
	}

	for (int i = 1; i < positions.size(); i++) {
		BallBody *ball = new BallBody();
		ball->setId(i);
		ball->setPosition(positions[i]);
		world->addBall(ball, false);
	}
}

