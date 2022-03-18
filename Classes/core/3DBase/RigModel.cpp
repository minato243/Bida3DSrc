#include "RigModel.h"

USING_NS_CC;

RigModel::RigModel()
{

}

RigModel::~RigModel()
{
	model->stopAllActions();
}

RigModel* RigModel::create(std::string dir, Sprite3D* model, cocos2d::Node* parent)
{
	RigModel * ret = new (std::nothrow) RigModel();
	if (ret && ret->init(dir, model, parent))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

bool RigModel::init(std::string dir, Sprite3D* model, cocos2d::Node* parent)
{
	Node::init();
	if (!dir.empty()) {
		this->model = GSprite3D::create(dir);
		addChild(this->model);
	}
	else {
		this->model = model;
		this->model->retain();
		this->model->removeFromParent();
		addChild(this->model);
		this->model->release();
		parent->addChild(this);
	}
	transitTimeMin = 0.275f;
	transitTime = 0.3f;

	reset();
	initModel();

	return true;
}

cocos2d::AttachNode* RigModel::getAttachNode(std::string name)
{
	return model->getAttachNode(name);
}

void RigModel::setCameraMask(int flag)
{
	model->setCameraMask(flag);
}

void RigModel::reset()
{
	_state = -1;
	_lastState = -1;
	_lastAction = nullptr;
	_currentAction = nullptr;
	_queueState = nullptr;
	_repeatState = false;
	_forceState = false;
	_remainAnimationTime = 0;

	Animate3D::setTransitionTime(transitTime);
	scheduleUpdate();
}

void RigModel::setState(int state)
{
	_state = state;
}

void RigModel::repeatState(int state)
{
	_state = state;
	_repeatState = true;
}

void RigModel::transitionState(float time)
{
	model->stopAction(_lastAction);
	model->stopAction(_queueState);
	_lastAction = _currentAction;

	this->runAction(Sequence::create(DelayTime::create(time), CallFunc::create([this]() {
		model->stopAction(_lastAction);
	}), nullptr));
}

void RigModel::update(float dt)
{
	if (!isVisible()) return;

	_remainAnimationTime -= dt;

	if ((_state != _lastState || _repeatState)) {
		changeState(_lastState, _state);
		_lastState = _state;
		_repeatState = false;
		_forceState = false;
	}
}
void RigModel::runStateAction(cocos2d::Action* action, float delayNextState, float actionTime, float transitionTime)
{
	transitionState(transitionTime);

	_currentAction = action;
	_queueState = Sequence::create(
		DelayTime::create(delayNextState),
		CallFunc::create([this]() {
		nextState();
	}),
		nullptr
		);
	_remainAnimationTime = actionTime;

	if (_currentAction) {
		_currentAction->retain();
		model->runAction(_currentAction);
	}
	if (_queueState) {
		_queueState->retain();
		model->runAction(_queueState);
	}
}
