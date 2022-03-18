#ifndef StaticWorld_h_
#define StaticWorld_h_

#include "../internal/ExtensionMath.hpp"
#include "StaticMotion.h"
#include "modules/core/ps/internal/PhysicsConstants.hpp"
#include "StaticBall.h"
#include "data/GameConstant.h"
#include <cocos2d.h>

namespace ps
{
	using namespace ExtMath;
	class StaticWorld
	{

	public:
		// Discontinue calculating events
		static const int TYPE_EVENT_OUTSIDE;
		static const int TYPE_EVENT_COLLIDE_BALL;
		static const int TYPE_EVENT_STOPPED;
		static const int TYPE_EVENT_COLLIDE_EDGE;
		static const int TYPE_EVENT_COLLIDE_POCKET_ZONE;

		// Continue calculating events
		static const int TYPE_EVENT_COLLIDE_CUSHION;
		static const int TYPE_EVENT_MOTION_EXPIRED;
		static const int TYPE_EVENT_COLLIDE_CUSHION_POINT;

	private:
		static const double SIDE_POCKET_WIDTH;
		static const double CORNER_POCKET_WIDTH;
		static const double CORNER_POCKET_RADIUS;
		static const double SIDE_POCKET_RADIUS;

		static const double MIDDLE_SEG_OFFSET;
		static const double CORN_SEG_OFFSET;
		static const double SIDE_POCKET_DEPTH;
		static const double CORNER_POCKET_DEPTH;

		static const double BALL_RADIUS;
		static const double TABLE_WIDTH;
		static const double TABLE_HEIGHT;
		static const double TABLE_MAX_X;
		static const double TABLE_MAX_Y;
		static const double TABLE_MIN_X;
		static const double TABLE_MIN_Y;

		static const double BOUNCE_CUSHION_COEF;
		static const double SPIN_CUSHION_FRICTION;
		static const double BALL_RADIUS_SQUARE;
		static const double BALL_DIAMETER;
		static const double BALL_DIAMETER_SQUARE;

		static const double CUSHION_HEIGHT;
		static const double CUSHION_Z;

		static const double MOTION_STEP;
		static const int MAX_GENERATED_POINTS = 150;
		static const double MIN_MOTION_VEL;

		static const double EDGE_Z;

		static const double EDGE_PADDING;
		static const double EDGE_MAX_X;
		static const double EDGE_MAX_Y;
		static const double EDGE_MIN_X;
		static const double EDGE_MIN_Y;

		static const double SIDE_POCKET_BALL_RADIUS;
		static const double CORNER_POCKET_BALL_RADIUS;

		static const int MAX_MOTIONS_LENGTH = 50;
		


	public:
		static const int CUSHION_HEAD_ID = PhysicsConstants::CUSHION_HEAD_ID;
		static const int CUSHION_FOOT_ID = PhysicsConstants::CUSHION_FOOT_ID;
		static const int CUSHION_SIDE_1_ID = PhysicsConstants::CUSHION_SIDE_1_ID;
		static const int CUSHION_SIDE_2_ID = PhysicsConstants::CUSHION_SIDE_2_ID;

	public:
		class CollisionEvent
		{
		public:
			double time;
			int type;

			CollisionEvent(const int & type);
			virtual ~CollisionEvent();
		};

		class CushionEvent
			:public CollisionEvent
		{
		public:
			int cushionIndex;
			vector contactPoint;
			vector collidedPosition;
			int cushionId;

			CushionEvent();
		};

		class BallEvent
			:public CollisionEvent
		{
		public:
			int ballIndex;
			BallEvent();

		};

		class EdgeEvent
			:public CollisionEvent
		{
		public:
			EdgeEvent();
		};

		class CushionPointEvent
			:public CollisionEvent
		{
		public:
			int pointIndex;
			int cushionId;

			CushionPointEvent();
		};

		class PocketZoneEvent
			:public CollisionEvent
		{
		public:
			int pocketIndex;

			PocketZoneEvent();
		};

		class Velocities
		{
		public:
			vector linearVelocity;
			vector angularVelocity;

			Velocities(const vector & linearVelocity, const vector & angularVelocity);
		};

		class MotionGroup
		{
		public:
			StaticMotion linearMotion;
			StaticMotion angularMotion;
			CollisionEvent *event;
			double endTime;

			MotionGroup();
			MotionGroup(const StaticMotion & linearMotion, const StaticMotion & angularMotion,
				double endTime, CollisionEvent * event);

			~MotionGroup();
		};

		class BasicSegment
		{
		public:
			vector start;
			vector end;
			vector dir;

		public:
			BasicSegment(const vector & s, const vector & e);

			BasicSegment(const vector & s, const vector & e, const vector & dir);

			virtual ~BasicSegment() {};
		};

		class M_Matrix
		{
		public:
			double dx2s1;
			double dy2s1;
			double dxdy;

		public:
			M_Matrix(double dx2s1, double dy2s1, double dxdy);
		};

		class CushionPoint
		{
		public:
			vector position;
			int cushionId;

		public:
			CushionPoint();
			CushionPoint(const vector & pos, int cushionId);
		};

		class CushionSegment
			:public BasicSegment
		{
		public:
			int id;
			BasicSegment *exclude;
			vector delta;
			M_Matrix *M;
			vector outNormal;

		public:
			CushionSegment(const vector &start, const vector & end);

			CushionSegment(const vector & start, const vector & end, const vector & dir);

			CushionSegment(const vector & start, const vector & end, const vector & dir, BasicSegment *exclude);

		};

		class Pocket
		{
		public:
			vector position;
			double radius;
			double totalRadius;

		public:
			Pocket() {};
			Pocket(const vector & position, double radius, double totalRadius);
		};

	private:
		static const double CUSHION_PENETRATION;
		static const double CUSHION_MAX_X;
		static const double CUSHION_MAX_Y;
		static const double CUSHION_MIN_X;
		static const double CUSHION_MIN_Y;

		static const double TABLE_PADDING;
		static const double TABLE_PADDING_MAX_X;
		static const double TABLE_PADDING_MIN_X;
		static const double TABLE_PADDING_MAX_Y;
		static const double TABLE_PADDING_MIN_Y;

		static const double TABLE_PADDING_EXTEND_MAX_X;
		static const double TABLE_PADDING_EXTEND_MIN_X;
		static const double TABLE_PADDING_EXTEND_MAX_Y;
		static const double TABLE_PADDING_EXTEND_MIN_Y;

		static const double BASIC_STEP_LENGTH;

		static const double DIR_LENGTH_THESHOLD;
		
		static const double DELTA_CUSHION_BALL;
		static const double CUSHION_DISTANCE_DELTA;

	public:
		static Pocket _pockets[NUM_POCKET];
		static std::vector<CushionSegment> _cushionSegments;
		static std::vector<CushionPoint> _cushionPoints;

		static const vector UP;

		static const Velocities EmptyVelocities;
		
		static bool isGT(const double & a, const double & b);
		
		static bool isLT(const double & a, const double & b);

		static vector vecFlipY(const vector & v);

		static void initCushionAndPockets();

		static vector multiplyMatrixM(const vector & v, M_Matrix & M);

	private:
		StaticBall _cueBall;
		std::vector<vector> _ballPositions;
		std::vector<bool> _ballEnabled;

	public:
		StaticWorld(int maxBall);

		void setBallPosition(int id, const vector & position);

		void setBallEnabled(int id, bool enabled);

		std::vector<MotionGroup *> calcCueBallMotions(vector & linearVelocity, vector & angularVelocity);

		void spawnMotionTrajectoryPoints(std::vector<std::list<ExtMath::vector>>& resultSegments, 
			int& resultBallIdx, const Velocities & vels, const double & maxLengthA, const double & maxLengthE);

		void spawnBasicTrajectoryPoints(std::vector<std::list<vector>> & pointLists, std::vector<segment> &segments);

		void buildBasicTrajectorySegments(std::vector<segment>& retSegments, int &ballIndex, const vector& dir, const double& maxLengthA, const double& maxLengthE);

	private:
		Velocities updateVelocitiesByEvent(CollisionEvent *event);

		Velocities calcCollideCushionVelocities(CushionEvent *event);

		static vector projectPointOnCushion(const vector & point, const CushionSegment & cushion);

		Velocities resolveSegmentDirectionCollision(vector & direction, vector & collisionPoint, vector & position,
			vector & linearVelocity, vector & angularVelocity);

		Velocities calcCollideEdgeVelocities(EdgeEvent *event);

		Velocities calcCollideBallVelocities(BallEvent *event);

		Velocities calcCollideCushionPointVelocities(CushionPointEvent *event);

		CollisionEvent *calcCueBallNextEvent(double time, CollisionEvent *prevEvent);

		CushionEvent *calcCueBallCollideWithCushion(double time, CollisionEvent *prevEvent, MotionResult & startMotion);

		CushionPointEvent *calcCueBallCollideWithCushionPoint(double time, CollisionEvent *prevEvent, MotionResult & startMotion);

		BallEvent *calcCueBallCollideWithBall(double time, CollisionEvent *prevEvent, MotionResult & startMotion);

		EdgeEvent *calcCueBallCollideWithEdge(double time, CollisionEvent *prevEvent, MotionResult & startMotion);

		PocketZoneEvent *calcCueBallCollideWithPocketZone(double time, CollisionEvent *prevEvent, MotionResult & startMotion);

		void calcCueBallMotionGroups(std::vector<MotionGroup> & motionGroups, const vector & linearVelocity, const vector & angularVelocity);

		void yieldCueBallMotionGroups(MotionGroup & motionGroup, const vector & linearVelocity, const vector & angularVelocity, CollisionEvent *event, double &time);

		void finalizeYieldCueBallMotionGroups();

		void calcMotioncReflectBall(std::vector<std::list<vector>> & segments, MotionGroup motionGroup, double maxLengthE);

		void calcBasicReflectBall(std::vector<segment> &segments, vector & velDir, vector & collidePosition, vector & targetPosition, double maxLength);

		void calcBasicReflectCushion(std::vector<segment>& segments, vector& velDir, const vector& collidedPosition, const vector& segDir, double maxLength);

		void calcBasicReflectCushionPoint(std::vector<segment>& segments, CushionPointEvent *basic, const vector& dir, const vector& collidedPosition, const CushionPoint& point, double maxLength);

		bool checkPositionOutside(const vector & point);

		bool checkVectorsParallel(vector & v1, vector & v2);

		BallEvent *calcBasicBallCollision(double maxLength, const vector& dir);

		CollisionEvent *calcBasicTrajectory(double maxLength, const vector& dir);

		CushionEvent *calcBasicCushionCollision(double maxLength, const vector& dir);

		CushionPointEvent *calcBasicCushionPointCollision(double maxLength, const vector& dir);

		PocketZoneEvent *calcBasicPocketCollision(double maxLength, const vector& dir);

		bool basicAbleToCollideCushion(const CushionSegment& cushion, const vector& position, double maxLength);

		bool basicValidCusColResult(const CushionSegment& cushion, double t, const vector& position, const vector& dir, double maxLength, double finalTime);

		bool basicAbleToCollideCushionPoint(const CushionPoint& point, const vector& position, double maxLength);
		
		bool basicAbleToCollidePocket(const Pocket& pocket, const vector& position, double maxLength);
	private:

		static double deepResolvePosition(
			double time, StaticMotion & linearMotion, vector & targetPosition, double targetDistance);

		static double distance2D(vector & p1, vector & p2);

		static double length2D(vector & p);

		static double deepResolvePosition2D(double time, StaticMotion & linearMotion, vector & targetPosition, double targetDistance);

		static bool checkCollideWithEdgeAtTime(double time, StaticMotion & linearMotion);

		bool weakEqualsZero(const double & num);

		bool vectorWeakEquals(const vector & v1, const vector & v2);

		static bool checkCueBallOutside(const StaticBall & cueBall, double time);

		static bool checkInsideCushion(const vector & point, const CushionSegment & seg);
	};
}

#endif
