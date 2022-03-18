#ifndef BALL_H_
#define BALL_H_

#include <cocos2d.h>
#include <modules/core/ps/internal/ExtensionMath.hpp>
#include <modules/core/ps/internal/BallBodyV2.h>

class Ball : public cocos2d::Node	{

private:
public:
	int _number;
	ps::BallBody* _body;

	double _specular;
	double metal;
	double rough;
	cocos2d::Material* _mat;
	bool _updateRotate;
	bool _extrapolatePosition;
	double _updateDelay;
	ps::ExtMath::vector _oldPos;
	ps::ExtMath::vector _newPos;
	cocos2d::DrawNode* _draw;

	static Ball* createBall();
	static Ball* createBall(int id);
	virtual bool init();
	void initBall();
	void initShader(int level);
	void initPhysicsBody();
	void setNumber(int number);
	void initShadow();
	void initHighlight();
	void setLights(std::vector<cocos2d::Vec3> lightPositions, cocos2d::Vec3 lightColor);

	virtual void update(float dt);
	void onBodyChange();
	void setBallPosition(ps::ExtMath::vector position);
	void setBallQuaternion(ps::ExtMath::quaternion quaternion);
	void setSpecular(double val);
	void setMetallic(double val);
	void setRoughness(double val);
	void highlight(bool visibility);
	void onChangeCameraPosition(cocos2d::Vec3 position);
	// DEBUG
	void initDebugIndicator();

	ps::BallBody* body();
	CREATE_FUNC(Ball);

	void setMode2D();

	void resetRoughness();

	void setBallHighlightVisible(bool isHightLight);

private:
	cocos2d::Sprite3D* _ball;
	cocos2d::Sprite3D* _shadow;
	cocos2d::Sprite* _highlight;
	cocos2d::DrawNode* _debugIndicator;
	cocos2d::GLProgramState *_state;

	int qualityLevel;
public:
	static const int NUM_DEFAULT_LIGHT = 4;
	static const int WHITE_BALL_ID = 0;
	static const cocos2d::Vec3 DefaultLights[NUM_DEFAULT_LIGHT];
	static const cocos2d::Vec3 DefaultLightColor;
	static const bool USING_BALL_SHADER = true;
	static const cocos2d::Vec3 DefaultTableLights[2];
	static const int NUM_TABLE_ETC_LIGHT = 2;
	static const cocos2d::Vec3 TableETCLights[NUM_TABLE_ETC_LIGHT];
	static const bool DYNAMIC_SHADOW = true;

	// Rectangle light will create better environment => no need to use env map (for iOS optimize)
	static const bool RECTANGLE_LIGHT = true;
	static const double ROUGHNESS_2D;
	static const double METAL;
	static const double ROUGHNESS;
	static const double ROUGHNESS_EFFECT;
};
#endif // !BALL_H_

