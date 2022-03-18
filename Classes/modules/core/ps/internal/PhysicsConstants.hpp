#ifndef __PHYSICS_CONSTANTS__
#define __PHYSICS_CONSTANTS__

#include "ExtensionMath.hpp"
#include <math.h>

namespace ps {
	using namespace ExtMath;

	class PhysicsConstants {
	public:
		// Unit of length is 'cm'
		// Unit of mass is 'kg'

		PhysicsConstants();

		static const double G;
		static const vector GRAVITY;
		static const vector REVERSE_GRAVITY;

		static const int CUE_BALL_ID;
		static const int LAST_BALL_ID;
		static const int LAG_BALL_ID;
		static const double BALL_RADIUS;
		static const double BALL_MASS;
		static const double BALL_DIAMETER;
		static const double BALL_DIAMETER_SQUARE;
		static const double BALL_RADIUS_SQUARE;
		static const double OFFSET_MAX_R;

		// For table static static constants
		static const double TABLE_WIDTH;
		static const double TABLE_HEIGHT;
		static const double CUSHION_HEIGHT;
		static const double CORNER_POCKET_WIDTH;
		static const double SIDE_POCKET_WIDTH;
		static const double CORNER_POCKET_RADIUS;
		static const double SIDE_POCKET_RADIUS;
		static const double CORNER_POCKET_DEPTH;
		static const double SIDE_POCKET_DEPTH;
		static const double TABLE_EDGE_THICKNESS_V;
		static const double TABLE_EDGE_THICKNESS_H;
		static const double SUB_SEG_OFFSET;

		// Cue static constants
		static const double CUE_MASS;
		static const double TIP_COEF;
		static const double TIP_E;
		static const double BALL_CUE_MASS_RATIO;
		static const double MAX_CUE_OFFSET;
		static const double MAX_TIP_COEF;

		// Motion static constants
		static const double SLIDING_FRICTION_COEF;
		static const double ROLLING_FRICTION_COEF;
		static const double SPINING_FRICTION_COEF;
		static const double SPINNING_FRICTION_ACCELERATION;
		static const double COLLISION_ANGULAR_COEF;

		// Bounce cushion
		static const double SPIN_CUSHION_FRICTION;
		static const double BOUNCE_CUSHION_COEF;
		static const double CUSHION_E;
		static const double CUSHION_U;

		// Bounce floor static constants
		static const double BOUNCE_FLOOR_REFLECT_COEF;
		static const double FLOOR_E;
		static const double FLOOR_U;

		// Initial positions
		static const vector CUE_BALL_LAG_PLACEHOLDER_POSITION;
		static const vector CUE_BALL_POSITION;

		static vector INIT_BALL_POSITIONS[15];
		void initBallPosition();

		static const int CUSHION_HEAD_ID = 2;
		static const int CUSHION_FOOT_ID = 0;
		static const int CUSHION_SIDE_1_ID = 1;
		static const int CUSHION_SIDE_2_ID = 3;

		// Pocket ID
		static const int TOP_RIGHT_POCKET_ID = 0;
		static const int TOP_SIDE_POCKET_ID = 1;
		static const int TOP_LEFT_POCKET_ID = 2;
		static const int BOTTOM_LEFT_POCKET_ID = 3;
		static const int BOTTOM_SIDE_POCKET_ID = 4;
		static const int BOTTOM_RIGHT_POCKET_ID = 5;

		static const int NOT_DETERMINED = 0;
		static const int SOLID = 1;
		static const int STROKE = 2;

		/*static const int LAG_SEPARATOR_COUNT = 3;
		static const vector * LAG_BALL_POSITION = new vector[LAG_SEPARATOR_COUNT];

		public static void init() {
			for (int i = 0; i < LAG_SEPARATOR_COUNT; i++) {
				LAG_BALL_POSITION[i] = vector.vec((-60 + i * 120) * 1f / (LAG_SEPARATOR_COUNT - 1), 0);
			}
		}*/

		static void loadConstants();

		static PhysicsConstants* instance;
		static PhysicsConstants* getInstance();

	};
};

#endif
