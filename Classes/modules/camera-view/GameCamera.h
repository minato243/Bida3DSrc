#ifndef GAMECAMERA_H_
#define GAMECAMERA_H_

#include <cocos2d.h>
#include <functional>
#include <modules/core/ps/internal/ExtensionMath.hpp>
#include <data/DataStruct.h>
#include <modules/core/utils/TriggerUtil.h>

struct CameraNearFar {
	double nearP;
	double farP;

	CameraNearFar(const double & nearP, const double & farP)
	{
		this->nearP = nearP;
		this->farP = farP;
	}

};

class GameCamera: public cocos2d::Node
{
public:
	typedef std::function<void(cocos2d::Vec3)> CameraChangePositionCallBack;

	GameCamera();
	~GameCamera();

	bool init();

	CREATE_FUNC(GameCamera);
	
	void updateCameraFoV();

	cocos2d::CameraFlag getCameraMask();

	void setCameraPosition(ps::ExtMath::vector position);

	void setCameraLookAt(ps::ExtMath::vector position, /* force, */ cocos2d::Vec3 lookAt = GameCamera::DEFAULT_UP);
	
	ps::ExtMath::vector windowLocationToFloorPosition(ps::ExtMath::vector location);
	
	cocos2d::Vec2 worldPositionToWindowLocation(ps::ExtMath::vector worldPos);

	void subscribe(const CameraChangePositionCallBack &callback);

	void unsubscribe(const CameraChangePositionCallBack &callback);

	void focus(float delta);

	void setPosition3D(cocos2d::Vec3 position);

	void lookAt(cocos2d::Vec3 position, cocos2d::Vec3 up = cocos2d::Vec3(0, 1, 0));

	void dispatchChangePosition(cocos2d::Vec3 position);

	float getFoVx();

	float getFoVy();

	void setFoVy(float fovy);

	void moveSpherical(double deltaPhi, double deltaTheta);

	void setCameraCoord(CamCoord camCoord);

	CamCoord getCameraCoord();

	cocos2d::Camera* getCamera();

	void transitionNearFarToIngame(float time);

	void updateNearFar(float nearPlane, float farPlane);

	void onEnter();

	void update(float dt);

private:
	cocos2d::Camera* _camera;
	float _fovy;
	float _fovx;
	float _width;
	float _height;
	CameraLookAtData _lookAt;
	CamCoord _camCoord;
	tu::PositionDispacher _positionDispatcher;
	tu::LimitedTimeTrigger *_camTrig;

	ps::ExtMath::Trigonometric _trigonometric;

public:
	static float DEFAULT_FOVy;
	static float DEFAULT_NEAR;
	static float DEFAULT_FAR;
	static cocos2d::Vec3 DEFAULT_UP;

	static const cocos2d::CameraFlag FLAG_BG_LOWEST = cocos2d::CameraFlag::USER1;
	static const cocos2d::CameraFlag FLAG_3D = cocos2d::CameraFlag::USER2;
	static const cocos2d::CameraFlag FLAG_UI = cocos2d::CameraFlag::USER3;
	static const cocos2d::CameraFlag FLAG_3D_ABOVE_UI = cocos2d::CameraFlag::USER4;
	static const cocos2d::CameraFlag FLAG_UI_ALWAYS_TOP = cocos2d::CameraFlag::USER5;
	static const cocos2d::CameraFlag FLAG_UI_HIGHEST = cocos2d::CameraFlag::USER6;
	// Don't set any view with this mask, reserved
	static cocos2d::CameraFlag FLAG_SCENE_TRANSITION;
	static const cocos2d::Vec3 UP;

	static const CameraNearFar INGAME_NEAR_FAR;
	static const CameraNearFar LOBBY_NEAR_FAR;
};


#endif // !GAMECAMERA_H_


