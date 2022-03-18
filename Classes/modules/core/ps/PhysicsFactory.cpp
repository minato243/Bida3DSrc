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
{/*
 0: 50.5714888134340, -0.3128188567975, 0.0000000000000
1: 47.8253102697220, 40.6018655229990, 0.0000000000000
2: 95.6177198720867, -50.8373184202265, 0.0000000000000
3: 54.6084859023335, 9.8394241613151, 0.0000000000000
4: 71.0270629445406, 6.4743195571188, 0.0000000000000
5: 86.8456746257777, 12.2039737959415, 0.0000000000000
6: 61.4213603838206, -24.2817199593785, 0.0000000000000
7: 113.1362905580029, 26.9052341920955, 0.0000000000000
8: 73.0553420760463, -0.4675329509825, 0.0000000000000
9: 80.3748692696834, -1.6239556246566, 0.0000000000000
10: 0.0877731146630, 67.9740605352408, -5.6156081198519
11: 50.4890523461481, 51.4182520356291, 0.0000000000000
12: 65.3451685725663, -58.9964109146583, 0.0000000000000
13: 90.8757942286021, 2.9306382627466, 0.0000000000000
14: 84.5115068477223, -24.0091973595908, 0.0000000000000
15: -2.4307475435076, 49.1992534892731, 0.0000000000000
 */
	std::vector<vector> positions = {
		vector(50.5714888134340, -0.3128188567975, 0.0000000000000),
		vector(47.8253102697220, 40.6018655229990, 0.0000000000000),
		vector(95.6177198720867, -50.8373184202265, 0.0000000000000),
		vector(54.6084859023335, 9.8394241613151, 0.0000000000000),
		vector(71.0270629445406, 6.4743195571188, 0.0000000000000),

		vector(86.8456746257777, 12.2039737959415, 0.0000000000000),
		vector(61.4213603838206, -24.2817199593785, 0.0000000000000),
		vector(113.1362905580029, 26.9052341920955, 0.0000000000000),
		vector(73.0553420760463, -0.4675329509825, 0.0000000000000),
		vector(80.3748692696834, -1.6239556246566, 0.0000000000000),

		vector(0.0877731146630, 67.9740605352408, -5.6156081198519),
		vector(50.4890523461481, 51.4182520356291, 0.0000000000000),
		vector(65.3451685725663, -58.9964109146583, 0.0000000000000),
		vector(90.8757942286021, 2.9306382627466, 0.0000000000000),
		vector(84.5115068477223, -24.0091973595908, 0.0000000000000),
		vector(-2.4307475435076, 49.1992534892731, 0.0000000000000)
	};

	StaticWorld::initCushionAndPockets();
	StaticWorld *staticWorld = new StaticWorld(NUM_BALL + 1);
	ps::PhysicsCue::ResultVelocities velocities;


	velocities.linearVelocity = vector(-307.86917874112, 245.75971396612061, 0.0);
	velocities.angularVelocity = vector(-23.4343432716959, -29.3567725196372, 4.653253273644162E-15);

	PhysicsWorldV2 *world = gameMgr->_table->getWorld();
	//generatePhysicsWorld(world);
	world->setBreak(false);
	world->setRandomSeed(805524306);
	world->setCueRandomSeed(1134407473);
	
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

