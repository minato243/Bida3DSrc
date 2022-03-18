#ifndef StaticBall_h_
#define StaticBall_h_

#include "../internal/ExtensionMath.hpp"
#include "StaticMotion.h"

namespace ps
{
	using namespace ExtMath;
	class StaticBall
	{
	public:
		StaticBall();
		~StaticBall();

	private:
		static const double BOUNCE_FLOOR_REFLECT_COEF;
		static const double ROLLING_FRICTION_LENGTH;
		static const double SLIDING_FRICTION_LENGTH;
		static const double SPINING_FRICTION_LENGTH;
		static const double INVERSE_BALL_RADIUS_SQUARE;
		static const double BALL_RADIUS_SQUARE;
		static const double G;
		static const vector R_DOWN_VEC;
		static const vector GRAVITY_VEC;

		static const double C1;
		static const double C2;
		static const double C3;

	public:
		StaticMotion _linearMotion;
		StaticMotion _angularMotion;

	public:
		MotionResult calcAll(double time);

		vector calcPosition(double time);

		vector calcLinearVelocity(double time);

		vector calcAngularVelocity(double time);

		void updateVelocities(double time, vector & linearVelocity, vector & angularVelocity);

		void updateVelocities(double time, vector & linearVelocity);

		void updateVelocities(double time);

	private:

		void updateVelocitiesOnTheTable(double time, vector & position, vector & linearVelocity, vector &angularVelocity);

		void updateVelocitiesNonJump(double time, vector & linearVelocity, vector & angularVelocity, bool lvEqZero);

		void updateVelocitiesSpiningOnly(double time, vector & angularVelocity);

		void updateVelocitiesPureRolling(double time, vector & linearVelocity, vector & angularVelocity);

		void updateVelocitiesNonPureRolling(double time, vector & linearVelocity, vector & angularVelocity, vector & vc);

		void updateVelocitiesJumpOnTheTable(double time, vector & linearVelocity, vector & angularVelocity);

		void updateVelocitiesOnTheSky(double time, vector & position, vector & linearVelocity, vector & angularVelocity);

		static double updateSpiningMotion(vector & angularVelocity, StaticMotion & angularMotion);

		static void updateLinearVelocityFloorBouncing(vector & linearVelocity);

		static void vectorSetZero(vector & v);
	};
};
#endif