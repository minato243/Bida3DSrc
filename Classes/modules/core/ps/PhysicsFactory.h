
#ifndef PhysicsFactory_h_
#define PhysicsFactory_h_

#include "internal/PhysicsWorldV2.h"

namespace ps
{
	class PhysicsFactory
	{
	public:
		PhysicsFactory();
		~PhysicsFactory();

		void test();

		void generatePhysicsWorld(PhysicsWorldV2 * world);
	};
}

#endif
