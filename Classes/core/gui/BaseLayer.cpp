#include "BaseLayer.h"
#include "../Constant.h"
#include "cocostudio/CCComExtensionData.h"
#include "../utils/StringUtility.h"
#include "../utils/LanguageMgr.h"
#include "modules/ui/GuiMgr.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

BaseLayer::BaseLayer()
{
	_layerAlpha = 0;
	_fog = nullptr;
	_btnTagsCount = 1000;
	_isShow = false;
	_keep = false;
}

BaseLayer::~BaseLayer()
{

}

bool BaseLayer::init(std::string id)
{
	Layer::init();

	_hasInit = false;

	_id = id;
	_layout = nullptr;
	_bg = nullptr;
	_layoutPath = "";
	_scale = -1;
	_scaleRealX = -1;
	_scaleRealY = -1;

	_layerGUI = nullptr;
	_aaPopup = false;

	_showHideAnimate = false;
	_bgShowHideAnimate = nullptr;
	_currentScaleBg = 1;

	_enableBack = false;

	Size size = cocos2d::Director::getInstance()->getVisibleSize();
	if (_scale < 0) {
		_scale = size.width / CONSTANT_WIDTH;
		_scale = (_scale > 1) ? 1 : _scale;
	}

	_scaleRealX = size.width / CONSTANT_WIDTH;
	_scaleRealY = size.height / CONSTANT_HEIGHT;

	_layerColor = Layout::create();
	_layerColor->setSize(size);
	_layerColor->setAnchorPoint(Vec2(0.5, 0.5));
	_layerColor->setPosition(Vec2(size.width / 2, size.height / 2));
	_layerColor->setBackGroundColor(Color3B(0, 0, 0));
	_layerColor->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	addChild(_layerColor);
	_layerColor->setVisible(false);

	_layerGUI = Layer::create();
	_layerGUI->setLocalZOrder(999);
	_layerGUI->setVisible(true);
	addChild(_layerGUI);


	_keyboardEvent = EventListenerKeyboard::create();
	_keyboardEvent->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event* event) {
		if (keyCode == EventKeyboard::KeyCode::KEY_BACK || keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
			this->backKeyPress();
		}
		
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_keyboardEvent, this);
	return true;
}

void BaseLayer::onEnter()
{
	Layer::onEnter();
	Size size = cocos2d::Director::getInstance()->getVisibleSize();
	setContentSize(size);
	setAnchorPoint(Vec2(0.5f, 0.5f));


	if (!_hasInit) {
		_hasInit = true;
		customizeGUI();
	}

	onEnterFinish();
	doEffect();
}

void BaseLayer::onExit()
{
	Layer::onExit();
	if (_aaPopup && _cachePopup) {
		retain();
	}
}

void BaseLayer::initWithJsonFile(std::string json)
{
	_layoutPath = json;

	auto size = Director::getInstance()->getVisibleSize();
	_layout = CSLoader::createNode(json);
	_layout->setContentSize(size);
	Helper::doLayout(_layout);
	addChild(_layout);
	initGUI();
}

void BaseLayer::initWithBinaryFile(std::string json)
{
	_layoutPath = json;
	auto size = Director::getInstance()->getVisibleSize();
	_layout = CSLoader::createNode(json);
	_layout->setContentSize(size);
	Helper::doLayout(_layout);
	addChild(_layout);
	localizedText(_layout);

	initGUI();
}

void BaseLayer::setAsPopup(bool value, bool isCache)
{
	_aaPopup = value;
	_cachePopup = isCache;

	if (value && _layerGUI) {
		_layerGUI->removeFromParent();
		_layerGUI = nullptr;
	}
}

cocos2d::ui::Button* BaseLayer::customizeButton(std::string name, int tag, cocos2d::Node* parent /*= nullptr*/)
{
	if (!_layout)
		return nullptr;

	Button* button = nullptr;
	if (parent) {
		button = (Button*)getControl(name, parent);
	}
	else {
		button = (Button*)getControl(name);
	}

	if (!button)
		return nullptr;
	button->setPressedActionEnabled(true);
	button->setTag(tag);
	button->addTouchEventListener(CC_CALLBACK_2(BaseLayer::onTouchEventHandler, this));

	return button;
}

cocos2d::ui::Button* BaseLayer::customButton(std::string name, int tag, cocos2d::Node* parent /*= nullptr*/, bool action /*= true*/)
{

	Button* btn = (Button*) getControl(name, parent);
	if (!btn) 
		return nullptr;
	btn->setPressedActionEnabled(action);
	btn->setTag(tag);
	btn->addTouchEventListener(CC_CALLBACK_2(BaseLayer::onTouchEventHandler, this));
	return btn;
}

void BaseLayer::setLabelWithLength(std::string text, cocos2d::ui::Text* label)
{
	if (label == nullptr) 
		return;

	label->setString(text);
	if (StringUtility::getLabelWidth(label) <= label->getContentSize().width) return;

	do {
		text = text.substr(0, text.size() - 1);
		label->setString(text + "...");
	} while (StringUtility::getLabelWidth(label) > label->getContentSize().width && !text.empty());
}

void BaseLayer::setLabelText(std::string text, cocos2d::ui::Text* control)
{
	std::string  str = control->getString();
	int l1 = str.size();
	int l2 = text.size();

	if (_subText.find(control) != _subText.end()) {
		l1 = _subText.at(control);
		if (l2 <= l1) {
			control->setString(text);
		}
		else {
			control->setString(text.substr(0, l1 - 2) + "...");
		}
	}
	else if (_wrapText.find(control) != _wrapText.end()) {
		float s1 = control->getContentSize().width;
		int num = text.size();
		std::string str = "";
		std::string result = "";
		for (int i = 0; i < num; i++) {
			str += text[i];
			result += text[i];
			control->setString(str);
			if (text[i] == ' ') {
				if (control->getContentSize().width > s1) {
					result += "\n";
					str = "";
				}
			}
		}
		control->setString(result);
	}
	else {
		control->setString(text);
	}
}
cocos2d::Node* BaseLayer::getControl(std::string cName, std::string parent)
{

	Node* p = Helper::seekWidgetByName((Widget*)_layout, parent);
	if (p == nullptr) 
		p = _layout->getChildByName(parent);
	return getControl(cName, parent);
}
cocos2d::Node* BaseLayer::getControl(std::string cName, cocos2d::Node* parent, std::string sParent)
{
	Node* p = nullptr;
	if (!parent) {
		p = _layout;
		sParent = "layout";
	}
	else {
		p = parent;
	}
	

	if (p == nullptr) {
		cocos2d::log("###################### getControl error parent %s/%s",cName.c_str(), sParent.c_str());
		return nullptr;
	}

	Node* control = Helper::seekWidgetByName((Widget*)p, cName);
	if (control == nullptr) 
		control = p->getChildByName(cName);
	if (control == nullptr) {
		cocos2d::log("###################### getControl error control %s/%s", cName.c_str(), sParent.c_str());
		return nullptr;
	}

	analyzeCustomControl(control);

	if (_defaultPos.find(control) == _defaultPos.end())
	{
		_defaultPos[control] = control->getPosition();
	}
	return control;
}

void BaseLayer::processScaleControl(cocos2d::Node* control, int direct /*= -1*/)
{
	if (direct == -1) {
		control->setScale(_scale);
	}
	else if (direct == 1) {
		control->setScaleX(_scale);
	}
	else {
		control->setScaleY(_scale);
	}
}

void BaseLayer::analyzeCustomControl(cocos2d::Node* control)
{
	if (!control->getComponent("ComExtensionData"))
	{
		if (control->getTag() < 0) 
		{
			processScaleControl(control);
		}
		return;
	}
	cocostudio::ComExtensionData* data = dynamic_cast<cocostudio::ComExtensionData*>(control->getComponent("ComExtensionData"));

	if (!data) {
		return;
	}

	std::string s = data->getCustomProperty();

	if (s.find("scale") > -1) // scale theo ty le nho nhat
	{
		if (s.find("scaleX") > -1) {
			processScaleControl(control, 1);
		}
		else if (s.find("scaleY") > -1) {
			processScaleControl(control, 0);
		}
		else {
			processScaleControl(control);
		}
	}

	if (s.find("subText") > -1) // set text gioi han string
	{
		Text* text = (Text*)control;
		if (text) {
			_subText[control] = text->getString().size();
		}
	}

	if (s.find("wrapText") > -1) // set text cat strign xuong dong
	{
		Text* text = (Text*)control;
		if (text) {
			_wrapText[control] = text->getString().size();
		}
	}
}

void BaseLayer::processListControl(std::string name, int num)
{
	for (int i = 0; i < num; i++) {
		getControl(StringUtils::format("%s%d", name.c_str(), i));
	}
}

void BaseLayer::setFog(bool isVisible, float alpha /*= 217*/)
{
	_layerColor->setVisible(isVisible);
	_layerColor->setTouchEnabled(isVisible);
	_layerColor->setOpacity(0);
	_layerColor->runAction(FadeTo::create(0.25f, alpha));
	_layerAlpha = alpha;
}

void BaseLayer::removeFog()
{
	_layerColor->setVisible(false);
}

void BaseLayer::enableFog()
{
	_fog = LayerColor::create(Color4B::BLACK);
	_fog->setVisible(true);
	addChild(_fog, -999);

	_listener = EventListenerTouchOneByOne::create();
	_listener->setSwallowTouches(true);

	_listener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event) {
		return true;
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_listener, this);
	_fog->runAction(FadeTo::create(0.25f, 255 * 0.85));
}

void BaseLayer::setDelayInit(float time /*= 0.35f*/)
{
	if (time < BaseLayer::TIME_APPEAR_GUI)
		time = BaseLayer::TIME_APPEAR_GUI;

	runAction(Sequence::create(
		DelayTime::create(time), 
		CallFunc::create(CC_CALLBACK_0(BaseLayer::functionDelayInit, this)),
		nullptr
	));
}

void BaseLayer::setShowHideAnimate(cocos2d::Node* parent, bool customScale /*= false*/)
{
	_showHideAnimate = true;
	if (parent == nullptr) {
		_bgShowHideAnimate = _layout;
	}
	else {
		_bgShowHideAnimate = parent;
	}

	_currentScaleBg = customScale ? _scale : 1;

	_bgShowHideAnimate->setScale(0.75f * _currentScaleBg);
	_bgShowHideAnimate->setOpacity(0);
	_bgShowHideAnimate->runAction(
		Sequence::create(
			Spawn::create(
				EaseBackOut::create(
					ScaleTo::create(0.25f, _currentScaleBg)
				), 
				FadeIn::create(0.25f),
				nullptr
			), CallFunc::create(CC_CALLBACK_0(BaseLayer::finishAnimate, this)),
			nullptr
		)
	);

	if (_layerColor) {
		cocos2d::log("_layerColor availabled");
		_layerColor->setVisible(true);
		_layerColor->runAction(FadeTo::create(0.25f, _layerAlpha));
	}

	if (_fog) {
		cocos2d::log("_fog availabled");
		_fog->setVisible(true);
		_fog->runAction(FadeTo::create(0.25f, _layerAlpha));
	}
}

void BaseLayer::onClose()
{
	if (_layerColor && _layerColor->isVisible())
		_layerColor->runAction(FadeTo::create(0.15f, 0));

	if (_fog && _fog->isVisible())
		_fog->runAction(FadeTo::create(0.15f, 0));

	if (_showHideAnimate) {
		_bgShowHideAnimate->setScale(_currentScaleBg);
		_bgShowHideAnimate->runAction(Spawn::create(ScaleTo::create(0.15f, 0.75f), FadeOut::create(0.15), nullptr));
		runAction(Sequence::create(
			DelayTime::create(0.15f), 
			CallFunc::create(CC_CALLBACK_0(BaseLayer::onCloseDone, this)),
			nullptr
		));
	}
	else {
		onCloseDone();
	}
}

void BaseLayer::onCloseDone()
{
	removeFromParent();
}

void BaseLayer::setBackEnable(bool enable)
{
	_enableBack = enable;
}

void BaseLayer::backKeyPress()
{
	if (!_enableBack) 
		return;

	onBack();
}


void BaseLayer::resetDefaultPosition(cocos2d::Node* control)
{
	if (!control) 
		return;

	if (_defaultPos.find(control) == _defaultPos.end()) {
		_defaultPos[control] = control->getPosition();
	}
	else {
		control->setPosition(_defaultPos[control]);
	}
}

void BaseLayer::onTouchEventHandler(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Button* sender = (Button*)pSender;
	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		onButtonTouched(sender, sender->getTag());
		break;
	case ui::Widget::TouchEventType::ENDED:
		onButtonRelease(sender, sender->getTag());
		break;
	case ui::Widget::TouchEventType::CANCELED:
		onButtonCanceled(sender, sender->getTag());
		break;
	}
}
cocos2d::Vec2 BaseLayer::getDefaultPos(cocos2d::Node* control)
{
	if (_defaultPos.find(control) != _defaultPos.end()) {
		return _defaultPos[control];
	}
	return control->getPosition();
}

void BaseLayer::localizedText(cocos2d::Node* node)
{
	Text* text = dynamic_cast<Text*>(node);
	if (text) {
		std::string str = text->getString();
		std::string sLocalized = "str_";
		int idxLocalized = str.find(sLocalized);
		int sizeLocalized = sLocalized.size();
		if (idxLocalized > -1)
		{
			text->setString(LocalizedString::to(str.substr(idxLocalized + sizeLocalized, str.size())));
		}
		else
		{
			if (str.find("text") == 0)
			{
				text->setString(LocalizedString::to(str));
			}
		}
	}

	Vector<Node*> childs = node->getChildren();
	for (int i = 0; i < childs.size(); i++) {
		localizedText(childs.at(i));
	}
}

void BaseLayer::autoInitComponent(cocos2d::Node *node, bool autoInitButtons)
{
	auto children = node->getChildren();

	for (int i = 0; i < children.size(); i++)
	{
		auto v = children.at(i);
		auto name = v->getName();
		_nodeMap.insert(pair<string, cocos2d::Node*>(name, v));
		_defaultPos.insert(pair<Node*, Vec2>(v, v->getPosition()));

		if (autoInitButtons)
		{
			if (name.find("btn") != -1)
			{
				if (_tagMap.find(name) != _tagMap.end())
				{
					auto tag = _tagMap[name];
					Button *btn = (Button *)v;
					btn->setPressedActionEnabled(true);
					btn->setTag(tag);
					btn->addTouchEventListener(CC_CALLBACK_2(BaseLayer::onTouchEventHandler, this));
				}
			}
		}
		
		autoInitComponent(v);
	}
}

cocos2d::Node * BaseLayer::getNodeInMap(const std::string & nodeName)
{
	if(_nodeMap.find(nodeName) != _nodeMap.end())
		return _nodeMap[nodeName];
	return NULL;
}

void BaseLayer::show()
{
	if (!_isShow) {
		_isShow = true;
		setVisible(true);
		guiMgr->show(this);
	}
}

void BaseLayer::hide()
{
	if (_isShow) {
		setVisible(false);
		_isShow = false;
		guiMgr->hide(this, _keep, this->_tag);
	}
}

void BaseLayer::refresh()
{

}

cocos2d::ui::Text* BaseLayer::createLabelText(std::string txt, cocos2d::Color3B color)
{
	Text* ret = Text::create();
	ret->setAnchorPoint(Vec2(0.5, 0.5));
	ret->setFontName("fonts/tahoma.ttf");
	ret->setFontSize(25);
	ret->setTextHorizontalAlignment(TextHAlignment::LEFT);
	ret->setString(txt);
	ret->setColor(color);
	return ret;
}

cocos2d::ui::EditBox* BaseLayer::createEditBox(cocos2d::ui::TextField* tf)
{
	EditBox* ret = EditBox::create(tf->getContentSize(), Scale9Sprite::create());
	ret->setFontName(tf->getFontName().c_str());
	ret->setFontSize(tf->getFontSize());
	ret->setPlaceHolder(tf->getPlaceHolder().c_str());
	ret->setPlaceholderFontName(tf->getFontName().c_str());
	ret->setPlaceholderFontSize(tf->getFontSize());
	ret->setPosition(tf->getPosition());
	ret->setAnchorPoint(tf->getAnchorPoint());
	ret->setReturnType(EditBox::KeyboardReturnType::DONE);
	return ret;
}

void BaseLayer::subLabelText(cocos2d::ui::Text* lb, std::string txt, std::string defaultString /*= ""*/)
{

	std::string lbStr = lb->getString();
	if (!defaultString.empty())
		lbStr = defaultString;

	int size = lbStr.size();
	if (txt.size() <= size) {
		lb->setString(txt);
	}
	else {
		lb->setString(txt.substr(0, size - 2) + "...");
	}
}

cocos2d::Vec2 BaseLayer::outsidePostion(cocos2d::Vec2 point)
{
	Size size = Director::getInstance()->getVisibleSize();
	Vec2 outPoint = Vec2(0, 0);
	Vec2 middleScreen = Vec2(size.width / 2, size.height / 2);

	outPoint.x = point.x + (point.x - middleScreen.x) / 2.5;
	outPoint.y = point.y + (point.y - middleScreen.y) / 2.5;

	return outPoint;
}

void BaseLayer::effectInOutside(cocos2d::Node* control, float time /*= 0*/)
{
	control->setPosition(BaseLayer::outsidePostion(getDefaultPos(control)));
	control->runAction(Sequence::create(
		DelayTime::create(time),
		EaseBackOut::create(MoveTo::create(1, getDefaultPos(control))),
		nullptr
	));
}

float BaseLayer::TIME_APPEAR_GUI = 0.35f;

