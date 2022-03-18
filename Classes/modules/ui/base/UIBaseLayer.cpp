#include "UIBaseLayer.h"
#include <cocos2d.h>

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;


UIBaseLayer::UIBaseLayer()
{
}

UIBaseLayer::~UIBaseLayer()
{
}

void UIBaseLayer::quickAnimationUIMove(cocos2d::Node * node, float offsetX, float offsetY, int priority)
{
	initUIShowHidePos(node, offsetX, offsetY);
	setUpUIShowAnimation(node,_inActionMap[node], _outActionMap[node], priority, _prepInActionMap[node], _prepOutActionMap[node]);
}

void UIBaseLayer::initUIShowHidePos(cocos2d::Node *node, float deltaX, float deltaY)
{
	auto hidePos = node->getPosition();
	hidePos.x += deltaX;
	hidePos.y += deltaY;
	auto showPos = node->getPosition();
	_hidePosMap.insert(pair<Node*, Vec2>(node, hidePos));
	_showPosMap.insert(pair<Node*, Vec2>(node, showPos));

	auto prepInAction = std::bind(&UIBaseLayer::setNodePosition, this, node, hidePos);
	auto prepOutAction = std::bind(&UIBaseLayer::setNodePosition, this, node, showPos);
	_prepInActionMap.insert(pair<Node*, PrepAction>(node, prepInAction));
	_prepOutActionMap.insert(pair<Node*, PrepAction>(node, prepOutAction));

	auto inAction = Sequence::create(
		EaseInOut::create(MoveTo::create(UIBaseLayer::SHOW_ANIM_TIME, showPos), 1), NULL);
	auto outAction = Sequence::create(
		EaseInOut::create(MoveTo::create(UIBaseLayer::SHOW_ANIM_TIME, hidePos), 1), NULL);

	_inActionMap.insert(pair<Node*, Action*>(node, inAction));
	_outActionMap.insert(pair<Node*, Action*>(node, outAction));
	inAction->retain();
	outAction->retain();
}

void UIBaseLayer::setUpUIShowAnimation(cocos2d::Node *node, cocos2d::Action *inAction, cocos2d::Action *outAction, int priority, PrepAction prepInAction, PrepAction PrepOutAction)
{
	if (_uiAnimationOrder.find(priority) == _uiAnimationOrder.end())
	{
		vector<Node*> nodes;
		_uiAnimationOrder.insert(pair<int, vector<Node*>>(priority, nodes));
	}
	_uiAnimationOrder[priority].push_back(node);

	if (__inActionMap.find(node) == __inActionMap.end())
	{
		map<int, Action*> actionMap;
		__inActionMap.insert(pair<Node*, map<int, Action*>>(node, actionMap));
	}
	__inActionMap[node].insert(pair<int, Action*>(priority, inAction));

	if (__outActionMap.find(node) == __outActionMap.end())
	{
		map<int, Action*> actionMap;
		__outActionMap.insert(pair<Node*, map<int, Action*>>(node, actionMap));
	}
	__outActionMap[node].insert(pair<int, Action*>(priority, outAction));

	if (__prepInActionMap.find(node) == __prepInActionMap.end())
	{
		map<int, PrepAction> actionMap;
		__prepInActionMap.insert(pair<Node*, map<int, PrepAction>>(node, actionMap));
	}
	__prepInActionMap[node].insert(pair<int, PrepAction>(priority, prepInAction));

	if (__prepOutActionMap.find(node) == __prepOutActionMap.end())
	{
		map<int, PrepAction> actionMap;
		__prepOutActionMap.insert(pair<Node*, map<int, PrepAction>>(node, actionMap));
	}
	__prepOutActionMap[node].insert(pair<int, PrepAction>(priority, PrepOutAction));
}

void UIBaseLayer::setNodePosition(cocos2d::Node *node, const cocos2d::Vec2 & pos)
{
	node->setPosition(pos);
}

void UIBaseLayer::textIgnore(cocos2d::Node *node)
{
	auto text = dynamic_cast<Text *>(node);
	if (text)
	{
		text->ignoreContentAdaptWithSize(true);
	}

	for (auto i = 0; i < node->getChildrenCount(); i++)
	{
		textIgnore(node->getChildren().at(i));
	}
}

const float UIBaseLayer::SCALE_TIME = 0.15;

const float UIBaseLayer::FADE_TIME = 0.4;

const float UIBaseLayer::SHOW_ANIM_TIME = 0.4;

const float UIBaseLayer::HIDE_ANIM_TIME = 0.4;
