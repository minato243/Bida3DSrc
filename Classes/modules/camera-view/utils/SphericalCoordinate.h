#ifndef 	_SphericalCoord_H_
#define 	_SphericalCoord_H_


#include <cocos2d.h>
#include <data/DataStruct.h>
#include <modules/core/ps/internal/ExtensionMath.hpp>

class SphericalCoord {
public:
	SphericalCoord();

	static double degreeToRadian (double degree);
	static double radianToDegree (double radian);
	static ps::ExtMath::Rpt gameDescartesToSpherical(double x, double y, double z);
	static ps::ExtMath::Rpt descartesToSpherical(cocos2d::Vec3 v);
	static ps::ExtMath::Rpt descartesToSpherical(ps::ExtMath::vector v);
	static cocos2d::Vec3 sphericalToDecartes(double r, double p, double t);
	static cocos2d::Vec3 sphericalToDecartes(ps::ExtMath::Rpt rpt);
	static ps::ExtMath::vector direction(ps::ExtMath::Rpt rpt);

	static ps::ExtMath::vector sphericalToIngameDecartes(ps::ExtMath::Rpt rpt);

	static double normalizeNearestAngle(double origin, double target);
private:
};
#endif