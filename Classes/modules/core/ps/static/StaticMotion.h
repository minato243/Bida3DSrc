#ifndef StaticMotion_h_
#define StaticMotion_h_

#include "../internal/ExtensionMath.hpp"

namespace ps
{
	using namespace ExtMath;

	class MotionResult
	{
	public:
		vector position;
		vector velocity;

		MotionResult(const vector & position, const vector & velocity);
		~MotionResult();
	};

	class StaticMotion
	{
	public:
		StaticMotion();
		StaticMotion(const StaticMotion & src);
		~StaticMotion();

		vector acceleration;
		vector velocity;
		vector origin;
		double startTime;
		double endTime;

		MotionResult calcAll(double time);

		vector calcVelocity(double time);

		vector calcPosition(double time);

		static EquationResults resolveMotionEquation(double d, vector p, vector v, vector a);

	};
}

#endif
