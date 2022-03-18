#include "SceneMgr.h"
#include "Waiting.h"
#include "../Constant.h"
#include "login/LoginScene.h"
#include "../Config.h"
#include "../GameMgr.h"
#include "lobby/CheckLogic.h"
#include "../utils/StringUtility.h"
#include "login/LoadingScene.h"
#include <modules/scene/loading/LoadingSceneNew.h>
#include "modules/ui/inventory/cue_collection/CueCollectionUI.h"
#include "modules/scene/GameScene.h"
#include "modules/ui/shop/ShopScene.h"
#include "modules/ui/global/DebugNode.h"
#include "modules/ui/inventory/CueInspectScene.h"

USING_NS_CC;
using namespace cocos2d::ui;
const int LOADING_TAG = 99999998;
const int WAITING_TAG = 99999997;

const int TOAST_FLOAT_TAG = 99999998;
const int LOADING_FLOAT_TAG = 99999999;


SceneMgr::SceneMgr()
{
	ccWhite = Color3B(203, 204, 206);
	ccYellow = Color3B(251, 212, 93);
	ccGreen = Color3B(9, 212, 9);
	ccBlue = Color3B(132, 140, 220);

	curGui = "";

	layerGUI = nullptr;
	layerSystem = nullptr;

	ignoreGuis.push_back("ChatScene");  // default scene not cache
}

SceneMgr::~SceneMgr()
{

}

SceneMgr* SceneMgr::getInstance()
{
	if (!_instance) {
		_instance = new SceneMgr();
	}
	return _instance;
}

BaseScene* SceneMgr::getRunningScene()
{
	Scene* currentScene = Director::getInstance()->getRunningScene();
	return (BaseScene*)currentScene;
}

cocos2d::Node* SceneMgr::getMainLayer()
{
	BaseScene* curScene = getRunningScene();
	if (!curScene) return nullptr;
	if (!curScene->getMainLayer()) return nullptr;
	return curScene->getMainLayer();
}

LoadingGui* SceneMgr::addLoading(std::string text, bool fog)
{
	Node* oldLoading = layerGUI->getChildByTag(LOADING_TAG);
	if (oldLoading) {
		oldLoading->stopAllActions();
		oldLoading->removeFromParent();
	}

	LoadingGui* loading = LoadingGui::create(text, fog);
	layerGUI->addChild(loading);

	loading->setLocalZOrder(LOADING_TAG);
	loading->setTag(LOADING_TAG);
	return loading;
}

void SceneMgr::clearLoading()
{
	Node* loading = layerGUI->getChildByTag(LOADING_TAG);
	if (loading) {
		loading->stopAllActions();
		loading->removeFromParent();
	}
}

void SceneMgr::addWaiting()
{
	Node* oldLoading = layerGUI->getChildByTag(WAITING_TAG);
	if (oldLoading) {
		oldLoading->stopAllActions();
		oldLoading->removeFromParent();
	}

	Waiting* loading = Waiting::create();
	layerGUI->addChild(loading);

	loading->setLocalZOrder(WAITING_TAG);
	loading->setTag(WAITING_TAG);
}

void SceneMgr::clearWaiting()
{
	Node* loading = layerGUI->getChildByTag(WAITING_TAG);
	if (loading) {
		loading->stopAllActions();
		loading->removeFromParent();
	}
}

void SceneMgr::updateCurrentGUI()
{
	BaseLayer* gui = (BaseLayer*)getRunningScene()->getMainLayer();
	gui->onUpdateGUI();
}

void SceneMgr::addQueueScene(std::string layer)
{
	for (int i = 0, size = backScenes.size(); i < size; i++) {
		if (backScenes[i] == layer) return;
	}

	backScenes.push_back(layer);
}

std::string SceneMgr::getQueueScene(bool direct /*= false*/)
{
	if (direct) {
		backScenes.clear();
	}

	if (backScenes.size() <= 0) 
		return "";

	int sIdx = backScenes.size() - 1;
	std::string sLayer = backScenes[sIdx];
	backScenes.pop_back();

	return sLayer;
}

BaseLayer* SceneMgr::openScene(std::string layerName, std::string callback /*= ""*/, bool direct /*= false*/, bool cache /*= false*/)
{
	bool isCallback = true;
	if (layerName.compare(LoginScene::className) == 0) {
		isCallback = false;
	}

	if (layerName.compare(curGui) == 0) {
		return nullptr;
	}

	if (layerGUI && layerGUI->getParent()) {
		layerGUI->removeAllChildren();
		layerGUI->retain();
	}

	if (layerSystem && layerSystem->getParent()) {
		layerSystem->removeAllChildren();
		layerSystem->retain();
	}


	if (isCallback) {
		std::string backLayer = getQueueScene(direct);
		if (!backLayer.empty()) {
			layerName = backLayer;
		}

		if (!callback.empty()) {
			addQueueScene(callback);
		}
	}
	else {
		backScenes.clear();
	}

	BaseLayer* curLayer = nullptr;
	if (arGuis.find(curGui) != arGuis.end()) {
		arGuis[curGui]->retain();
	}

	curGui = layerName;
	bool isCache = false;
	if (cache) 
		isCache = true;
	for (int i = 0; i < ignoreGuis.size(); i++) {
		if (ignoreGuis[i].compare(layerName) == 0) {
			isCache = false;
			break;
		}
	}
	if (cache == false) isCache = false;

	if (isCache) {
		if (arGuis.find(layerName) != arGuis.end()) {
			curLayer = arGuis[layerName];
		}
		else {
			curLayer = createLayerFromName(layerName);
			arGuis[layerName] = curLayer;
		}
	}
	else {
		curLayer = createLayerFromName(layerName);
		arGuis[layerName] = curLayer;
	}

	
	_curSceneTag = curLayer->getTag();
	BaseScene* scene = BaseScene::createWithLayer(curLayer);

	Director::getInstance()->replaceScene(scene);

	return curLayer;
}

BaseLayer* SceneMgr::openGUI(std::string slayer, int zoder, int tag, bool isCache)
{
	BaseLayer* layer = nullptr;

	if (isCache) {
		if (arPopups.find(slayer) != arPopups.end()) {
			layer = arPopups[slayer];
		}
		else {
			std::string _class = getClassGUI(slayer);
			layer = createLayerFromName(slayer);
			arPopups[slayer] = layer;
		}
	}
	else {
		std::string _class = getClassGUI(slayer);
		layer = createLayerFromName(slayer);
	}

	if (layer != nullptr) {
		if (layer->getParent()) {
			layer->removeFromParent();
		}
	}

	if (layer) {
		layer->setAsPopup(true, isCache);
		layerGUI->addChild(layer, zoder, tag);
	}
	return layer;
}
std::string SceneMgr::getClassGUI(std::string cName){
	if (cName.empty())
		return cName;

	int cIdx = cName.find("_");
	if (cIdx != std::string::npos) {
		cName = cName.substr(0, cIdx);
	}

	return cName;
}
void SceneMgr::preloadScene(std::string layer)
{
	if (arGuis.find(layer) == arGuis.end()) {
		BaseLayer* ll = createLayerFromName(layer);
		ll->retain();
		arGuis[layer] = ll;
	}
}

cocos2d::Node* SceneMgr::getGUI(int tag)
{
	return layerGUI->getChildByTag(tag);
}

void SceneMgr::initialLayer()
{
	if (layerGUI == nullptr) {
		layerGUI = Layer::create();
		layerGUI->retain();
	}
	else {
		if (layerGUI->getParent()) {
			layerGUI->removeFromParent();
		}
	}

	if (layerSystem == nullptr) {
		layerSystem = Layer::create();
		layerSystem->retain();
	}
	else {
		if (layerSystem->getParent()) {
			layerSystem->removeFromParent();
		}
	}

	getRunningScene()->addChild(layerGUI, BaseScene::TAG_GUI, BaseScene::TAG_GUI);
	getRunningScene()->addChild(layerSystem, BaseScene::TAG_GUI + 1, BaseScene::TAG_GUI + 1);

	//TODO: cheat
	//BaseLayer* sence = openGUI(CheatCenterScene.className, CheatCenterScene.TAG, CheatCenterScene.TAG, true);
	//sence->setVisible(Config::ENABLE_CHEAT);

	gameMgr->onEnterScene();
}

void SceneMgr::updateScene(float dt)
{
	effectMgr->updateEffect(dt);
	gameMgr->onUpdateScene(dt);
}

bool SceneMgr::checkBackAvailable(std::vector<std::string> ignores /*= {}*/)
{
	for (const auto& kv : arPopups) {
		bool check = true;
		for (int i = 0; i < ignores.size(); i++) {
			if (kv.first == ignores[i]) {
				check = false;
				break;
			}
		}

		if (check) {
			BaseLayer* g = kv.second;
			//TODO: cheat
			//if (g && g->getParent() && !(CheatCenterScene*)g) {
			if (g && g->getParent()) {
				return true;
			}
		}
	}

	return false;
}

void SceneMgr::showOkCancelDialog(std::string message, const DialogListener& callback)
{
	cocos2d::log("#showOkCancelDialog : %s",message.c_str());

	Dialog* dlg = (Dialog*)openGUI(CheckLogic::getDialogClassName(), Dialog::ZODER, Dialog::TAG, false);
	dlg->setOkCancel(message, callback);
}

void SceneMgr::showOkDialogWithAction(std::string message, const DialogListener& callback)
{
	Dialog* dlg = (Dialog*)openGUI(CheckLogic::getDialogClassName(), Dialog::ZODER, Dialog::TAG, false);
	dlg->setOkWithAction(message, callback);
}

void SceneMgr::showOKDialog(std::string message)
{
	Dialog* dlg = (Dialog*)openGUI(CheckLogic::getDialogClassName(), Dialog::ZODER, Dialog::TAG, false);
	dlg->setOKNotify(message);
}

void SceneMgr::showChangeGoldDialog(std::string message, const DialogListener& callback)
{
	Dialog* dlg = (Dialog*)openGUI(CheckLogic::getDialogClassName(), Dialog::ZODER, Dialog::TAG, false);
	dlg->setChangeGold(message, callback);
}


void SceneMgr::restartCacheGui()
{
	arGuis.clear();
	arPopups.clear();
}

void SceneMgr::showScene(cocos2d::Scene *scene, int tag)
{
	_curSceneTag = tag;
	Director::getInstance()->replaceScene(scene);
	
}

BaseScene * SceneMgr::getOrCreateScene(int tag)
{
	if (_sceneMap.find(tag) != _sceneMap.end())
	{
		return _sceneMap[tag];
	}

	BaseScene *scene;
	switch (tag)
	{
	case MAIN_SCENE:
		scene = (BaseScene *)GameScene::createScene();
		break;
	case SHOP_SCENE:
		scene = ShopScene::create();
		break;
	case INSPECT_SCENE:
		scene = CueInspectScene::create();
		break;

	default:
		scene = new BaseScene();
		scene->autorelease();
		break;
	}
	setUpDebugNode();
	_sceneMap.insert(std::pair<int, BaseScene *>(tag, scene));
	return scene;
}

void SceneMgr::setUpDebugNode()
{
	if (_debugNode == nullptr)
	{
		_debugNode = DebugNode::create();
		_debugNode->retain();
	}
	_debugNode->setCameraMask((int)GameCamera::FLAG_UI_ALWAYS_TOP);
	_debugNode->removeFromParent();
	_sceneMap[_curSceneTag]->addChild(_debugNode);
}

BaseScene *SceneMgr::loadSceneByTag(int tag)
{
	auto scene = getOrCreateScene(tag);
	showScene(scene, tag);
	setUpDebugNode();

	return scene;
}

BaseLayer* SceneMgr::createLayerFromName(std::string className)
{
	if (className.compare(LoginScene::className) == 0) {
		return LoginScene::create();
	}
	if (className.compare(LoadingScene::className) == 0) {
		return LoadingScene::create();
	}
	if (className.compare(Dialog::className) == 0) {
		return Dialog::create();
	}
	if (className.compare(LoadingSceneNew::className) == 0) {
		return LoadingSceneNew::create();
	}

	if (className.compare(CueCollectionUI::className) == 0) {
		return CueCollectionUI::create();
	}
	cocos2d::log("createLayerFromName: not found for %s", className.c_str());
	return nullptr;
}

SceneMgr* SceneMgr::_instance = nullptr;

const std::string SceneMgr::FONT_NORMAL = "fonts/tahoma.ttf";

const std::string SceneMgr::FONT_BOLD = "fonts/tahomabd.ttf";

const int SceneMgr::FONT_SIZE_DEFAULT = 26;

Toast::Toast()
{

}

Toast::~Toast()
{

}

bool Toast::init(float time, std::string message)
{
	Layer::init();
	_time = time;
	_message = message;
	_layerColor = LayerColor::create(Color4B::BLACK);
	this->addChild(_layerColor);
	return true;
}

void Toast::onEnter()
{
	Size size = Director::getInstance()->getVisibleSize();
	float scale = size.width / CONSTANT_WIDTH;
	scale = (scale > 1) ? 1 : scale;

	Layer::onEnter();

	_label = ui::Text::create();
	_label->setAnchorPoint(Vec2(0.5, 0.5));
	_label->ignoreContentAdaptWithSize(false);

	//this._label._setWidth(cc.winSize.width * 0.5);

	//this._label._setBoundingWidth(cc.winSize.width * 0.5);
	//this._label.setLineBreakOnSpace(true);
	//this._label.setTextAreaSize(cc.size(cc.winSize.width * 0.5, 40));

	_label->setFontName("fonts/tahoma.ttf");
	_label->setFontSize(SceneMgr::FONT_SIZE_DEFAULT);
	_label->setTextHorizontalAlignment(TextHAlignment::CENTER);
	_label->setColor(sceneMgr->ccWhite);
	_label->setString(_message);
	_label->setScale(scale);
	_label->setContentSize(Size(size.width * 0.9f, 70));

	_layerColor->addChild(_label);
	_layerColor->setPosition(0, 0);

	_label->setPosition(Vec2(size.width / 2, _label->getContentSize().height / 4));
	setPosition(Vec2(0, size.height));

	runAction(Sequence::create(
		EaseBackOut::create(
			MoveBy::create(0.3f, Vec2(0, -_label->getContentSize().height))
		),
		DelayTime::create(_time),
		EaseBackIn::create(
			MoveBy::create(0.3f, Vec2(0, _label->getContentSize().height / 2))
		), 
		RemoveSelf::create(), 
		nullptr));

}

Toast* Toast::makeToast(float time, std::string message)
{
	Toast* instance = new Toast();
	instance->init(time, message);
	instance->autorelease();
	sceneMgr->layerGUI->addChild(instance);
	instance->setLocalZOrder(LOADING_TAG);
	return instance;
}

const float Toast::SHORT = 1.0f;

const float Toast::LONG = 2.0f;

ToastFloat::ToastFloat()
{

}

ToastFloat::~ToastFloat()
{

}

bool ToastFloat::init()
{
	Node::init();

	timeDelay = -1;
	isRunningDelay = false;

	lb = nullptr;
	bg = nullptr;

	bg = ui::Scale9Sprite::create("Lobby/Common/9patch.png");
	addChild(bg);

	_scale = Director::getInstance()->getVisibleSize().width / CONSTANT_WIDTH;
	_scale = (_scale > 1) ? 1 : _scale;
	setScale(_scale);

	return true;
}

void ToastFloat::onEnter()
{
	Node::onEnter();

	bg->setOpacity(0);
	lb->setOpacity(0);

	bg->runAction(FadeIn::create(0.5f));
	lb->runAction(FadeIn::create(0.5f));

	runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create(CC_CALLBACK_0(ToastFloat::finishEffect, this)), nullptr));
}

void ToastFloat::finishEffect()
{
	isRunningDelay = true;
}

void ToastFloat::setToast(std::string txt, float time /*= 1.0f*/)
{
	if (!txt.empty()) {
		lb = BaseLayer::createLabelText(txt);
		lb->setTextHorizontalAlignment(TextHAlignment::CENTER);
		lb->setTextVerticalAlignment(TextVAlignment::CENTER);
		lb->ignoreContentAdaptWithSize(false);
		addChild(lb);
		Size winSize = Director::getInstance()->getVisibleSize();

		Size lbSize = lb->getContentSize();
		float deltaWidth = winSize.width * ToastFloat::DELTA_WIDTH;
		if (lbSize.width > deltaWidth) {

			lb->setContentSize(Size(deltaWidth, lbSize.height * 2));
		}

		bg->setContentSize(Size(lb->getContentSize().width + ToastFloat::PAD_SIZE, lb->getContentSize().height + ToastFloat::PAD_SIZE));
	}
	timeDelay = time;
	scheduleUpdate();
}

void ToastFloat::clearToast()
{
	bg->runAction(FadeOut::create(0.5f));
	lb->runAction(FadeOut::create(0.5f));

	runAction(Sequence::create(
		DelayTime::create(0.5),
		CallFunc::create(
			CC_CALLBACK_0(ToastFloat::removeFromParent, this)
		),
		nullptr
	));
}

void ToastFloat::update(float dt)
{
	if (timeDelay > 0 && isRunningDelay) {
		timeDelay -= dt;
		if (timeDelay <= 0) {
			clearToast();
		}
	}
}

void ToastFloat::makeToast(float time, std::string text)
{
	ToastFloat* toast = ToastFloat::create();
	toast->setToast(text, time);
	Size winSize = Director::getInstance()->getVisibleSize();
	toast->setPosition(Vec2(winSize.width / 2, winSize.height * ToastFloat::POSITION_Y));

	sceneMgr->layerGUI->addChild(toast);
	toast->setLocalZOrder(TOAST_FLOAT_TAG);
}

const float ToastFloat::SHORT = 1.0f;

const float ToastFloat::LONG = 3.0f;

const float ToastFloat::MEDIUM = 2.0f;

const float ToastFloat::POSITION_Y = 1.0f / 3;

const float ToastFloat::DELTA_WIDTH = 0.8f;

const int ToastFloat::PAD_SIZE = 35;



EffectMgr::EffectMgr()
{

}

EffectMgr::~EffectMgr()
{

}

EffectMgr* EffectMgr::getInstance()
{
	if (!_instance) {
		_instance = new EffectMgr();
	}
	return _instance;
}

void EffectMgr::runLabelPoint(cocos2d::ui::Text* label, long long cur, long long des, float delayTime /*= 0*/, int numChange /*= 50*/, int type /*= 1*/)
{
	LabelPoint* lb = nullptr;
	bool isNew = true;
	for (int i = 0, size = arLbPoints.size(); i < size; i++) {
		if (arLbPoints[i]->label == label) {
			lb = arLbPoints[i];
			isNew = false;
			break;
		}
	}

	if (isNew) {
		lb = new LabelPoint();
		lb->label = label;
	}

	lb->cur = cur;
	lb->des = des;
	lb->delta = (des - cur) / numChange;
	lb->delay = delayTime;
	lb->type = type; //type: 0 - formatNumberSymbol - use in table / 1 - pointNumber
	if (lb->type == 0) {
		lb->label->setString(StringUtility::formatNumberSymbol(lb->cur));
	}
	else {
		lb->label->setString(StringUtility::pointNumber(lb->cur));
	}

	if (isNew)
		arLbPoints.push_back(lb);
}

void EffectMgr::updateEffect(float dt)
{
	for (int i = arLbPoints.size() - 1; i >= 0; i--) {
		LabelPoint* lb = arLbPoints[i];
		if (lb->delay > 0) {
			lb->delay -= dt;
			continue;
		}

		lb->cur += lb->delta;
		if (lb->type == 0) {
			lb->label->setString(StringUtility::formatNumberSymbol(lb->cur));
		}
		else {
			lb->label->setString(StringUtility::pointNumber(lb->cur));
		}

		if ((lb->delta >= 0 && lb->cur >= lb->des) || (lb->delta <= 0 && lb->cur <= lb->des)) {
			if (lb->type == 0) {
				lb->label->setString(StringUtility::formatNumberSymbol(lb->des));
			}
			else {
				lb->label->setString(StringUtility::pointNumber(lb->des));
			}
			//lb.setString(StringUtility.pointNumber(lb.des));
			arLbPoints.erase(arLbPoints.begin() + i);
		}
	}
}

EffectMgr* EffectMgr::_instance = nullptr;
