#ifndef TRAJECTORY_BUILDER_H
#define	TRAJECTORY_BUILDER_H

#include <modules/core/ps/internal/PhysicsCue.hpp>
#include <modules/core/ps/internal/BallBodyV2.h>
#include <modules/core/ps/TrajectoryElement.h>

namespace ps{

	class TrajectoryBuilder
	{
	public:
		TrajectoryBuilder();
		~TrajectoryBuilder();

		TrajectoryElement *buildBasicTrajectory(PhysicsCue* cue, BallBody *cueBall, std::vector<BallBody*> balls);
		std::vector<ps::ExtMath::vector> buildBasicPoints(BasicTrajElem &trajectory);
		std::vector<TrajectoryElement*> buildTrajectories(double force, PhysicsCue* cue, BallBody* cueBall, std::vector<BallBody*> balls);
		std::vector<ps::ExtMath::vector> buildPoints(std::vector<TrajectoryElement*> trajectories);

		TrajectoryReflectData calcBasicReflectDir(TrajectoryElement* trajectory);

		TrajectoryReflectData calcReflectDir(TrajectoryElement* trajectory, double lengthVelocity = -1);

		static double calcAxisDuration(double position, double v, double a, double max);
		static bool isOutSize(vector position);
		
		
		static const ExtMath::vector R_VEC;
		static const double STEP_UNIT;
		static const double MIN_L_VEL;
		static const double ROLLING_ACCEL_LEN;
		static const double MAX_X;
		static const double MAX_Y;
		static const double MIN_X;
		static const double MIN_Y;
		static const double CST_2pG;
		static const double SLIDING_ACCEL_LEN;
	};
}

#endif // !TRAJECTORY_BUILDER_H


