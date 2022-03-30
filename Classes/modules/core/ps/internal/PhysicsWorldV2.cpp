#include "PhysicsWorldV2.h"
#include "../../../../data/DataStruct.h"
#include "CollisionResolver.h"
#include "../../inventory/CueInfo.h"
#include "PhysicsDispatcher.hpp"
#include "core/utils/Utility.h"
#include "../../utils/Utils.h"

using namespace ps;

const double ps::PhysicsWorldV2::DEFAULT_FRAME_TIME = 1./180.;

double ps::PhysicsWorldV2::_width;

double ps::PhysicsWorldV2::_height;

ps::ExtMath::AABB ps::PhysicsWorldV2::_limits;

ps::ExtMath::AABB ps::PhysicsWorldV2::_nonCushionCollisionZone;

std::vector<ps::CushionSegment> ps::PhysicsWorldV2::_cushionSegments;

std::vector<ps::CushionPoint> ps::PhysicsWorldV2::_cushionPoints;

ps::FloorSurface ps::PhysicsWorldV2::_floorSurface;

ps::Pocket ps::PhysicsWorldV2::_pockets[NUM_POCKET];

const double ps::PhysicsWorldV2::BALL_DIAMETER = PhysicsConstants::BALL_DIAMETER;;

bool ps::PhysicsWorldV2::_initedStatic = false;

const bool LOG_DEBUG = false;

const int PhysicsWorldV2::CUSHION_HEAD_ID = PhysicsConstants::CUSHION_HEAD_ID;
const int PhysicsWorldV2::CUSHION_FOOT_ID = PhysicsConstants::CUSHION_FOOT_ID;
const int PhysicsWorldV2::CUSHION_SIDE_1_ID = PhysicsConstants::CUSHION_SIDE_1_ID;
const int PhysicsWorldV2::CUSHION_SIDE_2_ID = PhysicsConstants::CUSHION_SIDE_2_ID;

ps::PhysicsWorldV2::PhysicsWorldV2()
{
}

ps::PhysicsWorldV2::PhysicsWorldV2(const int & id, const int & nBall)
{
	initStaticValues();
	_id = id;
	maxBall = (nBall == -1)?MAX_BALL:nBall;
	_balls.clear();
	_ballMapping.clear(); // Alias mapping ID => Ball
	_frameTime = DEFAULT_FRAME_TIME;
	_remainFrameTime = 0;
	_cueBall = NULL;
	_lagBall = NULL;
	_run = false;
	_simulateResult = new PhysicSimulateResult();
	_lastAccess = 0L;
	_timestamp = 0;
	_timeScale = 1;
	// _ballOverBallHelper = new BallOverBallHelper(maxBall);
	_break = false;
	_turnLag = false;
	_random = new PhysicsRandom();
	_cueRandom = new PhysicsRandom();
	_cues.clear();
	_staticWorld = new StaticWorld(maxBall);
	StaticWorld::initCushionAndPockets();
	_staticMotionCtrl = new StaticMotionController();
	for (auto i = 0; i < MAX_BALL; i++)
		_cacheMotionLess.push_back(false);

	_width = PhysicsConstants::TABLE_WIDTH;
	_height = PhysicsConstants::TABLE_HEIGHT;
	updateLimits(_width, _height);
	updateStaticSegmentsAndPockets();

	_dispatcher = new PhysicsDispatcher(this);
	_cue = new PhysicsCue();
}

ps::PhysicsWorldV2::~PhysicsWorldV2()
{
	deleteVector(_balls);
	CC_SAFE_DELETE(_random);
	CC_SAFE_DELETE(_cueRandom);
	CC_SAFE_DELETE(_cue);
}

void ps::PhysicsWorldV2::reset(const int & id)
{
	_id = id;
	_balls.clear();
	for (int i = 0; i < _ballMapping.size(); i++) {
		_ballMapping[i] = NULL;
	}
	_frameTime = DEFAULT_FRAME_TIME;
	_cueBall = NULL;
	_lagBall = NULL;
	_run = false;
	_simulateResult->reset();
	_lastAccess = 0L;
	_timestamp = 0;
	// _ballOverBallHelper.reset();
	_break = false;
	_random->reset();
	_cueRandom->reset();
	deleteVector(_cues);
}

void ps::PhysicsWorldV2::reset()
{
	_break = false;
}

const int & ps::PhysicsWorldV2::id()
{
	return _id;
}

void ps::PhysicsWorldV2::setRandomSeed(double seed)
{
	_random->setSeed(seed);
	_cueRandom->setSeed(seed);
}

void ps::PhysicsWorldV2::setCueRandomSeed(const double & seed)
{
	_cueRandom->setSeed(seed);
}

double ps::PhysicsWorldV2::getRandomSeed()
{
	return _random->getSeed();
}

double ps::PhysicsWorldV2::getCurrentRandomSeed()
{
	return _random->getCurrentSeed();
}

double ps::PhysicsWorldV2::getCurrentCueRandomSeed()
{
	return _cueRandom->getCurrentSeed();
}

bool ps::PhysicsWorldV2::setCueAttributes(int slot, PhysicsCue::PhysicsCueAttributes & attrs)
{
	if (slot < 0 || slot >= MAX_CUE_SLOT) return false;
	PhysicsCue* cue = _cues[slot];
	if (cue == NULL) {
		cue = new PhysicsCue();
		_cues[slot] = cue;
	}
	cue->setCueAttributes(attrs);
	return true;
}

void ps::PhysicsWorldV2::setCueAttributes(PhysicsCue::PhysicsCueAttributes & attrs)
{
	_cue->setCueAttributes(attrs);
}

ps::PhysicsCue * ps::PhysicsWorldV2::getCue(int slot)
{
	if (slot < 0 || slot >= MAX_CUE_SLOT) return NULL;
	return _cues[slot];
}

ps::BallBody * ps::PhysicsWorldV2::getBall(int ballId)
{
	if (ballId == 0) {
		return _cueBall; // Better performance
	}
	
	for (BallBody *ball: _balls)
	{
		if (ball->id() == ballId) return ball;
	}
	return NULL;
}

void ps::PhysicsWorldV2::addBall(BallBody *ball, bool isCueBall)
{
	_balls.push_back(ball);
	_ballMapping[ball->id()] = ball;
	ball->setWorld(this);

	if (isCueBall) _cueBall = ball;
}

void ps::PhysicsWorldV2::addLagBall(BallBody *ball)
{
	addBall(ball, false);
	ball->setWorld(this);
	ball->setId(PhysicsConstants::LAG_BALL_ID);
	_lagBall = ball;
}

void ps::PhysicsWorldV2::removeAllBalls()
{
	_balls.clear();
}

void ps::PhysicsWorldV2::setTurnLag(bool value)
{
	if (_turnLag && !value) {
		_break = false;
		_random->reset();
		for (int i = 0; i < _balls.size(); i++) {
			_balls.at(i)->resetShootingState();
		}
	}
	_turnLag = value;
}

void ps::PhysicsWorldV2::onBallFallToHole(BallBody * ball)
{
	Pocket* pocket = _floorSurface.findRegionPocket(ball->position());
	_dispatcher->dispatchBallFallen(_timestamp, ball, pocket);
	_simulateResult->fallenBalls.push_back(PhysicSimulateResult::FallenBallPair(_timestamp, ball->id(), pocket->id));
}

void ps::PhysicsWorldV2::onBallOutOfTable(BallBody * ball)
{
	//_dispatcher->dispatchBallOutTable(_timestamp, ball);
}

ps::PhysicSimulateResult * ps::PhysicsWorldV2::shoot(int ballId, double force, vector & direction, vector & offset)
{
	BallBody *ball = getBall(ballId);
	return shoot(ball, _cue, force, direction, offset);
}

ps::PhysicSimulateResult * ps::PhysicsWorldV2::runShootOnlyLogic(BallBody *ball, PhysicsCue::ResultVelocities & velocities)
{
	CUSTOMLOG("Before Shooting Seed %lf, %lf, break = %s", _random->getCurrentSeed(), _random->getSeed(), _break? "true":"false");
	for (int i = 0; i < _balls.size(); i++) {
		BallBody* currentBall = _balls.at(i);
		currentBall->resetShootingState();
		vector position = currentBall->position();
		CUSTOMLOG("%d: %.13lf, %.13lf, %.13lf", currentBall->id(), +position.x, position.y, position.z);
	}

	vector linearVelocity = velocities.linearVelocity;
	vector angularVelocity = velocities.angularVelocity;

	CUSTOMLOG("LinearVel: %.12lf, %.12lf, %.12lf", linearVelocity.x, linearVelocity.y, linearVelocity.z);
	CUSTOMLOG("LinearVel: %s, %s, %s", Utils::getBinary64(linearVelocity.x).c_str(), Utils::getBinary64(linearVelocity.y).c_str(), Utils::getBinary64(linearVelocity.z).c_str());
	CUSTOMLOG("AngularVel: %.12lf, %.12lf, %.12lf", angularVelocity.x, angularVelocity.y, angularVelocity.z);
	CUSTOMLOG("AngularVel: %s, %s, %s", Utils::getBinary64(angularVelocity.x).c_str(), Utils::getBinary64(angularVelocity.y).c_str(), Utils::getBinary64(angularVelocity.z).c_str());

	if (ENABLE_STATIC_WORLD) {
		for (int i = 0; i < _balls.size(); i++) {
			_staticWorld->setBallEnabled(i, !_balls.at(i)->isDisabled());
			_staticWorld->setBallPosition(i, _balls.at(i)->position());
		}
	}

	_simulateResult->reset();
	_simulateResult->valid = true;

	if (ENABLE_STATIC_WORLD) {
		auto motionGroups = _staticWorld->calcCueBallMotions(
			linearVelocity, angularVelocity
		);
		StaticMotionController *ctrl = _staticMotionCtrl;
		ctrl->setMotions(motionGroups, _cueBall->quaternionRotation());
		_simulateResult->setMasseCueBall(ctrl->calMasseCueBall());
		_simulateResult->flyingCueBalls = ctrl->calFlyingCueBall();
		ctrl->updateSimulateResult(*_simulateResult);

		StaticMotionResult *result = ctrl->calcFinalResult();

		ball->setPosition(result->position);
		ball->setLinearVelocity(result->linearVelocity);
		ball->setAngularVelocity(result->angularVelocity);
		updateSimulateStepTime(result->linearVelocity);
		_timestamp = result->endTime;

		CUSTOMLOG("Physics Static End Time: %.15lf", _timestamp);
		CUSTOMLOG("Cue Ball Pos: %.12lf, %.12lf, %.12lf", result->position.x, result->position.y, result->position.z);
		CUSTOMLOG("Cue Ball lv: %.12lf, %.12lf, %.12lf", result->linearVelocity.x, result->linearVelocity.y, result->linearVelocity.z);
		CUSTOMLOG("Cue Ball av: %.12lf, %.12lf, %.12lf", result->angularVelocity.x, result->angularVelocity.y, result->angularVelocity.z);
		
		delete result;
		deleteVector(motionGroups);
	}
	else {
		ball->setLinearVelocity(linearVelocity);
		ball->setAngularVelocity(angularVelocity);
		updateSimulateStepTime(linearVelocity);
		_timestamp = 0;
	}

	_run = true;

	int iteration = 0;
	dataLog = "";
	stepCount = 0;
	while (_run) {
		if (iteration == 66)
		{
			int a = 1;
		}
		step(_frameTime);
		iteration++;
	}

	CUSTOMLOG("iteration %d", iteration);

	if (!_break) {
		_break = true;
	}

	// Check out of table balls;
	checkOutsideTableBalls();

	CUSTOMLOG("After Shooting Seed %lf, %lf", _random->getCurrentSeed(), _random->getSeed());
	for (int i = 0; i < _balls.size(); i++) {
		BallBody* currentBall = _balls.at(i);
		vector position = currentBall->position();
		CUSTOMLOG("%d: %.12lf, %.12lf, %.12lf", currentBall->id(), position.x, position.y, position.z);
	}
	return _simulateResult;
}

void ps::PhysicsWorldV2::shoot(double force)
{
	
}

ps::PhysicSimulateResult * ps::PhysicsWorldV2::shoot(int ballId, PhysicsCue::ResultVelocities & velocities)
{
	BallBody *ball = getBall(ballId);
	//runShootOnlyLogic(ball, velocities);
	return shoot(ball, velocities);
	return NULL;
}

ps::PhysicSimulateResult * ps::PhysicsWorldV2::shoot(BallBody *ball, PhysicsCue *cue, double force, vector & direction, vector & offset)
{
	CUSTOMLOG("PhysicsWorldV2::shoot");
	CueInfo *cuePlay = cue->getCueInfo();
	CUSTOMLOG("shoot before rand direction %s %s %s",
		getBinaryDouble(direction.x).c_str(), getBinaryDouble(direction.y).c_str(), getBinaryDouble(direction.z).c_str());
	direction = randomDirection(direction, cuePlay);
	CUSTOMLOG("shoot after rand direction %s %s %s",
		getBinaryDouble(direction.x).c_str(), getBinaryDouble(direction.y).c_str(), getBinaryDouble(direction.z).c_str());
	offset = randomOffset(offset, cuePlay);

	bool valid = (ball != NULL && cue != NULL && checkValidShoot(force, direction, offset));

	if (!valid) {
		_simulateResult->valid = valid;
		return _simulateResult;
	}

	cue->setOffset(offset);
	cue->setDirection(direction);
	PhysicsCue::ResultVelocities velocities = cue->calcCueBallVelocities(force);

	// Take all ball captures for bug reports
	std::vector<BallBodyCapture> captures;
	makeAllBallsCapture(captures);

	return shoot(ball, velocities);
}

ps::PhysicSimulateResult * ps::PhysicsWorldV2::shoot(BallBody *ball, PhysicsCue::ResultVelocities & velocities)
{
	CUSTOMLOG("Before Shooting Seed %.13lf, %.13lf, %.13lf, break = %s", _random->getCurrentSeed(), _random->getSeed(), _cueRandom->getCurrentSeed(), _break ? "true" : "false");
	for (int i = 0; i < _balls.size(); i++) {
		BallBody* currentBall = _balls.at(i);
		currentBall->resetShootingState();
		vector position = currentBall->position();
		CUSTOMLOG("%d: %.13lf, %.13lf, %.13lf",currentBall->id(), + position.x, position.y, position.z);
	}

	vector linearVelocity = velocities.linearVelocity;
	vector angularVelocity = velocities.angularVelocity;

	CUSTOMLOG("LinearVel: %.13lf, %.13lf, %.13lf", linearVelocity.x, linearVelocity.y ,linearVelocity.z);
	CUSTOMLOG("LinearVel: %s, %s, %s", getBinaryDouble(linearVelocity.x).c_str(), getBinaryDouble(linearVelocity.y).c_str(), getBinaryDouble(linearVelocity.z).c_str());
	CUSTOMLOG("AngularVel: %.13lf, %.13lf, %.13lf", angularVelocity.x, angularVelocity.y, angularVelocity.z);
	CUSTOMLOG("AngularVel: %s, %s, %s", getBinaryDouble(angularVelocity.x).c_str(), getBinaryDouble(angularVelocity.y).c_str(), getBinaryDouble(angularVelocity.z).c_str());

	if (ENABLE_STATIC_WORLD) {
		for (int i = 0; i < _balls.size(); i++) {
			_staticWorld->setBallEnabled(i, !_balls.at(i)->isDisabled());
			_staticWorld->setBallPosition(i, _balls.at(i)->position());
		}

		auto motions = _staticWorld->calcCueBallMotions(linearVelocity, angularVelocity);
		_staticMotionCtrl->setMotions(motions, _cueBall->quaternionRotation());
	}

	_runStaticMotion = ENABLE_STATIC_WORLD;

	_simulateResult->reset();
	_simulateResult->valid = true;

	stepCount = 0;
	_run = true;

	int iteration = 0;
	dataLog = "";
	// Check out of table balls;
	checkOutsideTableBalls();

	return _simulateResult;
}


ps::ExtMath::vector ps::PhysicsWorldV2::randomDirection(const vector & dir, CueInfo* cue)
{
	const double PI = 3.141592653589793238;
	double xySqrt = sqrt(dir.x*dir.x + dir.y*dir.y);
	CUSTOMLOG("randomDirection xySqrt %s", getBinaryDouble(xySqrt).c_str());
	double angle = ExtMath::atan(dir.z / xySqrt) * 180.0 / PI;
	double angleMax = std::max(-cue->_angle[1], (2.0 - cue->_accuracy) * angle);
	double angleMin = std::min(-cue->_angle[0], cue->_accuracy * angle);

	double newAngle = _cueRandom->rand() * (angleMax - angleMin) + angleMin;

	vector newDir = vector(dir);
	newDir.z = ExtMath::tan(newAngle * PI / 180.0) * xySqrt;
	newDir = vector::unit(newDir);
	return newDir;
}

ps::ExtMath::vector ps::PhysicsWorldV2::randomOffset(const vector & offset, CueInfo* cue)
{
	double r = vector::length(offset);
	if (r == 0) return offset;
	double maxR = std::min(PhysicsConstants::OFFSET_MAX_R, r * (2.0 - cue->_accuracy));
	double minR = r * cue->_accuracy;

	double newR = _cueRandom->rand() * (maxR - minR) + minR;
	return vector::multiply(newR / r, offset);
}

ps::FloorSurface * ps::PhysicsWorldV2::floorSurface()
{
	return &_floorSurface;
}

ps::Pocket * ps::PhysicsWorldV2::pockets()
{
	return _pockets;
}

void ps::PhysicsWorldV2::setBreak(bool brk)
{
	_break = brk;
}

bool ps::PhysicsWorldV2::checkValidShoot(double force, const vector & direction, const vector & offset)
{
	if (force <= 0) return false;
	if (direction.z > 0) return false;
	if (ExtMath::isZeroNumber(vector::lengthSquare(direction))) return false;
	if (ExtMath::isGreatThanZeroNumber(
		vector::lengthSquare(offset) - PhysicsConstants::BALL_RADIUS_SQUARE
	)) return false;
	return true;
}

ps::PhysicsDispatcher* ps::PhysicsWorldV2::getDispatcher()
{
	return _dispatcher;
}

ps::StaticWorld * ps::PhysicsWorldV2::getStaticWorld()
{
	return _staticWorld;
}

std::vector<BallBody *> * ps::PhysicsWorldV2::allBalls()
{
	return &_balls;
}

bool ps::PhysicsWorldV2::isRunning()
{
	return _run;
}

void ps::PhysicsWorldV2::checkExtensionEvent()
{

}

void ps::PhysicsWorldV2::initStaticValues()
{
	if (_initedStatic) return;
	_initedStatic = true;
	updateLimits(PhysicsConstants::TABLE_WIDTH, PhysicsConstants::TABLE_HEIGHT);
	updateStaticSegmentsAndPockets();
}

void ps::PhysicsWorldV2::updateLimits(double w, double h)
{
	_width = w;
	_height = h;

	_limits.minX = -_width / 2;
	_limits.maxX = _width / 2;
	_limits.minY = -_height / 2;
	_limits.maxY = _height / 2;

	_nonCushionCollisionZone.minX = _limits.minX + PhysicsConstants::BALL_RADIUS;
	_nonCushionCollisionZone.maxX = _limits.maxX - PhysicsConstants::BALL_RADIUS;
	_nonCushionCollisionZone.minY = _limits.minY + PhysicsConstants::BALL_RADIUS;
	_nonCushionCollisionZone.maxY = _limits.maxY - PhysicsConstants::BALL_RADIUS;
}

void ps::PhysicsWorldV2::updateStaticSegmentsAndPockets()
{
	AABB & limits = _limits;
	double CORNER_POCKET_WIDTH = PhysicsConstants::CORNER_POCKET_WIDTH;
	double SIDE_POCKET_WIDTH = PhysicsConstants::SIDE_POCKET_WIDTH;

	double CORNER_POCKET_RADIUS = PhysicsConstants::CORNER_POCKET_RADIUS;
	double SIDE_POCKET_RADIUS = PhysicsConstants::SIDE_POCKET_RADIUS;

	double CORNER_POCKET_DEPTH = PhysicsConstants::CORNER_POCKET_DEPTH;
	double SIDE_POCKET_DEPTH = PhysicsConstants::SIDE_POCKET_DEPTH;

	double TABLE_EDGE_THICKNESS_V = PhysicsConstants::TABLE_EDGE_THICKNESS_V;
	double TABLE_EDGE_THICKNESS_H = PhysicsConstants::TABLE_EDGE_THICKNESS_H;

	double CORN_SEG_OFFSET = CORNER_POCKET_WIDTH / sqrt(2);
	double MIDDLE_SEG_OFFSET = SIDE_POCKET_WIDTH / 2;

	double SUB_SEG_OFFSET = PhysicsConstants::SUB_SEG_OFFSET;

	double cushionZ = PhysicsConstants::CUSHION_HEIGHT - PhysicsConstants::BALL_RADIUS;

	// TODO: Combine cushionTop_1 with cushionTop_2, cushionBottom_1 with cushionBottom_2
	// Adding exclude segment to reduce for loop checking
	CushionSegment cushionLeft = CushionSegment(PhysicsConstants::CUSHION_HEAD_ID, segment(
		vector(limits.minX, limits.minY + CORN_SEG_OFFSET + SUB_SEG_OFFSET, cushionZ),
		vector(limits.minX, limits.maxY - CORN_SEG_OFFSET - SUB_SEG_OFFSET, cushionZ)
	));

	CushionSegment cushionTop_1 = CushionSegment(PhysicsConstants::CUSHION_SIDE_1_ID, segment(
		vector(limits.minX + CORN_SEG_OFFSET + SUB_SEG_OFFSET, limits.maxY, cushionZ),
		vector(-MIDDLE_SEG_OFFSET - SUB_SEG_OFFSET, limits.maxY, cushionZ)
	));

	CushionSegment cushionTop_2 = CushionSegment(PhysicsConstants::CUSHION_SIDE_1_ID, segment(
		vector(MIDDLE_SEG_OFFSET + SUB_SEG_OFFSET, limits.maxY, cushionZ),
		vector(limits.maxX - CORN_SEG_OFFSET - SUB_SEG_OFFSET, limits.maxY, cushionZ)
	));

	CushionSegment cushionRight = CushionSegment(PhysicsConstants::CUSHION_FOOT_ID, segment(
		vector(limits.maxX, limits.minY + CORN_SEG_OFFSET + SUB_SEG_OFFSET, cushionZ),
		vector(limits.maxX, limits.maxY - CORN_SEG_OFFSET - SUB_SEG_OFFSET, cushionZ)
	));

	CushionSegment cushionBottom_1 = CushionSegment(PhysicsConstants::CUSHION_SIDE_2_ID, segment(
		vector(MIDDLE_SEG_OFFSET + SUB_SEG_OFFSET, limits.minY, cushionZ),
		vector(limits.maxX - CORN_SEG_OFFSET - SUB_SEG_OFFSET, limits.minY, cushionZ)
	));

	CushionSegment cushionBottom_2 = CushionSegment(PhysicsConstants::CUSHION_SIDE_2_ID, segment(
		vector(limits.minX + CORN_SEG_OFFSET + SUB_SEG_OFFSET, limits.minY, cushionZ),
		vector(-MIDDLE_SEG_OFFSET - SUB_SEG_OFFSET, limits.minY, cushionZ)
	));

	double CornerDepthSqrt2 = CORNER_POCKET_DEPTH / sqrt(2);

	Pocket &topLeftPocket = _pockets[PhysicsConstants::TOP_LEFT_POCKET_ID];
	topLeftPocket.id = PhysicsConstants::TOP_LEFT_POCKET_ID;
	topLeftPocket.position = vector(limits.minX - CornerDepthSqrt2, limits.maxY + CornerDepthSqrt2);
	topLeftPocket.radius = CORNER_POCKET_RADIUS;
	topLeftPocket.warnPolygon = PocketAABB(&topLeftPocket);
	
	_pockets[PhysicsConstants::TOP_RIGHT_POCKET_ID] = Pocket(
		PhysicsConstants::TOP_RIGHT_POCKET_ID,
		vector(limits.maxX + CornerDepthSqrt2, limits.maxY + CornerDepthSqrt2),
		CORNER_POCKET_RADIUS
	);
	_pockets[PhysicsConstants::BOTTOM_RIGHT_POCKET_ID] = Pocket(
		PhysicsConstants::BOTTOM_RIGHT_POCKET_ID,
		vector(limits.maxX + CornerDepthSqrt2, limits.minY - CornerDepthSqrt2),
		CORNER_POCKET_RADIUS
	);
	_pockets[PhysicsConstants::BOTTOM_LEFT_POCKET_ID] = Pocket(
		PhysicsConstants::BOTTOM_LEFT_POCKET_ID,
		vector(limits.minX - CornerDepthSqrt2, limits.minY - CornerDepthSqrt2),
		CORNER_POCKET_RADIUS
	);
	_pockets[PhysicsConstants::TOP_SIDE_POCKET_ID] = Pocket(
		PhysicsConstants::TOP_SIDE_POCKET_ID,
		vector(0, limits.maxY + SIDE_POCKET_DEPTH),
		SIDE_POCKET_RADIUS
	);
	_pockets[PhysicsConstants::BOTTOM_SIDE_POCKET_ID] = Pocket(
		PhysicsConstants::BOTTOM_SIDE_POCKET_ID,
		vector(0, limits.minY - SIDE_POCKET_DEPTH),
		SIDE_POCKET_RADIUS
	);
	Pocket & topRightPocket = _pockets[PhysicsConstants::TOP_RIGHT_POCKET_ID];
	Pocket & bottomRightPocket = _pockets[PhysicsConstants::BOTTOM_RIGHT_POCKET_ID];
	Pocket & bottomLeftPocket = _pockets[PhysicsConstants::BOTTOM_LEFT_POCKET_ID];
	Pocket & topSidePocket = _pockets[PhysicsConstants::TOP_SIDE_POCKET_ID];
	Pocket & bottomSidePocket = _pockets[PhysicsConstants::BOTTOM_SIDE_POCKET_ID];

	double CornerRadiusSqrt2 = CORNER_POCKET_RADIUS / sqrt(2);
	CushionSegment cushionTopLeftCorner_1 = CushionSegment(
		PhysicsConstants::CUSHION_HEAD_ID,
		segment(
			vector(
				cushionLeft.seg.e.x,
				cushionLeft.seg.e.y + SUB_SEG_OFFSET,
				cushionLeft.seg.e.z
			),
			vector::add(
				topLeftPocket.position,
				vector(-CornerRadiusSqrt2, -CornerRadiusSqrt2, cushionZ)
			)
		)
	);
	cushionTopLeftCorner_1.collapseStartPoint(SUB_SEG_OFFSET);

	CushionSegment cushionTopLeftCorner_2 = CushionSegment(
		PhysicsConstants::CUSHION_SIDE_1_ID,
		segment(
			vector(
				cushionTop_1.seg.s.x - SUB_SEG_OFFSET,
				cushionTop_1.seg.s.y,
				cushionTop_1.seg.s.z
			),
			vector::add(
				topLeftPocket.position,
				vector(CornerRadiusSqrt2, CornerRadiusSqrt2, cushionZ)
			)
		)
	);
	cushionTopLeftCorner_2.collapseStartPoint(SUB_SEG_OFFSET);

	CushionSegment cushionTopRightCorner_1 = CushionSegment(
		PhysicsConstants::CUSHION_SIDE_1_ID,
		segment(
			vector(
				cushionTop_2.seg.e.x + SUB_SEG_OFFSET,
				cushionTop_2.seg.e.y,
				cushionTop_2.seg.e.z
			),
			vector::add(
				topRightPocket.position,
				vector(-CornerRadiusSqrt2, CornerRadiusSqrt2, cushionZ)
			)
		)
	);
	cushionTopRightCorner_1.collapseStartPoint(SUB_SEG_OFFSET);

	CushionSegment cushionTopRightCorner_2 = CushionSegment(
		PhysicsConstants::CUSHION_FOOT_ID,
		segment(
			vector(
				cushionRight.seg.e.x,
				cushionRight.seg.e.y + SUB_SEG_OFFSET,
				cushionRight.seg.e.z
			),
			vector::add(
				topRightPocket.position,
				vector(CornerRadiusSqrt2, -CornerRadiusSqrt2, cushionZ)
			)
		)
	);
	cushionTopRightCorner_2.collapseStartPoint(SUB_SEG_OFFSET);

	CushionSegment cushionBottomRightCorner_1 = CushionSegment(
		PhysicsConstants::CUSHION_FOOT_ID,
		segment(
			vector(
				cushionRight.seg.s.x,
				cushionRight.seg.s.y - SUB_SEG_OFFSET,
				cushionRight.seg.s.z
			),
			vector::add(
				bottomRightPocket.position,
				vector(CornerRadiusSqrt2, CornerRadiusSqrt2, cushionZ)
			)
		)
	);
	cushionBottomRightCorner_1.collapseStartPoint(SUB_SEG_OFFSET);

	CushionSegment cushionBottomRightCorner_2 = CushionSegment(
		PhysicsConstants::CUSHION_SIDE_2_ID,
		segment(
			vector(
				cushionBottom_1.seg.e.x + SUB_SEG_OFFSET,
				cushionBottom_1.seg.e.y,
				cushionBottom_1.seg.e.z
			),
			vector::add(
				bottomRightPocket.position,
				vector(-CornerRadiusSqrt2, -CornerRadiusSqrt2, cushionZ)
			)
		)
	);
	cushionBottomRightCorner_2.collapseStartPoint(SUB_SEG_OFFSET);

	CushionSegment cushionBottomLeftCorner_1 = CushionSegment(
		PhysicsConstants::CUSHION_SIDE_2_ID,
		segment(
			vector(
				cushionBottom_2.seg.s.x - SUB_SEG_OFFSET,
				cushionBottom_2.seg.s.y,
				cushionBottom_2.seg.s.z
			),
			vector::add(
				bottomLeftPocket.position,
				vector(CornerRadiusSqrt2, -CornerRadiusSqrt2, cushionZ)
			)
		)
	);
	cushionBottomLeftCorner_1.collapseStartPoint(SUB_SEG_OFFSET);

	CushionSegment cushionBottomLeftCorner_2 = CushionSegment(
		PhysicsConstants::CUSHION_HEAD_ID,
		segment(
			vector(
				cushionLeft.seg.s.x,
				cushionLeft.seg.s.y - SUB_SEG_OFFSET,
				cushionLeft.seg.s.z
			),
			vector::add(
				bottomLeftPocket.position,
				vector(-CornerRadiusSqrt2, CornerRadiusSqrt2, cushionZ)
			)
		)
	);
	cushionBottomLeftCorner_2.collapseStartPoint(SUB_SEG_OFFSET);

	CushionSegment cushionMidTop_1 = CushionSegment(
		PhysicsConstants::CUSHION_SIDE_1_ID,
		segment(
			vector::add(topSidePocket.position, vector(-SIDE_POCKET_RADIUS, 0, cushionZ)),
			vector(
				cushionTop_1.seg.e.x + SUB_SEG_OFFSET,
				cushionTop_1.seg.e.y,
				cushionTop_1.seg.e.z
			)
		)
	);
	cushionMidTop_1.collapseEndPoint(SUB_SEG_OFFSET);

	CushionSegment cushionMidTop_2 = CushionSegment(
		PhysicsConstants::CUSHION_SIDE_1_ID,
		segment(
			vector(
				cushionTop_2.seg.s.x - SUB_SEG_OFFSET,
				cushionTop_2.seg.s.y,
				cushionTop_2.seg.s.z
			),
			vector::add(
				topSidePocket.position,
				vector(SIDE_POCKET_RADIUS, 0, cushionZ)
			)
		)
	);
	cushionMidTop_2.collapseStartPoint(SUB_SEG_OFFSET);

	CushionSegment cushionMidBottom_1 = CushionSegment(
		PhysicsConstants::CUSHION_SIDE_2_ID,
		segment(
			vector::add(
				bottomSidePocket.position,
				vector(SIDE_POCKET_RADIUS, 0, cushionZ)
			),
			vector(
				cushionBottom_1.seg.s.x - SUB_SEG_OFFSET,
				cushionBottom_1.seg.s.y,
				cushionBottom_1.seg.s.z
			)
		)
	);
	cushionMidBottom_1.collapseEndPoint(SUB_SEG_OFFSET);

	CushionSegment cushionMidBottom_2 = CushionSegment(
		PhysicsConstants::CUSHION_SIDE_2_ID,
		segment(
			vector(
				cushionBottom_2.seg.e.x + SUB_SEG_OFFSET,
				cushionBottom_2.seg.e.y,
				cushionBottom_2.seg.e.z
			),
			vector::add(bottomSidePocket.position, vector(-SIDE_POCKET_RADIUS, 0, cushionZ))
		)
	);
	cushionMidBottom_2.collapseStartPoint(SUB_SEG_OFFSET);

	CushionSegment subCushionBottomLeftCorner_2 = CushionSegment(
		cushionBottomLeftCorner_2.id,
		segment(cushionLeft.seg.s, cushionBottomLeftCorner_2.seg.s)
	);

	CushionSegment subCushionBottomLeftCorner_1 = CushionSegment(
		cushionBottomLeftCorner_1.id,
		segment(cushionBottom_2.seg.s, cushionBottomLeftCorner_1.seg.s)
	);

	CushionSegment subCushionMidBottom_2 = CushionSegment(
		cushionMidBottom_2.id,
		segment(cushionBottom_2.seg.e, cushionMidBottom_2.seg.s)
	);

	CushionSegment subCushionMidBottom_1 = CushionSegment(
		cushionMidBottom_1.id,
		segment(cushionBottom_1.seg.s, cushionMidBottom_1.seg.e)
	);

	CushionSegment subCushionBottomRightCorner_2 = CushionSegment(
		cushionBottomRightCorner_2.id,
		segment(cushionBottom_1.seg.e, cushionBottomRightCorner_2.seg.s)
	);

	CushionSegment subCushionBottomRightCorner_1 = CushionSegment(
		cushionBottomRightCorner_1.id,
		segment(cushionRight.seg.s, cushionBottomRightCorner_1.seg.s)
	);

	CushionSegment subCushionTopRightCorner_2 = CushionSegment(
		cushionTopRightCorner_2.id,
		segment(cushionRight.seg.e, cushionTopRightCorner_2.seg.s)
	);

	CushionSegment subCushionTopRightCorner_1 = CushionSegment(
		cushionTopRightCorner_1.id,
		segment(cushionTop_2.seg.e, cushionTopRightCorner_1.seg.s)
	);

	CushionSegment subCushionMidTop_2 = CushionSegment(
		cushionMidTop_2.id,
		segment(cushionTop_2.seg.s, cushionMidTop_2.seg.s)
	);

	CushionSegment subCushionMidTop_1 = CushionSegment(
		cushionMidTop_1.id,
		segment(cushionTop_1.seg.e, cushionMidTop_1.seg.e)
	);

	CushionSegment subCushionTopLeftCorner_2 = CushionSegment(
		cushionTopLeftCorner_2.id,
		segment(cushionTop_1.seg.s, cushionTopLeftCorner_2.seg.s)
	);

	CushionSegment subCushionTopLeftCorner_1 = CushionSegment(
		cushionTopLeftCorner_1.id,
		segment(cushionLeft.seg.e, cushionTopLeftCorner_1.seg.s)
	);

	_cushionSegments = {
		// mainSegments
		cushionLeft,
		cushionTop_1,
		cushionTop_2,
		cushionRight,
		cushionBottom_1,
		cushionBottom_2,

		// cornerSegments
		cushionTopLeftCorner_1,
		cushionTopLeftCorner_2,
		cushionMidTop_1,
		cushionMidTop_2,
		cushionTopRightCorner_1,
		cushionTopRightCorner_2,
		cushionBottomRightCorner_1,
		cushionBottomRightCorner_2,
		cushionMidBottom_1,
		cushionMidBottom_2,
		cushionBottomLeftCorner_1,
		cushionBottomLeftCorner_2,

		// subSegments
		subCushionBottomLeftCorner_2,
		subCushionBottomLeftCorner_1,
		subCushionMidBottom_2,
		subCushionMidBottom_1,
		subCushionBottomRightCorner_2,
		subCushionBottomRightCorner_1,
		subCushionTopRightCorner_2,
		subCushionTopRightCorner_1,
		subCushionMidTop_2,
		subCushionMidTop_1,
		subCushionTopLeftCorner_2,
		subCushionTopLeftCorner_1,
	};

	_cushionPoints ={
		CushionPoint(cushionLeft.seg.s, &cushionLeft),
		CushionPoint(cushionLeft.seg.e, &cushionLeft),
		CushionPoint(cushionTop_1.seg.s, &cushionTop_1),
		CushionPoint(cushionTop_1.seg.e, &cushionTop_1),
		CushionPoint(cushionTop_2.seg.s, &cushionTop_2),
		CushionPoint(cushionTop_2.seg.e, &cushionTop_2),
		CushionPoint(cushionRight.seg.s, &cushionRight),
		CushionPoint(cushionRight.seg.e, &cushionRight),
		CushionPoint(cushionBottom_1.seg.s, &cushionBottom_1),
		CushionPoint(cushionBottom_1.seg.e, &cushionBottom_1),
		CushionPoint(cushionBottom_2.seg.s, &cushionBottom_2),
		CushionPoint(cushionBottom_2.seg.e, &cushionBottom_2),
		CushionPoint(cushionTopLeftCorner_1.seg.s, &cushionLeft),
		CushionPoint(cushionTopLeftCorner_2.seg.s, &cushionTop_1),
		CushionPoint(cushionTopRightCorner_1.seg.s, &cushionTop_2),
		CushionPoint(cushionTopRightCorner_2.seg.s, &cushionRight),
		CushionPoint(cushionBottomRightCorner_1.seg.s, &cushionRight),
		CushionPoint(cushionBottomRightCorner_2.seg.s, &cushionBottom_1),
		CushionPoint(cushionBottomLeftCorner_1.seg.s, &cushionBottom_2),
		CushionPoint(cushionBottomLeftCorner_2.seg.s, &cushionLeft),
		CushionPoint(cushionMidTop_1.seg.e, &cushionTop_1),
		CushionPoint(cushionMidTop_2.seg.s, &cushionTop_2),
		CushionPoint(cushionMidBottom_1.seg.e, &cushionBottom_1),
		CushionPoint(cushionMidBottom_2.seg.s, &cushionBottom_2)
	};

	AABB safeZone = AABB(
		cushionTop_1.seg.s.x,
		cushionTop_2.seg.e.x,
		cushionLeft.seg.s.y,
		cushionLeft.seg.e.y
	);

	AABB tableEdgeLimits = AABB(
		limits.minX - TABLE_EDGE_THICKNESS_H,
		limits.maxX + TABLE_EDGE_THICKNESS_H,
		limits.minY - TABLE_EDGE_THICKNESS_V,
		limits.maxY + TABLE_EDGE_THICKNESS_V
	);

	_floorSurface.setLimits(
		safeZone, &limits, _pockets, tableEdgeLimits
	);
}

void ps::PhysicsWorldV2::updateSimulateStepTime(const vector & initialVelocity)
{
	_frameTime = DEFAULT_FRAME_TIME;
}

void ps::PhysicsWorldV2::update(double dt)
{
	if (!_run) return;

	if (_runStaticMotion) {
		stepStaticMotion(dt);
	}
	else {
		stepDifferentialMotion(dt);
	}
}

void ps::PhysicsWorldV2::resetStaticWorldBallPosition()
{
	for (auto ball : _balls) {
		_staticWorld->setBallPosition(ball->id(), ball->position());
	}
}

void ps::PhysicsWorldV2::step(double frameTime)
{
	if (LOG_DEBUG)
	{
		CUSTOMLOG("stepCounter = %d\n", stepCount);
		CUSTOMLOG("%s", log().c_str());
	}
	
	int iMotionLess = 0;
	bool motionLess;
	for (BallBody *ball : _balls)
	{
		ball->backupPosition();
		motionLess = !ball->step(frameTime);
		if (iMotionLess < _cacheMotionLess.size()) {
			_cacheMotionLess[iMotionLess++] = motionLess;
		}
	}
	if (LOG_DEBUG) 
	{
		CUSTOMLOG("%s", log().c_str());
		dataLog += log();
	//if (stepCount == 1030) {
	//	std::string logballs;
	//	logAllBall(logballs);
	//	dataLog += logballs;
	//}
	}

	CushionCollisions cushionCollisions;
	std::list<CollisionCushionPoint> &cushionPointCollisions = cushionCollisions.cushionPointCollisions;
	std::list<CollisionCushionSegment> &cushionSegmentCollisions = cushionCollisions.cushionSegmentCollisions;
	std::vector<CollisionBall> ballCollisions;
	std::vector<CollisionFloor> floorCollisions;
	std::vector<CollisionFloor> edgeCollisions;

	checkCollisionsWithCushions(cushionCollisions, _cacheMotionLess);
	checkBallCollisions(ballCollisions, _cacheMotionLess);
	checkCollisionsWithFloor(floorCollisions, _cacheMotionLess);
	checkCollisionsWithEdge(edgeCollisions, _cacheMotionLess);

	int totalCollisions = cushionPointCollisions.size() + cushionSegmentCollisions.size() +
		ballCollisions.size() + floorCollisions.size() + edgeCollisions.size();

	double dt = frameTime;

	if (totalCollisions > 0) {
		double minTime = 0;
		double n_frametimeLimit = -2 * frameTime;

		for (CollisionCushionSegment col : cushionSegmentCollisions)
		{
			if (col.time >= n_frametimeLimit) {
				minTime = MIN(minTime, col.time);
			}
		}

		for (CollisionCushionPoint col : cushionPointCollisions)
		{
			if (col.time >= n_frametimeLimit) {
				minTime = MIN(minTime, col.time);
			}
		}

		for (CollisionBall col : ballCollisions)
		{
			if (col.time >= n_frametimeLimit) {
				minTime = MIN(minTime, col.time);
			}
		}

		for (CollisionFloor col : floorCollisions)
		{
			if (col.time >= n_frametimeLimit) {
				minTime = MIN(minTime, col.time);
			}
		}

		for (CollisionFloor col : edgeCollisions)
		{
			if (col.time >= n_frametimeLimit) {
				minTime = MIN(minTime, col.time);
			}
		}

		if (minTime < 0) {
			dt = frameTime + minTime;
			for (auto ball : _balls)
			{
				ball->restorePosition();
				ball->step(dt);
			}
		}
		_timestamp += dt;

		for (CollisionCushionPoint collision : cushionPointCollisions)
		{
			if (collision.time == minTime) {
				_dispatcher->dispatchCushionCollision(_timestamp,collision.ball, collision.cushionPoint->cushion);
				collision.ball->bounceIntersectSegmentPoint(collision.cushionPoint->position);
				_simulateResult->cushionCollisions.push_back(
					PhysicSimulateResult::BallCushionPair(_timestamp, collision.ball->id(), collision.cushionPoint->cushion->id)
				);
			}
		}

		for (CollisionCushionSegment collision : cushionSegmentCollisions)
		{
			if (collision.time == minTime) {
				_dispatcher->dispatchCushionCollision(_timestamp, collision.ball, collision.cushionSegment);
				collision.ball->bounceSegment(collision.cushionSegment->seg);
				_simulateResult->cushionCollisions.push_back(
					PhysicSimulateResult::BallCushionPair(_timestamp, collision.ball->id(), collision.cushionSegment->id)
				);
			}
		}

		for (CollisionBall collision : ballCollisions)
		{
			if (collision.time == minTime) {
				resolveBallsCollision(*collision.ball_1, *collision.ball_2);
				_simulateResult->ballCollisionIdPairs.push_back(
					PhysicSimulateResult::BallIDPair(_timestamp, collision.ball_1->id(), collision.ball_2->id(),
						collision.ball_1->linearVelocity(), collision.ball_2->linearVelocity()));
				_dispatcher->dispatchBallCollision(_timestamp, collision.ball_1, collision.ball_2);
			}
		}
		
		for (CollisionFloor collision : floorCollisions)
		{
			if (collision.time == minTime) {
				// if (collision.normal.x == 0 && collision.normal.y == 0) {
				collision.ball->bounceFloor(collision.normal);
				_simulateResult->floorCollisionIds.push_back(collision.ball->id());
				_dispatcher->dispatchFloorCollision(_timestamp, collision.ball);
				// }
			}
		}

		for (CollisionFloor collision : edgeCollisions)
		{
			if (collision.time == minTime) {
				collision.ball->bounceEdge(collision.normal);
				_dispatcher->dispatchEdgeCollision(_timestamp, collision.ball);
			}
		}
	}

	bool allBallMotionLess = true;
	for (int i = 0; i < _balls.size(); i++) {
		BallBody* ball = _balls.at(i);
		if (!ball->isMotionless()) {
			if (ball->isDisabled());
			ball->updateVelocity(dt);
			ball->updateQuaternion(dt);
			ball->finalizeStep();
			allBallMotionLess = false;
		}
	}
	stepCount++;
	if (allBallMotionLess) {
		_run = false;
		CUSTOMLOG("iteration %d", stepCount);
		CUSTOMLOG("After Shooting Seed %.0lf , %.0lf", _random->getCurrentSeed(), _random->getSeed());
		for (int i = 0; i < _balls.size(); i++) {
			BallBody* currentBall = _balls.at(i);
			currentBall->resetShootingState();
			vector position = currentBall->position();
			CUSTOMLOG("%d: %.12lf, %.12lf, %.12lf", currentBall->id(), +position.x, position.y, position.z);
		}
		
		onWorldPause();
	}
	
	checkExtensionEvent();
}

void ps::PhysicsWorldV2::stepStaticMotion(double dt)
{
	auto ctrl = _staticMotionCtrl;
	auto cueBall = _cueBall;

	ctrl->update(dt * _timeScale);
	auto position = ctrl->position();
	auto quaternion = ctrl->quat();
	cueBall->setMotionStatus(position, quaternion);
	cueBall->dispatch();

	if (ctrl->isFinished()) {
		
		CUSTOMLOG("Physics Static End Time: %.15lf", ctrl->getEndTime());
		CUSTOMLOG("Physics Static End Time: %s", getBinaryDouble(ctrl->getEndTime()).c_str());
		_runStaticMotion = false;

		auto result = ctrl->calcFinalResult();
		CUSTOMLOG("Cue Ball Pos: %.13lf, %.13lf, %.13lf", result->position.x, result->position.y, result->position.z);
		CUSTOMLOG("Cue Ball lv: %.13lf, %.13lf, %.13lf", result->linearVelocity.x, result->linearVelocity.y, result->linearVelocity.z);
		CUSTOMLOG("Cue Ball av: %.13lf, %.13lf, %.13lf", result->angularVelocity.x, result->angularVelocity.y, result->angularVelocity.z);

		cueBall->setPosition(result->position);
		cueBall->setLinearVelocity(result->linearVelocity);
		cueBall->setAngularVelocity(result->angularVelocity);
	}
}

void ps::PhysicsWorldV2::stepDifferentialMotion(double dt)
{
	_remainFrameTime = _remainFrameTime + dt * _timeScale;
	int loop = 0;
	while (_remainFrameTime > _frameTime) {
		loop++;
		_remainFrameTime -= _frameTime;
		step(_frameTime);
		if (loop > LIMIT_LOOP_PER_FRAME) {
			_remainFrameTime = 0;
			break;
		}
	}
	dispatch();
}

void ps::PhysicsWorldV2::checkCollisionsWithCushions(CushionCollisions & collisions, std::vector<bool> & cacheMotionLess)
{
	std::list<CollisionCushionPoint> & cushionPointCollisions = collisions.cushionPointCollisions;
	std::list<CollisionCushionSegment> & cushionSegmentCollisions = collisions.cushionSegmentCollisions;

	int i = 0;
	for (BallBody* ball: _balls) 
	{
		if (checkCacheMotionLess(cacheMotionLess, i++)) continue;

		if (ball->isDisabled() || ball->isFallingInPocket()) continue;

		if (!isPosibleToBeColliding(ball)) continue;

		// boolean foundCollision = false;
		vector position = ball->position();

		for (int j = 0; j < _cushionPoints.size(); j++) {
			CushionPoint &cushionPoint = _cushionPoints[j];

			if (ExtMath::isLessThanZeroNumber(
				vector::distanceSquare(position, cushionPoint.position) -
				PhysicsConstants::BALL_RADIUS_SQUARE
			)) {
				CollisionCushionPoint collision;

				collision.time = ball->calcPenetrationPointTimeBack(cushionPoint.position);
				collision.ball = ball;
				collision.cushionPoint = &cushionPoint;

				cushionPointCollisions.push_back(collision);
				break;
			}
		}

		for (int j = 0; j < _cushionSegments.size(); j++) {
			CushionSegment &cushionSegment = _cushionSegments[j];
			// Using distance square for better performance & precision
			if (ExtMath::isLessThanZeroNumber(
				segment::distanceSquare(position, cushionSegment.seg) -
				PhysicsConstants::BALL_RADIUS_SQUARE
			)) {
				CollisionCushionSegment collision;

				collision.time = ball->calcPenetrationSegmentTimeBack(cushionSegment.seg);
				collision.cushionSegment = &cushionSegment;
				collision.ball = ball;

				cushionSegmentCollisions.push_back(collision);
				break;
			}
		}

	}

}

inline bool ps::PhysicsWorldV2::checkCacheMotionLess(std::vector<bool> & cacheMotionLess, int index)
{
	if (index >= MAX_BALL) {
		return false;
	}
	return cacheMotionLess[index];
}

bool ps::PhysicsWorldV2::isPosibleToBeColliding(BallBody *ball)
{
	vector p = ball->position();
	return p.x < _nonCushionCollisionZone.minX
		|| p.x > _nonCushionCollisionZone.maxX
		|| p.y < _nonCushionCollisionZone.minY
		|| p.y > _nonCushionCollisionZone.maxY;
}

void ps::PhysicsWorldV2::checkBallCollisions(std::vector<CollisionBall> & ballCollisions, std::vector<bool> & cacheMotionLess)
{
	auto balls = _balls;

	std::vector<CollisionBall> & collisions = ballCollisions;
	int size = balls.size();
	for (int i = 0; i < size - 1; i++) {
		BallBody *ball_1 = balls.at(i);

		if (ball_1->isDisabled() || ball_1->isFallingInPocket()) continue;

		for (int j = i + 1; j < size; j++) {
			if (checkCacheMotionLess(cacheMotionLess, i) && checkCacheMotionLess(cacheMotionLess, j)) continue;

			BallBody *ball_2 = balls.at(j);
			if (ball_2->isDisabled() || ball_2->isFallingInPocket()) continue;

			if (!checkAABBOverlap(*ball_1, *ball_2)) continue;
			if (!checkCollision(*ball_1, *ball_2)) continue;
			if (vector::equalsZero(ball_1->linearVelocity())
				&& vector::equalsZero(ball_2->linearVelocity())) continue;
			CollisionBall collision;
			collision.ball_1 = ball_1;
			collision.ball_2 = ball_2;
			collision.time = calcPenetrationTimeBack(*collision.ball_1, *collision.ball_2);
			collisions.push_back(collision);
		}
	}
}

void ps::PhysicsWorldV2::checkCollisionsWithFloor(std::vector<CollisionFloor> & floorCollisions, std::vector<bool> & cacheMotionLess)
{
	std::vector<CollisionFloor> & collisions = floorCollisions;
	int i = 0;
	for (BallBody* ball: _balls) {

		if (checkCacheMotionLess(cacheMotionLess, i++)) continue;

		if (ball->isDisabled() || ball->isFallingInPocket()) continue;
		vector position = ball->position();
		if (ExtMath::isLessThanZeroNumber(position.z) && position.z > -PhysicsConstants::BALL_RADIUS) {
			vector normal = _floorSurface.normalAcceleration(position);
			if (
				ExtMath::isZeroNumber(normal.x) &&
				ExtMath::isZeroNumber(normal.y) &&
				!ExtMath::isZeroNumber(normal.z)
				) {
				double time = calcFloorPenetrationTimeBack(*ball);
				if (time < 0) {
					CollisionFloor collision;
					collision.ball = ball;
					collision.normal = normal;
					collision.time = time;
					collisions.push_back(collision);
				}
			}
		}
	}
}

void ps::PhysicsWorldV2::checkCollisionsWithEdge(std::vector<CollisionFloor> & edgeCollisions, std::vector<bool> & cacheMotionLess)
{
	std::vector<CollisionFloor> & collisions = edgeCollisions;
	const double CUSHION_HEIGHT = PhysicsConstants::CUSHION_HEIGHT;
	int i = 0;
	for (BallBody *ball: _balls) {
		if (checkCacheMotionLess(cacheMotionLess, i++)) continue;

		if (ball->isDisabled() || ball->isFallingInPocket()) continue;
		vector position = ball->position();

		if (!isPosibleToBeColliding(ball)) continue;

		if (!_floorSurface.isOnTableEdges(position)) {
			continue;
		}

		if (ExtMath::isLessThanZeroNumber(position.z - CUSHION_HEIGHT) && position.z > CUSHION_HEIGHT - PhysicsConstants::BALL_RADIUS) {
			vector normal = _floorSurface.normalAcceleration(position);
			if (ExtMath::isZeroNumber(normal.x) &&
				ExtMath::isZeroNumber(normal.y) &&
				!ExtMath::isZeroNumber(normal.z)
				) {
				double time = calcFloorPenetrationTimeBack(*ball);
				if (time < 0) {
					CollisionFloor collision;
					collision.ball = ball;
					collision.normal = normal;
					collision.time = time;
					collisions.push_back(collision);
				}
			}
		}
	}
}

bool ps::PhysicsWorldV2::checkAABBOverlap(BallBody & ball_1, BallBody & ball_2)
{
	vector p1 = ball_1.position();
	vector p2 = ball_2.position();
	return ExtMath::isGreatThanZeroNumber(p1.x + BALL_DIAMETER - p2.x)
		&& ExtMath::isLessThanZeroNumber(p1.x - p2.x - BALL_DIAMETER)

		&& ExtMath::isGreatThanZeroNumber(p1.y + BALL_DIAMETER - p2.y)
		&& ExtMath::isLessThanZeroNumber(p1.y - p2.y - BALL_DIAMETER)

		&& ExtMath::isGreatThanZeroNumber(p1.z + BALL_DIAMETER - p2.z)
		&& ExtMath::isLessThanZeroNumber(p1.z - p2.z - BALL_DIAMETER);
}

bool ps::PhysicsWorldV2::checkCollision(BallBody & ball_1, BallBody & ball_2)
{
	vector v = vector::sub(ball_2.position(), ball_1.position());
	return ExtMath::isLessThanZeroNumber(
		vector::dot(v, v) - PhysicsConstants::BALL_DIAMETER_SQUARE
	);
}

double ps::PhysicsWorldV2::calcPenetrationTimeBack(BallBody & ball_1, BallBody & ball_2)
{
	vector p1 = ball_1.position();
	vector p2 = ball_2.position();

	vector dp = vector::sub(p2, p1);

	double distance = vector::length(dp);
	double penetrationLength = PhysicsConstants::BALL_DIAMETER - distance;

	if (ExtMath::isLessThanZeroNumber(penetrationLength)) return 0;

	vector v1 = ball_1.linearVelocity();
	vector v2 = ball_2.linearVelocity();

	vector dv = vector::sub(v2, v1);
	double dpdv = vector::dot(dp, dv);
	double dvdv = vector::dot(dv, dv);
	double dpdp = vector::dot(dp, dp);
	double delta = dpdv * dpdv - dvdv * (dpdp - PhysicsConstants::BALL_DIAMETER_SQUARE);

	double t = (-dpdv - sqrt(delta)) / dvdv;

	return t;
}

double ps::PhysicsWorldV2::calcFloorPenetrationTimeBack(BallBody & ball)
{
	double minZ = _floorSurface.minimumZOfPosition(ball.position());
	if (minZ == NEG_INFINITY) return 0;
	double penetration = minZ - ball.position().z;
	double vz = ball.linearVelocity().z;
	double t = MIN(0, penetration / vz);
	return t;
}

double ps::PhysicsWorldV2::randomAngle()
{
	double r = _random->rand();
	// const r = Math.random();
	double sign = (r >= 0.5) ? 1 : -1;
	double value = ExtMath::PI * (0.05 + 0.05 * 2 * abs(r - 0.5));
	double angle = sign * value;
	// cc.log("Angle: " + angle);
	return angle;
}

void ps::PhysicsWorldV2::resolveBallsCollision(BallBody & ball_1, BallBody & ball_2)
{
	double noiseAngle = 0;
	if (!_break && ball_1.id() != 0 && ball_2.id() != 0) {
		noiseAngle = randomAngle();
	}

	std::vector<vector> velocities;
	CollisionResolver::resolveCollisionBallWithBall(
		velocities,
		ball_1.position(), ball_2.position(),
		ball_1.linearVelocity(), ball_2.linearVelocity(),
		noiseAngle
	);

	ball_1.setLinearVelocity(velocities[0]);
	ball_2.setLinearVelocity(velocities[1]);

	ball_1.setPureRolling(false);
	ball_2.setPureRolling(false);
}

void ps::PhysicsWorldV2::makeAllBallsCapture(std::vector<BallBodyCapture> & captures)
{
	for (BallBody *ball : _balls)
	{
		BallBodyCapture capture;
		ball->makeCapture(capture);
		captures.push_back(capture);
	}
}

void ps::PhysicsWorldV2::checkOutsideTableBalls()
{
	for (BallBody * ball : _balls)
	{
		if (ball->isDisabled() && !ball->isFallingInPocket())
		{
			_simulateResult->outsideTableBallIds.push_back(ball->id());
		}
	}
}

std::string ps::PhysicsWorldV2::log()
{
	std::string ret = "";
	char text[2000];
	vector pos;
	vector vel;
	vector aVel;
	for (BallBody* ball : _balls)
	{
		pos.x += ball->position().x;
		pos.y += ball->position().y;
		pos.z += ball->position().z;

		vel.x += ball->linearVelocity().x;
		vel.y += ball->linearVelocity().y;
		vel.z += ball->linearVelocity().z;
		
		aVel.x += ball->angularVelocity().x;
		aVel.y += ball->angularVelocity().y;
		aVel.z += ball->angularVelocity().z;
	}
	sprintf(text, "position (%.12lf, %.12lf, %.12lf),\n velocity (%s, %s, %s),\n aVel(%s %s %s) \n", pos.x, pos.y, pos.z,
		getBinaryDouble(vel.x).c_str(), getBinaryDouble(vel.y).c_str(), getBinaryDouble(vel.z).c_str(),
		getBinaryDouble(aVel.x).c_str(), getBinaryDouble(aVel.y).c_str(), getBinaryDouble(aVel.z).c_str());
	ret += std::string(text);
	return ret;
}

void ps::PhysicsWorldV2::logAllBall(std::string &ret)
{
	char text[2000];
	vector pos;
	vector vel;
	for (BallBody* ball : _balls)
	{
		pos = ball->position();
		vel = ball->linearVelocity();
		vector aVel = ball->angularVelocity();

		sprintf(text, "%d: position (%.12lf, %.12lf, %.12lf), \nvelocity (%s, %s, %s), \naVelocity (%s, %s, %s)\n", 
			ball->id(),
			pos.x, pos.y, pos.z,
			getBinaryDouble(vel.x).c_str(), getBinaryDouble(vel.y).c_str(), getBinaryDouble(vel.z).c_str(),
			getBinaryDouble(aVel.x).c_str(), getBinaryDouble(aVel.y).c_str(), getBinaryDouble(aVel.z).c_str());
		ret += std::string(text);
		CUSTOMLOG(text);
	}
}

void ps::PhysicsWorldV2::onWorldPause()
{
	_break = true;
	_remainFrameTime = 0;

	_dispatcher->dispatchWorldPaused(_timestamp);

	resetStaticWorldBallPosition();
}

void ps::PhysicsWorldV2::dispatch()
{
	for (auto ball : _balls)
	{
		ball->dispatch();
	}
}

