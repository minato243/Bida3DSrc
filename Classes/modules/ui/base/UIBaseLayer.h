#ifndef UIBaseLayer_h_
#define UIBaseLayer_h_

#include "../../../core/gui/BaseLayer.h"
#include <functional>

class UIBaseLayer
	: public BaseLayer
{
public:
	typedef std::function<void()> PrepAction;

	UIBaseLayer();
	~UIBaseLayer();

	void quickAnimationUIMove(cocos2d::Node * node, float offsetX, float offsetY, int priority);

	void initUIShowHidePos(cocos2d::Node *node, float deltaX, float deltaY);

	void setUpUIShowAnimation(cocos2d::Node *node, cocos2d::Action *inAction, cocos2d::Action *outAction, int priority, PrepAction prepInaction, PrepAction PrepOutAction);

	void setNodePosition(cocos2d::Node *node, const cocos2d::Vec2 & pos);

	virtual void initGUI() {};

	void textIgnore(cocos2d::Node *node);

public:
	

private:
	std::map<cocos2d::Node *, cocos2d::Vec2> _hidePosMap;
	std::map<cocos2d::Node *, cocos2d::Vec2> _showPosMap;

	std::map<cocos2d::Node *, cocos2d::Action *> _inActionMap;
	std::map<cocos2d::Node *, cocos2d::Action *> _outActionMap;

	std::map<cocos2d::Node *, PrepAction> _prepInActionMap;
	std::map<cocos2d::Node *, PrepAction> _prepOutActionMap;

	std::map<int, std::vector<cocos2d::Node*>> _uiAnimationOrder;

	std::map<cocos2d::Node*, std::map<int, cocos2d::Action *>> __inActionMap;
	std::map<cocos2d::Node*, std::map<int, cocos2d::Action *>> __outActionMap;
	std::map<cocos2d::Node*, std::map<int, PrepAction>> __prepInActionMap;
	std::map<cocos2d::Node*, std::map<int, PrepAction>> __prepOutActionMap;

public:
	static const float SCALE_TIME;
	static const float FADE_TIME;
	static const float SHOW_ANIM_TIME;
	static const float HIDE_ANIM_TIME;
};

#endif