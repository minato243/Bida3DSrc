#ifndef BallBodyV2_h_
#define BallBodyV2_h_

#include "ExtensionMath.hpp"
#include <functional>

namespace ps
{
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

	class BallBody
	{
	public:
	private:
		int _id;
		int _ballType;

		PhysicsWorldV2 *_world;
		
		quaternion _quaternion;

		bool _disabled;

		bool _inHole;
		bool _totallyInHole;
		bool _outOfTable;

		bool _fallingInPocket;
		vector _lockPos;
		int _lockType;
		bool _isLockPos;

		vector _positionBackup;

		vector _frictionAccel;
		bool _hasFrictionAccel;
		bool _pureRolling;

		std::function<void()> _subscribe;

		static const double HOLE_LIMIT_Z;
		static const double POCKET_LIMIT_Z;
		static const double DISABLED_Z;

		static const double NEG_ROLLING_FRICTION_LENGTH;
		static const double BALL_RADIUS_SQUARE;
		static const double NEG_SLIDING_FRICTION_LENGTH;
		static const double DW_CONSTANTS;

		static const double SPINNING_FRICTION_ACCELERATION;
		static const double BOUNCE_FLOOR_REFLECT_COEF;
		static const double BOUNCE_DW_CONSTANTS;
		static const double BOUNCE_CUSHION_COEF;

		static const double INVERSE_BALL_RADIUS;

	public:
		vector _linearVelocity;
		vector _angularVelocity;
		vector _position;
	public:
		BallBody();

		void setId(const int & id);

		const int & id();

		void setWorld(PhysicsWorldV2 *world);

		void outOfTable();

		void setMotionStatus(const vector & position, const quaternion & quat);

		void setQuaternion(const quaternion & quat);

		void fallToPocket();

		bool isOutOfTable();

		inline bool isInHolde();

		void reset();

		void resetShootingState();

		void resetToLockPos();

		void resetWithoutLockInfo();

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

		void bounceFloor(vector & normalAcceleration);

		void bounceEdge(vector & normalAcceleration);

		void bounceSegment(segment & seg);

		void bounceIntersectSegmentPoint(vector & p);

		void bounceSegmentDirection(vector & segmentDirection, vector & collisionPoint);

		double calcPenetrationSegmentTimeBack(segment & seg);

		double calcPenetrationPointTimeBack(vector & point);

		void syncPureRollingAngularVelocity();

		void finalizeStep();

		void enable();

		void disable();

		vector lockPos();

		int isLockPos();

		void setLockPos(const vector & pos);

		void makeCapture(BallBodyCapture &capture);

		vector & position();

		vector & linearVelocity();

		vector & angularVelocity();

		quaternion & quaternionRotation();

		void setPosition(const vector & pos);

		void setLinearVelocity(const vector & value);

		void setAngularVelocity(const vector & value);

		bool isFallingInPocket();

		bool isInHole();

		void dispatch();

		void setSubcribe(std::function<void()> subcribe);

		bool isTotallyInHole();

		bool isDisabled();

		void cheatTotalInHole(bool totalInHole);

	private:
		void bounceSegmentDirectionOnFloor(vector & linearVelocity, vector & segDir);

		void bounceSegmentDirectionOnSky(vector & linearVelocity, vector & rVec);
	};
}

#endif
