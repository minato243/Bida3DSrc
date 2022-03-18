#include "CameraPosition.h"
#include <modules/core/ps/internal/PhysicsConstants.hpp>
#include <data/GameConstant.h>

using namespace ps;
using namespace ps::ExtMath;

CameraPosition::CameraPosition()
{
	_rv = RestrainValues();
	_ec = ElipseCombination(ELIPSE_WIDTH, ELIPSE_HEIGHT);
	_ptbEC = ElipseCombination(ELIPSE_WIDTH + 90, ELIPSE_HEIGHT + 40);
	_br = BorderedRectangle(RECT_WIDTH, RECT_HEIGHT, RECT_RADIUS);

	_mode = CameraPosition::Mode::NORMAL;
}

CameraPosition::~CameraPosition()
{
}

void CameraPosition::setMode(Mode mode)
{
	_mode = mode;
}

void CameraPosition::setFov(double fovx, double fovy)
{
	_rv.setFov(fovx, fovy);
}

RestrainData CameraPosition::calcRestrains(ps::ExtMath::vector viewPos, double p)
{
	auto cache = _restrainCache;
	if (p == cache.p && vector::equals(viewPos, cache.rootPos)) {
		return cache.restrains;
	}
	auto dir = vector(std::cos(p), std::sin(p));
	auto low = _br.calcIntersection(viewPos, dir);
	auto ec = _mode == CameraPosition::Mode::NORMAL ? _ec : _ptbEC;
	auto tangentPoint = ec.calcTangentPoint(vector::reverse(dir));
	auto seg = segment(viewPos, vector::add(viewPos, dir));
	auto high = segment::projectPointOnLine(tangentPoint, seg);
	// Update cache
	cache.restrains = RestrainData(low, high);
	cache.p = p;
	cache.rootPos.x = viewPos.x;
	cache.rootPos.y = viewPos.y;
	cache.rootPos.z = viewPos.z;

	return cache.restrains;
}

CamCoord CameraPosition::calcCamCoord(Rpt rpt, ps::ExtMath::vector viewPos)
{
	auto rv = _rv;
	auto restrains = calcRestrains(viewPos, rpt.p);

	auto dir = vector(ExtMath::cos(rpt.p), ExtMath::sin(rpt.p), 0);

	auto maxR = vector::distance(restrains.low, viewPos);

	rv.setBeta(rpt.t);

	if (rpt.r > maxR) {
		auto stCoord = rv.calcCamCoordinate(restrains.low, restrains.high);
		auto stR = vector::distance(viewPos, restrains.low);
		auto scale = rpt.r / stR;

		auto camDir = vector::sub(stCoord.position, stCoord.lookAt);
		stCoord.position = vector::add(
			stCoord.lookAt,
			vector::multiply(scale, camDir)
		);
		return stCoord;
	}
	else {
		auto low = vector::add(viewPos, vector::multiply(rpt.r, dir));
		return rv.calcCamCoordinate(low, restrains.high);
	}
}

double CameraPosition::interpolateT(double r)
{
	if (r < MIN_RESTRAIN_LOW) {
		r = MIN_RESTRAIN_LOW;
	}
	else if (r > MAX_RESTRAIN_HIGH) {
		r = MAX_RESTRAIN_HIGH;
	}

	return MIN_INTERACT_T + (r - MIN_RESTRAIN_LOW) * INTERPOLATE_R_RATIO;
}

Rpt CameraPosition::calcCamRPT(Rpt & viewRPT, ps::ExtMath::vector & viewPos, double underPSign, OptionalOutputs * optionalOutputs)
{
	auto camRPT = Rpt(0, viewRPT.p, 0);

	auto restrains = calcRestrains(viewPos, viewRPT.p);
	auto maxR = vector::distance(restrains.low, viewPos);

	// TODO: Add constants here
	double baseT;
	if (viewRPT.t > 0.4) {
		baseT = 0;
		if (viewRPT.r < 53) {
			camRPT.p -= 0.13;
		}
	}
	else {
		baseT = MAX(0, viewRPT.t - 0.2);
	}

	if (optionalOutputs != nullptr) {
		optionalOutputs->rOutRange = false;
	}

	if (viewRPT.r == Infinity) {
		viewRPT.r = camRPT.r = maxR;
		if (viewRPT.t == Infinity) {
			camRPT.t = MAX_T;
		}
		else {
			camRPT.t = baseT + interpolateT(camRPT.r);
		}
	}
	else if (viewRPT.r == -Infinity) {
		viewRPT.r = camRPT.r = MIN_RESTRAIN_LOW;
		camRPT.t = baseT + MIN_INTERACT_T;
	}
	else if (viewRPT.r > maxR) {
		camRPT.r = maxR;
		if (viewRPT.r - maxR > SNAP_THRESHOLD) {
			camRPT.t = MAX_OVER_T;
			auto maxViewR = maxR + DOUBLE_SNAP_THRESHOLD;
			if (viewRPT.r > maxViewR) {
				viewRPT.r = maxViewR;
			}
			if (optionalOutputs) {
				optionalOutputs->rOutRange = true;
			}
		}
		else {
			camRPT.t = baseT + interpolateT(camRPT.r);
		}
	}
	else if (viewRPT.r < MIN_RESTRAIN_LOW) {
		if (underPSign == 0) {
			camRPT.r = viewRPT.r = MIN_RESTRAIN_LOW;
			camRPT.t = baseT + MIN_INTERACT_T;
		}
		else {
			if (viewRPT.r < MIN_RESTRAIN_LOW - SNAP_THRESHOLD) {
				camRPT.p = viewRPT.p + UNDER_DP * underPSign;
				camRPT.r = MIN_RESTRAIN_LOW;

				camRPT.t = MIN_INTERACT_T + UNDER_DT;

				if (viewRPT.r < MIN_VIEW_R) {
					viewRPT.r = MIN_VIEW_R;
				}
				if (optionalOutputs) {
					optionalOutputs->rOutRange = true;
				}
			}
			else {
				camRPT.r = MIN_RESTRAIN_LOW;
				camRPT.t = baseT + MIN_INTERACT_T;
			}
		}
	}
	else {
		camRPT.r = viewRPT.r;
		camRPT.t = baseT + interpolateT(camRPT.r);
	}

	return camRPT;
}

bool CameraPosition::isOverR(Rpt viewRPT, ps::ExtMath::vector viewPos)
{
	auto restrains = calcRestrains(viewPos, viewRPT.p);
	auto maxR = vector::distance(restrains.low, viewPos);

	return viewRPT.r > maxR;
}

bool CameraPosition::isUnderR(Rpt viewRPT, ps::ExtMath::vector viewPos)
{
	return viewRPT.r < MIN_RESTRAIN_LOW;
}

double CameraPosition::calcExtendedR(RestrainData restrains, Rpt rpt, ps::ExtMath::vector viewPos)
{
	auto rv = _rv;
	rv.setBeta(rpt.t);

	auto low = restrains.low;
	auto high = restrains.high;
	auto r1 = rv.calcLowRestrainDistance(low, high, viewPos, TOP_LEFT);
	auto r2 = rv.calcLowRestrainDistance(low, high, viewPos, TOP_RIGHT);
	auto r3 = rv.calcLowRestrainDistance(low, high, viewPos, BOT_LEFT);
	auto r4 = rv.calcLowRestrainDistance(low, high, viewPos, BOT_RIGHT);

	return MAX(MAX(r1, r2), MAX(r3, r4));
}

const double CameraPosition::ELIPSE_WIDTH = PhysicsConstants::TABLE_WIDTH + 120;
const double CameraPosition::ELIPSE_HEIGHT = PhysicsConstants::TABLE_HEIGHT + 120;
const double CameraPosition::RECT_WIDTH = PhysicsConstants::TABLE_WIDTH + 50;
const double CameraPosition::RECT_HEIGHT = PhysicsConstants::TABLE_HEIGHT + 50;
const double CameraPosition::RECT_RADIUS = 70;

const double CameraPosition::MIN_RESTRAIN_LOW = 15;
const double CameraPosition::MAX_RESTRAIN_HIGH = 200;
const double CameraPosition::MIN_INTERACT_T = PI / 9;
const double CameraPosition::MAX_INTERACT_T = PI / 5;
const double CameraPosition::DELTA_CAM_CUE_T_MIN = -PI / 36;
const double CameraPosition::DELTA_CAM_CUE_T_MAX = PI / 36;
const double CameraPosition::DELTA_CAM_CUE_P_HIGH = -PI / 30;

const double CameraPosition::OVER_R_T_SPEED = 0.01;
const double CameraPosition::MAX_OVER_T = 1.1;
const double CameraPosition::MIN_UNDER_R = -15;
const double CameraPosition::UNDER_DR_DP = 0.005;
const double CameraPosition::UNDER_DR_DT = 0.005;

const double CameraPosition::UNDER_DT = 0.12;
const double CameraPosition::UNDER_DP = 0.12;

const double CameraPosition::MAX_T = PI / 2 - 1e-3;

const double CameraPosition::SNAP_THRESHOLD = 30;
const double CameraPosition::DOUBLE_SNAP_THRESHOLD = SNAP_THRESHOLD * 2;

const double CameraPosition::MIN_VIEW_R = MIN_RESTRAIN_LOW - DOUBLE_SNAP_THRESHOLD;

const double CameraPosition::INTERPOLATE_R_RATIO =
(MAX_INTERACT_T - MIN_INTERACT_T) / (MAX_RESTRAIN_HIGH - MIN_RESTRAIN_LOW);

const ps::ExtMath::vector CameraPosition::TOP_LEFT 
	= ps::ExtMath::vector(-CameraPosition::RECT_WIDTH / 2, CameraPosition::RECT_HEIGHT / 2);
const ps::ExtMath::vector CameraPosition::TOP_RIGHT 
	= ps::ExtMath::vector(CameraPosition::RECT_WIDTH / 2, CameraPosition::RECT_HEIGHT / 2);
const ps::ExtMath::vector CameraPosition::BOT_LEFT 
	= ps::ExtMath::vector(-CameraPosition::RECT_WIDTH / 2, -CameraPosition::RECT_HEIGHT / 2);
const ps::ExtMath::vector CameraPosition::BOT_RIGHT 
	= ps::ExtMath::vector(CameraPosition::RECT_WIDTH / 2, -CameraPosition::RECT_HEIGHT / 2);

BorderedRectangle::BorderedRectangle(double width, double height, double radius)
{
	_w = width / 2;
	_h = height / 2;
	_a = _w - radius;
	_b = _h - radius;
	_r2 = radius * radius;
}

BorderedRectangle::BorderedRectangle()
{
}

BorderedRectangle::~BorderedRectangle()
{
}

double BorderedRectangle::calcCircleIntersection(double a, double b, ps::ExtMath::vector start, ps::ExtMath::vector dir)
{
	auto dx = start.x - a;
	auto dy = start.y - b;

	auto va = dir.x * dir.x + dir.y * dir.y;
	auto vb = dir.x * dx + dir.y * dy;
	auto vc = dx * dx + dy * dy - _r2;

	auto delta = vb * vb - va * vc;
	if (delta < 0) {
		return -0;
	}
	else {
		return (sqrt(delta) - vb) / va;
	}
}

ps::ExtMath::vector BorderedRectangle::calcIntersection(ps::ExtMath::vector start, ps::ExtMath::vector dir)
{
	auto maxT = -Infinity;
	double t, x, y;

	if (dir.y > 0) {
		// Top line
		t = (_h - start.y) / dir.y;
		if (abs(start.x + dir.x * t) <= _a) {
			maxT = t;
		}
	}
	else if (dir.y < 0) {
		// Bot line
		t = (-_h - start.y) / dir.y;
		if (abs(start.x + dir.x * t) <= _a) {
			maxT = t;
		}
	}


	if (dir.x > 0) {
		// Right
		t = (_w - start.x) / dir.x;
		if (t > maxT) {
			if (abs(start.y + dir.y * t) <= _b) {
				maxT = t;
			}
		}
	}
	else if (dir.x < 0) {
		// Left
		t = (-_w - start.x) / dir.x;
		if (t > maxT) {
			if (abs(start.y + dir.y * t) <= _b) {
				maxT = t;
			}
		}
	}

	// Circle I
	t = calcCircleIntersection(_a, _b, start, dir);
	if (t > maxT) {
		x = start.x + dir.x * t;
		y = start.y + dir.y * t;
		if (x >= _a && y >= _b) {
			maxT = t;
		}
	}

	// Circle II
	t = calcCircleIntersection(-_a, _b, start, dir);
	if (t > maxT) {
		x = start.x + dir.x * t;
		y = start.y + dir.y * t;
		if (x <= -_a && y >= _b) {
			maxT = t;
		}
	}

	// Circle III
	t = calcCircleIntersection(-_a, -_b, start, dir);
	if (t > maxT) {
		x = start.x + dir.x * t;
		y = start.y + dir.y * t;
		if (x <= -_a && y <= -_b) {
			maxT = t;
		}
	}

	// Circle IV
	t = calcCircleIntersection(_a, -_b, start, dir);
	if (t > maxT) {
		x = start.x + dir.x * t;
		y = start.y + dir.y * t;
		if (x >= _a && y <= -_b) {
			maxT = t;
		}
	}

	return vector(start.x + dir.x * maxT, start.y + dir.y * maxT);
}

// RestrainValues----------------
RestrainValues::RestrainValues()
{
	_beta = M_PI / 6;
	_fovx = M_PI / 6;
	_fovy = M_PI / 6;
	updateValues();
}

RestrainValues::RestrainValues(double beta)
{
	_beta = beta;
	_fovx = M_PI / 6;
	_fovy = M_PI / 6;
	updateValues();
}

RestrainValues::RestrainValues(double beta, double fovx, double fovy)
{
	_beta = beta;
	_fovx = fovx;
	_fovy = fovy;
	updateValues();
}

RestrainValues::~RestrainValues()
{
}

void RestrainValues::updateValues()
{
	auto alpha = _fovy / 2;
	auto theta = alpha + _beta;

	auto c_theta = cot(theta);
	auto c_beta = cot(_beta);

	_kCD = 1 / (cot(_beta - alpha) - c_theta);
	_kAM = (c_beta - c_theta) * _kCD;
	_kMD = -c_beta * _kCD;

	// Update Extended Values, Tested
	auto alp_y = alpha;
	auto alp_x = _fovx / 2;

	auto bet = _beta;
	auto gam = bet + alp_y;
	auto del = bet - alp_y;

	// TODO: Add Cache to improve performance
	// cc.log("===" + bet);

	auto sin_bet = ExtMath::sin(bet);
	auto tan_gam_rev = 1 / std::tan(gam);

	auto f_par = sin_bet * ExtMath::cos(alp_y) * std::tan(alp_x);

	auto n_AN = nAN = f_par / std::sin(gam);
	auto n_BP = nBP = f_par / std::sin(del);

	auto n_AB = nAB = sin_bet * (1 / std::tan(del) - tan_gam_rev);
	auto n_AL = nAL = std::cos(bet) - sin_bet * tan_gam_rev;

	// cc.log("nAB = " + n_AB);

	auto n_BP_AN = n_BP - n_AN;

	nJB = n_AB * n_BP / n_BP_AN;
	nJL = n_AB * n_AN / n_BP_AN + n_AL;

	nBP_nJL = nBP * nJL;
}

void RestrainValues::setBeta(double beta)
{
	if (beta == _beta) return;
	_beta = beta;
	updateValues();
	clearCache();
}

void RestrainValues::setFov(double fovx, double fovy)
{
	if (fovx == _fovx && fovy == _fovy) return;
	_fovx = fovx;
	_fovy = fovy;
	updateValues();
	clearCache();
}

CamCoord RestrainValues::calcCamCoordinate(ps::ExtMath::vector lowRestrain, ps::ExtMath::vector highRestrain)
{
	auto vAB = vector::sub(highRestrain, lowRestrain);
	auto vOM = vector(
		lowRestrain.x + _kAM * vAB.x,
		lowRestrain.y + _kAM * vAB.y
	);
	auto vOC = vector(
		vOM.x + _kMD * vAB.x,
		vOM.y + _kMD * vAB.y,
		_kCD * vector::length(vAB)
	);
	CamCoord camcoord;
	camcoord.position = vOC;
	camcoord.lookAt = vOM;
	return camcoord;
}

void RestrainValues::clearCache()
{
	//TODO
}

double RestrainValues::calcLowRestrainDistance(ps::ExtMath::vector low, ps::ExtMath::vector high, ps::ExtMath::vector viewPos, ps::ExtMath::vector T)
{
	auto U = segment::projectPointOnLine(T, segment(low, high));

	auto vAB = vector::sub(high, low);
	auto L = vector::add(low, vector::multiply(nAL / nAB, vAB));

	auto UT = vector::distance(U, T);
	auto vLU = vector::sub(U, L);
	auto LU = vector::length(vLU);

	double R;
	if (vector::dot(vAB, vLU) > 0) {
		R = (nJB * UT - nBP * LU) / nBP_nJL;
	}
	else {
		R = (nJB * UT + nBP * LU) / nBP_nJL;
	}

	auto AL = nAL * R;
	auto vLA = vector::multiply(-AL, vector::unit(vAB));
	auto A = vector::add(L, vLA);
	return vector::distance(A, viewPos);
}

ElipseCombination::ElipseCombination(double width, double height)
{
	_width = width;
	_height = height;
	_rect.topRight = vector(_width / 2, _height / 2);
	_rect.topLeft = vector(-_width / 2, _height / 2);
	_rect.botRight = vector(_width / 2, -_height / 2);
	_rect.botLeft = vector(-_width / 2, -_height / 2);

	ElipseData e = ElipseData(_width / sqrt(2), _height / sqrt(2));
	ElipseData e2 = ElipseData(_width / sqrt(3), _height);
	_elipses.push_back(e);
	_elipses.push_back(e2);
	for (int i = 0; i < _elipses.size(); i++) {
		_elipses[i].updateValues();
	}

	_elipseBounding = ElipseBoundingData(
		vector(0, _elipses[0].b),
		vector(0, -_elipses[0].b),
		vector(-_elipses[1].a, 0),
		vector(_elipses[1].a, 0)
	);

	std::vector<double> tans;
	auto w = _width;
	for (int i = 0; i < _elipses.size(); i++) {
		ElipseData elipse = _elipses[i];
		auto a = elipse.a;
		auto b = elipse.b;
		double tan = (-b / a) * (w / sqrt(4 * a * a - w * w));
		tans.push_back(tan);
	}

	//_phi 
	_phi.A2 = std::atan(tans[1]) + ExtMath::PI;
	_phi.A1 = std::atan(tans[0]) + ExtMath::PI;

	_phi.B2 = -_phi.A2 + 2 * ExtMath::PI;
	_phi.B1 = -_phi.A1 + 2 * ExtMath::PI;

	_phi.C2 = _phi.A2 + ExtMath::PI;
	_phi.C1 = _phi.A1 + ExtMath::PI;

	_phi.D2 = _phi.B2 - ExtMath::PI;
	_phi.D1 = _phi.B1 - ExtMath::PI;
}

ElipseCombination::ElipseCombination()
{
}

ElipseCombination::~ElipseCombination()
{
}

ps::ExtMath::vector ElipseCombination::calcTangentPoint(ps::ExtMath::vector normalDir)
{
	auto tanDirX = -normalDir.y;
	auto tanDirY = normalDir.x;

	auto phi = ExtMath::acos(tanDirX);
	if (tanDirY < 0) {
		phi = ExtMath::TWO_PI - phi;
	}
	if (phi == ExtMath::PI_P2) {
		return _elipseBounding.right;
	}
	else if (phi == ExtMath::PI_3P2) {
		return _elipseBounding.left;
	}
	else {
		if (ExtMath::PI_P2 < phi && phi < ExtMath::PI_3P2) {
			ElipseData elipse;
			if (phi < _phi.A2) {
				elipse = _elipses[1];
			}
			else if (phi < _phi.A1) {
				return _rect.topRight;
			}
			else if (phi < _phi.B1) {
				elipse = _elipses[0];
			}
			else if (phi < _phi.B2) {
				return _rect.topLeft;
			}
			else {
				elipse = _elipses[1];
			}
			auto tanPhi = tanDirY / tanDirX;
			auto tanPhia2 = tanPhi * elipse.a2;
			auto x = -tanPhia2 / sqrt(elipse.b2 + tanPhi * tanPhia2);
			auto y = elipse.bpa * sqrt(elipse.a2 - x * x);
			return vector(x, y);
		}
		else {
			ElipseData elipse;
			if (phi > ExtMath::PI_3P2) {
				if (phi < _phi.C2) {
					elipse = _elipses[1];
				}
				else if (phi < _phi.C1) {
					return _rect.botLeft;
				}
				else {
					elipse = _elipses[0];
				}
			}
			else {
				if (phi < _phi.D1) {
					elipse = _elipses[0];
				}
				else if (phi < _phi.D2) {
					return _rect.botRight;
				}
				else {
					elipse = _elipses[1];
				}
			}
			auto tanPhi = tanDirY / tanDirX;
			auto tanPhia2 = tanPhi * elipse.a2;
			auto x = tanPhia2 / sqrt(elipse.b2 + tanPhi * tanPhia2);
			auto y = -elipse.bpa * sqrt(elipse.a2 - x * x);
			return vector(x, y);
		}
	}
}
