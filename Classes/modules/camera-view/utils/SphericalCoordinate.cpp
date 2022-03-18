#include "SphericalCoordinate.h"
#include <modules/core/ps/internal/ExtensionMath.hpp>
#include <data/DataStruct.h>
#include "../../utils/CocosUtils.h"

using namespace ps;
USING_NS_CC;


const double DEGREE_PER_RADIAN = 180 / ExtMath::PI;
const double RADIAN_PER_DEGREE = ExtMath::PI / 180;

SphericalCoord::SphericalCoord()
{
}

double SphericalCoord::degreeToRadian (double degree) {
	return degree * RADIAN_PER_DEGREE;
}

double SphericalCoord::radianToDegree (double radian) {
	return radian * DEGREE_PER_RADIAN;
}

ps::ExtMath::Rpt SphericalCoord::gameDescartesToSpherical(double x, double y, double z) {
	//using game coord
	auto r = sqrt(x * x + y * y + z * z);
	auto r0 = sqrt(x * x + y * y);
	double p;
	if (y < 0) {
		p = 2 * ExtMath::PI - ExtMath::acos(x / r0);
	} else {
		p = acos(x / r0);
	}
	double t = acos(r0/ r);
	return ExtMath::Rpt{ r,
		p,
		t
	};

}

ExtMath::Rpt SphericalCoord::descartesToSpherical(cocos2d::Vec3 v)
{
	ExtMath::vector gameVec = CocosUtils::transCCVecToGameVec(v);
	return gameDescartesToSpherical(gameVec.x, gameVec.y, gameVec.z);
}

ExtMath::Rpt SphericalCoord::descartesToSpherical(ps::ExtMath::vector v)
{
	return gameDescartesToSpherical(v.x, v.y, v.z);
}

cocos2d::Vec3 SphericalCoord::sphericalToDecartes(ExtMath::Rpt rpt)
{
	return sphericalToDecartes(rpt.r, rpt.p, rpt.t);
}

ps::ExtMath::vector SphericalCoord::direction(ExtMath::Rpt rpt)
{
	return ps::ExtMath::vector(cos(rpt.t) * cos(rpt.p),
		cos(rpt.t) * sin(rpt.p),
		sin(rpt.t));
}

ps::ExtMath::vector SphericalCoord::sphericalToIngameDecartes(ps::ExtMath::Rpt rpt)
{
	ps::ExtMath::vector v;
	double RcosT = rpt.r * cos(rpt.t);
	v.x = RcosT * cos(rpt.p);
	v.y = RcosT * sin(rpt.p);
	v.z = rpt.r * sin(rpt.t);

	return v;
}

double SphericalCoord::normalizeNearestAngle(double origin, double target)
{
	int n = (int)((target - origin) / ExtMath::PI_P2);
	return target - n * ExtMath::PI_P2;
}

cocos2d::Vec3 SphericalCoord::sphericalToDecartes (double r, double p, double t) {
	auto RcosT = r * cos(t);
	return Vec3(RcosT * cos(p),
		r * sin(t),
		RcosT * sin(p)
	);

}

// cc.log(SphericalCoord.descartesToSpherical(1, 1, 1).p);
// cc.log(SphericalCoord.descartesToSpherical(-1, 1, 1).p);
// cc.log(SphericalCoord.descartesToSpherical(-1, 1, -1).p);
// cc.log(SphericalCoord.descartesToSpherical(1, 1, -1).p);
