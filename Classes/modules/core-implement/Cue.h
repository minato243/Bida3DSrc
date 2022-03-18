#ifndef CUE_H_
#define CUE_H_

#include <cocos2d.h>
#include <modules/core/ps/internal/PhysicsCue.hpp>
#include <2d/CCActionInstant.h>
#include <modules/core/utils/LerpUtils.h>
#include "../core/inventory/CueInfo.h"

struct SwingParam
{
	bool delay;
	float swingCurrentInterval;
	float swingPullbackInterval;
	float swingInterval;
	cocos2d::Vec3 swingStartPos;
	CUSTOM_CALLBACK callback;
	int direction;

	SwingParam()
	{
		delay = 0;
		swingCurrentInterval = 0;
		swingPullbackInterval = 0;
		swingInterval = 0;
		swingStartPos = cocos2d::Vec3(0, 0, 0);
		callback = NULL;
		direction = 1;
	}
};

class Cue 
	: public cocos2d::Node, public ps::PhysicsCue
{
public:
	typedef std::function<void()> CueAnimationCallBack;

	static const double OFFSET_DISTANCE;
	static const double MOVE_IN_DISTANCE;
	static const double MOVE_OUT_DISTANCE;

	Cue();
	~Cue();

	virtual bool init();
	CREATE_FUNC(Cue);

	void initCue();

	void initShader(cocos2d::GLProgram *shader, int level);

	void setupShader(cocos2d::Sprite3D * cue, cocos2d::GLProgram *shader, int level);

	void loadModel(ModelPathData model);

	void cueMoveByForce(double forcePercent);

	void updateCueMoveIn(double x);

	void updateCueMoveOut(double x);

	void setAllCueModelPosition3D(cocos2d::Vec3 pos);

	void setCuePosition(ps::ExtMath::vector position);

	void updateCueRotate(ps::ExtMath::vector direction);

	void setCueDirection(const ps::ExtMath::vector& direction);

	void doShootAnimation(double force, const CueAnimationCallBack &callback);

	void doShootAnimationComplete(double force);

	void chainMoveTo3D(std::vector<float> internals, std::vector<cocos2d::Vec3> positions, std::function<void()> callback);

	void moveTo3D(float interval, cocos2d::Vec3 position, std::function<void()> callback);

	virtual void onEnter();

	void update(float dt);

	void moveTo3DUpdate(float dt);

	void cueSwingUpdate(float dt);

	void animateVisible(bool delay = false, CUSTOM_CALLBACK callback = NULL);

	void animationVisibleComplete();

	void animateInvisible(CUSTOM_CALLBACK callback = NULL);

	void animationInvisibleComplete();

	void setVisible(bool visible);

	void onChangeCameraPosition(cocos2d::Vec3 position);

	double getForceMax();

	double getMinAngle();

	double getMaxAngle();

	double getMinRadian();

	double getMaxRadian();

	const double *getAngle();

	void setCueInfo(CueInfo *cueInfo);

	void swingCueOutOfView(bool swingIn, bool delay = false, CUSTOM_CALLBACK callback = NULL);

	void onSwingCueOfViewComplete(bool swingIn);

	bool isInCueSwingAnimation();

	void resetSwingParam(bool swingIn, bool delay = false);

	void fastForwardSwing();

	void setOpacity(int opacity);

	void setLights(const cocos2d::Vec3 *lightPos, int num, cocos2d::Vec3 lightColor);

	void stopAllAnimateMovingActions();

	void setCueOffset(const ps::ExtMath::vector & offset);

	void setOffset(const ps::ExtMath::vector & offset);

private:
	cocos2d::Sprite3D* _cue;
	std::vector<cocos2d::Sprite3D *> _cues;
	cocos2d::GLProgramState *_state;
	std::map<cocos2d::Sprite3D*, cocos2d::GLProgramState *> _states;
	int _opacity;
	std::string _MRAOTexturePath;

	cocos2d::Vec3 _moveToStartPos;
	cocos2d::Vec3 _moveTo;
	double _moveToInterval;
	std::function<void()> _moveToCallback;
	double _moveToCurrentInterval;
	NumberLerp* _moveInLerp;
	NumberLerp* _moveOutLerp;
	VectorLerp* _rotateLerp;
	
	cocos2d::GLProgram* shaderPrograms[2];
	int qualityLevel;

	std::map <cocos2d::Sprite3D*, std::string> _paths;
	SwingParam _swingParams;
	CUSTOM_CALLBACK _swingCallback;

	CueAnimationCallBack _shootAnimCallback;

	CueAnimationCallBack _moveInCallback;
	CueAnimationCallBack _moveOutCallback;
	cocos2d::Action *_moveInCallbackAction;
	cocos2d::Action *_moveOutCallbackAction;

public:
	static const float SWING_OUT_TIME;
	static const float SWING_OUT_PULL_BACK_TIME;
	static const float SWING_OUT_TRANSPARENT_THRESHOLD;
	static const float SWING_OUT_TRANSPARENT_DURATION;
	static const float SWING_OUT_DELAY;
	
	static const double DEFAULT_ANGLE[2];
	static const double DEFAULT_FORCE;
};
#endif // !CUE_H_
