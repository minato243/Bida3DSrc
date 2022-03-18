#ifndef CueAngle_h_
#define CueAngle_h_

#include <modules/core/ps/internal/ExtensionMath.hpp>
#include <limits>
#include <modules/core/ps/internal/BallBodyV2.h>
#include <map>

struct CueAngleValues
{
	float offsetV;
	float theta;
	float sinTheta;

	CueAngleValues(float offsetV, float sinTheta)
	{
		this->offsetV = offsetV;
		this->sinTheta = sinTheta;
		this->theta = 0.f;
	}

	CueAngleValues(float offsetV, float theta, float sinTheta)
	{
		this->offsetV = offsetV;
		this->sinTheta = sinTheta;
		this->theta = theta;
	}
};

struct CueAngleCushion
{
	ps::ExtMath::vector normal;
	ps::ExtMath::vector origin;

	CueAngleCushion(ps::ExtMath::vector normal, ps::ExtMath::vector origin)
	{
		this->normal = normal;
		this->origin = origin;
	}
};

class CueAngle
{
public:
	CueAngle();
	~CueAngle();

	void setBalls(std::vector<ps::BallBody*> *balls);

	void setCueOffset(ps::ExtMath::vector offset);

	void setCueDirection(ps::ExtMath::vector direction);

	void setCueShape(float cueHeadRadius, float cueTailRadius, float cueLength, float headBallDistance);

	CueAngleValues calcMinValuesWithTheta(float theta);

	CueAngleValues calcMinValuesWithRPT(ps::Rpt rpt);

	CueAngleValues calcMinValuesWithBalls(float theta, ps::ExtMath::vector dir, float dirLen, float maxOffsetV);

	CueAngleValues calcMinValueWithBall(const ps::ExtMath::vector & cueBallPos, const ps::ExtMath::vector & ballPos, float theta, ps::ExtMath::vector dir, float dirLen, float maxOffsetV);

	CueAngleValues calcMinValuesWithCushions(float theta, ps::ExtMath::vector dir, float dirLen, float maxOffsetV);

	CueAngleValues calcMinValuesWithCushion(float theta, ps::ExtMath::vector position, CueAngleCushion cushion, ps::ExtMath::vector dir, float dirLen, float maxOffsetV);

	float solveTrigonometricEquation(float a, float b, float c);

	// Find k : { 0 < (alpha + k * 2 * PI) < (PI / 2) }
	float findK(float alpha);

	float calcAngle(float sinSign, float cosValue);

	float calcCueRadius(float pointToCueBallDistance);
	

private:

public:
	std::map<int, ps::ExtMath::vector *> _positions;
	std::vector<ps::BallBody*> *_balls;

	ps::ExtMath::vector _offset;
	ps::ExtMath::vector _direction;

	float _cueHeadRadius;
	float _cueTailRadius;
	float _cueLength;
	float _headBallDistance;
	float _deltaRadiusPerLength;
	float _lenLimit;
	float _lenLimit2;

public:
	static const float MAX_OFFSET;

	static const float MAX_OFFSET_2;

	static const float MIN_OFFSET_V;

	static const int CUSHION_LENGTH = 4;

	static CueAngleCushion CUSHIONS[CUSHION_LENGTH];

	static const float CUSHION_DELTA_HEIGHT;
	
	static const float MIN_FLOAT;

};
#endif // !CueAngle_h_
