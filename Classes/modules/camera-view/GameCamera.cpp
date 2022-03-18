#include "GameCamera.h"
#include <modules/core/ps/internal/ExtensionMath.hpp>
#include <modules/camera-view/utils/SphericalCoordinate.h>
#include <modules/utils/CocosUtils.h>
#include "SphericalCamera.h"

USING_NS_CC;
using namespace ps;
using namespace ps::ExtMath;

GameCamera::GameCamera() 
{
	_positionDispatcher = tu::PositionDispacher();
	_camTrig = new tu::LimitedTimeTrigger(NULL);
}

GameCamera::~GameCamera() 
{
	CC_SAFE_DELETE(_camTrig);
}

bool GameCamera::init() {
	if (!Node::init()) {
		return false;
	}

	auto winSize = Director::getInstance()->getVisibleSize();
	auto fovy = SphericalCoord::degreeToRadian(GameCamera::DEFAULT_FOVy);
	_fovy = fovy;
	_fovx = 2 * std::atan(std::tan(fovy / 2) * (_width / _height));

	_camera = Camera::createPerspective(
		GameCamera::DEFAULT_FOVy,
		winSize.width / winSize.height,
		GameCamera::DEFAULT_NEAR,
		GameCamera::DEFAULT_FAR
	);

	_camera->setCameraFlag(getCameraMask());
	addChild(_camera);
	lookAt(Vec3(0, 0, 0));
	//_positionDispatcher = new gv.TriggerUtils.Dispatcher(); todo

	return true;
}

void GameCamera::updateCameraFoV() {
	auto winSize = Director::getInstance()->getVisibleSize();
	_camera->initPerspective(
		SphericalCoord::radianToDegree(getFoVy()),
		winSize.width / winSize.height,
		GameCamera::DEFAULT_NEAR,
		GameCamera::DEFAULT_FAR
	);
}

CameraFlag GameCamera::getCameraMask() {
	return GameCamera::FLAG_3D;
}

void GameCamera::setCameraPosition(vector position /*, force */) {
   // if(_stopMoving && !force) return;
   // auto transform = transformVecToCocosVec(position);
   auto transform = CocosUtils::transVecToCCVec(position);
   _camera->setPosition3D(transform);
   _positionDispatcher.dispatch(transform);
}

// getPosition: function() {
// 	auto position = _camera->getPosition3D();
// 	return transformCocosVecToVec(position);
// },

void GameCamera::setCameraLookAt(ps::ExtMath::vector position, /* force, */ Vec3 lookAt) {
   // if(_stopMoving && !force) return;
   // _camera->lookAt(transformVecToCocosVec(position), lookAt);
   _camera->lookAt(CocosUtils::transVecToCCVec(position), lookAt);
}

// setFieldOfView: function(fov) {
// 	// _camera->initPerspective(
// 	// 	fov,
// 	// 	GameCamera::DEFAULT_RATIO,
// 	// 	GameCamera::DEFAULT_NEAR,
// 	// 	GameCamera::DEFAULT_FAR
// 	// );
// },

// update: function(dt) {
// 	// tick(dt);
// },


// transformCocosVecToVec: function(vec3) {
// 	return gv.ExtensionMath.vector.vec(vec3.x, -vec3.z, vec3.y);
// },

// getRayCastFromLocation: function(location){
// 	auto size = cc.director.getWinSize();
// 	auto nearPoint = _camera->unproject(size, Vec3(location.x, location.y, 0));
// 	auto farPoint = _camera->unproject(size, Vec3(location.x, location.y, 1));
// 	auto dir = Vec3Normalize(Vec3Sub(farPoint, nearPoint));

// 	return {P: nearPoint, dir: dir};
// },

// TODO: Moving this function to core
vector GameCamera::windowLocationToFloorPosition(ps::ExtMath::vector location) {
	auto winSize = Director::getInstance()->getVisibleSize();
	auto nearPoint = _camera->getPosition3D();
	auto farPoint = _camera->unproject(Vec3(
			location.x, winSize.height - location.y, 0));

	auto direction = farPoint - nearPoint;
	direction.normalize();
	auto ray = new Ray(nearPoint, direction);
	auto t = -ray->_origin.y / ray->_direction.y;
	vector pos = vector(
		ray->_origin.x + t * ray->_direction.x,
		-ray->_origin.z - t * ray->_direction.z,
		0
	);
	delete ray;
	return pos;
}

// TODO: Moving this function to core
Vec2 GameCamera::worldPositionToWindowLocation(ps::ExtMath::vector worldPos) {
	auto winSize = Director::getInstance()->getVisibleSize();
	auto glLocation = _camera->project(CocosUtils::transVecToCCVec(worldPos));
	glLocation.y = winSize.height - glLocation.y;
	return glLocation;
}

float GameCamera::getFoVx()
{
	return _fovx;
}

float GameCamera::getFoVy()
{
	return _fovy;
}

void GameCamera::setFoVy(float fovy)
{
	_fovy = fovy;
	_fovx = 2 * atan(
		tan(fovy / 2) * (_width / _height)
	);
	updateCameraFoV();
}

void GameCamera::moveSpherical(double deltaPhi, double deltaTheta)
{
	auto cameraPosition = this->_camera->getPosition3D();
	auto lookAtPosition = this->_lookAt.position;
	auto backToRootPosition = cameraPosition - lookAtPosition;

	auto sphericalPosition = SphericalCoord::descartesToSpherical(backToRootPosition);
	auto beforeP = sphericalPosition.p;
	sphericalPosition.p += deltaPhi;
	sphericalPosition.t += deltaTheta;
	sphericalPosition.t = std::min(SphericalCamera::MAX_P, std::max(SphericalCamera::MIN_P, sphericalPosition.t));

	auto newCameraPosition = SphericalCoord::sphericalToDecartes(sphericalPosition);
	auto restoreToLookAtPosition = newCameraPosition + lookAtPosition;
	this->setPosition3D(restoreToLookAtPosition);
}

void GameCamera::setCameraCoord(CamCoord camCoord)
{
	_camCoord = camCoord;
	setCameraPosition(camCoord.position);
	setCameraLookAt(camCoord.lookAt);
}

CamCoord GameCamera::getCameraCoord()
{
	return _camCoord;
}

cocos2d::Camera * GameCamera::getCamera()
{
	return _camera;
}

void GameCamera::transitionNearFarToIngame(float time)
{
	CUSTOM_CALLBACK callback = std::bind(&GameCamera::updateNearFar, this, INGAME_NEAR_FAR.nearP, INGAME_NEAR_FAR.farP);
	_camTrig->onceCompleted(callback);

	_trigonometric = Trigonometric::smoothTransition(
		GameCamera::LOBBY_NEAR_FAR.nearP,
		GameCamera::INGAME_NEAR_FAR.nearP,
		time);
	_camTrig->enable(time);
}

void GameCamera::updateNearFar(float nearPlane, float farPlane)
{
	float fovY = getFoVy();
	float fov = SphericalCoord::radianToDegree(fovY);

	auto winSize = Director::getInstance()->getWinSize();
	float ratio = winSize.width / winSize.height;
	
	_camera->initPerspective(fov, ratio, nearPlane, farPlane);
}

void GameCamera::onEnter()
{
	Node::onEnter();
	scheduleUpdate();
}

void GameCamera::update(float dt)
{
	_camTrig->update(dt);
	if (_camTrig->isEnable())
	{
		double nearP = _trigonometric.step(dt);
		updateNearFar(nearP, GameCamera::LOBBY_NEAR_FAR.farP);
	}
}

void GameCamera::subscribe(const CameraChangePositionCallBack &callback) {
	_positionDispatcher.subscribe(callback);
}

void GameCamera::unsubscribe(const CameraChangePositionCallBack &callback) {
	_positionDispatcher.unsubscribe(callback);
}

void GameCamera::focus(float delta) {
	auto cameraPosition = this->_camera->getPosition3D();
	auto lookAtPosition = this->_lookAt.position;
	auto backToRootPosition = cameraPosition - lookAtPosition;

	auto sphericalPosition = SphericalCoord::descartesToSpherical(backToRootPosition);
	// sphericalPosition.r = min(SphericalCamera.MAX_R, max(SphericalCamera.MIN_R,
	// 		sphericalPosition.r + sphericalPosition.r * delta / SphericalCamera.MAX_R
	// ));

	sphericalPosition.r = sphericalPosition.r + sphericalPosition.r * delta * 0.001;

	auto newCameraPosition = SphericalCoord::sphericalToDecartes(sphericalPosition);
	auto restoreToLookAtPosition = newCameraPosition + lookAtPosition;
	this->setPosition3D(restoreToLookAtPosition);
}

void GameCamera::setPosition3D(cocos2d::Vec3 position)
{
	this->_camera->setPosition3D(position);
	this->lookAt(this->_lookAt.position, this->_lookAt.up);
	this->dispatchChangePosition(position);
}

void GameCamera::lookAt(cocos2d::Vec3 position, cocos2d::Vec3 up) {
	this->_lookAt = CameraLookAtData{ position, up };
	this->_camera->lookAt(position, up);
}

void GameCamera::dispatchChangePosition(cocos2d::Vec3 position)
{
	_positionDispatcher.dispatch(position);
}


float GameCamera::DEFAULT_FOVy = 26;
float GameCamera::DEFAULT_NEAR = 1;
float GameCamera::DEFAULT_FAR = 10000;
Vec3 GameCamera::DEFAULT_UP = Vec3(0, 1, 0);

//const CameraFlag GameCamera::FLAG_BG_LOWEST = cocos2d::CameraFlag::USER1;
//const CameraFlag GameCamera::FLAG_3D = cocos2d::CameraFlag::USER2;
//const CameraFlag GameCamera::FLAG_UI = cocos2d::CameraFlag::USER3;
//const CameraFlag GameCamera::FLAG_3D_ABOVE_UI = cocos2d::CameraFlag::USER4;
//const CameraFlag GameCamera::FLAG_UI_ALWAYS_TOP = cocos2d::CameraFlag::USER5;
//const CameraFlag GameCamera::FLAG_UI_HIGHEST = cocos2d::CameraFlag::USER6;

// Don't set any view with this mask, reserved
CameraFlag GameCamera::FLAG_SCENE_TRANSITION = cocos2d::CameraFlag::USER6;

const CameraNearFar GameCamera::INGAME_NEAR_FAR= CameraNearFar(20, 3000);
const CameraNearFar GameCamera::LOBBY_NEAR_FAR = CameraNearFar(211, 4600);

