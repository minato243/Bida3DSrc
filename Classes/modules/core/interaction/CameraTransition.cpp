#include "CameraTransition.h"
#include "modules/camera-view/utils/SphericalCoordinate.h"

using namespace ps::ExtMath;
using namespace tu;
USING_NS_CC;

CameraTransition::CameraTransition()
{
	_camera = nullptr;
	_lerp = new RPTOLerp(LERP_VALUE, CC_CALLBACK_2(CameraTransition::lerpHandler, this));
	_trig = new LimitedTimeTrigger(CC_CALLBACK_2(CameraTransition::trigHandler, this));
	_cp = new CameraPosition();
	_rpt = Rpt(CameraPosition::MIN_RESTRAIN_LOW, INIT_P, CameraPosition::MIN_INTERACT_T);
	_off = vector::ZERO;

	_lerp->setCurrent(_rpt, _off);
	_lerp->setTarget(_rpt, _off);
	_lerp->fastForward();

	_2DLerp = new RPTOLerp(LERP_2D_VALUE, CC_CALLBACK_2(CameraTransition::lerp2DHandler, this));
	_2DLerp->setEnabled(false);
	_flag2DLerpCompleted = true;

	_updateListener = nullptr;

	// Entities
	_cue = nullptr; // For Setting Cue Opacity
	_currentP = 0;
	_lockP = false;

	_rOutOfRange = false;
	_listenerROutOfRange = nullptr;
}

CameraTransition::~CameraTransition()
{
	CC_SAFE_DELETE(_lerp);
	CC_SAFE_DELETE(_trig);
	CC_SAFE_DELETE(_cp);
	CC_SAFE_DELETE(_2DLerp);
}

void CameraTransition::update(float dt)
{
	_lerp->update(dt);
	//CCLOG("CameraTransition::update current (%f, %f, %d)", _lerp->_rptLerp->_current.p, _lerp->_rptLerp->_target.p);
	_trig->update(dt);
	_2DLerp->update(dt);
}

void CameraTransition::setCamera(GameCamera * camera)
{
	_camera = camera;
	_cp->setFov(camera->getFoVx(), camera->getFoVy());

	updateCameraCoord();
	_lerp->setEnabled(true);
}

void CameraTransition::setCue(Cue * cue)
{
	_cue = cue; // For setting opacity
}

void CameraTransition::transLerp(ps::ExtMath::Rpt & viewRPT, ps::ExtMath::vector & viewPos, double underPSign, CUSTOM_CALLBACK callback)
{
	_trig->disable();

	CameraPosition::OptionalOutputs optionalOutputs;
	underPSign = 1;
	auto camRPT = _cp->calcCamRPT(viewRPT, viewPos, underPSign, &optionalOutputs);

	if (camRPT.p == viewRPT.p) {
		_cue->setOpacity(CUE_OPACITY_HIGH);
	}
	else {
		_cue->setOpacity(CUE_OPACITY_LOW);
	}

	onROutRange(optionalOutputs.rOutRange);
	_lockP = optionalOutputs.rOutRange;

	_lerp->setTarget(camRPT, viewPos);
	_lerp->setEnabled(true);
	if (callback) {
		_lerp->onceCompleted(callback);
	}

	if (!_flag2DLerpCompleted) {
		_2DLerp->setEnabled(false);
		_flag2DLerpCompleted = true;
	}
}

void CameraTransition::dispatchLerpImmediately()
{
	_lerp->dispatch();
}

void CameraTransition::lerpSimplifyRadian()
{
	_lerp->simplifyRadian();
}

double CameraTransition::calcMaxViewR(ps::ExtMath::Rpt viewRPT, ps::ExtMath::vector viewPos)
{
	auto restrains = _cp->calcRestrains(viewPos, viewRPT.p);
	return vector::distance(restrains.low, viewPos);
}

void CameraTransition::enable2DCam()
{
	auto currentCoord = _camera->getCameraCoord();
	auto currentRPT = convertCoordToRPT(currentCoord);
	_2DLerp->setCurrent(currentRPT, currentCoord.lookAt);
	_2DLerp->setTarget(TARGET_2D_CAM_RPT, vector::ZERO);
	_2DLerp->setEnabled(true);
	_2DLerp->clearOnceCompletedListener();
	_lerp->setEnabled(false);
	_flag2DLerpCompleted = false;
}

void CameraTransition::release2DCam(ps::ExtMath::Rpt viewRPT, ps::ExtMath::vector viewPos, CUSTOM_CALLBACK callback)
{
	auto camRPT = _cp->calcCamRPT(viewRPT, viewPos);
	auto targetCoord = _cp->calcCamCoord(camRPT, viewPos);
	auto targetRPT = convertCoordToRPT(targetCoord);

	// TODO: Normalize p
	_lerp->setTarget(camRPT, viewPos);
	_lerp->fastForward();

	_2DLerp->setTarget(targetRPT, targetCoord.lookAt);
	_2DLerp->setEnabled(true);
	_2DLerp->onceCompleted(CC_CALLBACK_0(CameraTransition::release2DCamComplete, this));
}

void CameraTransition::release2DCamComplete()
{
	_flag2DLerpCompleted = true;
}

void CameraTransition::lerp2DHandler(ps::ExtMath::Rpt rpt, ps::ExtMath::vector off)
{
	CCLOG("CameraTransition::lerp2DHandler, RPT = (%f, %f, %f)", rpt.r, rpt.p, rpt.t);
	auto coord = convertRPTtoCoord(rpt, off);
	_camera->setCameraCoord(coord);
}

void CameraTransition::subscribeOnUpdate(CUSTOM_CALLBACK callback)
{
	if (callback == nullptr) {
		_updateListener = nullptr;
	}
	else {
		_updateListener = callback;
	}
}

void CameraTransition::dispatchUpdateListener()
{
	if (_updateListener) {
		auto listener = _updateListener;
		listener();
	}
}

void CameraTransition::onROutRange(bool outRange)
{
	if (_rOutOfRange != outRange) {
		_rOutOfRange = outRange;
		if (_listenerROutOfRange) {
			_listenerROutOfRange(!outRange);
		}
	}
}

void CameraTransition::subscribeOnROutOfRange(ROutOfRangeCallBack listener)
{
	_listenerROutOfRange = listener;
}

bool CameraTransition::isLockP()
{
	return _lockP;
}

void CameraTransition::releaseInteract(ps::ExtMath::Rpt & viewRPT, ps::ExtMath::vector viewPos)
{
	if (_cp->isOverR(viewRPT, viewPos)) {
		viewRPT.r = Infinity;
		transLerp(viewRPT, viewPos);
	}
	else if (_cp->isUnderR(viewRPT, viewPos)) {
		viewRPT.r = -Infinity;
		transLerp(viewRPT, viewPos);
	}
}

void CameraTransition::transTrig(ps::ExtMath::Rpt viewRPT, ps::ExtMath::vector viewPos, double time, CUSTOM_CALLBACK callback)
{
	CCLOG("CameraTransition::transTrig viewPos %f %f %f", viewPos.x, viewPos.y, viewPos.z);
	_lerp->setEnabled(false);

	auto camRPT = _cp->calcCamRPT(viewRPT, viewPos);

	auto targetCoord = _cp->calcCamCoord(camRPT, viewPos);
	auto currentCoord = _camera->getCameraCoord();
	makeTransTrig(currentCoord, targetCoord, time);

	_transCompleteCallback = callback;
	_trig->onceCompleted(std::bind(&CameraTransition::onTransComplete, this, camRPT, viewPos));
}

void CameraTransition::makeTransTrig(CamCoord current, CamCoord target, double time)
{
	auto data = _trig->data();

	auto currentRPT = convertCoordToRPT(current);
	auto targetRPT = convertCoordToRPT(target);

	auto deltaP = targetRPT.p - currentRPT.p;
	if (abs(deltaP) > M_PI) {
		targetRPT.p = currentRPT.p + deltaP + 2 * (deltaP < 0 ? 1 : -1) * M_PI;
	}

	data->rptTrans = RPTTrigonometric::smoothTransition(currentRPT, targetRPT, time);
	data->oTrans = VectorTrigonometric::smoothTransition(current.lookAt, target.lookAt, time);

	_trig->enable(time);
}

void CameraTransition::onTransComplete(ps::ExtMath::Rpt camRPT, ps::ExtMath::vector viewPos)
{
	_lerp->setTarget(camRPT, viewPos);
	_lerp->fastForward();
	if (_transCompleteCallback) {
		_transCompleteCallback();
	}
}

void CameraTransition::lerpHandler(ps::ExtMath::Rpt rpt, ps::ExtMath::vector viewPos)
{
	//CCLOG("CameraTransition::lerpHandler viewPos %f %f %f", viewPos.x, viewPos.y, viewPos.z);
	_off = viewPos;
	_rpt = rpt;

	updateCameraCoord();
}

void CameraTransition::trigHandler(double dt, TransitionData* data)
{
	
	auto rpt = data->rptTrans.step(dt);
	auto off = data->oTrans.step(dt);
    //CCLOG("CameraTransition::trigHandler rpt = (%f, %f, %f), off = (%f, %f, %f)", rpt.r, rpt.p, rpt.t, off.x, off.y, off.z);
	auto coord = convertRPTtoCoord(rpt, off);
	_camera->setCameraCoord(coord);
	dispatchUpdateListener();
}

void CameraTransition::updateCameraCoord()
{
	auto coord = _cp->calcCamCoord(_rpt, _off);
	_camera->setCameraCoord(coord);
	dispatchUpdateListener();
}

Rpt CameraTransition::convertCoordToRPT(const CamCoord & coord)
{
	return SphericalCoord::descartesToSpherical(vector::sub(coord.position, coord.lookAt));
}

CamCoord CameraTransition::convertRPTtoCoord(ps::ExtMath::Rpt rpt, ps::ExtMath::vector off)
{
	CamCoord coord;
	ps::ExtMath::vector v = SphericalCoord::sphericalToIngameDecartes(rpt);
	coord.position = vector::add(off, v);
	coord.lookAt = off;
	return coord;
}

const ps::ExtMath::Rpt CameraTransition::TARGET_2D_CAM_RPT = Rpt(400, M_PI / 2, M_PI / 2 - 1e-3);
const double CameraTransition::LERP_VALUE = 0.125;
const double CameraTransition:: LERP_2D_VALUE = 0.125;
const double CameraTransition::INIT_P = M_PI;
