#ifndef PhysicSimulateResultV2_h_
#define PhysicSimulateResultV2_h_

#include <list>
#include "ExtensionMath.hpp"
#include "MasseCueBall.h"

namespace ps
{
	using namespace ExtMath;
	class PhysicSimulateResult
	{
	public:
		class TimeStampResult
		{
		public:
			double timestamp;

			TimeStampResult(const double & timestamp);
			~TimeStampResult() {};
		};

		class BallIDPair : public TimeStampResult
		{
		public:
			int id_1;
			int id_2;
			vector vector_1;
			vector vector_2;

			BallIDPair(const double & timestamp, const int & id_1, const int & id_2, 
				const vector & v1, const vector & v2);

		};

		class BallCushionPair : public TimeStampResult
		{
		public:
			int ballId;
			int cushionId;

			BallCushionPair(const double & timestamp, const int & ballId, const int & cushion);
		};

		class FallenBallPair
			: public TimeStampResult
		{
		public:
			int ballId;
			int pocketId;

			FallenBallPair(const double timestamp, const int ballId, const int pocketId);
		};

		class BallOverBall 
			:public TimeStampResult
		{
		public:
			int underBallId;
			int overBallId;

			BallOverBall(double const & timestamp, const int & underBallId, const int & overBallId);
		};

		class FlyingCueBall
		{
		public:
			double startTime;
			double endTime;
			double highestZ;

			FlyingCueBall(double startTime, double endTime, double highestZ);
		};

	public:
		std::list<BallIDPair> ballCollisionIdPairs;
		std::list<BallCushionPair> cushionCollisions;
		std::list<int> floorCollisionIds;
		std::list<FallenBallPair> fallenBalls;
		std::list<int> outsideTableBallIds;
		std::list<BallOverBall> ballOverBallPairs;
		std::list<FlyingCueBall> flyingCueBalls;
		MasseCueBall *masseCueBall;

	public:
		bool valid;
		std::list<int> _scoreRegions;

	public:

		PhysicSimulateResult();

		~PhysicSimulateResult();

		void reset();

		void setMasseCueBall(MasseCueBall *masseCueBall);
	};
}

#endif