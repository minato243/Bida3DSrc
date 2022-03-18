#ifndef BallPocket3DEffect_h_
#define BallPocket3dEffect_h_

#include <cocos2d.h>


class BallPocket3DEffect
	:public cocos2d::Sprite3D
{
public:
	BallPocket3DEffect();
	~BallPocket3DEffect();

	static BallPocket3DEffect *create(int num);
	bool init(int num);

	bool initShader();

	void play();

	void setOpacity(int opacity);

	void update(float dt);

	void updateRotationByCamera();

private:
	int _ballId;

	cocos2d::GLProgramState *_state;

	cocos2d::Vec3 _startPos;
	float _time;
	float _flyUpTime;
	float _flyDownTime;
	float _stayTime;
	float _totalTime;
	bool _isPlaying;
};
#endif
