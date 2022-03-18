#ifndef StaticEvent_h_
#define StaticEvent_h_

class StaticEvent
{
public:
	enum class StaticEventType
	{
		// Discontinue calculating events
		OUTSIDE = -1,
		COLLIDE_BALL = -2,
		STOPPED = -3,
		COLLIDE_EDGE = -4,
		COLLIDE_POCKET_ZONE = -5,

		// Continue calculating events
		COLLIDE_CUSHION = 1,
		MOTION_EXPIRED = 2,
		COLLIDE_CUSHION_POINT = 3,
	};
	StaticEvent();
	~StaticEvent();

	
private:

public:
	StaticEventType type;
};
#endif