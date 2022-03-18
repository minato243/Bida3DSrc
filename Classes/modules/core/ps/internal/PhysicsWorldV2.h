#ifndef PhysicsWorldV2_h_
#define PhysicsWorldV2_h_

#include "PhysicsConstants.hpp"
#include "ExtensionMath.hpp"
#include "PhysicsRandom.hpp"
#include "PhysicsEntities.hpp"
#include "FloorSurface.hpp"
#include "PhysicSimulateResultV2.h"
#include "PhysicsCue.hpp"
#include "../static/StaticMotionController.h"
#include "PhysicsDispatcher.hpp"

namespace ps {
	class PhysicsDispatcher;
	class PhysicsWorldV2
	{
	private:
		class CushionCollisions
		{
		public:
			std::list<CollisionCushionPoint> cushionPointCollisions;
			std::list<CollisionCushionSegment> cushionSegmentCollisions;
		};
	public:
		static const bool ENABLE_STATIC_WORLD = true;

		static const int CUSHION_HEAD_ID = PhysicsConstants::CUSHION_HEAD_ID;
		static const int CUSHION_FOOT_ID = PhysicsConstants::CUSHION_FOOT_ID;
		static const int CUSHION_SIDE_1_ID = PhysicsConstants::CUSHION_SIDE_1_ID;
		static const int CUSHION_SIDE_2_ID = PhysicsConstants::CUSHION_SIDE_2_ID;

	private:
		static const int RUN_TIMEOUT = 10000;
		static const double DEFAULT_FRAME_TIME;
		static const int MAX_BALL = 16; //15 bi mau + bi trang
		static const int MAX_CUE_SLOT = 4;
		static const int LIMIT_LOOP_PER_FRAME = 15;

		static double _width;
		static double _height;
		static AABB _limits;
		static AABB _nonCushionCollisionZone;
		static std::vector<CushionSegment> _cushionSegments;
		static std::vector<CushionPoint> _cushionPoints;
		static FloorSurface _floorSurface;
		static Pockets _pockets;
		//  static CueAttribute _defaultCueAttr;
		//  static PhysicsCue _defaultCue; // For compatibility

		static const double BALL_DIAMETER;
		static bool _initedStatic;

		int maxBall;
		int stepCount;
		int _id;
		std::vector<BallBody *> _balls;
		std::map<int, BallBody*> _ballMapping;
		BallBody *_cueBall;
		BallBody *_lagBall;
		double _frameTime;
		double _remainFrameTime;
		bool _run;
		PhysicSimulateResult *_simulateResult;
		
		double _timestamp;
		// protected BallOverBallHelper _ballOverBallHelper;
		bool _break;
		bool _turnLag;
		PhysicsRandom *_random;

		// Random for cue accuracy
		PhysicsRandom *_cueRandom;
		std::vector<PhysicsCue *> _cues;
		
		StaticWorld *_staticWorld;
		StaticMotionController *_staticMotionCtrl;

		PhysicsDispatcher *_dispatcher;
		std::vector<bool> _cacheMotionLess;

		bool _runStaticMotion;
		double _timeScale;
		std::string dataLog;
	public:
		long long _lastAccess;
		PhysicsCue *_cue;

	public:
		PhysicsWorldV2();
		PhysicsWorldV2(const int & id, const int & maxBall);
		~PhysicsWorldV2();

		void reset(const int & id);

		const int & id();

		void setRandomSeed(double seed);

		void setCueRandomSeed(const double & seed);

		double getRandomSeed();

		double getCurrentRandomSeed();

		double getCurrentCueRandomSeed();

		bool setCueAttributes(int slot, PhysicsCue::PhysicsCueAttributes & attrs);

		void setCueAttributes(PhysicsCue::PhysicsCueAttributes & attrs);

		PhysicsCue *getCue(int slot);

		BallBody *getBall(int ballId);

		void addBall(BallBody *ball, bool isCueBall);

		void addLagBall(BallBody *ball);

		void removeAllBalls();

		void setTurnLag(bool value);

		void onBallFallToHole(BallBody * ball);

		void onBallOutOfTable(BallBody * ball);

		PhysicSimulateResult *shoot(int ballId, double force, vector & direction, vector & offset);

		PhysicSimulateResult *shoot(BallBody *ball, PhysicsCue *cue, double force, vector & direction, vector & offset);

		PhysicSimulateResult *shoot(int ballId, PhysicsCue::ResultVelocities & velocities);

		PhysicSimulateResult *shoot(BallBody *ball, PhysicsCue::ResultVelocities & velocities);

		PhysicSimulateResult *runShootOnlyLogic(BallBody *ball, PhysicsCue::ResultVelocities & velocities);

		void shoot(double force);

		vector randomDirection(const vector & dir, CueInfo* cue);

		vector randomOffset(const vector & dir, CueInfo* cue);

		static FloorSurface * floorSurface();

		static Pockets *pockets();

		void setBreak(bool brk);

		bool checkValidShoot(double force, const vector & direction, const vector & offset);

		PhysicsDispatcher* getDispatcher();

		StaticWorld *getStaticWorld();

		std::vector<BallBody *> *allBalls();

		bool isRunning();

		void update(double dt);

		void resetStaticWorldBallPosition();

	protected:
		virtual void checkExtensionEvent();

	private:
		static void initStaticValues();

		static void updateLimits(double w, double h);

		static void updateStaticSegmentsAndPockets();

		void updateSimulateStepTime(const vector & initialVelocity);

		void step(double frameTime);

		void stepStaticMotion(double dt);

		void stepDifferentialMotion(double dt);

		void checkCollisionsWithCushions(CushionCollisions & cusionCollisions, std::vector<bool> & cacheMotionLess);

		inline bool checkCacheMotionLess(std::vector<bool> & cacheMotionLess, int i);

		bool isPosibleToBeColliding(BallBody *ball);

		void checkBallCollisions(std::vector<CollisionBall> & ballCollisions, std::vector<bool> & cacheMotionLess);

		void checkCollisionsWithFloor(std::vector<CollisionFloor> & floorCollisions, std::vector<bool> & cacheMotionLess);

		void checkCollisionsWithEdge(std::vector<CollisionFloor> & edgeCollisions, std::vector<bool> & cacheMotionLess);

		bool checkAABBOverlap(BallBody & ball_1, BallBody & ball_2);

		bool checkCollision(BallBody & ball_1, BallBody & ball_2);

		double calcPenetrationTimeBack(BallBody & ball_1, BallBody & ball_2);

		double calcFloorPenetrationTimeBack(BallBody & ball);

		double randomAngle();

		void resolveBallsCollision(BallBody & ball_1, BallBody & ball_2);

		void makeAllBallsCapture(std::vector<BallBodyCapture> & captures);

		void checkOutsideTableBalls();

		std::string log();

		void logAllBall(std::string &ret);

		void onWorldPause();

		void dispatch();
	};
}
#endif