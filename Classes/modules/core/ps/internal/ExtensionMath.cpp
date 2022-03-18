#include "ExtensionMath.hpp"
#include <algorithm>
#include <cmath>
#include <cfloat>

using namespace ps;

#define MIN_ZERO_THRESHOLD -1e-12
#define MAX_ZERO_THRESHOLD 1e-12
#define PRECISION_TRIGONOMETRY 10e13
#define EXT_MATH_PI 3.14159265358979323846

const double ExtMath::PI = EXT_MATH_PI;
const double ExtMath::TWO_PI = 2 * ExtMath::PI;
const double ExtMath::PI_P2 = ExtMath::PI / 2;
const double ExtMath::PI_3P2 = 3 * ExtMath::PI / 2;

bool ExtMath::isZeroNumber(double num) {
    if (num == 0) return true;
    return MIN_ZERO_THRESHOLD < num && num < MAX_ZERO_THRESHOLD;
}

bool ExtMath::isLessThanZeroNumber(double num) {
    return num <= MIN_ZERO_THRESHOLD;
}

bool ExtMath::isGreatThanZeroNumber(double num) {
    return MAX_ZERO_THRESHOLD <= num;
}

double ExtMath::acos(double value) {
    return std::acos(std::min(1.0, std::max(-1.0, value)));
}

double ExtMath::asin(double value) {
    return std::asin(std::min(1.0, std::max(-1.0, value)));
}

double ExtMath::cos(double value) {
    double ret = std::cos(value);
    return std::round(ret * PRECISION_TRIGONOMETRY) / PRECISION_TRIGONOMETRY;
}

double ExtMath::sin(double value) {
    double ret = std::sin(value);
    return std::round(ret * PRECISION_TRIGONOMETRY) / PRECISION_TRIGONOMETRY;
}

double ps::ExtMath::tan(const double & value)
{
	double ret = std::tan(value);
	return round(ret * PRECISION_TRIGONOMETRY) / PRECISION_TRIGONOMETRY;
}

double ps::ExtMath::atan(const double & value)
{
	double a = std::min(1., std::max(-1., value));
	double ret = std::atan(a);
	return round(ret * PRECISION_TRIGONOMETRY) / PRECISION_TRIGONOMETRY;
}

double ps::ExtMath::cot(double value)
{
	return 1 / tan(value);
}

double ps::ExtMath::interpolate(double min, double max, double current, double minInter, double maxInter)
{
	if (current > max) {
		current = max;
	}
	if (current < min) {
		current = min;
	}
	auto ratio = (current - min) / (max - min);
	return minInter + ratio * (maxInter - minInter);
}

double ExtMath::minifyAngle(double value) {
    if (value < 0) {
        return value + ExtMath::TWO_PI * ceil(-value / TWO_PI);
    }
    else if (value > TWO_PI) {
        return value - TWO_PI * floor(value / TWO_PI);
    }
    return value;
}

double ps::ExtMath::simplifyRadian(double value)
{
	if (value < 0) {
		return value + M_PI_2 * ceil(-value / M_PI_2);
	}
	else if (value > M_PI_2) {
		return value - M_PI_2 * floor(value / M_PI_2);
	}
	return value;
}

std::vector<double> ps::ExtMath::resolveQuarticEquation(double a, double b, double c, double d, double e)
{
    if (a != 1) {
        b = b / a;
        c = c / a;
        d = d / a;
        e = e / a;
    }

    auto b2 = b * b;
    auto b3 = b2 * b;

    auto bias = -0.25 * b;

    auto p = c - 0.375 * b2;
    auto q = d - 0.5 * b * c + 0.125 * b3;
    auto r = e + bias * d + 0.0625 * b2 * c - 0.01171875 * b3 * b;
    std::vector<double> res;
    if (ExtMath::isZeroNumber(q)) {
        auto delta = p * p - 4 * r;
        if (delta < 0) {
            return res;
        }
        else if (delta == 0) {
            if (p > 0) {
                return res;
            }
            else {
                auto sqrt_x2 = sqrt(-0.5 * p);
                res.push_back(-sqrt_x2);res.push_back(sqrt_x2);
            }
        }
        else {
            auto sqrt_delta = sqrt(delta);
            auto x1_2 = sqrt_delta - p;
            auto x2_2 = -sqrt_delta - p;
            
            if (x1_2 == 0) {
                res.push_back(bias);
            }
            else if (x1_2 > 0) {
                auto sqrt_x1_2 = sqrt(0.5 * x1_2);
                res.push_back(sqrt_x1_2 + bias); res.push_back(bias - sqrt_x1_2);
            }
            if (x2_2 == 0) {
                res.push_back(bias);
            }
            else if (x2_2 > 0) {
                auto sqrt_x2_2 = sqrt(0.5 * x2_2);
                res.push_back(sqrt_x2_2 + bias); res.push_back(bias - sqrt_x2_2);
            }
            return res;
        }
    }

    auto m = eq_resolveSimpleCubicEquation(1, -0.5 * p, -r, 0.5 * p * r - 0.125 * q * q);

    auto v1 = 2 * m - p;

    if (v1 < 0) {
        return res;
    }

    auto sqrt_v1_p2 = 0.5 * sqrt(v1);

    auto v2 = q / sqrt_v1_p2;
    auto v3 = -2 * m - p;

    std::vector<double>result;

    auto vx12 = v3 - v2;
    auto vx34 = v3 + v2;

    if (vx12 >= 0) {
        auto base12 = bias + sqrt_v1_p2;
        if (vx12 == 0) {
            result.push_back(base12);
        }
        else {
            auto sqrt_vx12_p2 = 0.5 * sqrt(vx12);
            result.push_back(base12 + sqrt_vx12_p2);result.push_back(base12 - sqrt_vx12_p2);
        }
    }
    if (vx34 >= 0) {
        auto base34 = bias - sqrt_v1_p2;
        if (vx34 == 0) {
            result.push_back(base34);
        }
        else {
            auto sqrt_vx34_p2 = 0.5 * sqrt(vx34);
            result.push_back(base34 + sqrt_vx34_p2);
            result.push_back(base34 - sqrt_vx34_p2);
        }
    }

    return result;
}

double ps::ExtMath::eq_resolveSimpleCubicEquation(double a, double b, double c, double d)
{
    if (a != 1) {
        b = b / a;
        c = c / a;
        d = d / a;
    }

    auto b2 = b * b;
    auto b3 = b2 * b;
    auto delta = b2 - 3 * c;
    
    double x;

    if (delta == 0) {
        auto inside = b3 - 27 * d;
        if (inside == 0) {
            x = -b;
        }
        else {
            x = -b + cbrt(inside);
        }
    }
    else {
        auto abs_delta = (delta < 0) ? -delta : delta;
        auto sqrt_delta = sqrt(abs_delta);
        auto k = (4.5 * b * c - b3 - 13.5 * d) / (sqrt_delta * abs_delta);

        if (delta > 0) {
            auto abs_k = (k < 0) ? -k : k;
            if (abs_k > 1) {
                auto right_cbrt = sqrt(k * k - 1);
                auto cube_combine = cbrt(abs_k + right_cbrt) + cbrt(abs_k - right_cbrt);
                if (abs_k == k) {
                    x = sqrt_delta * cube_combine - b;
                }
                else {
                    x = -sqrt_delta * cube_combine - b;
                }
            }
            else {
                x = 2 * sqrt_delta * cos(acos(k) / 3) - b;
            }
        }
        else {
            auto right_cbrt = sqrt(k * k + 1);
            auto cube_combine = cbrt(k + right_cbrt) + cbrt(k - right_cbrt);
            x = sqrt_delta * cube_combine - b;
        }
    }

    return x / 3;
}

ExtMath::vector::vector() : x(0), y(0), z(0) {}

ExtMath::vector::vector(double x, double y) : x(x), y(y), z(0) {}

ExtMath::vector::vector(double x, double y, double z) : x(x), y(y), z(z) {}

ExtMath::vector::vector(const vector & v) : x(v.x), y(v.y), z(v.z) {}

ExtMath::vector & ExtMath::vector::operator=(const vector & v) {
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
}

void ExtMath::vector::reverse() {
    x = -x;
    y = -y;
    z = -z;
}

void ExtMath::vector::fixZeroVectorPrecision() {
    if (isZeroNumber(x)) {
        x = 0;
    }
    if (isZeroNumber(y)) {
        y = 0;
    }
    if (isZeroNumber(z)) {
        z = 0;
    }
}

void ExtMath::vector::rotate2D(double angle) {
    double c = ExtMath::cos(angle);
    double s = ExtMath::sin(angle);
    double nx = c * x - s * y;
    double ny = s * x + c * y;
    x = nx;
    y = ny;
}

void ExtMath::vector::applyQuaternion(const quaternion & q) {
    quaternion r = quaternion::reverse(q);
    quaternion vq(0, x, y, z);
    quaternion result = quaternion::multiply(quaternion::multiply(q, vq), r);
    x = result.x;
    y = result.y;
    z = result.z;
}

void ExtMath::vector::assignZero() {
    x = y = z = 0;
}

ExtMath::plane::plane(const vector & p, const vector & n) : p(p), n(n) {}

ExtMath::quaternion::quaternion() : w(1), x(0), y(0), z(0) {}

ExtMath::quaternion::quaternion(double w, double x, double y, double z) : x(x), y(y), z(z), w(w) {}

ExtMath::quaternion::quaternion(const vector & v) {
    double length = vector::length(v);
    if (isZeroNumber(length)) {
        w = 1;
        x = y = z = 0;
    } else {
        double sinHalfThetaPerLength = sin(0.5 * length) / length;
        w = ExtMath::cos(0.5 * length);
        x = v.x * sinHalfThetaPerLength;
        y = v.y * sinHalfThetaPerLength;
        z = v.z * sinHalfThetaPerLength;
    }
}

void ExtMath::quaternion::reverse() {
    double square = w * w + x * x + y * y + z * z;
    w = w / square;
    x = -x / square;
    y = -y / square;
    z = -z / square;
}

ExtMath::quaternion ExtMath::quaternion::multiply(const quaternion & p, const quaternion & q) {
	return quaternion(
		p.w * q.w - p.x * q.x - p.y * q.y - p.z * q.z,
		p.w * q.x + p.x * q.w + p.y * q.z - p.z * q.y,
		p.w * q.y - p.x * q.z + p.y * q.w + p.z * q.x,
		p.w * q.z + p.x * q.y - p.y * q.x + p.z * q.w
	);
}

ExtMath::quaternion ExtMath::quaternion::reverse(const quaternion & q) {
	double square = q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z;
	return quaternion(q.w / square, -q.x / square, -q.y / square, -q.z / square);
}

bool ExtMath::quaternion::equals(const quaternion & p, const quaternion & q) {
	return p.x == q.x && p.y == q.y && p.z == q.z && p.w == q.w;
}

ps::ExtMath::quaternion & ps::ExtMath::quaternion::operator=(const quaternion & q)
{
	w = q.w;
	x = q.x;
	y = q.y;
	z = q.z;
	return *this;
}

ps::ExtMath::quaternion ps::ExtMath::quaternion::quatFromVec(const vector & v)
{
	auto length = vector::length(v);
	if (isZeroNumber(length)) {
		return quaternion(1, 0, 0, 0);
	}
	auto dir = vector(v.x / length, v.y / length, v.z / length);
	auto sinHalfTheta = sin(0.5 * length);

	return quaternion(
		cos(0.5 * length),
		dir.x * sinHalfTheta,
		dir.y * sinHalfTheta,
		dir.z * sinHalfTheta
	);
}

ExtMath::segment::segment(const vector & s, const vector & e) : s(s), e(e) {}

bool ExtMath::AABB::checkPointInside(const vector & p) {
    return minX <= p.x && p.x <= maxX
        && minY <= p.y && p.y <= maxY;
}

ExtMath::Polygon2D::Polygon2D() {}

const ExtMath::vector ExtMath::Polygon2D::OUTSIDE_ORIGIN(0, 1e10);

std::vector<ExtMath::vector> & ExtMath::Polygon2D::polygon() {
    return points;
}

bool ExtMath::Polygon2D::checkPointInside(const vector & p) {
    int sides = points.size() - 1;
    // const origin = { x: 0, y: p.y };
    bool result = false;
    for (int i = 0; i < sides; i++) {
        vector & s1 = points[i];
        vector & s2 = points[i + 1];
        // origin.x = Math.min(origin.x, Math.min(s1.x, s2.x) - 1);
        if (intersection(OUTSIDE_ORIGIN, p, s1, s2)) {
            result = !result;
        }
    }

	vector & s1 = points[sides];
	vector & s2 = points[0];
    if (intersection(OUTSIDE_ORIGIN, p, s1, s2)) {
        result = !result;
    }

    return result;
}

bool ExtMath::Polygon2D::ccw(const vector & a, const vector & b, const vector & c) {
    return (c.y - a.y) * (b.x - a.x) >= (b.y - a.y) * (c.x - a.x);
}

bool ExtMath::Polygon2D::intersection(const vector & a, const vector & b, const vector & c, const vector & d) {
    return ccw(a, c, d) != ccw(b, c, d) && ccw(a, b, c) != ccw(a, b, d);
}

// Vector operators
bool ExtMath::vector::equals(const vector & u, const vector & v) {
    return isZeroNumber(u.x - v.x) && isZeroNumber(u.y - v.y) && isZeroNumber(u.z - v.z);
}

bool ExtMath::vector::equalsZero(const vector & u) {
    return isZeroNumber(u.x) && isZeroNumber(u.y) && isZeroNumber(u.z);
}

double ExtMath::vector::dot(const vector & u, const vector & v) {
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

ExtMath::vector ExtMath::vector::cross(const vector & u, const vector & v) {
    return vector(
        u.y * v.z - v.y * u.z,
        v.x * u.z - u.x * v.z,
        u.x * v.y - v.x * u.y
    );
}

double ExtMath::vector::crossZ(const vector & u, const vector & v) {
    return u.x * v.y - v.x * u.y;
}

ExtMath::vector ExtMath::vector::add(const vector & u, const vector & v) {
    return vector(u.x + v.x, u.y + v.y, u.z + v.z);
}

ExtMath::vector ExtMath::vector::add(const vector & u, const vector & v, const vector & w) {
    return vector(u.x + v.x + w.x, u.y + v.y + w.y, u.z + v.z + w.z);
}

ExtMath::vector ExtMath::vector::sub(const vector & u, const vector & v) {
    return vector(u.x - v.x, u.y - v.y, u.z - v.z);
}

ExtMath::vector ExtMath::vector::multiply(double n, const vector & u) {
    return vector(n * u.x, n * u.y, n * u.z);
}

ExtMath::vector ExtMath::vector::reverse(const vector & u) {
    return vector(-u.x, -u.y, -u.z);
}

ExtMath::vector ExtMath::vector::project(const vector & u, const vector & v) {
    return multiply(dot(u, v) / dot(v, v), v);
}

double ExtMath::vector::length(const vector & v) {
    return std::sqrt(dot(v, v));
}

double ExtMath::vector::lengthSquare(const vector & v) {
    return dot(v, v);
}

ExtMath::vector ExtMath::vector::unit(const vector & v) {
    if (equalsZero(v)) {
        return vector(); // Zero vector
    }
    return multiply(1 / length(v), v);
}

ExtMath::vector ExtMath::vector::fromSegment(const segment & seg) {
    return sub(seg.e, seg.s);
}

ExtMath::vector ExtMath::vector::reflect(const vector & dir, const vector & nor) {
    return sub(
        multiply(2, project(dir, nor)),
        dir
    );
}

double ExtMath::vector::distance(const vector & u, const vector & v) {
    return length(sub(u, v));
}

double ExtMath::vector::distanceSquare(const vector & u, const vector & v) {
    return lengthSquare(sub(u, v));
}

ExtMath::vector ExtMath::vector::applyQuaternion(const vector & v, const quaternion & q) {
    quaternion r = quaternion::reverse(q);
    quaternion vq = quaternion(0, v.x, v.y, v.z);
    quaternion result = quaternion::multiply(quaternion::multiply(q, vq), r);
    return vector(result.x, result.y, result.z);
}

ExtMath::vector ps::ExtMath::vector::mix(const vector& u, const vector& v, double range)
{
    return vector(u.x + (v.x - u.x) * range,
        u.y + (v.y - u.y) * range,
        u.z + (v.z - u.z) * range);
    
}

ExtMath::vector ExtMath::vector::normalize(const vector& v)
{
    return multiply(1. / length(v), v);
}

double ps::ExtMath::vector::angle2D(const vector & v1, const vector & v2)
{
	auto lengthV1V2 = vector::length(v1) * vector::length(v2);
	if (isZeroNumber(lengthV1V2)) return 0;
	auto cosVal = vector::dot(v1, v2) / lengthV1V2;
	auto angle = acos(cosVal);
	if (vector::crossZ(v1, v2) < 0) {
		angle = TWO_PI - angle;
	}
	return angle;
}

ExtMath::vector ps::ExtMath::vector::rotate2D(const vector &v, double angle)
{
	auto c = cos(angle);
	auto s = sin(angle);

	return vector(
		c * v.x - s * v.y,
		s * v.x + c * v.y,
		v.z
	);
}

ps::ExtMath::vector ps::ExtMath::vector::crossZVecWithVector(double z, const vector & v)
{
	return vector(-v.y *z, v.x *z, 0);
}

ExtMath::vector ExtMath::vector::crossVectorWithZVec(const vector & u, double z)
{
	return vector(u.y * z, -u.x * z, 0);
}

ExtMath::vector ps::ExtMath::vector::middle(const vector & v1, const vector & v2)
{
	return vector((v1.x + v2.x) / 2, (v1.y + v2.y) / 2, (v1.z + v2.z) / 2);
}

void ps::ExtMath::vector::clamp(vector & v, const vector & u1, const vector & u2)
{
	v.x = ExtMath::clamp(v.x, u1.x, u2.x);
	v.y = ExtMath::clamp(v.y, u1.y, u2.y);
	v.z = ExtMath::clamp(v.z, u1.z, u2.z);
}

bool ps::ExtMath::vector::checkVectorsParallel(const vector & u, const vector & v)
{
	double lengthValue = vector::length(u) * vector::length(v);
	if (lengthValue == 0) return true;
	double cosValue = vector::dot(u, v) / lengthValue;
	double aCosValue = cosValue < 0 ? -cosValue : cosValue;
	return isZeroNumber(1 - aCosValue);
}

// return a point which give (A,p1) same direction with (p2, p1) and Length(A, p1) = length
ps::ExtMath::vector ps::ExtMath::vector::getFixDistancePoint(const vector & p1, const vector & p2, double length)
{
	return vector::add(p1, vector::multiply(length, vector::unit(vector::sub(p2, p1))));
}

const ExtMath::vector ExtMath::vector::ZERO = ExtMath::vector(0, 0, 0);


ExtMath::Rpt::Rpt() : r(0), p(0), t(0)
{
}

ExtMath::Rpt::Rpt(double r, double p, double t)
{
	this->r = r;
	this->p = p;
	this->t = t;
}

ps::ExtMath::Rpt & ps::ExtMath::Rpt::operator=(const Rpt & rpt)
{
	this->r = rpt.r;
	this->p = rpt.p;
	this->t = rpt.t;
	return *this;
}


//------------------------------------------------------------------------------------
ExtMath::vector ExtMath::plane::projectPoint(const vector & q, const plane & pl) {
    vector qp = vector::sub(pl.p, q);
    double t = vector::dot(qp, pl.n) / vector::dot(pl.n, pl.n);
    return vector(pl.n.x * t + q.x, pl.n.y * t + q.y, pl.n.z * t + q.z);
}

ExtMath::vector ExtMath::plane::projectVector(const vector & v, const plane & pl) {
    vector q = vector::add(pl.p, v);
    vector q_project = projectPoint(q, pl);
    return vector::sub(q_project, pl.p);
}

bool ExtMath::segment::projectPoint(const vector & p, const segment & seg, vector & output) {
    vector res = projectPointOnLine(p, seg);
    // Validate z is in seg
    vector z1 = vector::sub(seg.s, res);
    vector z2 = vector::sub(seg.e, res);
    if (vector::dot(z1, z2) <= 0) {
        output = res;
        return true;
    }
    return false;
}

ExtMath::vector ExtMath::segment::projectPointOnLine(const vector & p, const segment & seg) {
    vector segVec = vector::sub(seg.e, seg.s);
    vector v = vector::sub(p, seg.s);
    vector u = vector::project(v, segVec);
    return vector::add(seg.s, u);
}

double ExtMath::segment::distance(const vector & p, const segment & seg) {
    vector out;
    bool valid = segment::projectPoint(p, seg, out);
    if (!valid) return POS_INFINITY;
    return vector::distance(p, out);
}

double ExtMath::segment::distanceSquare(const vector & p, const segment & seg) {
    vector out;
    bool valid = segment::projectPoint(p, seg, out);
    if (!valid) return POS_INFINITY;
    return vector::distanceSquare(p, out);
}

bool ExtMath::segment::isCoPlanar(const segment & s1, const segment & s2) {
    vector v1 = vector::sub(s1.s, s1.e);
    vector v2 = vector::sub(s1.s, s2.s);
    vector v3 = vector::sub(s1.s, s2.e);
    double dot = vector::dot(v1, vector::cross(v2, v3));
    return isZeroNumber(dot);
}

bool ExtMath::segment::differentSide(const segment & seg, const vector & p1, const vector & p2) {
    vector v1 = vector::sub(seg.s, p1);
    vector v2 = vector::sub(seg.e, p1);
    vector v3 = vector::sub(seg.s, p2);
    vector v4 = vector::sub(seg.e, p2);

    double dot = vector::dot(vector::cross(v1, v2), vector::cross(v3, v4));
    return dot <= 0;
}

bool ExtMath::segment::intersect(const segment & s1, const segment & s2) {
    return segment::isCoPlanar(s1, s2)
        && segment::differentSide(s1, s2.s, s2.e)
        && segment::differentSide(s2, s1.s, s1.e);
}

// a * t^2 + b * t +c
ps::ExtMath::Quadratic::Quadratic()
{
}

ps::ExtMath::Quadratic::Quadratic(double a, double b, double c)
{
	_a = a;
	_b = b;
	_c = c;
	_t = 0;
}

double ps::ExtMath::Quadratic::step(float dt)
{
	_t += dt;
	return calculate(_t, _a, _b, _c);
}

double ps::ExtMath::Quadratic::calculate(double t, double a, double b, double c)
{
	double x = a * t* t + b * t + c;
	return x;
}

void ps::ExtMath::Quadratic::reset(double a, double b, double c)
{
	_a = a;
	_b = b; 
	_c = c;
	_t = 0;
}

ExtMath::Quadratic ps::ExtMath::Quadratic::smoothTransition(double current, double target, double time)
{
	double remain = target - current;
	double a = -remain / (time * time);
	double b = 2 * remain / time;
	double c = current;
	return Quadratic(a, b, c);
}

//==========================================================
ps::ExtMath::VectorQuadratic::VectorQuadratic()
{
}

ps::ExtMath::VectorQuadratic::VectorQuadratic(vector a, vector b, vector c)
{
	_a = a;
	_b = b;
	_c = c;
	_t = 0;
}

ExtMath::vector ps::ExtMath::VectorQuadratic::calculate(double t, vector a, vector b, vector c)
{
	vector x = vector::add(vector::add(vector::multiply(t * t, a), vector::multiply(t, b)), c);
	return x;
}

ExtMath::VectorQuadratic ps::ExtMath::VectorQuadratic::smoothTransition(vector current, vector target, double time)
{
	auto remain = vector::sub(target, current);
	vector a = vector::multiply(-1 / (time * time), remain);
	vector b = vector::multiply(2 / time, remain);
	vector c = current;
	return VectorQuadratic(a, b, c);
}

//=================================================================================
// a * cos (w * t + p) + c
ps::ExtMath::Trigonometric::Trigonometric()
{
}

ps::ExtMath::Trigonometric::Trigonometric(double a, double w, double p, double c)
{
	_a = a;
	_w = w;
	_p = p;
	_c = c;
	_t = 0;
}

double ps::ExtMath::Trigonometric::step(float dt)
{
	_t += dt;
	return calculate(_t, _a, _w, _p, _c);
}

void ps::ExtMath::Trigonometric::reset()
{
	_t = 0;
}

double ps::ExtMath::Trigonometric::calculate(double t, double a, double w, double p, double c)
{
	return a * ExtMath::cos(w * t + p) + c;
}

ExtMath::Trigonometric ps::ExtMath::Trigonometric::smoothTransition(double current, double target, double time)
{
	auto s = target - current;
	auto w = ExtMath::PI / time;
	auto vm = w * s / (1 - ExtMath::cos(w * time));
	auto c = current + vm / w;

	return Trigonometric(-vm / w, w, 0, c);
}

ps::ExtMath::VectorTrigonometric::VectorTrigonometric()
{
	
}

ps::ExtMath::VectorTrigonometric::VectorTrigonometric(vector a, double w, double p, vector c)
{
	_a = a;
	_w = w;
	_p = p;
	_c = c;
	_t = 0;
}

ExtMath::vector ps::ExtMath::VectorTrigonometric::step(float dt)
{
	_t += dt;
	return calculate(_t, _a, _w, _p, _c);
	return vector();
}

ExtMath::vector ps::ExtMath::VectorTrigonometric::calculate(double t, vector a, double w, double p, vector c)
{
	ExtMath::vector x = ExtMath::vector::add(ExtMath::vector::multiply(ExtMath::cos(w * t + p), a), c);
	return x;
}

ExtMath::VectorTrigonometric ps::ExtMath::VectorTrigonometric::smoothTransition(vector current, vector target, double time)
{
	auto s = ExtMath::vector::sub(target, current);
	auto w = ExtMath::PI / time;
	auto vm = ExtMath::vector::multiply(w / (1 - ExtMath::cos(w * time)), s);
	auto c = ExtMath::vector::add(current, ExtMath::vector::multiply(1 / w, vm));

	return VectorTrigonometric(ExtMath::vector::multiply(-1 / w, vm), w, 0, c);
}

ps::ExtMath::RPTTrigonometric::RPTTrigonometric()
{
}

ps::ExtMath::RPTTrigonometric::RPTTrigonometric(Rpt a, double w, double p, Rpt c)
{
	_a = a;
	_w = w;
	_p = p;
	_c = c;
	_t = 0;
}

ExtMath::Rpt ps::ExtMath::RPTTrigonometric::step(float dt)
{
	_t += dt;
	return calculate(_t, _a, _w, _p, _c);
}

ExtMath::Rpt ps::ExtMath::RPTTrigonometric::calculate(double t, Rpt a, double w, double p, Rpt c)
{
	auto mul = ExtMath::cos(w * t + p);
	return Rpt(
		a.r * mul + c.r,
		a.p * mul + c.p,
		a.t * mul + c.t
	);
}

ExtMath::RPTTrigonometric ps::ExtMath::RPTTrigonometric::smoothTransition(Rpt current, Rpt target, double time)
{
	auto s = Rpt(
		target.r - current.r,
		target.p - current.p,
		target.t - current.t
	);
	auto w = ExtMath::PI / time; 
	auto vmMul = w / (1 - ExtMath::cos(w * time));
	auto vm = Rpt(vmMul * s.r, vmMul * s.p, vmMul * s.t);
	auto c = Rpt(
		current.r + vm.r / w,
		current.p + vm.p / w,
		current.t + vm.t / w
	);
	auto a = Rpt(
		-vm.r / w,
		-vm.p / w,
		-vm.t / w
	);
	return RPTTrigonometric(a, w, 0, c);
}


//Equation
ps::Equation::Equation()
{

}

ps::Equation::~Equation()
{

}

ps::EquationResults ps::Equation::resolveQuarticEquation(double a, double b, double c, double d, double e)
{
	if (a != 1.0)
	{
		b = b / a;
		c = c / a;
		d = d / a;
		e = e / a;
	}

	double b2 = b * b;
	double b3 = b2 * b;

	double bias = -0.25 * b;

	double p = c - 0.375 * b2;
	double q = d - 0.5 * b * c + 0.125 * b3;
	double r = e + bias * d + 0.0625 * b2 * c - 0.01171875 * b3 * b;

	EquationResults results;

	if (ExtMath::isZeroNumber(q)) {
		double delta = p * p - 4 * r;
		if (delta < 0) {
			return results;
		}
		else if (delta == 0) {
			if (p > 0) {
				return results;
			}
			else {
				double sqrt_x2 = sqrt(-0.5 * p);
				results.add(-sqrt_x2);
				results.add(sqrt_x2);
				return results;
			}
		}
		else {
			double sqrt_delta = sqrt(delta);
			double x1_2 = sqrt_delta - p;
			double x2_2 = -sqrt_delta - p;

			if (x1_2 == 0) {
				results.add(bias);
			}
			else if (x1_2 > 0) {
				double sqrt_x1_2 = sqrt(0.5 * x1_2);
				results.add(sqrt_x1_2 + bias);
				results.add(bias - sqrt_x1_2);
			}
			if (x2_2 == 0) {
				results.add(bias);
			}
			else if (x2_2 > 0) {
				double sqrt_x2_2 = sqrt(0.5 * x2_2);
				results.add(sqrt_x2_2 + bias);
				results.add(bias - sqrt_x2_2);
			}

			return results;
		}
	}

	double m = resolveSimpleCubicEquation(1, -0.5 * p, -r, 0.5 * p * r - 0.125 * q * q);

	double v1 = 2 * m - p;

	if (v1 < 0) {
		return results;
	}

	double sqrt_v1_p2 = 0.5 * sqrt(v1);

	double v2 = q / sqrt_v1_p2;
	double v3 = -2 * m - p;

	double vx12 = v3 - v2;
	double vx34 = v3 + v2;

	if (vx12 >= 0) {
		double base12 = bias + sqrt_v1_p2;
		if (vx12 == 0) {
			results.add(base12);
		}
		else {
			double sqrt_vx12_p2 = 0.5 * sqrt(vx12);
			results.add(base12 + sqrt_vx12_p2);
			results.add(base12 - sqrt_vx12_p2);
		}
	}
	if (vx34 >= 0) {
		double base34 = bias - sqrt_v1_p2;
		if (vx34 == 0) {
			results.add(base34);
		}
		else {
			double sqrt_vx34_p2 = 0.5 * sqrt(vx34);
			results.add(base34 + sqrt_vx34_p2);
			results.add(base34 - sqrt_vx34_p2);
		}
	}

	return results;
}

ps::EquationResults ps::Equation::applyNRMethodQuarticAll(EquationResults results, double a, double b, double c, double d, double e)
{
	int i, j;
	double xn, formular, derivative, x, da, db, dc, dx, pdx_1, pdx_2;
	int resultCounter = results.size;
	double *arr = results.arr;

	da = 4 * a;
	db = 3 * b;
	dc = 2 * c;

	for (j = 0; j < resultCounter; ++j) {
		pdx_1 = pdx_2 = 0;
		x = arr[j];
		for (i = 0; i < 6; ++i) {
			xn = x; // xn = x
			formular = e + d * xn; // formular = e + d*x
			derivative = d + dc * xn; // derivative = d + 2c*x

			xn *= x; // xn = x^2
			formular += c * xn; // formular = e + d*x + c*x^2
			derivative += db * xn; // derivative = d + 2c*x + 3b*x^2

			xn *= x; // xn = x^3
			formular += b * xn; // formular = e + d*x + c*x^2 + b*x^3
			derivative += da * xn; // derivative = d + 2c*x + 3b*x^2 + 4a*x^3

			formular += a * xn * x;

			if (derivative != 0) {
				dx = formular / derivative;
				if (dx == pdx_1 || dx == pdx_2) break;
				pdx_2 = pdx_1;
				pdx_1 = dx;

				x -= dx;
			}
			else break;
		}
		arr[j] = x;
	}

	return results;
}

double ps::Equation::resolveSimpleCubicEquation(double a, double b, double c, double d)
{
	if (a != 1.0) {
		b = b / a;
		c = c / a;
		d = d / a;
	}

	double b2 = b * b;
	double b3 = b2 * b;
	double delta = b2 - 3 * c;

	double x;

	if (delta == 0) {
		double inside = b3 - 27 * d;
		if (inside == 0) {
			x = -b;
		}
		else {
			x = -b + cbrt(inside);
		}
	}
	else {
		double abs_delta = (delta < 0) ? -delta : delta;
		double sqrt_delta = sqrt(abs_delta);
		double k = (4.5 * b * c - b3 - 13.5 * d) / (sqrt_delta * abs_delta);

		if (delta > 0) {
			double abs_k = (k < 0) ? -k : k;
			if (abs_k > 1) {
				double right_cbrt = sqrt(k * k - 1);
				double cube_combine = cbrt(abs_k + right_cbrt) + cbrt(abs_k - right_cbrt);
				if (abs_k == k) {
					x = sqrt_delta * cube_combine - b;
				}
				else {
					x = -sqrt_delta * cube_combine - b;
				}
			}
			else {
				x = 2 * sqrt_delta * cos(acos(k) / 3) - b;
			}
		}
		else {
			double right_cbrt = sqrt(k * k + 1);
			double cube_combine = cbrt(k + right_cbrt) + cbrt(k - right_cbrt);
			x = sqrt_delta * cube_combine - b;
		}
	}

	return x / 3;
}

ps::EquationResults ps::Equation::resolveQuadraticEquation(double a, double b, double c)
{
	double d = b * b - 4 * a * c;
	if (ExtMath::isLessThanZeroNumber(d)) {
		return EquationResults::empty();
	}

	double double_a = 2 * a;
	EquationResults results;

	if (ExtMath::isZeroNumber(d)) {
		results.add(-b / double_a);
		return results;
	}

	double sd = sqrt(d);
	results.add((-b + sd) / double_a);
	results.add((-b - sd) / double_a);
	return results;
}

const int ps::EquationResults::MAX_RESULT;

ps::EquationResults::EquationResults()
{
	size = 0;
}

void ps::EquationResults::add(double result)
{
	arr[size++] = result;
}

ps::EquationResults ps::EquationResults::empty()
{
	return EquationResults();
}

ps::ExtMath::AABB::AABB(double minX, double maxX, double minY, double maxY)
	:minX(minX), minY(minY), maxX(maxX), maxY(maxY)
{

}

