#include "InteractionManager.h"
#include "cocos2d.h"
#include <core/GameMgr.h>
#include <modules/core/ps/internal/PhysicsConstants.hpp>
#include <modules/core/ps/internal/ExtensionMath.hpp>
#include <modules/camera-view/utils/SphericalCoordinate.h>
#include <modules/core/utils/TimeUtils.h>
#include <cmath>

USING_NS_CC;
using namespace ps;
using namespace ps::ExtMath;
using namespace tu;


InteractionManager::InteractionManager()
	:_flagCamMode(CamModes::MODE_3D), _lastDirectionUpdateTime(0)
{
	auto size = Director::getInstance()->getVisibleSize();
	_width = size.width;
	_height = size.height;

	_cueRPT = Rpt(0, M_PI, 0);
	_viewRPT = Rpt(
		CameraPosition::MIN_RESTRAIN_LOW + 5,
		_cueRPT.p,
		_cueRPT.t
	);

	_cueLerp = SphericalLerp::makeLerp(0.125, CC_CALLBACK_1(InteractionManager::handleCueLerp, this));
	_cueBallLerp = VectorLerp::makeLerp(0.125, CC_CALLBACK_1(InteractionManager::handerCueBallLerp, this));
	_cueLerp->setEnabled(true);
	_cueBallLerp->setEnabled(true);

	//init trigger 
	_ballOutsideDetect	= TimeTrigger::makeTrigger(CC_CALLBACK_0(InteractionManager::ballOutsideDetect, this), 0.05);
	_movingOverview		= LimitedTimeTrigger::makeTrigger(CC_CALLBACK_2(InteractionManager::movingOverView, this));
	_focusOnTarget		= LimitedTimeTrigger::makeTrigger(CC_CALLBACK_2(InteractionManager::focusOnTarget, this));
	_topOverview		= TimeoutTrigger::makeTrigger(CC_CALLBACK_0(InteractionManager::topOverView, this));

	_ct = new CameraTransition();
	_cas = new CueAngleSupporter();

	_lastCueBallUpdateTime = 0;
	_camLerpWhileSetBall = false;
	_flagTurn = Turns::MY_TURN;

	_firstTurn = false;

	_lockTransition = false;
	_waitTransitionCB = NULL;
}

InteractionManager::~InteractionManager()
{
	CC_SAFE_DELETE(_ct);
	CC_SAFE_DELETE(_cas);
}

InteractionManager * InteractionManager::getInstance()
{
	if (_instance == nullptr) {
		_instance = new InteractionManager();
	}
	return _instance;
}

void InteractionManager::destroyInstance()
{
	delete _instance;
	_instance = nullptr;
}

void InteractionManager::setEntities(GameCamera *camera, Cue* cue, Ball* cueBall, IngameBaseUI *ingameUI, TouchManager *touchManager, CueAngle *cueAngle)
{
	_camera = camera;
	_cue = cue;
	_cueBall = cueBall;
	_ingameUI = ingameUI;
	_touchMgr = touchManager;
	_cueAngle = cueAngle;

	updateSpecialCameraPosition();
	subscribeTouchHandlers();

	_cueLerp->setTarget(_cueRPT);
	_cueLerp->fastForward();

	_ingameUI->subscribeToggleCamMode(CC_CALLBACK_1(InteractionManager::toggleCamMode, this));
	//_ingameUI->subscribeReleasePower(this, handleShoot);
	ingameUI->subscribeChangeCueAngle(CC_CALLBACK_1(InteractionManager::onChangeAngle, this));

	_ct->setCamera(camera);
	_ct->setCue(cue);
	_ct->subscribeOnROutOfRange([this](bool inside){
		_ingameUI->updateTurnLayout(inside);
	});


}

void InteractionManager::startShoting()
{
	_flagRunning = true;
	if (_flagTurn == Turns::MY_TURN) {
		_touchMgr->lock(_handleInteractMovingId);
		_touchMgr->lock(_handleTouchDirectionId);
		_touchMgr->lock(_handleTouchOnPocketId);
		
		_ballOutsideDetect->setEnabled(true);
	}
}

void InteractionManager::startNewMatch()
{
	resetFlagTurn();
	_firstTurn = true;
}

void InteractionManager::resetFlagTurn()
{
	_flagTurn = Turns::UNDEFINED;
}

void InteractionManager::switchTurn(Turns turn, bool isPlacingCueBall, bool isCallingPocket)
{
	CCLOG("InteractionManager::switchTurn turn = %d, isCallingPocket = %s", turn, isCallingPocket?"true":"false");
	_flagTurn = turn;
	if (turn == Turns::MY_TURN) {
		_flagCamMode = CamModes::MODE_3D;
		updateSuggestedDirection();
		if(!isCallingPocket) startFocusOnTarget();
		if(isPlacingCueBall) enablePlayingMode();
	}
	else {
		enableViewerMode();
	}
	
	if (turn == Turns::MY_TURN)
	{
		if (isCallingPocket) {
			enableCallPocketMode(isPlacingCueBall);
			enablePlayingMode();
			_ingameUI->setEnemyTurnLayout();
		}
	}

	if (!isCallingPocket)
	{
		_cue->animateVisible(false, [this]() {
			gameMgr->setEnableGuildLine(true);
			gameMgr->_table->setGuildLineVisible(true);
			gameMgr->updateBasicGuildLine();
		});
	}
	

	if (isPlacingCueBall) {
		enablePlacingMode();
	}
	else {
		_flagPlacing = false;
	}
}

void InteractionManager::startChangingCueOffset()
{
	gameMgr->_table->getWhiteBall()->_updateRotate = true;
}

void InteractionManager::stopChangingCueOffset()
{
	gameMgr->_table->getWhiteBall()->_updateRotate = false;
	notifyDirection(true);
}

void InteractionManager::stopPlacingCueBall()
{
	if (_flagPlacing) {
		_flagPlacing = false;
		_ingameUI->disablePlacingBall();
	}
}

void InteractionManager::setCueOffset(ps::ExtMath::vector offset)
{
	_cas->setOffsetV(offset.y);
	double offsetY = _cas->getFinalOffsetV();

	auto cue = gameMgr->_table->getCue();
	cue->setOffset(vector(offset.x, offsetY));
	
	gameMgr->_ingameUI->setBallIndicatorOffset(Vec2(offset.x / PhysicsConstants::BALL_RADIUS, offsetY / PhysicsConstants::BALL_RADIUS), true);
}

void InteractionManager::toggleCamMode(bool isPress)
{
	//if (_flagRunning || _flagCallPocket) return;

	if (isPress) {
		_touchMgr->lock(_handleInteractMovingId);
		_touchMgr->lock(_handleTouchDirectionId);
		_ct->enable2DCam();
		gameMgr->setBallMode2D(true);
	}
	else {
		_touchMgr->unlock(_handleInteractMovingId);
		_touchMgr->unlock(_handleTouchDirectionId);
		_ct->release2DCam(_viewRPT, cueBallLookAt());
		gameMgr->setBallMode2D(false);
	}
}

void InteractionManager::onChangeAngle(float angle)
{
	_cueRPT.t = ExtMath::clamp((double)angle, _cue->getMinRadian(), _cue->getMaxRadian());

	_cas->setTheta(_cueRPT.t);
	updateCueAngleMinValues();

	_viewRPT.t = _cueRPT.t;
	_cueLerp->setTarget(_cueRPT);
}

void InteractionManager::updateCueRPTDirection(Rpt rpt)
{
	_cue->updateDirection(ps::ExtMath::vector::reverse(SphericalCoord::direction(rpt)));
	gameMgr->updateTrajectory();
	updateUIAngle();
}

void InteractionManager::updateUIAngle()
{
	gameMgr->_ingameUI->setUIAngle(_cueRPT.t);
}

void InteractionManager::handleShoot(float percent)
{
	CCLOG("InteractionManager::handleShoot TODO");
	
	if (_flagTurn != Turns::MY_TURN) 
		return;
	gameMgr->shoot(percent);
	stopPlacingCueBall();
}

void InteractionManager::subscribeTouchHandlers()
{
	_handleInteractMovingId =_touchMgr->subscribe(CC_CALLBACK_4( InteractionManager::handleInteractMoving, this));
	_handleTouchDirectionId = _touchMgr->subscribeTouch(CC_CALLBACK_2(InteractionManager::handleTouchDirection, this));
	_handleTouchOnPocketId = _touchMgr->subscribeTouch(CC_CALLBACK_2(InteractionManager::handleTouchOnPocket, this));
	_touchMgr->lock(_handleInteractMovingId);
	_touchMgr->lock(_handleTouchDirectionId);
	_touchMgr->lock(_handleTouchOnPocketId);
}

void InteractionManager::handleInteractMoving(int event, cocos2d::Vec2 location, cocos2d::Vec2 delta, cocos2d::Vec2 velocity)
{
	if (event == TouchManager::Events::ENDED)
	{
		if (_flagCamMode == CamModes::MODE_3D)
		{
			_ct->releaseInteract(_viewRPT, cueBallLockAt());
			return;
		}
	}

	if (velocity.x == 0 && velocity.y == 0) return;

	switch (_flagCamMode) {
	case CamModes::MODE_2D: {
		// const position = _camera->windowLocationToFloorPosition(location);
		// if (event === TouchManager.Events.BEGAN) {
		// 	_i2h.init(position);
		// } else {
		// 	_i2h.dispatch(position);
		// }
		return;
	}
	case CamModes::MODE_3D: 
	{
		handleInteractMoving3D(velocity);
		break;
	}
	case CamModes::MODE_FREE: 
	{
		if (abs(velocity.x) > abs(velocity.y)) {
			_viewRPT.p -= velocity.x * DX_DELTA_MOVING;
		}
		_viewRPT.r = Infinity;
		_ct->transLerp(_viewRPT, _freeCamRoot);
		break;
	}
	}
}

void InteractionManager::handleInteractMoving3D(cocos2d::Vec2 velocity)
{
	auto lookAt = cueBallLockAt();

	if (abs(velocity.x) > abs(velocity.y)) {
		//if (_ct->isLockP())
		//	return;

		_cueRPT.p -= velocity.x * DX_DELTA_MOVING;
		_cas->setOffsetV(0);
		updateCueAngleMinValues();
		_cueLerp->setTarget(_cueRPT);

		double maxR = _ct->calcMaxViewR(_viewRPT, lookAt);
		_viewRPT.p = _cueRPT.p;

		if (_viewRPT.r < maxR)
		{
			auto newMaxR = _ct->calcMaxViewR(_viewRPT, lookAt);
			if (_viewRPT.r > newMaxR)
				_viewRPT.r = newMaxR;
		}
	}
	else {
		_viewRPT.r += velocity.y * DY_DELTA_MOVING;
		if (_viewRPT.r < 0) _viewRPT.r = 0;
	}
	_viewRPT.t = _cueRPT.t;
	_ct->transLerp(_viewRPT, lookAt);
	notifyDirection();
}

void InteractionManager::updateCueAngleMinValues()
{
	auto value = _cueAngle->calcMinValuesWithRPT(_cueRPT);
	_cas->setMinValues(value.theta, value.offsetV);

	_cueRPT.t = _cas->getFinalTheta();
	double offsetV = _cas->getFinalOffsetV();
	auto offset = _cue->getOffset();
	setCueOffset(ps::ExtMath::vector(offset.x, offsetV));
}

void InteractionManager::handleTouchDirection(cocos2d::Vec2 location, float duration)
{
	if (duration > ACCEPTABLE_TOUCH_DURATION) return;
	
	auto position = _camera->windowLocationToFloorPosition(ps::ExtMath::vector(location.x, location.y));
	if (position.x < MAX_TOUCH_X && position.x > MIN_TOUCH_X &&
		position.y < MAX_TOUCH_Y && position.y > MIN_TOUCH_Y) 
	{
		if (_flagCamMode == CamModes::MODE_3D) {
			auto dir = vector::sub(_cueBall->body()->position(), position);
			auto rpt = SphericalCoord::descartesToSpherical(dir);
			_cueRPT.p = ExtMath::simplifyRadian(_cueRPT.p);
			auto delta = rpt.p - _cueRPT.p;
			auto aDelta = abs(delta);
			if (aDelta > M_PI) {
				if (delta > 0) {
					delta = aDelta - M_PI_2;
				}
				else {
					delta = M_PI_2 - aDelta;
				}
			}
			_cueRPT.p += delta;
			_cueLerp->simplifyRadian();
			_cueLerp->setTarget(_cueRPT);

			_ct->lerpSimplifyRadian();
			_viewRPT.p = _cueRPT.p;
            auto lookAt = cueBallLockAt();
			_ct->transLerp(_viewRPT, lookAt);

			notifyDirection();
		} if (_flagCamMode == CamModes::MODE_FREE) {
			_freeCamRoot = position;
			_viewRPT.r = Infinity;
			_ct->transLerp(_viewRPT, _freeCamRoot);
		}
	}
}

void InteractionManager::handleTouchOnPocket(cocos2d::Vec2 location, float duration)
{
	CCLOG("InteractionManager::handleTouchOnPocket");
	auto pos = _camera->windowLocationToFloorPosition(vector(location.x, location.y));
	auto pocketId = gameMgr->_table->getPocketRegion(pos);
	if (pocketId > -1) {
		_pocketId = pocketId;
		gameMgr->_table->highlightPocket(pocketId);

		submitSelectPocket();
	}
}

void InteractionManager::submitSelectPocket()
{
	CCLOG("InteractionManager::submitSelectPocket _pocketId = %d", _pocketId);
	if (_pocketId > -1)
	{
		_cue->setVisible(true);
		_flagCallPocket = false;
		gameMgr->_table->highlightBalls();
		gameMgr->_table->highlightCueAndEightBall(false);
		
		gameMgr->_ebpCtrl->onCallPocketFinish(_pocketId);
		_pocketId = -1;
		gameMgr->_ingameUI->setMyTurnLayout();
		
		enablePlayingMode();
		startFocusOnTarget();
	}
}

void InteractionManager::notifyDirection(bool force)
{
	auto current = timeUtils->current();
	if (force || current - _lastDirectionUpdateTime > 500) {
		auto v = vector(_cueRPT.r, _cueRPT.p, _cueRPT.t);
		double offsetX = _cue->getOffset().x;
		double offsetY = _cue->getOffset().y;
		gameHandler->sendUpdateCueDirection(v, offsetX, offsetY);
		_lastDirectionUpdateTime = current;
	}
}

void InteractionManager::handleCueLerp(Rpt rpt)
{
	updateCueRPTDirection(rpt);
}

void InteractionManager::handerCueBallLerp(ps::ExtMath::vector pos)
{
	//CCLOG("InteractionManager::handerCueBallLerp pos(%f %f %f)", pos.x, pos.y, pos.z);
	if (!_flagPlacing) {
		_cueBall->body()->setPosition(pos); 
		gameMgr->_table->updateCuePosition();
	}
}

void InteractionManager::update(float dt)
{
	_ct->update(dt);
	_cueLerp->update(dt);
	_cueBallLerp->update(dt);

	_ballOutsideDetect->update(dt);
	//_movingOverview->update(dt);
	_topOverview->update(dt);
	//_focusOnTarget->update(dt);

}

ps::ExtMath::vector InteractionManager::cueBallLockAt()
{
	return _cueBall->body()->position();
}

void InteractionManager::updateIngamePlacingPosition()
{
	auto cueBallPosition = _cueBall->body()->position();
		auto ballLocation = _camera->worldPositionToWindowLocation(cueBallPosition);
	ballLocation.x += BALL_PLACING_OFFSET.x;
	ballLocation.y += BALL_PLACING_OFFSET.y;

	gameMgr->updateBasicGuildLine();

	// if(_moving) return;
	// const ingameUI = gameMgr.gameUI.getIngameUI();
	gameMgr->_ingameUI->setPlacingPosition(ballLocation);

}

void InteractionManager::updateSuggestedDirection()
{
	auto ball = gameMgr->getSuggestedBall();
	if (!ball) {
		
		return;
	}
	auto dir = vector::sub(_cueBall->body()->position(), ball->body()->position());
	CCLOG("dir= (%f, %f)", dir.x, dir.y);
	auto p = vector::angle2D(vector(1, 0, 0), dir);
	CCLOG("vector::angle2D %f", p / M_PI * 180);
	_cueRPT.p = p;
	_cueLerp->setTarget(_cueRPT);
	_cueLerp->fastForward();
}

void InteractionManager::updateSpecialCameraPosition()
{
	auto visibleWidth = _width - 20;
	auto visibleHeight = _height - 20;

	auto totalTableWidth = PhysicsConstants::TABLE_WIDTH + 2 * TABLE_PADDING;
	auto totalTableHeight = PhysicsConstants::TABLE_HEIGHT + 2 * TABLE_PADDING;

	auto overallCamDepth = calcMinimumDepthForRetangle(
		visibleWidth, visibleHeight,
		totalTableWidth + 10, totalTableHeight + 10
	);
	auto secondViewCamDepth = calcMinimumDepthForRetangle(
		visibleWidth, visibleHeight,
		totalTableWidth, totalTableHeight * std::sin(SECOND_VIEW_ANGLE)
	);
	auto callPocketViewCamDepth = calcMinimumDepthForRetangle(
		visibleWidth, visibleHeight,
		totalTableWidth + 30, totalTableHeight + 30
	);

	auto viewCamLength = secondViewCamDepth + totalTableHeight * std::cos(SECOND_VIEW_ANGLE) / 2;
	// 	+ calcMinimumDepthForRetangle(tableVisibleWidth, tableVisibleHeight * Math.sin(VIEW_ANGLE));
	auto y = viewCamLength * std::cos(SECOND_VIEW_ANGLE);
	auto z = viewCamLength * std::sin(SECOND_VIEW_ANGLE);

	_secondViewCamPos = vector(0, y, z);
	_topViewCamPos = vector(0, 0, overallCamDepth);
	_callPocketCamPos = vector(0, y, z);

	//
	auto fovy = _camera->getFoVy();
	_minBallViewAlpha = fovy / 6;
	_maxBallViewAlpha = fovy / 2.5;

	_safeViewZone.minX = 120;
	_safeViewZone.maxX = _width - 120;
	_safeViewZone.minY = 100;
	_safeViewZone.maxY = _height - 100;
}

double InteractionManager::calcMinimumDepthForRetangle(float visibleWidth, float visibleHeight, float tableWidth, float tableHeight)
{
	auto fovx = _camera->getFoVx();
	// auto fovy = _camera->getFoVy();

	auto dx = visibleWidth / 2;
	auto dy = visibleHeight / 2;

	auto fov_depth = _width / (2 * tan(fovx / 2));

	auto tanAlphaX = dx / fov_depth;
	auto tanAlphaY = dy / fov_depth;

	auto h1 = tableWidth / (2 * tanAlphaX);
	auto h2 = tableHeight / (2 * tanAlphaY);
	auto h = MAX(h1, h2);
	return h;
}

void InteractionManager::enable2D()
{
	//TODO
	CCLOG("enable2D TODO");
}

void InteractionManager::enable3D()
{
	_cue->setOpacity(255);
	_ingameUI->setCamMode3DLayout();
}

void InteractionManager::enableFreeCam()
{
}

void InteractionManager::enablePlayingMode()
{
	_touchMgr->unlock(_handleInteractMovingId);
	_touchMgr->unlock(_handleTouchDirectionId);
	_touchMgr->lock(_handleTouchOnPocketId);

	_cueLerp->setEnabled(true);
	_cue->setCuePosition(_cueBall->body()->position());

	updateCueRPTDirection(_cueRPT);
	gameMgr->_table->setGuildLineVisible(true);
}

void InteractionManager::startMovingOverview(CUSTOM_CALLBACK callback)
{
	CCLOG("startMovingOverview");
	lockTransition();
	_viewRPT.lr = _viewRPT.r; // Save r
	_viewRPT.r = Infinity;
	double p = findNearestAngle(_viewRPT.p, OVERVIEW_VIEWER_P_CANDIDATES, OVERVIEW_VIEWER_P_CANDIDATES_LENGHT);
	_viewRPT.p = SphericalCoord::normalizeNearestAngle(_viewRPT.p, p);
	_flagCamMode = CamModes::MODE_FREE;
	_movingOverViewCallback = callback;
	_ct->transTrig(_viewRPT, vector::ZERO, OVERVIEW_TOTAL_TIME, [&callback, this](){
		CCLOG("done moving overview");
		if (_movingOverViewCallback)
		{
			_movingOverViewCallback();
		}
		notifyWaitTransition();
	});
}

void InteractionManager::startFocusOnTarget()
{
	CCLOG("InteractionManager::startFocusOnTarget");
	if (_flagCamMode == CamModes::MODE_FREE) {
		_viewRPT.r = _viewRPT.lr; // Restore r
	}

	_viewRPT.p = _cueRPT.p;
	auto maxR = _ct->calcMaxViewR(_viewRPT, cueBallLookAt());
	if (_viewRPT.r > maxR) {
		_viewRPT.r = maxR;
	}
	_flagCamMode = CamModes::MODE_3D;
	lockTransition();

	auto focusTime = FOCUS_TOTAL_TIME;

	if (_firstTurn) {
		focusTime = FOCUS_TOTAL_TIME * 1.5;
		_firstTurn = false;
		_camera->transitionNearFarToIngame(focusTime);
	}

	_ct->transTrig(_viewRPT, cueBallLookAt(), focusTime, [this]() {
		notifyWaitTransition();
	});
}

void InteractionManager::topOverView()
{
	_flagCamMode = CamModes::MODE_2D;
	enable2D();
}

void InteractionManager::onMovingOverviewComplete()
{
	/*auto currentCamCoord = _camera->getCameraCoord();
	auto targetCamCoord = calcCameraCoord(_camRPT);

	auto currentDir = vector::sub(currentCamCoord.position, currentCamCoord.lookAt);
	auto targetDir = vector::sub(targetCamCoord.position, targetCamCoord.lookAt);

	auto currentRPT = SphericalCoord::descartesToSpherical(currentDir);
	auto targetRPT = SphericalCoord::descartesToSpherical(targetDir);
	auto deltaP = targetRPT.p - currentRPT.p;
	if (abs(deltaP) > ExtMath::PI) {
		targetRPT.p = currentRPT.p + deltaP + 2 * (deltaP < 0 ? 1 : -1) * ExtMath::PI;
	}

	auto data = _focusOnTarget->data();
	data->rptTrans = RPTTrigonometric::smoothTransition(currentRPT, targetRPT, FOCUS_TOTAL_TIME);
	data->oTrans = VectorTrigonometric::smoothTransition(currentCamCoord.lookAt, targetCamCoord.lookAt, FOCUS_TOTAL_TIME);
	_focusOnTarget->enable(FOCUS_TOTAL_TIME);
	_focusOnTarget->onceCompleted(CC_CALLBACK_0(InteractionManager::onFocusOnTargetComplete, this));*/
}

void InteractionManager::onFocusOnTargetComplete()
{
	enablePlayingMode();
	gameMgr->_table->setGuildLineVisible(true);
}

void InteractionManager::ballOutsideDetect()
{
	auto balls = gameMgr->_table->getWorld()->allBalls();
	auto camera = _camera;
	auto safeZone = _safeViewZone;

	for (auto i = 0; i < balls->size(); i++) {
		BallBody* ball = balls->at(i);
		if (ball->isMotionless()) continue;
		auto location = _camera->worldPositionToWindowLocation(ball->position());
		if (location.x < safeZone.minX ||
			location.x > safeZone.maxX ||
			location.y < safeZone.minY) 
		{
			startMovingOverview([this]() 
				{
					enableFreeCam();
				});
			_ballOutsideDetect->setEnabled(false);
			break;
		}
	}
}

void InteractionManager::movingOverView(float dt, TransitionData *data)
{
	auto rpt = data->rptTrans.step(dt);
	auto lookAt = data->oTrans.step(dt);
	Vec3 v = SphericalCoord::sphericalToDecartes(rpt);
	auto position = vector::add(lookAt, vector(v.x, v.y, v.z));
	CamCoord camCoord = CamCoord(position, lookAt);
	_camera->setCameraCoord(camCoord);
}

void InteractionManager::focusOnTarget(float dt, TransitionData * data)
{
	auto rpt = data->rptTrans.step(dt);
	auto lookAt = data->oTrans.step(dt);
	Vec3 v = SphericalCoord::sphericalToDecartes(rpt);
	auto position = vector::add(lookAt, vector(v.x, v.y, v.z));
	CamCoord camCoord = CamCoord(position, lookAt);
	_camera->setCameraCoord(camCoord);
}

GameCamera * InteractionManager::getCamera()
{
	return _camera;
}

void InteractionManager::setSelectingPocket(bool isSelecting)
{
	_selectingPocket = isSelecting;
}

void InteractionManager::enableCallPocketMode(bool placing)
{
	_flagCallPocket = true;
	_flagCamMode = CamModes::MODE_CALL_POCKET;

	// _focusOnTarget.disable();
	// enableCamCallPocket();

	// _cue.setOpacity(_flagTurn === Turns.MY_TURN ? 100 : 255);
	_cue->setVisible(false);
	_cueLerp->fastForward();
	_touchMgr->cancelLockUtilRelease();

	_touchMgr->lock(_handleTouchDirectionId);
	_touchMgr->lock(_handleInteractMovingId);
	_touchMgr->unlock(_handleTouchOnPocketId);

	auto table = gameMgr->_table;
	table->highlightPocket();
	table->hightLightAllBalls();
	table->highlightCueAndEightBall(true);

	_ct->enable2DCam();
	_ingameUI->setCallPocketLayout(placing);
}

void InteractionManager::enablePlacingMode()
{
	_flagPlacing = true;
	_ingameUI->enablePlacingBall(Vec2(0,0), CC_CALLBACK_2(InteractionManager::handleChangePlacingBall, this));
	_ct->subscribeOnUpdate(CC_CALLBACK_0(InteractionManager::updateIngamePlacingPosition, this));
	updateIngamePlacingPosition();
}

void InteractionManager::enableViewerMode()
{
	_cueLerp->setEnabled(false);

	_ingameUI->setEnemyTurnLayout();
	_touchMgr->lock(_handleTouchOnPocketId);

	if (_flagCamMode != CamModes::MODE_FREE) {
		_touchMgr->lockUntilReleaseIfTouching();
		startMovingOverview([this](){
			_touchMgr->cancelLockUtilRelease();
		});
	}
}

void InteractionManager::restorePlayingMode()
{
	updateSuggestedDirection();
	switch (_flagCamMode) {
	case CamModes::MODE_2D:
		enablePlayingMode();
		break;
	case CamModes::MODE_3D:
		startFocusOnTarget();
		break;
	case CamModes::MODE_FREE:
		_flagCamMode = CamModes::MODE_3D;
		startFocusOnTarget();
		break;
	}
}

void InteractionManager::handleChangePlacingBall(Vec2 location, bool finish)
{
	CCLOG("handleChangePlacingBall location (%f %f), finish %s", location.x, location.y, finish?"true":"false");
	if (_flagCamMode != CamModes::MODE_3D) return;
	
	auto position = _camera->windowLocationToFloorPosition(vector(
			location.x - BALL_PLACING_OFFSET.x,
			location.y - BALL_PLACING_OFFSET.y
		));
	if (!gameMgr->_table->isCueBallPosValid(position)) return;
	gameMgr->_ebpCtrl->placingCueBallPosition(position);

	_cue->setCuePosition(position);
	gameMgr->updateBasicGuildLine();

	auto current = timeUtils->current();
	if (current - _lastCueBallUpdateTime > 500) {
		gameHandler->sendSetBall(0, position, false);
		_lastCueBallUpdateTime = current;
	}
	
	if (finish)
	{
		vector ballPos = cueBallLookAt();
		auto maxR = _ct->calcMaxViewR(_viewRPT, ballPos);
		if (_viewRPT.r > maxR)
			_viewRPT.r = maxR;
		_ct->transLerp(_viewRPT, ballPos);
	}
}

void InteractionManager::handleChangePlacingBallNoLocation()
{
	if (_flagCamMode != CamModes::MODE_3D) return;

		//if (location) { // Moving
		//	const position = _camera.windowLocationToFloorPosition(vector.vec(
		//		location.x - BALL_PLACING_OFFSET.x,
		//		location.y - BALL_PLACING_OFFSET.y
		//	));
		//	if (!gameMgr.table.isCueBallPosValid(position)) return;
		//	gameMgr.ebpCtrl.placingCueBallPosition(position);

		//	_cue.setCuePosition(position);
		//	gameMgr.updateBasicGuildLine();
		//	// _ct.transLerp(_viewRPT, position); // Buggy

		//	const current = gameMgr.timeUtils.current();
		//	if (current - _lastCueBallUpdateTime > 500) {
		//		gv.gameHandler.sendSetBall(0, position, false);
		//		_lastCueBallUpdateTime = current;
		//	}
		//}

		auto ballPos = cueBallLookAt();
		auto maxR = _ct->calcMaxViewR(_viewRPT, ballPos);
		if (_viewRPT.r > maxR) {
			_viewRPT.r = maxR;
		}
		_ct->transLerp(_viewRPT, ballPos); // Buggy
		
}

void InteractionManager::receiveCueDirection(const ps::ExtMath::vector &  direction)
{
	_cueLerp->setEnabled(true);
	Rpt rpt = Rpt(direction.x, direction.y, direction.z);
	_cueLerp->setTarget(rpt);
}

void InteractionManager::receiveCueBallPosition(const ps::ExtMath::vector & pos)
{
	_cueBallLerp->setEnabled(true);
	_cueBallLerp->setTarget(pos);
}

void InteractionManager::receiveStopInterpolateCueBall(const ps::ExtMath::vector & pos)
{
	_cueBallLerp->setTarget(pos);
	_cueBallLerp->fastForward();
	_cueBallLerp->setEnabled(false);
}

void InteractionManager::doMovingMili(float d)
{
	Vec2 velocity = Vec2(MOVING_MILI_VELOCITY, 0);

	if (d < 0) {
		velocity.x *= -1;
	}
	handleInteractMoving(0, Vec2(0,0), Vec2(0,0), velocity);
}

ps::ExtMath::vector InteractionManager::cueBallLookAt()
{
	return _cueBall->body()->position();
}

double InteractionManager::findNearestAngle(const double & current, const double candidates[], int length)
{
	double candidate = candidates[0];
	double angleDis = abs(SphericalCoord::normalizeNearestAngle(current, candidate) - current);

	for (int i = 1; i < length; i++)
	{
		double angle = candidates[i];
		double distance = abs(SphericalCoord::normalizeNearestAngle(current, candidate) - current);
		if (distance < angleDis)
		{
			distance = angleDis;
			candidate = angle;
		}
	}

	return candidate;
}

InteractionManager *InteractionManager::_instance = nullptr;

void InteractionManager::lockTransition()
{
	//TODO
}

void InteractionManager::notifyWaitTransition()
{
	CCLOG("notifyWaitTransition Todo");
	auto callback = _waitTransitionCB;
	_lockTransition = false;
	if (!callback) return;
	_waitTransitionCB = NULL;
	if(callback) callback();
}

void InteractionManager::setGhostBallPosition(const vector & pos)
{

}

void InteractionManager::setGhostBallPosition(const vector& pos, const vector& dir)
{

}

const double InteractionManager::ACCEPTABLE_TOUCH_DURATION = 0.15;
const double InteractionManager::MIN_TOUCH_X = -PhysicsConstants::TABLE_WIDTH / 2;
const double InteractionManager::MAX_TOUCH_X = PhysicsConstants::TABLE_WIDTH / 2;
const double InteractionManager::MIN_TOUCH_Y = -PhysicsConstants::TABLE_HEIGHT / 2;
const double InteractionManager::MAX_TOUCH_Y = PhysicsConstants::TABLE_HEIGHT / 2;
const double InteractionManager:: MIN_RESTRAIN_LOW = 15;
const double InteractionManager:: MAX_RESTRAIN_HIGH = 200;

const double InteractionManager::DX_DELTA_MOVING = 1e-5 * M_PI;
const double InteractionManager::DY_DELTA_MOVING = 0.005;
const double InteractionManager::DY_DELTA_BETA = 1e-5 * M_PI;
const double InteractionManager::MIN_INTERACT_T = M_PI / 9;
const double InteractionManager::MAX_INTERACT_T = M_PI / 5;
const double InteractionManager::FREE_CAM_MIN_BETA = M_PI / 6;
const double InteractionManager::FREE_CAM_MAX_BETA = M_PI / 4;

const double InteractionManager::DELTA_CAM_CUE_T_MIN = -M_PI / 36;
const double InteractionManager::DELTA_CAM_CUE_T_MAX = M_PI / 36;
const double InteractionManager::DELTA_CAM_CUE_P_HIGH = -M_PI / 30;

const double InteractionManager::ELIPSE_WIDTH = PhysicsConstants::TABLE_WIDTH + 120;
const double InteractionManager::ELIPSE_HEIGHT = PhysicsConstants::TABLE_HEIGHT + 120;
const double InteractionManager::RECT_WIDTH = PhysicsConstants::TABLE_WIDTH + 50;
const double InteractionManager::RECT_HEIGHT = PhysicsConstants::TABLE_HEIGHT + 50;
const double InteractionManager::RECT_RADIUS = 70;

const double InteractionManager::TABLE_PADDING = 20;
const double InteractionManager::SECOND_VIEW_ANGLE = M_PI / 4;

const double InteractionManager::TARGET_THETA_OVERVIEW = M_PI / 6;
const double InteractionManager::OVERVIEW_TOTAL_TIME = 1.5;
const double InteractionManager::OVER_TO_TOP_TIME = 2;

const double InteractionManager::FOCUS_TOTAL_TIME = 1;

const double InteractionManager::THRESH_HOLD_TOP_VIEW = 200 * 200;

const cocos2d::Vec3 InteractionManager::LOOKAT_2D_UP = Vec3(0, 0, 1);

double const InteractionManager::OVERVIEW_VIEWER_P_CANDIDATES[OVERVIEW_VIEWER_P_CANDIDATES_LENGHT] = {0, ExtMath::PI };

const ExtMath::vector InteractionManager::BALL_PLACING_OFFSET = ExtMath::vector(150, 120);



