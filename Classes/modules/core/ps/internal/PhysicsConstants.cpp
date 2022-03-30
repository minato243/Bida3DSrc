#include "PhysicsConstants.hpp"

using namespace ps;

// Unit of length is 'cm'
// Unit of mass is 'kg'

const double PhysicsConstants::G = 981;
const vector PhysicsConstants::GRAVITY(0, 0, -G);
const vector PhysicsConstants::REVERSE_GRAVITY(0, 0, G);

// Ball constants
const int PhysicsConstants::CUE_BALL_ID = 0;
const int PhysicsConstants::LAST_BALL_ID = 8;
const int PhysicsConstants::LAG_BALL_ID = 100;
const double PhysicsConstants::BALL_RADIUS = 3;
const double PhysicsConstants::BALL_MASS = 0.16;
const double PhysicsConstants::BALL_DIAMETER = BALL_RADIUS * 2;
const double PhysicsConstants::BALL_DIAMETER_SQUARE = BALL_DIAMETER * BALL_DIAMETER;
const double PhysicsConstants::BALL_RADIUS_SQUARE = BALL_RADIUS * BALL_RADIUS;
const double PhysicsConstants::OFFSET_MAX_R = PhysicsConstants::BALL_RADIUS *0.8;

// For table static constants
const double PhysicsConstants::TABLE_WIDTH = 255.7;
const double PhysicsConstants::TABLE_HEIGHT = 127.7;
const double PhysicsConstants::CUSHION_HEIGHT = 7 * BALL_RADIUS /5;
const double PhysicsConstants::CORNER_POCKET_WIDTH = 11.7;
const double PhysicsConstants::SIDE_POCKET_WIDTH = 15.5;
const double PhysicsConstants::CORNER_POCKET_RADIUS = 7;
const double PhysicsConstants::SIDE_POCKET_RADIUS = 6;
const double PhysicsConstants::CORNER_POCKET_DEPTH = 4;
const double PhysicsConstants::SIDE_POCKET_DEPTH = 7;
const double PhysicsConstants::TABLE_EDGE_THICKNESS_V = 17.7;
const double PhysicsConstants::TABLE_EDGE_THICKNESS_H = 17.3;
const double PhysicsConstants::SUB_SEG_OFFSET = 2.5;

// Cue constants
const double PhysicsConstants::CUE_MASS = 0.6;
const double PhysicsConstants::TIP_COEF = 0.5;
const double PhysicsConstants::TIP_E = 0.2;
const double PhysicsConstants::BALL_CUE_MASS_RATIO = CUE_MASS / (BALL_MASS + CUE_MASS);
const double PhysicsConstants::MAX_CUE_OFFSET = 2;
const double PhysicsConstants::MAX_TIP_COEF = 1;

// Motion constants
const double PhysicsConstants::SLIDING_FRICTION_COEF = 0.2;
const double PhysicsConstants::ROLLING_FRICTION_COEF = 0.007;
const double PhysicsConstants::SPINING_FRICTION_COEF = 0.03;
const double PhysicsConstants::SPINNING_FRICTION_ACCELERATION = SPINING_FRICTION_COEF * G;
const double PhysicsConstants::COLLISION_ANGULAR_COEF = 50;

// Bounce cushion
const double PhysicsConstants::SPIN_CUSHION_FRICTION = 0.07;
const double PhysicsConstants::BOUNCE_CUSHION_COEF = 0.6; // 0.9 * 21 / 25
const double PhysicsConstants::CUSHION_E = 0.9;
const double PhysicsConstants::CUSHION_U = 0.2;

// Bounce floor constants
const double PhysicsConstants::BOUNCE_FLOOR_REFLECT_COEF = 30;
const double PhysicsConstants::FLOOR_E = 0.5;
const double PhysicsConstants::FLOOR_U = 0.1;

// Initial positions
const vector PhysicsConstants::CUE_BALL_LAG_PLACEHOLDER_POSITION(-60, 30);
const vector PhysicsConstants::CUE_BALL_POSITION(-60, 0);

const double InitVertBallDistance = PhysicsConstants::BALL_RADIUS + 0.05;

int TOTAL_LAYER = 5;
double SQRT_3_InitVertBallDistance = sqrt(3) * InitVertBallDistance;

const int PhysicsConstants::CUSHION_HEAD_ID = 2;
const int PhysicsConstants::CUSHION_FOOT_ID = 0;
const int PhysicsConstants::CUSHION_SIDE_1_ID = 1;
const int PhysicsConstants::CUSHION_SIDE_2_ID = 3;


const int PhysicsConstants::TOP_RIGHT_POCKET_ID = 0;
const int PhysicsConstants::TOP_SIDE_POCKET_ID = 1;
const int PhysicsConstants::TOP_LEFT_POCKET_ID = 2;
const int PhysicsConstants::BOTTOM_LEFT_POCKET_ID = 3;
const int PhysicsConstants::BOTTOM_SIDE_POCKET_ID = 4;
const int PhysicsConstants::BOTTOM_RIGHT_POCKET_ID = 5;

ps::PhysicsConstants::PhysicsConstants()
{
	initBallPosition();
}

vector PhysicsConstants::INIT_BALL_POSITIONS[15];
void ps::PhysicsConstants::initBallPosition()
{
	int i = 0;
	for (auto layer = 0; layer < TOTAL_LAYER; layer++) {
		for (auto index = layer; index >= -layer; index -= 2) {
			ExtMath::vector pos = ExtMath::vector(
				layer * SQRT_3_InitVertBallDistance + 60,
				index * InitVertBallDistance
			);
			PhysicsConstants::INIT_BALL_POSITIONS[i++] = pos;
			// cc.log(JSON.stringify(pos));
		}
	}
}

void PhysicsConstants::loadConstants() {
	// TODO: Load constants from config files
}

PhysicsConstants* PhysicsConstants::instance = new PhysicsConstants();
PhysicsConstants* ps::PhysicsConstants::getInstance()
{
	if (PhysicsConstants::instance == nullptr) {
		PhysicsConstants::instance = new PhysicsConstants();
	}
	return PhysicsConstants::instance;
}

