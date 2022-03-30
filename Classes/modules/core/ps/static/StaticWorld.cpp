#include "StaticWorld.h"
#include "../internal/PhysicsConstants.hpp"
#include "../internal/CollisionResolver.h"
#include "cocos2d.h"
#include "data/GameConstant.h"
#include "core/GameMgr.h"

using namespace ps;

const int StaticWorld::TYPE_EVENT_OUTSIDE = -1;
const int StaticWorld::TYPE_EVENT_COLLIDE_BALL = -2;
const int StaticWorld::TYPE_EVENT_STOPPED = -3;
const int StaticWorld::TYPE_EVENT_COLLIDE_EDGE = -4;
const int StaticWorld::TYPE_EVENT_COLLIDE_POCKET_ZONE = -5;
// Continue calculating events
const int StaticWorld::TYPE_EVENT_COLLIDE_CUSHION = 1;
const int StaticWorld::TYPE_EVENT_MOTION_EXPIRED = 2;
const int StaticWorld::TYPE_EVENT_COLLIDE_CUSHION_POINT = 3;

const double ps::StaticWorld::SIDE_POCKET_WIDTH = PhysicsConstants::SIDE_POCKET_WIDTH;
const double ps::StaticWorld::CORNER_POCKET_WIDTH = PhysicsConstants::CORNER_POCKET_WIDTH;
const double ps::StaticWorld::CORNER_POCKET_RADIUS = PhysicsConstants::CORNER_POCKET_RADIUS;
const double ps::StaticWorld::SIDE_POCKET_RADIUS = PhysicsConstants::SIDE_POCKET_RADIUS;

const double ps::StaticWorld::SUB_SEG_OFFSET = PhysicsConstants::SUB_SEG_OFFSET;

const double ps::StaticWorld::MIDDLE_SEG_OFFSET = SIDE_POCKET_WIDTH / 2;
const double ps::StaticWorld::CORN_SEG_OFFSET = CORNER_POCKET_WIDTH / sqrt(2);
const double ps::StaticWorld::SIDE_POCKET_DEPTH = PhysicsConstants::SIDE_POCKET_DEPTH;
const double ps::StaticWorld::CORNER_POCKET_DEPTH = PhysicsConstants::CORNER_POCKET_DEPTH;

const double ps::StaticWorld::BALL_RADIUS = PhysicsConstants::BALL_RADIUS;
const double ps::StaticWorld::TABLE_WIDTH = PhysicsConstants::TABLE_WIDTH;
const double ps::StaticWorld::TABLE_HEIGHT = PhysicsConstants::TABLE_HEIGHT;
const double ps::StaticWorld::TABLE_MAX_X = TABLE_WIDTH / 2 - PhysicsConstants::BALL_RADIUS;
const double ps::StaticWorld::TABLE_MAX_Y = TABLE_HEIGHT / 2 - PhysicsConstants::BALL_RADIUS;
const double ps::StaticWorld::TABLE_MIN_X = -TABLE_MAX_X;
const double ps::StaticWorld::TABLE_MIN_Y = -TABLE_MAX_Y;

const double ps::StaticWorld::BOUNCE_CUSHION_COEF = PhysicsConstants::BOUNCE_CUSHION_COEF;
const double ps::StaticWorld::SPIN_CUSHION_FRICTION = PhysicsConstants::SPIN_CUSHION_FRICTION;
const double ps::StaticWorld::BALL_RADIUS_SQUARE = PhysicsConstants::BALL_RADIUS_SQUARE;
const double ps::StaticWorld::BALL_DIAMETER = PhysicsConstants::BALL_DIAMETER;

const double ps::StaticWorld::BALL_DIAMETER_SQUARE = PhysicsConstants::BALL_DIAMETER_SQUARE;

const double ps::StaticWorld::CUSHION_HEIGHT = PhysicsConstants::CUSHION_HEIGHT;
const double ps::StaticWorld::CUSHION_Z = CUSHION_HEIGHT - BALL_RADIUS;

const double ps::StaticWorld::MOTION_STEP =1.5;

const double ps::StaticWorld::MIN_MOTION_VEL = StaticWorld::MOTION_STEP;

const double ps::StaticWorld::EDGE_Z = CUSHION_Z + PhysicsConstants::BALL_RADIUS;

const double ps::StaticWorld::EDGE_PADDING = 20;
const double ps::StaticWorld::EDGE_MAX_X = TABLE_WIDTH / 2 + EDGE_PADDING;
const double ps::StaticWorld::EDGE_MAX_Y = TABLE_HEIGHT / 2 + EDGE_PADDING;
const double ps::StaticWorld::EDGE_MIN_X = -EDGE_MAX_X;
const double ps::StaticWorld::EDGE_MIN_Y = -EDGE_MAX_Y;

const double ps::StaticWorld::SIDE_POCKET_BALL_RADIUS = BALL_RADIUS + SIDE_POCKET_RADIUS;
const double ps::StaticWorld::CORNER_POCKET_BALL_RADIUS = BALL_RADIUS + CORNER_POCKET_RADIUS;

const double ps::StaticWorld::CUSHION_PENETRATION = 3 * BALL_RADIUS;
const double ps::StaticWorld::CUSHION_MAX_X = TABLE_MAX_X + CUSHION_PENETRATION;
const double ps::StaticWorld::CUSHION_MAX_Y = TABLE_MAX_Y + CUSHION_PENETRATION;
const double ps::StaticWorld::CUSHION_MIN_X = TABLE_MIN_X - CUSHION_PENETRATION;
const double ps::StaticWorld::CUSHION_MIN_Y = TABLE_MIN_Y - CUSHION_PENETRATION;

const double ps::StaticWorld::TABLE_PADDING = BALL_RADIUS - sqrt(BALL_RADIUS_SQUARE - CUSHION_Z * CUSHION_Z);
const double ps::StaticWorld::TABLE_PADDING_MAX_X = TABLE_MAX_X + TABLE_PADDING;
const double ps::StaticWorld::TABLE_PADDING_MIN_X = TABLE_MIN_X - TABLE_PADDING;
const double ps::StaticWorld::TABLE_PADDING_MAX_Y = TABLE_MAX_Y + TABLE_PADDING;
const double ps::StaticWorld::TABLE_PADDING_MIN_Y = TABLE_MIN_Y - TABLE_PADDING;

const double ps::StaticWorld::TABLE_PADDING_EXTEND_MAX_X = StaticWorld::TABLE_PADDING_MAX_X + 5;
const double ps::StaticWorld::TABLE_PADDING_EXTEND_MIN_X = StaticWorld::TABLE_PADDING_MIN_X - 5;
const double ps::StaticWorld::TABLE_PADDING_EXTEND_MAX_Y = StaticWorld::TABLE_PADDING_MAX_Y + 5;
const double ps::StaticWorld::TABLE_PADDING_EXTEND_MIN_Y = StaticWorld::TABLE_PADDING_MIN_Y - 5;

const double ps::StaticWorld::BASIC_STEP_LENGTH = 1.5;

const double ps::StaticWorld::DIR_LENGTH_THESHOLD = 1e-2 /20;

const double ps::StaticWorld::DELTA_CUSHION_BALL = StaticWorld::CUSHION_HEIGHT - StaticWorld::BALL_RADIUS;

const double ps::StaticWorld::CUSHION_DISTANCE_DELTA = sqrt(BALL_RADIUS_SQUARE - DELTA_CUSHION_BALL * DELTA_CUSHION_BALL);

const ps::StaticWorld::Velocities ps::StaticWorld::EmptyVelocities = Velocities(vector::ZERO, vector::ZERO);

const int ps::StaticWorld::CUSHION_HEAD_ID = PhysicsConstants::CUSHION_HEAD_ID;
const int ps::StaticWorld::CUSHION_FOOT_ID = PhysicsConstants::CUSHION_FOOT_ID;
const int ps::StaticWorld::CUSHION_SIDE_1_ID = PhysicsConstants::CUSHION_SIDE_1_ID;
const int ps::StaticWorld::CUSHION_SIDE_2_ID = PhysicsConstants::CUSHION_SIDE_2_ID;


bool ps::StaticWorld::isGT(const double & a, const double & b)
{
	return ExtMath::isGreatThanZeroNumber(a - b);
}

bool ps::StaticWorld::isLT(const double & a, const double & b)
{
	return ExtMath::isLessThanZeroNumber(a - b);
}

ps::ExtMath::vector ps::StaticWorld::vecFlipY(const vector & v)
{
	return vector(v.x, -v.y, v.z);
}

void ps::StaticWorld::initCushionAndPockets()
{
	AABB limits;
	limits.minX = -TABLE_WIDTH / 2;
	limits.maxX = TABLE_WIDTH / 2;
	limits.minY = -TABLE_HEIGHT / 2;
	limits.maxY = TABLE_HEIGHT / 2;

	double CornerDepthSqrt2 = CORNER_POCKET_DEPTH / sqrt(2);

	_pockets[0] = Pocket(
		vector(limits.minX - CornerDepthSqrt2, limits.maxY + CornerDepthSqrt2),
		CORNER_POCKET_RADIUS,
		CORNER_POCKET_BALL_RADIUS
	);
	_pockets[1]= Pocket(
		vector(limits.maxX + CornerDepthSqrt2, limits.maxY + CornerDepthSqrt2),
		CORNER_POCKET_RADIUS,
		CORNER_POCKET_BALL_RADIUS
	);
	_pockets[2] = Pocket(
		vector(limits.maxX + CornerDepthSqrt2, limits.minY - CornerDepthSqrt2),
		CORNER_POCKET_RADIUS,
		CORNER_POCKET_BALL_RADIUS
	);
	_pockets[3] = Pocket(
		vector(limits.minX - CornerDepthSqrt2, limits.minY - CornerDepthSqrt2),
		CORNER_POCKET_RADIUS,
		CORNER_POCKET_BALL_RADIUS
	);
	_pockets[4] = Pocket(
		vector(0, limits.maxY + SIDE_POCKET_DEPTH),
		SIDE_POCKET_RADIUS,
		SIDE_POCKET_BALL_RADIUS
	);
	_pockets[5]= Pocket(
		vector(0, limits.minY - SIDE_POCKET_DEPTH),
		SIDE_POCKET_RADIUS,
		SIDE_POCKET_BALL_RADIUS
	);
	Pocket & topLeftPocket = _pockets[0];
	Pocket & topRightPocket = _pockets[1];
	Pocket & bottomRightPocket = _pockets[2];
	Pocket & bottomLeftPocket = _pockets[3];
	Pocket & topSidePocket = _pockets[4];
	Pocket & bottomSidePocket = _pockets[5];

	// vector bottomLeft = vector(-TABLE_WIDTH / 2, -TABLE_HEIGHT / 2, CUSHION_Z);
	// vector bottomRight = vector(TABLE_WIDTH / 2, -TABLE_HEIGHT / 2, CUSHION_Z);
	// vector topLeft = vector(-TABLE_WIDTH / 2, TABLE_HEIGHT / 2, CUSHION_Z);
	// vector topRight = vector(TABLE_WIDTH / 2, TABLE_HEIGHT / 2, CUSHION_Z);

	CushionSegment segBottom = CushionSegment(
		CUSHION_SIDE_2_ID,
		vector(-TABLE_WIDTH / 2 + CORN_SEG_OFFSET + SUB_SEG_OFFSET, -TABLE_HEIGHT / 2, CUSHION_Z),
		vector(TABLE_WIDTH / 2 - CORN_SEG_OFFSET - SUB_SEG_OFFSET, -TABLE_HEIGHT / 2, CUSHION_Z),
		vector(1, 0, 0),
		new BasicSegment(
			vector(-MIDDLE_SEG_OFFSET - SUB_SEG_OFFSET, -TABLE_HEIGHT / 2, CUSHION_Z),
			vector(MIDDLE_SEG_OFFSET + SUB_SEG_OFFSET, -TABLE_HEIGHT / 2, CUSHION_Z),
			vector(1, 0, 0)
		)
	);

	CushionSegment segRight = CushionSegment(
		CUSHION_FOOT_ID,
		vector(TABLE_WIDTH / 2, -TABLE_HEIGHT / 2 + CORN_SEG_OFFSET + SUB_SEG_OFFSET, CUSHION_Z),
		vector(TABLE_WIDTH / 2, TABLE_HEIGHT / 2 - CORN_SEG_OFFSET - SUB_SEG_OFFSET, CUSHION_Z),
		vector(0, 1, 0)
	);

	CushionSegment segTop = CushionSegment(
		CUSHION_SIDE_1_ID,
		vector(TABLE_WIDTH / 2 - CORN_SEG_OFFSET - SUB_SEG_OFFSET, TABLE_HEIGHT / 2, CUSHION_Z),
		vector(-TABLE_WIDTH / 2 + CORN_SEG_OFFSET + SUB_SEG_OFFSET, TABLE_HEIGHT / 2, CUSHION_Z),
		vector(-1, 0, 0),
		new BasicSegment(
			vector(MIDDLE_SEG_OFFSET + SUB_SEG_OFFSET, TABLE_HEIGHT / 2, CUSHION_Z),
			vector(-MIDDLE_SEG_OFFSET - SUB_SEG_OFFSET, TABLE_HEIGHT / 2, CUSHION_Z),
			vector(-1, 0, 0)
		)
	);

	CushionSegment segLeft = CushionSegment(
		CUSHION_HEAD_ID,
		vector(-TABLE_WIDTH / 2, TABLE_HEIGHT / 2 - CORN_SEG_OFFSET - SUB_SEG_OFFSET, CUSHION_Z),
		vector(-TABLE_WIDTH / 2, -TABLE_HEIGHT / 2 + CORN_SEG_OFFSET + SUB_SEG_OFFSET, CUSHION_Z),
		vector(0, -1, 0)
	);

	std::vector<CushionSegment> &segments = _cushionSegments;
	std::vector<CushionPoint> &points = _cushionPoints;

	segments.push_back(segBottom);
	segments.push_back(segRight);
	segments.push_back(segTop);
	segments.push_back(segLeft);

	for (int i = 0; i < segments.size(); i++) {
		CushionSegment & seg = segments.at(i);
		points.push_back(CushionPoint(seg.start, seg.id));
		points.push_back(CushionPoint(seg.end, seg.id));
		if (seg.exclude != NULL) {
			points.push_back(CushionPoint(seg.exclude->start, seg.id));
			points.push_back(CushionPoint(seg.exclude->end, seg.id));
		}
	}

	// Cushion custom segments
	double CornerRadiusSqrt2 = CORNER_POCKET_RADIUS / sqrt(2);
	CushionSegment segTopLeftCorner_1 = CushionSegment(
		CUSHION_HEAD_ID,
		vector::add(topLeftPocket.position,
			vector(-CornerRadiusSqrt2, -CornerRadiusSqrt2, CUSHION_Z)
		),
		vector(
			segLeft.start.x,
			segLeft.start.y + SUB_SEG_OFFSET,
			segLeft.start.z
		)
	);
	segTopLeftCorner_1.collapseEndPoint(SUB_SEG_OFFSET);

	CushionSegment segTopLeftCorner_2 = CushionSegment(
		CUSHION_SIDE_1_ID,
		vector(segTop.end.x - SUB_SEG_OFFSET, segTop.end.y, segTop.end.z),
		vector::add(topLeftPocket.position,
			vector(CornerRadiusSqrt2, CornerRadiusSqrt2, CUSHION_Z)
		)
	);
	segTopLeftCorner_2.collapseStartPoint(SUB_SEG_OFFSET);

	CushionSegment segTopRightCorner_1 = CushionSegment(
		CUSHION_SIDE_1_ID,
		vector::add(topRightPocket.position,
			vector(-CornerRadiusSqrt2, CornerRadiusSqrt2, CUSHION_Z)
		),
		vector(segTop.start.x + SUB_SEG_OFFSET, segTop.start.y, segTop.start.z)
	);
	segTopRightCorner_1.collapseEndPoint(SUB_SEG_OFFSET);

	CushionSegment segTopRightCorner_2 = CushionSegment(
		CUSHION_FOOT_ID,
		vector(
			segRight.end.x,
			segRight.end.y + SUB_SEG_OFFSET,
			segRight.end.z
		),
		vector::add(topRightPocket.position,
			vector(CornerRadiusSqrt2, -CornerRadiusSqrt2, CUSHION_Z)
		)
	);
	segTopRightCorner_2.collapseStartPoint(SUB_SEG_OFFSET);

	CushionSegment segBottomRightCorner_1 = CushionSegment(
		CUSHION_FOOT_ID,
		vector::add(bottomRightPocket.position,
			vector(CornerRadiusSqrt2, CornerRadiusSqrt2, CUSHION_Z)
		),
		vector(
			segRight.start.x,
			segRight.start.y - SUB_SEG_OFFSET,
			segRight.start.z
		)
	);
	segBottomRightCorner_1.collapseEndPoint(SUB_SEG_OFFSET);

	CushionSegment segBottomRightCorner_2 = CushionSegment(
		CUSHION_SIDE_2_ID,
		vector(
			segBottom.end.x + SUB_SEG_OFFSET,
			segBottom.end.y,
			segBottom.end.z
		),
		vector::add(bottomRightPocket.position,
			vector(-CornerRadiusSqrt2, -CornerRadiusSqrt2, CUSHION_Z)
		)
	);
	segBottomRightCorner_2.collapseStartPoint(SUB_SEG_OFFSET);

	CushionSegment segBottomLeftCorner_1 = CushionSegment(
		CUSHION_SIDE_2_ID,
		vector::add(
			bottomLeftPocket.position,
			vector(CornerRadiusSqrt2, -CornerRadiusSqrt2, CUSHION_Z)
		),
		vector(
			segBottom.start.x - SUB_SEG_OFFSET,
			segBottom.start.y,
			segBottom.start.z
		)
	);
	segBottomLeftCorner_1.collapseEndPoint(SUB_SEG_OFFSET);

	CushionSegment segBottomLeftCorner_2 = CushionSegment(
		CUSHION_HEAD_ID,
		vector(
			segLeft.end.x,
			segLeft.end.y - SUB_SEG_OFFSET,
			segLeft.end.z
		),
		vector::add(bottomLeftPocket.position,
			vector(-CornerRadiusSqrt2, CornerRadiusSqrt2, CUSHION_Z)
		)
	);
	segBottomLeftCorner_2.collapseStartPoint(SUB_SEG_OFFSET);

	CushionSegment segMidTop_1 = CushionSegment(
		CUSHION_SIDE_1_ID,
		vector::add(topSidePocket.position,
			vector(-SIDE_POCKET_RADIUS, 0, CUSHION_Z)),
		vector(
			segTop.exclude->end.x + SUB_SEG_OFFSET,
			segTop.exclude->end.y,
			segTop.exclude->end.z
		)
	);
	segMidTop_1.collapseEndPoint(SUB_SEG_OFFSET);

	CushionSegment segMidTop_2 = CushionSegment(
		CUSHION_SIDE_1_ID,
		vector(
			segTop.exclude->start.x - SUB_SEG_OFFSET,
			segTop.exclude->start.y,
			segTop.exclude->start.z
		),
		vector::add(topSidePocket.position,
			vector(SIDE_POCKET_RADIUS, 0, CUSHION_Z))
	);
	segMidTop_2.collapseStartPoint(SUB_SEG_OFFSET);

	CushionSegment segMidBottom_1 = CushionSegment(
		CUSHION_SIDE_2_ID,
		vector(
			segBottom.exclude->start.x + SUB_SEG_OFFSET,
			segBottom.exclude->start.y,
			segBottom.exclude->start.z
		),
		vector::add(bottomSidePocket.position,
			vector(-SIDE_POCKET_RADIUS, 0, CUSHION_Z))
	);

	CushionSegment segMidBottom_2 = CushionSegment(
		CUSHION_SIDE_2_ID,
		vector::add(bottomSidePocket.position,
			vector(SIDE_POCKET_RADIUS, 0, CUSHION_Z)),
		vector(
			segBottom.exclude->end.x - SUB_SEG_OFFSET,
			segBottom.exclude->end.y,
			segBottom.exclude->end.z
		)
	);
	segMidBottom_2.collapseEndPoint(SUB_SEG_OFFSET);

	segments.push_back(segTopLeftCorner_1);
	segments.push_back(segTopLeftCorner_2);
	segments.push_back(segTopRightCorner_1);
	segments.push_back(segTopRightCorner_2);
	segments.push_back(segBottomRightCorner_1);
	segments.push_back(segBottomRightCorner_2);
	segments.push_back(segBottomLeftCorner_1);
	segments.push_back(segBottomLeftCorner_2);
	segments.push_back(segMidTop_1);
	segments.push_back(segMidTop_2);
	segments.push_back(segMidBottom_1);
	segments.push_back(segMidBottom_2);

	// Sub-segments
	CushionSegment subSegTopLeft_1 = CushionSegment(
		segLeft.id,
		segTopLeftCorner_1.end,
		segLeft.start
	);

	CushionSegment subSegTopLeft_2 = CushionSegment(
		segTop.id,
		segTop.end,
		segTopLeftCorner_2.start
	);

	CushionSegment subSegTopRight_1 = CushionSegment(
		segTop.id,
		segTopRightCorner_1.end,
		segTop.start
	);

	CushionSegment subSegTopRight_2 = CushionSegment(
		segRight.id,
		segRight.end,
		segTopRightCorner_2.start
	);

	CushionSegment subSegBottomLeft_1 = CushionSegment(
		segBottom.id,
		segBottomLeftCorner_1.end,
		segBottom.start
	);

	CushionSegment subSegBottomLeft_2 = CushionSegment(
		segLeft.id,
		segLeft.end,
		segBottomLeftCorner_2.start
	);

	CushionSegment subSegBottomRight_1 = CushionSegment(
		segRight.id,
		segBottomRightCorner_1.end,
		segRight.start
	);

	CushionSegment subSegBottomRight_2 = CushionSegment(
		segBottom.id,
		segBottom.end,
		segBottomRightCorner_2.start
	);

	CushionSegment subSegMidTop_1 = CushionSegment(
		segTop.id,
		segMidTop_1.end,
		segTop.exclude->end
	);

	CushionSegment subSegMidTop_2 = CushionSegment(
		segTop.id,
		segTop.exclude->start,
		segMidTop_2.start
	);

	CushionSegment subSegMidBottom_1 = CushionSegment(
		segBottom.id,
		segBottom.exclude->start,
		segMidBottom_1.start
	);

	CushionSegment subSegMidBottom_2 = CushionSegment(
		segBottom.id,
		segMidBottom_2.end,
		segBottom.exclude->end
	);
	segments.push_back(subSegTopLeft_1);
	segments.push_back(subSegTopLeft_2);
	segments.push_back(subSegTopRight_1);
	segments.push_back(subSegTopRight_2);
	segments.push_back(subSegBottomLeft_1);
	segments.push_back(subSegBottomLeft_2);
	segments.push_back(subSegBottomRight_1);
	segments.push_back(subSegBottomRight_2);
	segments.push_back(subSegMidTop_1);
	segments.push_back(subSegMidTop_2);
	segments.push_back(subSegMidBottom_1);
	segments.push_back(subSegMidBottom_2);

	points.push_back(CushionPoint(subSegTopLeft_1.start, subSegTopLeft_1.id));
	points.push_back(CushionPoint(subSegTopLeft_2.end, subSegTopLeft_2.id));
	points.push_back(CushionPoint(subSegTopRight_1.start, subSegTopRight_1.id));
	points.push_back(CushionPoint(subSegTopRight_2.end, subSegTopRight_2.id));
	points.push_back(CushionPoint(subSegBottomLeft_1.start, subSegBottomLeft_1.id));
	points.push_back(CushionPoint(subSegBottomLeft_2.end, subSegBottomLeft_2.id));
	points.push_back(CushionPoint(subSegBottomRight_1.start, subSegBottomRight_1.id));
	points.push_back(CushionPoint(subSegBottomRight_2.end, subSegBottomRight_2.id));
	points.push_back(CushionPoint(subSegMidTop_1.start, subSegMidTop_1.id));
	points.push_back(CushionPoint(subSegMidTop_2.end, subSegMidTop_2.id));
	points.push_back(CushionPoint(subSegMidBottom_1.end, subSegMidBottom_1.id));
	points.push_back(CushionPoint(subSegMidBottom_2.start, subSegMidBottom_2.id));

	// Customize values for non-special segements
	for (int i = 0; i < segments.size(); ++i) {
		CushionSegment & cushion = segments.at(i);
		vector dir = cushion.dir;
		vector start = cushion.start;

		double dot = vector::dot(start, dir);

		cushion.delta = vector(
			start.x - dir.x * dot,
			start.y - dir.y * dot,
			start.z
		);

		// values for matrix M
		cushion.M = new M_Matrix(
			dir.x * dir.x - 1,
			dir.y * dir.y - 1,
			dir.x * dir.y
		);

		cushion.outNormal = vector::unit(vector::crossVectorWithZVec(cushion.dir, 1));
	}
}

ps::ExtMath::vector ps::StaticWorld::multiplyMatrixM(const vector & v, M_Matrix & M)
{
	return vector(
		M.dx2s1 * v.x + M.dxdy * v.y,
		M.dxdy * v.x + M.dy2s1 * v.y,
		-v.z
	);
}

ps::StaticWorld::StaticWorld(int maxBall)
{
	for (int i = 0; i < maxBall; i++) {
		_ballPositions.push_back(vector::ZERO);
		_ballEnabled.push_back(true);
	}
}

void ps::StaticWorld::setBallPosition(int id, const vector & position)
{
	if (id == 0) {
		StaticBall & cueBall = _cueBall;
		// Place ball and make it motionless
		cueBall._linearMotion.origin = position;
		cueBall._linearMotion.startTime = cueBall._angularMotion.startTime = -1;
		_ballPositions[0] = position;
	}
	else {
		vector & ballPosition = _ballPositions[id];
		ballPosition.x = position.x;
		ballPosition.y = position.y;
		ballPosition.z = position.z;
	}
}

void ps::StaticWorld::setBallEnabled(int id, bool enabled)
{
	_ballEnabled.at(id) = enabled;
}

std::vector<ps::StaticWorld::MotionGroup *> ps::StaticWorld::calcCueBallMotions(vector & linearVelocity, vector & angularVelocity)
{
	std::vector<StaticWorld::MotionGroup *> groups;

	StaticBall & ball = _cueBall;
	StaticMotion & linearMotion = ball._linearMotion;
	StaticMotion & angularMotion = ball._angularMotion;
	vector backupPosition = vector(linearMotion.origin);

	double time = 0;
	CollisionEvent *event = NULL;
	Velocities velocities = Velocities(linearVelocity, angularVelocity);

	// Main motions
	do {
		if (vector::equalsZero(velocities.linearVelocity) && vector::equalsZero(velocities.angularVelocity)) 
			ball.updateVelocities(time);
		else ball.updateVelocities(time, velocities.linearVelocity, velocities.angularVelocity);
		event = calcCueBallNextEvent(time, event);
		velocities = updateVelocitiesByEvent(event);

		time = event->time;

		if (event->type != TYPE_EVENT_STOPPED) {
			linearMotion.endTime = angularMotion.endTime = time;
			groups.push_back(new MotionGroup(linearMotion, angularMotion, time, event));
			CCLOG("event.time: %f", event->time);
		}
		
	} while (event->type > 0 && groups.size() < MAX_MOTIONS_LENGTH);

	// Addition motions
	if (event != NULL && (event->type == TYPE_EVENT_COLLIDE_EDGE)) {
		ball.updateVelocities(time, velocities.linearVelocity);
		event = calcCueBallNextEvent(time, event);
		velocities = updateVelocitiesByEvent(event);

		time = event->time;

		groups.push_back(new MotionGroup(linearMotion, angularMotion, time, event));
	}

	// Restore original cueBall position after calculating
	_cueBall._linearMotion.origin = backupPosition;
	_cueBall._linearMotion.startTime = _cueBall._angularMotion.startTime = -1;

	return groups;
}

ps::StaticWorld::Velocities ps::StaticWorld::updateVelocitiesByEvent(CollisionEvent *event)
{
	switch (event->type)
	{
	case TYPE_EVENT_COLLIDE_CUSHION:
		return calcCollideCushionVelocities((CushionEvent *)event);
	case TYPE_EVENT_COLLIDE_EDGE:
		return calcCollideEdgeVelocities((EdgeEvent *)event);
	case TYPE_EVENT_COLLIDE_BALL:
		return calcCollideBallVelocities((BallEvent *)event);
	case TYPE_EVENT_COLLIDE_CUSHION_POINT:
		return calcCollideCushionPointVelocities((CushionPointEvent *)event);
	default:
		return EmptyVelocities;
	}
}

ps::StaticWorld::Velocities ps::StaticWorld::calcCollideCushionVelocities(CushionEvent * event)
{
	StaticBall cueBall = _cueBall;
	double time = event->time;
	MotionResult motion = cueBall.calcAll(time);
	vector linearVelocity = motion.velocity;
	vector position = motion.position;
	vector angularVelocity = cueBall.calcAngularVelocity(time);

	CushionSegment cushion = _cushionSegments[event->cushionIndex];
	vector collisionPoint;

	if (cushion.dir.x == 0) {
		collisionPoint = vector(
			cushion.start.x,
			position.y,
			cushion.start.z
		);
	}
	else if (cushion.dir.y == 0) {
		collisionPoint = vector(
			position.x,
			cushion.start.y,
			cushion.start.z
		);
	}
	else {
		// collisionPoint = projectPointOnCushion(position, cushion);
		collisionPoint = event->contactPoint;
	}

	return resolveSegmentDirectionCollision(
		cushion.dir, collisionPoint, position, linearVelocity, angularVelocity
	);
}

ps::ExtMath::vector ps::StaticWorld::projectPointOnCushion(const vector & point, const CushionSegment & cushion)
{
	vector start = cushion.start;
	return vector::add(
		start,
		vector::project(
			vector::sub(point, start),
			vector::sub(cushion.end, start)
		)
	);
}

ps::StaticWorld::Velocities ps::StaticWorld::resolveSegmentDirectionCollision(vector & direction, vector & collisionPoint, vector & position, vector & linearVelocity, vector & angularVelocity)
{
	if (ExtMath::isZeroNumber(position.z)) {
		Velocities velocities = CollisionResolver::resolveCollisionWithStaticPoint(
			position, collisionPoint,
			linearVelocity, angularVelocity,
			PhysicsConstants::CUSHION_U, PhysicsConstants::CUSHION_E
		);

		velocities.linearVelocity.z = 0;

		return velocities;
	}
	else {
		return CollisionResolver::resolveCollisionWithStaticPoint(
			position, collisionPoint,
			linearVelocity, angularVelocity,
			PhysicsConstants::CUSHION_U, PhysicsConstants::CUSHION_E
		);
	}
}

ps::StaticWorld::Velocities ps::StaticWorld::calcCollideEdgeVelocities(EdgeEvent *event)
{
	vector linearVelocity = _cueBall.calcLinearVelocity(event->time);
	linearVelocity.z = abs(linearVelocity.z);

	return Velocities(linearVelocity, vector::ZERO);
}

ps::StaticWorld::Velocities ps::StaticWorld::calcCollideBallVelocities(BallEvent *event)
{
	StaticBall cueBall = _cueBall;
	vector targetPosition = _ballPositions[event->ballIndex];
	MotionResult motion = cueBall.calcAll(event->time);
	vector r = vector::sub(motion.position, targetPosition);
	vector v2 = vector::project(motion.velocity, r);

	return Velocities(vector::sub(motion.velocity, v2), vector::ZERO);
}

ps::StaticWorld::Velocities ps::StaticWorld::calcCollideCushionPointVelocities(CushionPointEvent *event)
{
	StaticBall cueBall = _cueBall;
	double time = event->time;
	MotionResult motion = cueBall.calcAll(time);
	vector linearVelocity = motion.velocity;
	vector position = motion.position;
	vector angularVelocity = cueBall.calcAngularVelocity(time);

	CushionPoint point = _cushionPoints[event->pointIndex];

	vector r = vector::sub(position, point.position);
	vector direction = vector::unit(vector::cross(r, UP));
	return resolveSegmentDirectionCollision(
		direction, point.position, position, linearVelocity, angularVelocity
	);
}

ps::StaticWorld::CollisionEvent *ps::StaticWorld::calcCueBallNextEvent(double time, CollisionEvent * prevEvent)
{
	StaticBall & ball = _cueBall;

	StaticMotion & linearMotion = ball._linearMotion;
	StaticMotion & angularMotion = ball._angularMotion;

	if (linearMotion.startTime == -1 && angularMotion.startTime == -1) {
		return new CollisionEvent(TYPE_EVENT_STOPPED);
	}

	MotionResult startMotion = ball._linearMotion.calcAll(0);

	CushionEvent *cushionCollision = calcCueBallCollideWithCushion(time, prevEvent, startMotion);
	CushionPointEvent *cushionPointCollision = calcCueBallCollideWithCushionPoint(time, prevEvent, startMotion);
	BallEvent *ballCollision = calcCueBallCollideWithBall(time, prevEvent, startMotion);
	EdgeEvent *edgeCollision = calcCueBallCollideWithEdge(time, prevEvent, startMotion);
	PocketZoneEvent *pocketZoneCollision = calcCueBallCollideWithPocketZone(time, prevEvent, startMotion);

	double minTime = Infinity;
	if (linearMotion.startTime != -1) {
		minTime = linearMotion.endTime;
	}
	if (angularMotion.startTime != -1 && minTime > angularMotion.endTime) {
		minTime = angularMotion.endTime;
	}

	CollisionEvent *event = NULL;

	if (cushionCollision != NULL && cushionCollision->time < minTime) {
		minTime = cushionCollision->time;
		event = cushionCollision;
	}

	if (ballCollision != NULL && ballCollision->time < minTime) {
		minTime = ballCollision->time;
		event = ballCollision;
	}

	if (edgeCollision != NULL && edgeCollision->time < minTime) {
		minTime = edgeCollision->time;
		event = edgeCollision;
	}

	if (pocketZoneCollision != NULL && pocketZoneCollision->time < minTime) {
		minTime = pocketZoneCollision->time;
		event = pocketZoneCollision;
	}

	if (cushionPointCollision != NULL && cushionPointCollision->time < minTime) {
		minTime = cushionPointCollision->time;
		event = cushionPointCollision;
	}

	if (event == NULL) {
		event = new CollisionEvent(TYPE_EVENT_MOTION_EXPIRED);
		if (checkCueBallOutside(_cueBall, minTime)) {
			event->type = TYPE_EVENT_OUTSIDE;
		}
		event->time = minTime;
	}

	return event;
}

ps::StaticWorld::CushionEvent * ps::StaticWorld::calcCueBallCollideWithCushion(double time, CollisionEvent *prevEvent, MotionResult & startMotion)
{
	StaticMotion linearMotion = _cueBall._linearMotion;

	double motionEndTime = linearMotion.endTime;
	vector p = startMotion.position;
	vector v = startMotion.velocity;
	vector a = linearMotion.acceleration;

	int i, j;
	vector dp, dv, da;
	double result;
	EquationResults results;
	double finalResult = Infinity;
	int cushionIndex = -1;
	vector contactPoint;
	vector currentContactPoint;
	vector collidedPosition;

	for (i = 0; i < _cushionSegments.size(); ++i) {
		CushionSegment cushion = _cushionSegments[i];

		if (cushion.dir.y == 0) {
			dp = vector(0, p.y - cushion.start.y, p.z - cushion.start.z);
			dv = vector(0, v.y, v.z);
			da = vector(0, a.y, a.z);
		}
		else if (cushion.dir.x == 0) {
			dp = vector(0, p.x - cushion.start.x, p.z - cushion.start.z);
			dv = vector(0, v.x, v.z);
			da = vector(0, a.x, a.z);
		}
		else {
			M_Matrix M = *cushion.M;
			vector delta = cushion.delta;

			dp = vector::add(multiplyMatrixM(p, M), delta);
			dv = multiplyMatrixM(v, M);
			da = multiplyMatrixM(a, M);
		}

		results = StaticMotion::resolveMotionEquation(BALL_RADIUS, dp, dv, da);

		for (j = 0; j < results.size; ++j) {
			result = results.arr[j];
			if (isGT(result, time) && result < motionEndTime && result < finalResult) {
				collidedPosition = linearMotion.calcPosition(result);
				currentContactPoint = projectPointOnCushion(collidedPosition, cushion);

				if (checkInsideCushion(currentContactPoint, cushion)) {
					if (prevEvent != NULL && prevEvent->type == TYPE_EVENT_COLLIDE_CUSHION &&
						((CushionEvent *)prevEvent)->cushionIndex == i
						) {
						if (!vectorWeakEquals(collidedPosition, ((CushionEvent *)prevEvent)->collidedPosition)) {
							finalResult = result;
							cushionIndex = i;
							contactPoint = currentContactPoint;
						}
					}
					else {
						finalResult = result;
						cushionIndex = i;
						contactPoint = currentContactPoint;
					}
				}
			}
		}
	}

	if (finalResult == Infinity) {
		return NULL;
	}

	finalResult = MAX(finalResult, time);

	CushionSegment seg = _cushionSegments[cushionIndex];
	segment cushionSeg = segment(seg.start, seg.end);

	collidedPosition = linearMotion.calcPosition(finalResult);
	double distance = segment::distance(collidedPosition, cushionSeg);

	if (distance < BALL_RADIUS) {
		double delta = distance - BALL_RADIUS;
		double velocityLength = vector::length(linearMotion.calcVelocity(finalResult));
		if (velocityLength > 0) {
			double deltaTime = delta / velocityLength;
			if (deltaTime < 0) {
				deltaTime *= 10e5;
				do {
					finalResult += deltaTime;
					collidedPosition = linearMotion.calcPosition(finalResult);
					distance = segment::distance(collidedPosition, cushionSeg);
					deltaTime *= 2;
				} while (distance < BALL_RADIUS && abs(deltaTime) < 1e-4);
			}
		}
	}

	CushionEvent *event = new CushionEvent();
	event->time = finalResult;
	event->cushionIndex = cushionIndex;
	event->collidedPosition = collidedPosition;
	event->contactPoint = contactPoint;
	event->cushionId = seg.id;

	return event;
}

ps::StaticWorld::CushionPointEvent * ps::StaticWorld::calcCueBallCollideWithCushionPoint(double time, CollisionEvent *prevEvent, MotionResult & startMotion)
{
	StaticMotion &linearMotion = _cueBall._linearMotion;

	double motionEndTime = linearMotion.endTime;
	vector &p = startMotion.position;
	vector &v = startMotion.velocity;
	vector &a = linearMotion.acceleration;

	CushionPoint point;
	vector dp;
	EquationResults results;
	double result;
	int i, j;
	double finalResult = Infinity;
	int pointIndex = -1;

	for (i = 0; i < _cushionPoints.size(); ++i) {
		point = _cushionPoints[i];

		dp = vector::sub(p, point.position);

		results = StaticMotion::resolveMotionEquation(BALL_RADIUS, dp, v, a);
		for (j = 0; j < results.size; ++j) {
			result = results.arr[j];

			if (isGT(result, time) && result < motionEndTime && result < finalResult) {
				finalResult = result;
				pointIndex = i;
			}
		}
	}

	if (finalResult == Infinity) {
		return NULL;
	}

	finalResult = deepResolvePosition(
		MAX(finalResult, time), linearMotion,
		_cushionPoints[pointIndex].position, BALL_RADIUS
	);

	point = _cushionPoints[pointIndex];

	CushionPointEvent *event = new CushionPointEvent();
	event->time = finalResult;
	event->pointIndex = pointIndex;
	event->cushionId = point.cushionId;

	return event;
}

ps::StaticWorld::BallEvent * ps::StaticWorld::calcCueBallCollideWithBall(double time, CollisionEvent *prevEvent, MotionResult & startMotion)
{
	StaticBall &cueBall = _cueBall;
	auto positions = _ballPositions;
	StaticMotion & linearMotion = cueBall._linearMotion;

	double motionEndTime = linearMotion.endTime;
	vector p = startMotion.position;
	vector v = startMotion.velocity;
	vector a = linearMotion.acceleration;

	vector dp;
	EquationResults results;
	int i, j, ballIndex;
	double result, finalResult;

	finalResult = Infinity;
	i = 1;
	ballIndex = -1;
	while (i < positions.size()) {
		if (
			(!_ballEnabled[i]) ||
			(prevEvent != NULL && prevEvent->type == TYPE_EVENT_COLLIDE_BALL && ((BallEvent *)prevEvent)->ballIndex == i)
			) {
			++i;
			continue;
		}
		dp = vector::sub(p, positions[i]);
		results = StaticMotion::resolveMotionEquation(BALL_DIAMETER, dp, v, a);
		j = 0;
		while (j < results.size) {
			result = results.arr[j++];
			if (isGT(result, time) && result < motionEndTime && result < finalResult) {
				finalResult = result;
				ballIndex = i;
			}
		}
		++i;
	}

	if (finalResult == Infinity) {
		return NULL;
	}

	finalResult = deepResolvePosition(finalResult, linearMotion, positions[ballIndex], BALL_DIAMETER);

	BallEvent *event = new BallEvent();
	event->time = finalResult;
	event->ballIndex = ballIndex;

	return event;
}

ps::StaticWorld::EdgeEvent * ps::StaticWorld::calcCueBallCollideWithEdge(double time, CollisionEvent *prevEvent, MotionResult & startMotion)
{
	StaticMotion linearMotion = _cueBall._linearMotion;
	double v = startMotion.velocity.z;
	double p = startMotion.position.z;
	double a = linearMotion.acceleration.z;

	if (ExtMath::isZeroNumber(a) && ExtMath::isZeroNumber(v)) {
		return NULL;
	}

	double t = Infinity;
	double vt = 0;

	if (ExtMath::isZeroNumber(a)) {
		t = (CUSHION_Z - p) / v;
	}
	else {
		double delta = v * v - 2 * a * (p - EDGE_Z);
		if (delta < 0) return NULL;
		double sqrt_delta = sqrt(delta);
		double t1 = (-v + sqrt_delta) / a;
		double t2 = (-v - sqrt_delta) / a;

		double vt_1 = v + a * t1;
		double vt_2 = v + a * t2;

		if (vt_1 < 0 && t1 > time && checkCollideWithEdgeAtTime(t1, linearMotion)) {
			t = t1;
			vt = vt_1;
		}
		if (vt_2 < 0 && t2 > time && t2 < t && checkCollideWithEdgeAtTime(t2, linearMotion)) {
			t = t2;
			vt = vt_2;
		}
	}

	if (t == Infinity) return NULL;

	double a_2 = 0.5 * a;
	double result = p + v * t + t * t * a_2;

	if (result < EDGE_Z) {
		double del = result - EDGE_Z;
		if (vt != 0) {
			double deltaTime = del / abs(vt);
			if (deltaTime < 0) {
				// TODO: Update fixing penetration parameters
				deltaTime *= 10e5;
				do {
					// if (deltaTime === -Infinity) break;
					t += deltaTime;
					result = p + v * t + t * t * a_2;
					del = result - EDGE_Z;
					deltaTime *= 2;
				} while (result < EDGE_Z && abs(deltaTime) < 1e-4);
			}
		}
	}

	EdgeEvent *event = new EdgeEvent();
	event->time = t;

	return event;
}

ps::StaticWorld::PocketZoneEvent * ps::StaticWorld::calcCueBallCollideWithPocketZone(double time, CollisionEvent *prevEvent, MotionResult & startMotion)
{
	StaticMotion linearMotion = _cueBall._linearMotion;
	vector p = startMotion.position;
	vector v = vector(startMotion.velocity);
	vector a = vector(linearMotion.acceleration);

	double motionEndTime = linearMotion.endTime;
	double finalResult = Infinity;

	int i = 0;
	double result;
	int j, pocketIndex;
	vector dp;
	Pocket pocket;
	EquationResults results;

	v.z = 0;
	a.z = 0;
	pocketIndex = 0;

	while (i < NUM_POCKET) {
		pocket = _pockets[i];
		dp = vector::sub(p, pocket.position);
		dp.z = 0;

		results = StaticMotion::resolveMotionEquation(pocket.totalRadius, dp, v, a);

		j = 0;
		while (j < results.size) {
			result = results.arr[j++];
			if (isGT(result, time) && result < motionEndTime && result < finalResult) {
				finalResult = result;
				pocketIndex = i;
			}
		}
		++i;
	}

	if (finalResult == Infinity) {
		return NULL;
	}

	finalResult = MAX(finalResult, time);
	pocket = _pockets[pocketIndex];

	finalResult = deepResolvePosition2D(
		finalResult, linearMotion, pocket.position, pocket.totalRadius
	);

	// TODO: Resolve deep results here

	PocketZoneEvent *event = new PocketZoneEvent();
	event->time = finalResult;
	event->pocketIndex = pocketIndex;

	return event;
}

void ps::StaticWorld::spawnMotionTrajectoryPoints(std::vector<std::list<vector>>& resultSegments, int& resultBallIdx,
	const Velocities & vels, const double & maxLengthA, const double & maxLengthE)
{
	double distance;
	vector start;
	vector end;
	vector delta;
	vector dir;

	MotionGroup motionGroup;
	MotionGroup collidedMG;
	bool hasCollideMG = false;
	CollisionEvent *event = NULL;
	double time = 0;

	vector lv = vels.linearVelocity;
	vector av = vels.angularVelocity;
	std::vector<MotionGroup> motionGroups;
	motionGroups.reserve(4);
	yieldCueBallMotionGroups(motionGroup, lv, av, event, time);
	calcCueBallMotionGroups(motionGroups, lv, av);
	vector lastPoint = motionGroup.linearMotion.origin;
	std::vector<std::list<vector>> &segments = resultSegments;
	segments.push_back(std::list<vector>());
	std::list<vector> &points = segments.back();
	points.push_back(lastPoint);
	bool loop = true;
	time = 0;
	double trajLen = 0;
	int ballIndex = -1;
	if (motionGroups.size() > 1)
	{
		int a = 1;
	}
	for (auto i = 0; i < motionGroups.size(); i++) {
		StaticMotion &motion = motionGroup.linearMotion;
		if (motion.endTime < time) {
			vector point = motion.calcPosition(motion.endTime);
			if (!vector::equals(lastPoint, point)) {
				distance = vector::distance(lastPoint, point);

				if ((trajLen + distance) >= maxLengthA) {
					distance = maxLengthA - trajLen;
					point = vector::getFixDistancePoint(
						lastPoint, point, distance
					);
					loop = false;
				}

				trajLen += distance;
				points.push_back(point);
				lastPoint = point;
			}
		}
		else {
			if (checkVectorsParallel(motion.acceleration, motion.velocity)) {
				// Straight motion
				start = motion.calcPosition(motion.startTime);
				end = motion.calcPosition(motion.endTime);
				delta = vector::sub(end, start);
				distance = vector::length(delta);
				dir = vector::multiply(1 / distance, delta);

				if ((distance + trajLen) >= maxLengthA) {
					distance = maxLengthA - trajLen;
					end = vector::add(
						start,
						vector::multiply(distance, dir)
					);
					loop = false;
				}

				auto count = floor(distance / BASIC_STEP_LENGTH);
				auto remain = distance - count * BASIC_STEP_LENGTH;
				vector point = vector::add(start, vector::multiply(remain, dir));
				auto step = vector::multiply(BASIC_STEP_LENGTH, dir);

				points.push_back(point);
				count = MIN(
					count,
					MAX_GENERATED_POINTS - points.size() + 1
				);

				for (auto i = 1; i < count; ++i) {
					point = vector::add(point, step);
					points.push_back(point);
				}

				trajLen += distance;
				if (points.size() < MAX_GENERATED_POINTS) {
					points.push_back(end);
					lastPoint = end;
				}

				time = motion.endTime;
			}
			else {
				while (
					time <= motion.endTime &&
					loop &&
					points.size() < MAX_GENERATED_POINTS) {
					auto pv = motion.calcAll(time);
					auto point = pv.position;

					distance = vector::distance(lastPoint, point);

					if ((trajLen + distance) >= maxLengthA) {
						distance = maxLengthA - trajLen;
						point = vector::getFixDistancePoint(
							lastPoint, point, distance
						);
						loop = false;
					}

					trajLen += distance;
					points.push_back(point);
					lastPoint = point;

					time += (MOTION_STEP / MAX(
						MIN_MOTION_VEL, vector::length(pv.velocity)
					));
				}
			}
		}

		if (loop) {
			if (!hasCollideMG) {
				auto evtType = motionGroup.event->type;
				if (evtType == TYPE_EVENT_COLLIDE_BALL ||
					evtType == TYPE_EVENT_COLLIDE_CUSHION ||
					evtType == TYPE_EVENT_COLLIDE_CUSHION_POINT ||
					evtType == TYPE_EVENT_COLLIDE_EDGE
					) {
					collidedMG = motionGroup;
					hasCollideMG = true;
					segments.push_back(std::list<vector>());
					std::list<vector> &points = segments.back();
				}
			}
		}
		
	}

	for (auto i = 0; i < segments.size(); ++i) {
		std::list<vector>& points = segments[i];
		for (auto ittr = points.begin(); ittr != points.end(); ittr ++) {
			ittr->z += BALL_RADIUS;
		}
	}

	if (hasCollideMG && collidedMG.event->type == TYPE_EVENT_COLLIDE_BALL) {
		calcMotioncReflectBall(segments, collidedMG, maxLengthE);
	}

	finalizeYieldCueBallMotionGroups();

	//if (hasCollideMG && (dynamic_cast<BallEvent *>(collidedMG.event))) {
	//	ballIndex = dynamic_cast<BallEvent *>(collidedMG.event)->ballIndex;
	//}

}

void ps::StaticWorld::calcCueBallMotionGroups(std::vector<MotionGroup> & motionGroups, const vector & linearVelocity, const vector & angularVelocity)
{
	StaticBall &cueBall = _cueBall;
	auto cueBallPosition = _ballPositions[0];

	double time = 0;
	CollisionEvent *event = NULL;
	Velocities velocities = Velocities(linearVelocity, angularVelocity);
	do {
		cueBall.updateVelocities(time, velocities.linearVelocity, velocities.angularVelocity);
		event = calcCueBallNextEvent(time, event);
		velocities = updateVelocitiesByEvent(event);
		time = event->time;

		if (event->type != TYPE_EVENT_STOPPED) {
			MotionGroup motionGroup = MotionGroup(
				cueBall._linearMotion,
				cueBall._angularMotion,
				time,
				event
			);
			motionGroup.linearMotion.endTime = motionGroup.angularMotion.endTime = time;
			motionGroups.push_back(motionGroup);
		}
	} while (event->type > 0 && motionGroups.size() < MAX_MOTIONS_LENGTH);

	if (event && event->type == TYPE_EVENT_COLLIDE_EDGE){
		cueBall.updateVelocities(time, velocities.linearVelocity, velocities.angularVelocity);
		event = calcCueBallNextEvent(time, event);
		velocities = updateVelocitiesByEvent(event);
		time = event->time;

		MotionGroup motionGroup = MotionGroup(
			cueBall._linearMotion,
			cueBall._angularMotion,
			time,
			event
		);
		motionGroup.linearMotion.endTime = motionGroup.angularMotion.endTime = time;
		motionGroups.push_back(motionGroup);
	}

	cueBall._linearMotion.origin = cueBallPosition;
	cueBall._linearMotion.startTime = cueBall._angularMotion.startTime = -1;
}

void ps::StaticWorld::yieldCueBallMotionGroups(MotionGroup & motionGroup, const vector & linearVelocity, const vector & angularVelocity, CollisionEvent *event, double & time)
{
	StaticBall &cueBall = _cueBall;
	auto cueBallPosition = _ballPositions[0];

	Velocities velocities = Velocities(linearVelocity, angularVelocity);
	do {
		cueBall.updateVelocities(time, velocities.linearVelocity, velocities.angularVelocity);
		event = calcCueBallNextEvent(time, event);
		velocities = updateVelocitiesByEvent(event);
		time = event->time;

		if (event->type != TYPE_EVENT_STOPPED) {
			motionGroup = MotionGroup(
				cueBall._linearMotion,
				cueBall._angularMotion,
				time,
				event
			);
			motionGroup.linearMotion.endTime = motionGroup.angularMotion.endTime = time;
			return;
		}
	} while (event->type > 0);

	if (event && event->type == TYPE_EVENT_COLLIDE_EDGE) {
		cueBall.updateVelocities(time, velocities.linearVelocity, velocities.angularVelocity);
		event = calcCueBallNextEvent(time, event);
		velocities = updateVelocitiesByEvent(event);
		time = event->time;

		motionGroup = MotionGroup(
			cueBall._linearMotion,
			cueBall._angularMotion,
			time,
			event
		);
		motionGroup.linearMotion.endTime = motionGroup.angularMotion.endTime = time;
		return;
	}

}

void ps::StaticWorld::finalizeYieldCueBallMotionGroups()
{
	StaticBall &cueBall = _cueBall;
	cueBall._linearMotion.origin = _ballPositions[0];
	cueBall._linearMotion.startTime = cueBall._angularMotion.startTime = -1;
}

void ps::StaticWorld::calcMotioncReflectBall(std::vector<std::list<vector>> & segments, MotionGroup motionGroup, double maxLengthE)
{
	BallEvent *event = dynamic_cast<BallEvent *>(motionGroup.event);
	auto linearMotion = motionGroup.linearMotion;

	MotionResult motion = linearMotion.calcAll(linearMotion.endTime);

	auto targetPosition = _ballPositions[event->ballIndex];
	vector dirVel = vector::unit(motion.velocity);

	std::vector<segment> reflectSegs;
	calcBasicReflectBall(
		reflectSegs, dirVel, motion.position,
		targetPosition, maxLengthE
	);
	std::vector<std::list<vector>> reflectPointSegs;
	spawnBasicTrajectoryPoints(reflectPointSegs, reflectSegs);
	for (auto i = 0; i < reflectPointSegs.size(); i++) {
		segments.push_back(reflectPointSegs[i]);
	}
}

void ps::StaticWorld::calcBasicReflectBall(std::vector<segment> &segments, vector & velDir, vector & collidedPosition, vector & targetPosition, double maxLength)
{
	auto r = vector::sub(targetPosition, collidedPosition);

	auto targetDir = vector::project(velDir, r);
	auto cueBallDir = vector::sub(velDir, targetDir);
	auto targetDirLength = vector::length(targetDir);
	auto cueBallDirLength = vector::length(cueBallDir);

	vector start, end;

	if (targetDirLength > DIR_LENGTH_THESHOLD) {
		start = vector(targetPosition);
		start.z += BALL_RADIUS;

		end = vector::add(
			start,
			vector::multiply(
				maxLength + BALL_RADIUS / targetDirLength,
				targetDir
			)
		);

		segments.push_back(segment(start, end));
	}

	if (cueBallDirLength > DIR_LENGTH_THESHOLD) {
		start = vector(collidedPosition);
		start.z += BALL_RADIUS;
		end = vector::add(
			start,
			vector::multiply(
				maxLength + BALL_RADIUS / cueBallDirLength,
				cueBallDir
			)
		);

		segments.push_back(segment(start, end));
	}
}

void ps::StaticWorld::calcBasicReflectCushion(std::vector<segment>& segments, vector& velDir, const vector& collidedPosition, const vector& segDir, double maxLength)
{
	vector prjDir = vector::project(velDir, segDir);
	vector subDir = vector::sub(velDir, prjDir);
	vector reflectDir = vector::sub(prjDir, subDir);

	vector start = vector(collidedPosition);
	start.z = BALL_RADIUS;
	vector end = vector::add(start, vector::multiply(maxLength, reflectDir));

	segment seg = segment(start, end);
	segments.push_back(seg);
}

void ps::StaticWorld::calcBasicReflectCushionPoint(std::vector<segment>& segments, CushionPointEvent *basic, const vector& dir, const vector& collidedPosition, const CushionPoint& point, double maxLength)
{
	vector r = vector::sub(collidedPosition, point.position);
	r.z = 0;

	vector prjDir = vector::project(dir, r);
	vector subDir = vector::sub(dir, prjDir);

	vector reflectDir = vector::sub(subDir, prjDir);

	vector start = vector(collidedPosition);
	start.z = BALL_RADIUS;
	vector end = vector::add(start, vector::multiply(maxLength, reflectDir));

	segments.push_back(segment(start, end));
}

void ps::StaticWorld::spawnBasicTrajectoryPoints(std::vector<std::list<vector>> & pointLists, std::vector<segment> &segments)
{
	// const segments = basicTrajectory.segments;

	for (auto i = 0; i < segments.size(); ++i) {
		auto seg = segments[i];

		vector delta = vector::sub(seg.e, seg.s);
		double len = vector::length(delta);
		vector dir = vector::multiply(1 / len, delta);

		auto step = vector::multiply(BASIC_STEP_LENGTH, dir);
		auto count = floor(len / BASIC_STEP_LENGTH);
		double remain = len - count * BASIC_STEP_LENGTH;

		vector point = vector::add(seg.s, vector::multiply(remain, dir));
		std::list<vector> points;
		for (auto j = 1; j < count; ++j) {
			point = vector::add(point, step);
			if (checkPositionOutside(point)) break;
			points.push_back(point);
		}

		if (!checkPositionOutside(seg.e)) {
			points.push_back(seg.e);
		}

		pointLists.push_back(points);
	}
}

void ps::StaticWorld::buildBasicTrajectorySegments(std::vector<segment>& retSegments, int &ballIndex, const vector& direction, const double& maxLengthA, const double& maxLengthE)
{
	auto dir = vector::unit(vector(direction.x, direction.y, 0));
	CollisionEvent* basic = calcBasicTrajectory(maxLengthA, dir);
	bool collided;
	if (basic) {
		vector start = vector(_ballPositions[0]);
		vector collidedPosition = vector::add(start, vector::multiply(basic->time, dir));
		vector end = collidedPosition;
		start.z = end.z = BALL_RADIUS;
		retSegments.push_back(segment(start, end));
		auto type = basic->type;

		if (type == TYPE_EVENT_COLLIDE_BALL) {
			BallEvent *collision = (BallEvent *)(basic);
			collided = true;
			ballIndex = collision->ballIndex;

			calcBasicReflectBall(
				retSegments, dir, collidedPosition,
				_ballPositions[collision->ballIndex], maxLengthE
			);
		}
		else if (type == TYPE_EVENT_COLLIDE_CUSHION) {
			CushionEvent *collision = (CushionEvent *)(basic);
			collided = true;

			calcBasicReflectCushion(
				retSegments, dir, collidedPosition,
				_cushionSegments[collision->cushionIndex].dir, maxLengthE
			);
		}
		else if (type == TYPE_EVENT_COLLIDE_CUSHION_POINT) {
			CushionPointEvent *collision = (CushionPointEvent *)(basic);
			collided = true;

			calcBasicReflectCushionPoint(
				retSegments, collision, dir, collidedPosition,
				_cushionPoints[collision->pointIndex], maxLengthE
			);
		}
		delete basic;
	}
	else {
		vector start = vector(_ballPositions[0]);
		start.z = BALL_RADIUS;
		vector end = vector::add(start, vector::multiply(maxLengthA, dir));

		retSegments.push_back(segment(start, end ));
	}
}

bool ps::StaticWorld::checkPositionOutside(const vector & position)
{
	bool ret = position.x > TABLE_PADDING_EXTEND_MAX_X ||
		position.x < TABLE_PADDING_EXTEND_MIN_X ||
		position.y > TABLE_PADDING_EXTEND_MAX_Y ||
		position.y < TABLE_PADDING_EXTEND_MIN_Y;
	return ret;
}

bool ps::StaticWorld::checkVectorsParallel(vector & v1, vector & v2)
{
	auto lengthValue = vector::length(v1) * vector::length(v2);
	if (lengthValue == 0) return true;
	auto cosValue = vector::dot(v1, v2) / lengthValue;
	auto aCosValue = cosValue < 0 ? -cosValue : cosValue;
	return ExtMath::isZeroNumber(1 - aCosValue);
}

ps::StaticWorld::BallEvent* ps::StaticWorld::calcBasicBallCollision(double maxLength, const vector& dir)
{
	auto balls = gameMgr->_table->getWorld()->allBalls();
	auto finalTime = Infinity;

	auto cueBallPos = _ballPositions[0];
	auto maxDist = maxLength + BALL_DIAMETER;
	int ballIndex = -1;
	for (auto i = 1; i < balls->size(); ++i) {
		if (balls->at(i)->isDisabled()) continue;

		auto position = _ballPositions[i];

		double dx = cueBallPos.x - position.x;
		double dy = cueBallPos.y - position.y;

		double distanceSquare = dx * dx + dy * dy;
		if (distanceSquare > maxDist * maxDist) continue;

		double b = dx * dir.x + dy * dir.y;
		double d = b * b - distanceSquare + BALL_DIAMETER_SQUARE;
		if (d < 0) continue;

		d = sqrt(d);

		double t = -b - d;

		if (t > 0 && t < maxLength && t < finalTime) {
			finalTime = t;
			ballIndex = i;
		}

		t = -b + d;
		if (t > 0 && t < maxLength && t < finalTime) {
			finalTime = t;
			ballIndex = i;
		}
	}

	if (finalTime == Infinity) return NULL;
	BallEvent *ballEvent = new BallEvent();
	ballEvent->time = finalTime;
	ballEvent->ballIndex = ballIndex;
	return ballEvent;
}

ps::StaticWorld::CollisionEvent * ps::StaticWorld::calcBasicTrajectory(double maxLength, const vector& dir)
{
	auto type = TYPE_EVENT_OUTSIDE;
	double time = Infinity;
	CollisionEvent *collision = NULL;

	auto ballCollision = calcBasicBallCollision(maxLength, dir);
	if (ballCollision) {
		time = ballCollision->time;
		type = TYPE_EVENT_COLLIDE_BALL;
		collision = ballCollision;
	}

	auto cushionCollision = calcBasicCushionCollision(maxLength, dir);
	if (cushionCollision && cushionCollision->time < time) {
		time = cushionCollision->time;
		type = TYPE_EVENT_COLLIDE_CUSHION;
		delete collision;
		collision = cushionCollision;
	}
	else CC_SAFE_DELETE(cushionCollision);
	

	auto pointCollision = calcBasicCushionPointCollision(maxLength, dir);
	if (pointCollision != NULL && pointCollision->time < time) {
		time = pointCollision->time;
		type = TYPE_EVENT_COLLIDE_CUSHION_POINT;
		delete collision;
		collision = pointCollision;
	} 
	else CC_SAFE_DELETE(pointCollision);
	

	auto pocketCollision = calcBasicPocketCollision(maxLength, dir);
	if (pocketCollision && pocketCollision->time < time) {
		time = pocketCollision->time;
		type = TYPE_EVENT_COLLIDE_POCKET_ZONE;
		delete collision;
		collision = pocketCollision;
	} 
	else CC_SAFE_DELETE(pocketCollision);

	if (time == Infinity) return NULL;

	return collision;
}

ps::StaticWorld::CushionEvent * ps::StaticWorld::calcBasicCushionCollision(double maxLength, const vector& dir)
{
	vector position = _ballPositions[0];
	double finalTime = Infinity;
	int cushionIndex = -1;

	vector dp, dv;
	for (auto i = 0; i < _cushionSegments.size(); ++i) {
		auto cushion = _cushionSegments[i];

		if (!basicAbleToCollideCushion(cushion, position, maxLength)) continue;

		if (cushion.dir.y == 0) {
			dp = vector(0,position.y - cushion.start.y, -cushion.start.z);
			dv = vector(0, dir.y, 0);
		}
		else if (cushion.dir.x == 0) {
			dp = vector(position.x - cushion.start.x, 0, -cushion.start.z);
			dv = vector(dir.x, 0, 0);
		}
		else {
			auto M = cushion.M;
			dp = vector::add(multiplyMatrixM(position, *M), cushion.delta);
			dv = multiplyMatrixM(dir, *M);
		}

		double a = vector::dot(dv, dv);
		double b = vector::dot(dp, dv);
		double c = vector::dot(dp, dp) - BALL_RADIUS_SQUARE;
		double d = b * b - a * c;

		if (d < 0) continue;

		d = sqrt(d);

		double t = (-b - d) / a;
		if (basicValidCusColResult(cushion, t, position, dir, maxLength, finalTime)) {
			finalTime = t;
			cushionIndex = i;
		}

		t = (-b + d) / a;
		if (basicValidCusColResult(cushion, t, position, dir, maxLength, finalTime)) {
			finalTime = t;
			cushionIndex = i;
		}
	}

	if (finalTime == Infinity) return NULL;

	auto cushionEvent = new CushionEvent();
	cushionEvent->time = finalTime;
	cushionEvent->cushionIndex = cushionIndex;
	return cushionEvent;
}

ps::StaticWorld::CushionPointEvent * ps::StaticWorld::calcBasicCushionPointCollision(double maxLength, const vector& dir)
{
	vector position = _ballPositions[0];
	

	double finalTime = Infinity;
	int pointIndex = -1;

	for (auto i = 0; i < _cushionPoints.size(); ++i) {
		auto point = _cushionPoints[i];

		if (!basicAbleToCollideCushionPoint(point, position, maxLength)) continue;

		vector p = vector::sub(position, point.position);
		double b = vector::dot(p, dir);
		double d = b * b - vector::dot(p, p) + BALL_RADIUS_SQUARE;

		if (d < 0) continue;

		d = sqrt(d);

		double t = -b - d;
		if (t > 0 && t < maxLength && t < finalTime) {
			finalTime = t;
			pointIndex = i;
		}

		t = -b + d;
		if (t > 0 && t < maxLength && t < finalTime) {
			finalTime = t;
			pointIndex = i;
		}
	}

	if (finalTime == Infinity) return NULL;
	CushionPointEvent *event = new CushionPointEvent();
	event->time = finalTime;
	event->pointIndex = pointIndex;
	return event;
}

ps::StaticWorld::PocketZoneEvent * ps::StaticWorld::calcBasicPocketCollision(double maxLength, const vector& dir)
{
	vector position = _ballPositions[0];
	int pocketIndex = -1;
	double finalTime = Infinity;
	for (auto i = 0; i < NUM_POCKET; ++i) {
		Pocket pocket = _pockets[i];

		if (!basicAbleToCollidePocket(pocket, position, maxLength)) continue;

		double dx = position.x - pocket.position.x;
		double dy = position.y - pocket.position.y;
		double b = dx * dir.x + dy * dir.y;
		double d = b * b - dx * dx - dy * dy + pocket.radius * pocket.radius;

		if (d < 0) continue;

		d = sqrt(d);

		double t = -b - d;
		if (t > 0 && t < maxLength && t < finalTime) {
			finalTime = t;
			pocketIndex = i;
		}

		t = -b + d;
		if (t > 0 && t < maxLength && t < finalTime) {
			finalTime = t;
			pocketIndex = i;
		}
	}

	if (finalTime == Infinity) return NULL;

	PocketZoneEvent *event = new PocketZoneEvent();
	event->time = finalTime;
	event->pocketIndex = pocketIndex;
	return event;
}

bool ps::StaticWorld::basicAbleToCollideCushion(const CushionSegment& cushion, const vector& position, double maxLength)
{
	vector pp = projectPointOnCushion(position, cushion);

	vector pps = vector::sub(pp, cushion.start);
	vector ppe = vector::sub(pp, cushion.end);

	double minDistSqr;
	vector vDelta;

	if (vector::dot(pps, ppe) >= 0) {
		vDelta = vector::sub(cushion.start, position);
		minDistSqr = vDelta.x * vDelta.x + vDelta.y * vDelta.y;
		vDelta = vector::sub(cushion.end, position);
		minDistSqr = MIN(
			minDistSqr,
			vDelta.x * vDelta.x + vDelta.y * vDelta.y
		);
	}
	else {
		vDelta = vector::sub(pp, position);
		minDistSqr = vDelta.x * vDelta.x + vDelta.y * vDelta.y;
	}

	double maxDist = CUSHION_DISTANCE_DELTA + maxLength;
	return minDistSqr <= (maxDist * maxDist);
}

bool ps::StaticWorld::basicValidCusColResult(const CushionSegment& cushion, double t, const vector& position, const vector& dir, double maxLength, double finalTime)
{
	vector stopPos, projPos, pps, ppe;

	if (t > 0 && t < maxLength && t < finalTime) {
		stopPos = vector::add(position, vector::multiply(t, dir));
		projPos = projectPointOnCushion(stopPos, cushion);
		pps = vector::sub(cushion.start, projPos);
		ppe = vector::sub(cushion.end, projPos);
		if (vector::dot(pps, ppe) < 0) {
			if (cushion.exclude) {
				pps = vector::sub(cushion.exclude->start, projPos);
				ppe = vector::sub(cushion.exclude->end, projPos);
				if (vector::dot(pps, ppe) < 0) {
					return false;
				}
			}
			return true;
		}
	}
	return false;
}

bool ps::StaticWorld::basicAbleToCollideCushionPoint(const CushionPoint& point, const vector& position, double maxLength)
{
	vector vDelta = vector::sub(point.position, position);
	double minDistSqr = vDelta.x * vDelta.x + vDelta.y * vDelta.y;

	double maxDist = CUSHION_DISTANCE_DELTA + maxLength;
	bool ret = (minDistSqr <= (maxDist * maxDist));

	return ret;
}

bool ps::StaticWorld::basicAbleToCollidePocket(const Pocket& pocket, const vector& position, double maxLength)
{
	double maxDist = maxLength + pocket.radius;
	double dx = position.x - pocket.position.x;
	double dy = position.y - pocket.position.y;
	double distSqr = dx * dx + dy * dy;

	bool ret= (distSqr <= (maxDist * maxDist));
	return ret;
}

double ps::StaticWorld::deepResolvePosition(double time, StaticMotion & linearMotion, vector & targetPosition, double targetDistance)
{
	vector position = linearMotion.calcPosition(time);
	double distance = vector::distance(position, targetPosition);

	if (distance >= targetDistance) return time;

	double delta = distance - targetDistance;
	double velocityLength = vector::length(linearMotion.calcVelocity(time));

	if (velocityLength > 0) {
		double deltaTime = delta / velocityLength;

		if (deltaTime < 0) {
			deltaTime *= 10e5;
			do {
				time += deltaTime;
				position = linearMotion.calcPosition(time);
				distance = vector::distance(position, targetPosition);
				deltaTime *= 2;
			} while (distance < targetDistance && abs(deltaTime) < 1e-4);
		}
	}

	return time;
}

double ps::StaticWorld::distance2D(vector & p1, vector & p2)
{
	double dx = p1.x - p2.x;
	double dy = p1.y - p2.y;
	return sqrt(dx * dx + dy * dy);
}

double ps::StaticWorld::length2D(vector & p)
{
	return sqrt(p.x * p.x + p.y * p.y);
}

double ps::StaticWorld::deepResolvePosition2D(double time, StaticMotion & linearMotion, vector & targetPosition, double targetDistance)
{
	vector position = linearMotion.calcPosition(time);
	double distance = distance2D(position, targetPosition);

	if (distance >= targetDistance) return time;

	double delta = distance - targetDistance;
    vector vel =linearMotion.calcVelocity(time);
	double velocityLength = length2D(vel);

	if (velocityLength > 0) {
		double deltaTime = delta / velocityLength;

		if (deltaTime < 0) {
			deltaTime *= 10e5;
			do {
				time += deltaTime;
				position = linearMotion.calcPosition(time);
				distance = distance2D(position, targetPosition);
				deltaTime *= 2;
			} while (distance < targetDistance && abs(deltaTime) < 1e-4);
		}
	}

	return time;
}

bool ps::StaticWorld::checkCollideWithEdgeAtTime(double time, StaticMotion & linearMotion)
{
	vector p = linearMotion.calcPosition(time);
	return !(
		(p.x > EDGE_MAX_X || p.x < EDGE_MIN_X || p.y > EDGE_MAX_Y || p.y < EDGE_MIN_Y) || // outside
		(p.x >= TABLE_MIN_X && p.x <= TABLE_MAX_X && p.y >= TABLE_MIN_Y && p.y <= TABLE_MAX_Y) // inside
		);
}

ps::StaticWorld::Pocket ps::StaticWorld::_pockets[NUM_POCKET];
std::vector<StaticWorld::CushionSegment> StaticWorld::_cushionSegments;
std::vector<StaticWorld::CushionPoint> StaticWorld::_cushionPoints;

const ps::ExtMath::vector ps::StaticWorld::UP = vector(0,0,1);

bool ps::StaticWorld::weakEqualsZero(const double & num)
{
	if (num == 0) return true;

	return -1e-5 < num && num < 1e-5;
}

bool ps::StaticWorld::vectorWeakEquals(const vector & v1, const vector & v2)
{
	int counter = 0;
	if (weakEqualsZero(v1.x - v2.x)) {
		counter = 1;
	}
	if (weakEqualsZero(v1.y - v2.y)) {
		++counter;
	}
	if (weakEqualsZero(v1.z - v2.z)) {
		++counter;
	}
	return counter > 1;
}

bool ps::StaticWorld::checkCueBallOutside(const StaticBall & cueBall, double time)
{
	StaticBall ball = cueBall;
	vector position = ball.calcPosition(time);
	return (
		position.x > TABLE_PADDING_MAX_X ||
		position.x < TABLE_PADDING_MIN_X ||
		position.y > TABLE_PADDING_MAX_Y ||
		position.y < TABLE_PADDING_MIN_Y
		);
}

bool ps::StaticWorld::checkInsideCushion(const vector & point, const CushionSegment & seg)
{
	if (point.x > CUSHION_MAX_X || point.x < CUSHION_MIN_X ||
		point.y > CUSHION_MAX_Y || point.y < CUSHION_MIN_Y) return false;

	BasicSegment * exclude = seg.exclude;
	vector v1 = vector::sub(seg.start, point);
	vector v2 = vector::sub(seg.end, point);

	bool ret = vector::dot(v1, v2) <= 0;

	if (!ret || exclude == NULL) return ret;

	v1 = vector::sub(exclude->start, point);
	v2 = vector::sub(exclude->end, point);

	return vector::dot(v1, v2) >= 0;
}

ps::StaticWorld::Velocities::Velocities(const vector & lVelocity, const vector & aVelocity)
	:linearVelocity(lVelocity), angularVelocity(aVelocity)
{
}


ps::StaticWorld::CollisionEvent::CollisionEvent(const int & t)
	:type(t)
{

}

ps::StaticWorld::CollisionEvent::~CollisionEvent()
{

}
ps::StaticWorld::CushionEvent::CushionEvent()
	:CollisionEvent(TYPE_EVENT_COLLIDE_CUSHION)
{

}

ps::StaticWorld::BallEvent::BallEvent()
	:CollisionEvent(TYPE_EVENT_COLLIDE_BALL)
{

}

ps::StaticWorld::EdgeEvent::EdgeEvent()
	:CollisionEvent(TYPE_EVENT_COLLIDE_EDGE)
{

}

ps::StaticWorld::CushionPointEvent::CushionPointEvent()
	:CollisionEvent(TYPE_EVENT_COLLIDE_CUSHION_POINT)
{

}

ps::StaticWorld::PocketZoneEvent::PocketZoneEvent()
	:CollisionEvent(TYPE_EVENT_COLLIDE_POCKET_ZONE)
{

}

ps::StaticWorld::MotionGroup::MotionGroup(const StaticMotion & lMotion, const StaticMotion & aMotion, double eTime, CollisionEvent * cEvent)
	:linearMotion(lMotion), angularMotion(aMotion), endTime(eTime), event(cEvent)
{
	
}

ps::StaticWorld::MotionGroup::MotionGroup()
	:endTime(0), event(NULL)
{

}

ps::StaticWorld::MotionGroup::~MotionGroup()
{
	//CC_SAFE_DELETE(event);
}

ps::StaticWorld::BasicSegment::BasicSegment(const vector & s, const vector & e)
	:start(s), end(e)
{
	dir = vector::unit(vector::sub(e, s));
}

void ps::StaticWorld::BasicSegment::collapseEndPoint(double delta)
{
	end = vector::add(
		start,
		vector::multiply(
			vector::distance(start, end) - delta,
			dir
		)
	);
}

void ps::StaticWorld::BasicSegment::collapseStartPoint(double delta)
{
	start = vector::add(
		end,
		vector::multiply(
			-vector::distance(start, end) + delta,
			dir
		)
	);
}

ps::StaticWorld::BasicSegment::BasicSegment(const vector & s, const vector & e, const vector & d)
	:start(s), end(e), dir(d)
{

}

ps::StaticWorld::M_Matrix::M_Matrix(double dx2s1, double dy2s1, double dxdy)
	:dx2s1(dx2s1), dy2s1(dy2s1), dxdy(dxdy)
{

}

ps::StaticWorld::CushionPoint::CushionPoint(const vector & pos, int cushionId)
	:position(pos), cushionId(cushionId)
{

}

ps::StaticWorld::CushionPoint::CushionPoint()
{

}

ps::StaticWorld::CushionSegment::CushionSegment(const vector &start, const vector & end)
	:BasicSegment(start, end), exclude(NULL)
{
}

ps::StaticWorld::CushionSegment::CushionSegment(int id, const vector & start, const vector & end)
	: BasicSegment(start, end), id(id), exclude(NULL)
{

}

ps::StaticWorld::CushionSegment::CushionSegment(int id, const vector & start, const vector & end, const vector & dir)
	: BasicSegment(start, end, dir), id(id), exclude(NULL)
{
}

ps::StaticWorld::CushionSegment::CushionSegment(const vector & start, const vector & end, const vector & dir, BasicSegment *exclude)
	:BasicSegment(start, end, dir), exclude(exclude)
{

}

ps::StaticWorld::CushionSegment::CushionSegment(const vector & start, const vector & end, const vector & dir)
	:BasicSegment(start, end, dir), exclude(NULL)
{
}

ps::StaticWorld::CushionSegment::CushionSegment(int id, const vector & start, const vector & end, const vector & dir, BasicSegment *exclude)
	: BasicSegment(start, end, dir), exclude(exclude), id(id)
{
}

ps::StaticWorld::Pocket::Pocket(const vector & position, double radius, double totalRadius)
	:position(position), radius(radius), totalRadius(totalRadius)
{
}
