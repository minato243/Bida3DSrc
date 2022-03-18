#ifndef __BALL_BODY__
#define __BALL_BODY__

#include "ExtensionMath.hpp"
#include <functional>

namespace ps {
	using namespace ExtMath;

	class PhysicsWorldV2;

	class BallBodyCapture
	{
	public:
		int id;
		vector position;
		bool disabled;
		bool fallingInPocket;
	};

	class BallBody {
	public:
		BallBody();

		void setId(int id);

		int id();

		void setWorld(PhysicsWorldV2 * world);

		void outOfTable();

		void fallToPocket();

		void reset();

		void resetShootingState();

		void setPosition(const vector & position);

		void setQuaternion(const quaternion & quat);

		void setMotionStatus(const vector & position, const quaternion & quat);

		void backupPosition();

		void restorePosition();
		
		bool step(double dt);

		void setPureRolling(bool pureRolling);

		bool checkDisabled();

		void updateFlyingVelocity(double dt);

		void updatePureRollingVelocity(double dt);

		void updateNonPureRollingVelocity(double dt);

		void updateSpinAngular(double dt);

		void updateVelocityWithGravity(double dt);

		void updateVelocity(double dt);

		void updateQuaternion(double dt);

		bool isMotionless();

		void bounceFloor(const vector & normalAcceleration);

		void bounceEdge(const vector & normalAcceleration);

		bool isPureRolling();

		void finalizeStep(double dt);

		void bounceSegment(const segment & seg);

		void bounceIntersectSegmentPoint(const vector & p);

		void bounceSegmentDirection(const vector & segmentDirection, const vector & collisionPoint);

		void bounceSegmentDirectionOnFloor(const vector & linearVelocity, const vector & segDir);

		void bounceSegmentDirectionOnSky(const vector & linearVelocity, const vector & rVec);

		double calcPenetrationSegmentTimeBack(const segment & seg);

		double calcPenetrationPointTimeBack(const vector & point);

		void syncPureRollingAngularVelocity();

		bool isDisabled();

		vector & position();

		vector & linearVelocity();

		vector & angularVelocity();

		quaternion & quaternionRotation();

		void setLinearVelocity(const vector & value);

		void setAngularVelocity(const vector & value);

		bool isFallingInPocket();

		void enable();

		void disable();

		bool isInHole();

		void dispatch();

		void setSubcribe(std::function<void()> subcribe);

		bool isTotallyInHole();

		bool isDisable();

		void cheatTotalInHole(bool totalInHole);

		void makeCapture(BallBodyCapture & capture);

	private:
		int _id;
		PhysicsWorldV2 * _world;
		vector _linearVelocity;
		vector _angularVelocity;
		vector _position;
		quaternion _quaternion;
		bool _disabled;
		bool _fallingInPocket;
		vector _positionBackup;
		vector _frictionAccel;
		bool _hasFrictionAccel;
		bool _pureRolling;
		// LocNB, TODO: Refactor inhole and totally in hole
		bool _inHole;
		bool _totallyInHole;
		std::function<void()> _subscribe;

		bool _outOfTable;

		static const double HOLE_LIMIT_Z;

		static const double POCKET_LIMIT_Z;

		static const double DISABLED_Z;

		static const double NEG_SLIDING_FRICTION_LENGTH;

		static const double DW_CONSTANTS;
	};
};
#endif
