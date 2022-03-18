#ifndef StaticCollision_h_
#define StaticCollision_h_

#include "../internal/PhysicsEntities.hpp"

namespace ps {
	class StaticCollisionBall
		: public Collision
	{

	};

	class StaticCollisionCushionPoint
		: public Collision
	{
	public:
		int pointIdx;
	};

	class StaticCollisionCushion 
		:public Collision
	{
	public:
		int cushionIndex;
		ExtMath::vector collidedPosition;
		ExtMath::vector contactPoint;
		int segmentId;

		StaticCollisionCushion();
		~StaticCollisionCushion();
	};

	class StaticCollisionEdge
		: public Collision
	{
	public:
		StaticCollisionEdge();
		~StaticCollisionEdge();
	};

	class StaticCollisionPocketZone
		:public Collision 
	{
	public:
		int pocketIdx;

		StaticCollisionPocketZone();
		~StaticCollisionPocketZone();
	};

};

#endif