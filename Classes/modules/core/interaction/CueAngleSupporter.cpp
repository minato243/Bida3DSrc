#include "CueAngleSupporter.h"
#include "../../../data/GameConstant.h"

CueAngleSupporter::CueAngleSupporter()
{
	_theta = 0;
	_minTheta = 0;
	_finalTheta = 0;

	_offsetV = 0;
	_minOffsetV = -Infinity;
	_finalOffsetV = 0;
}

CueAngleSupporter::~CueAngleSupporter()
{
	
}

void CueAngleSupporter::setTheta(double theta)
{
	_theta = theta;
	if (theta > _minTheta)
	{
		_finalTheta = theta;
	}
	else
	{
		_finalTheta = _minTheta;
	}
}

void CueAngleSupporter::setOffsetV(double offsetV)
{
	_offsetV = offsetV;
	if (offsetV > _minOffsetV) {
		_finalOffsetV = offsetV;
	}
	else {
		_finalOffsetV = _minOffsetV;
	}
}

void CueAngleSupporter::setMinValues(double minTheta, double minOffsetV)
{
	_minTheta = minTheta;
	_minOffsetV = minOffsetV;

	if (_theta > _minTheta) {
		_finalTheta = _theta;
	}
	else {
		_finalTheta = _minTheta;
	}

	if (_offsetV > _minOffsetV) {
		_finalOffsetV = _offsetV;
	}
	else {
		_finalOffsetV = _minOffsetV;
	}
}

void CueAngleSupporter::reset()
{
	_minTheta = 0;
	_minOffsetV = -Infinity;
}

const double & CueAngleSupporter::getFinalTheta()
{
	return _finalTheta;
}

const double & CueAngleSupporter::getFinalOffsetV()
{
	return _finalOffsetV;
}

