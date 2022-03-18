#ifndef __PHYSICS_ENTITIES__
#define __PHYSICS_ENTITIES__
#include "ExtensionMath.hpp"
#include "BallBodyV2.h"

namespace ps {
	using namespace ExtMath;

	class Pocket;
	class PocketAABB
	{
	public:
		static const double POCKET_AABB_PADDING;
		double minX, minY, maxX, maxY;

		PocketAABB();
		PocketAABB(const Pocket *pocket);

		bool checkPointInside(vector point);

		static PocketAABB *create(Pocket *pocket);
	};
	class Pocket {
	public:
		Pocket();

		Pocket(const int & id, const vector & position, const double & radius);
		~Pocket();

		PocketAABB warnPolygon;
		vector position;
		double radius;
		int id;
	};

	class Pockets {
	public:
		Pocket topLeftPocket;
		Pocket topRightPocket;
		Pocket bottomRightPocket;
		Pocket bottomLeftPocket;
		Pocket topSidePocket;
		Pocket bottomSidePocket;
	};

	class Cushion {
	public:
		Cushion(int id, const segment & seg);
		
		segment seg;
		int id;

		void collapseStartPoint(double delta);

		void collapseEndPoint(double delta);
	};

	class CushionSegment {
	public:
		CushionSegment(int id, const segment & seg);

		segment seg;
		int id;

		void collapseStartPoint(double delta);

		void collapseEndPoint(double delta);
	};

	class CushionPoint 
	{
	public:
		vector position;
		CushionSegment *cushion;

		CushionPoint(vector & position, CushionSegment *cushion);

		~CushionPoint();

		static CushionPoint *create(vector & position, CushionSegment *cushion);

	};

	class Collision {
	public:
		double time;
	};

	class CollisionCushion : public Collision {
	public:
		BallBody * ball = nullptr;
		Cushion * cushion = nullptr;
		vector point;
		bool collidePoint;
	};

	class CollisionCushionSegment : public Collision 
	{
	public:
		BallBody * ball = nullptr;
		CushionSegment * cushionSegment = nullptr;
	};

	class CollisionCushionPoint : public Collision
	{
	public:
		BallBody *ball = nullptr;
		CushionPoint *cushionPoint = nullptr;
	};

	class CollisionFloor : public Collision {
	public:
		BallBody * ball = nullptr;
		vector normal;
	};

	class CollisionBall : public Collision {
	public:
		BallBody * ball_1 = nullptr;
		BallBody * ball_2 = nullptr;
	};

	class CueAttribute {
	public:
		double tip_e;
		double tip_coef;
		double mass_ratio;
	};

	
}

#endif
