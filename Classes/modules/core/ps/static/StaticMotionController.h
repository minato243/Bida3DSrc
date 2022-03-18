#ifndef StaticMotionController_h_
#define StaticMotionController_h_

#include <list>
#include "../internal/ExtensionMath.hpp"
#include "StaticWorld.h"
#include "../internal/PhysicSimulateResultV2.h"

namespace ps
{
	using namespace ExtMath;
	class StaticMotionResult
	{
	public:
		StaticMotionResult();
		~StaticMotionResult();

		vector position;
		vector linearVelocity;
		vector angularVelocity;
		double endTime;
	};

	class QuaternionCalculator
	{
	public:
		vector _eulerAngle;
		quaternion _quaternion;

		QuaternionCalculator();

		void resetAngle(const vector & eulerAngle, const quaternion & quat);

		void updateAngle(const vector & eulerAngle);

	};
	class MasseCueBall;

	class StaticMotionController
	{
	private:
		std::vector<StaticWorld::MotionGroup *> _motionGroups;
		int _index;
		double _endTime;
		double _t;
		double _curMotionEndTime;

		QuaternionCalculator _quatCalc;

	public:
		StaticMotionController();
		~StaticMotionController();

		void setMotions(std::vector<StaticWorld::MotionGroup *> motionGroups, const quaternion & init);

		vector calcFinalPosition();

		StaticMotionResult *calcFinalResult();

		double getEndTime();

		void updateSimulateResult(PhysicSimulateResult & result);

		MasseCueBall * calMasseCueBall();

		std::list<PhysicSimulateResult::FlyingCueBall> calFlyingCueBall();

		vector position();

		void update(double dt);

		inline bool isFinished();

		void updateMotionIndex();

		void updateQuaternion();

		void onMotionFinishedRunning(StaticWorld::MotionGroup *motionGroup);

		void processCollideWithCushion(StaticWorld::MotionGroup *motionGroup);

		void processCollideWithCushionPoint(StaticWorld::MotionGroup *motionGroup);

		double getMotionEndTime(StaticWorld::MotionGroup *motionGroup);

		quaternion quat();
	};
}

#endif