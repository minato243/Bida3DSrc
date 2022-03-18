#ifndef RigModel_h__
#define RigModel_h__

#include "cocos2d.h"
#include "G3D/GSprite3D.h"

class RigModel :
	public cocos2d::Node {
public:
	RigModel();
	~RigModel();
	static RigModel* create(std::string dir, Sprite3D* model = nullptr, cocos2d::Node* parent = nullptr);
	bool init(std::string dir, Sprite3D* model = nullptr, cocos2d::Node* parent = nullptr);
public:
	cocos2d::AttachNode* getAttachNode(std::string name);
	virtual void initModel() {};
	virtual void setCameraMask(int flag);
	virtual void reset();
	virtual void setState(int state);
	virtual void repeatState(int state);
	virtual void transitionState(float time = 0.3f);
	virtual void update(float dt);
	virtual void changeState(int lastState, int state) {};
	virtual void nextState() {};
	virtual void runStateAction(cocos2d::Action* action, float delayNextState, float actionTime, float transitionTime = 0.3f);
public:
	cocos2d::Sprite3D* model;
	std::vector<cocos2d::Vec2> _boxPoints;
	float transitTime;
	float transitTimeMin;
	float _remainAnimationTime;
	bool _repeatState;
	bool _forceState;
	int _lastState;
	int _state;

	cocos2d::Action* _lastAction;
	cocos2d::Action* _queueState;
	cocos2d::Action* _currentAction;

};
#endif // RigModel_h__
