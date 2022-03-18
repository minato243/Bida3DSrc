#include "CueInspectUI.h"
#include "data/Resource.h"
#include "core/utils/LanguageMgr.h"
#include "InventoryElementUI.h"
#include "../../core/utils/Utils.h"
#include "base/UIFactory.h"
#include "../../core/inventory/InventoryMgr.h"
#include "core/utils/StringUtility.h"
#include "../../core/inventory/ItemConstant.h"
#include "ModelOffset.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;
using namespace ps::ExtMath;

CueInspectUI::CueInspectUI()
{
	_cue = NULL;
	_cueNode = NULL;

	_data = NULL;
	_force = 0;
	_angular = 0;
	_angle[0] = 0; _angle[1] = 0;
	_generalPoint = 0;

	_animationTime = 0.f;
	_updateInfo = false;
}

CueInspectUI::~CueInspectUI()
{
	_enableLights = true;
}

bool CueInspectUI::init()
{
	if (!BaseLayer::init("CueInspectUI"))
	{
		return false;
	}

	_rpt = Rpt(ORIGINAL_R, 0, 0 );
	_zoom = 0.8f;
	_cueLerp = new SphericalLerp(0.125, CC_CALLBACK_1(CueInspectUI::updateCueRotation, this));
	_zoomLerp = new NumberLerp(0.125, CC_CALLBACK_1(CueInspectUI::updateCueZoom, this));

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::CueInspectUIv2_2);
	this->initWithBinaryFile(filePath);

	this->_keep = true;

	return true;
}

void CueInspectUI::initGUI()
{
	autoInitComponent(_layout);
	_bg = _nodeMap["bg"];

	_imgTitleBar = (cocos2d::ui::ImageView *)_nodeMap["imgTitleBar"];
	_lbTitle = (cocos2d::ui::Text *) _nodeMap["lbTitle"];
	_imgType = (cocos2d::Sprite *) _nodeMap["imgType"];
	_imgType1 = (cocos2d::Sprite *) _nodeMap["imgType1"];
	_nodeStars = _nodeMap["nodeStars"];

	_rtQuantity = (cocos2d::ui::Text *) _nodeMap["rtQuantity"];
	
	_btnWorkshop = (cocos2d::ui::Button *)_nodeMap["btnWorkshop"];
	_btnEquip = (cocos2d::ui::Button *)_nodeMap["btnEquip"];
	_btnCharge = (cocos2d::ui::Button *)_nodeMap["btnCharge"];
	_lbBtnWorkShop = (cocos2d::ui::Text *) getControl("lb", _btnWorkshop);
	_lbBtnCharge = (cocos2d::ui::Text *) getControl("lb", _btnCharge);
	_lbBtnEquip = (cocos2d::ui::Text *) getControl("lb", _btnEquip);

	_lbLevel = (cocos2d::ui::Text *) _nodeMap["lbLevel"];
	_lbBattery = (cocos2d::ui::Text *)_nodeMap["lbBattery"];
	_lbAuto = (cocos2d::ui::Text *) _nodeMap["lbAuto"];
	_imgTick = (cocos2d::ui::ImageView *)_nodeMap["imgTick"];

	_pnBelow = (cocos2d::ui::Widget *)_nodeMap["pnBelow"];
	_pnAura = (Widget*)_nodeMap["pnAura"];

	_pnForce	= (cocos2d::ui::Widget *) _nodeMap["pnForce"];
	_pnSpin		= (cocos2d::ui::Widget *) _nodeMap["pnSpin"];
	_pnAim		= (cocos2d::ui::Widget *) _nodeMap["pnAim"];
	_pnAccuracy = (cocos2d::ui::Widget *) _nodeMap["pnAccuracy"];
	_pnAngle	= (cocos2d::ui::Widget *) _nodeMap["pnAngle"];

	_barPnForce		= (cocos2d::ui::Widget *) getControl("bar", _pnForce);
	_barPnSpin		= (cocos2d::ui::Widget *) getControl("bar", _pnSpin);
	_barPnAim		= (cocos2d::ui::Widget *) getControl("bar", _pnAim);
	_barPnAccuracy	= (cocos2d::ui::Widget *) getControl("bar", _pnAccuracy);
	_barPnAngle		= (cocos2d::ui::Widget *) getControl("bar", _pnAngle);

	_pnGold = (cocos2d::ui::Widget *) _nodeMap["pnGold"];
	_imgPnGold = (cocos2d::ui::ImageView *)getControl("img", _pnGold);
	_rtGold = (cocos2d::ui::Text *)getControl("rtGold", _pnGold);

	_btnBack = (cocos2d::ui::Button *) _nodeMap["btnBack"];
	_imgRightSlot = (cocos2d::ui::ImageView *) _nodeMap["imgRightSlot"];
	_imgImgRightSlot = (cocos2d::ui::ImageView *) getControl("img", _imgRightSlot);

	_imgBgBox = (cocos2d::ui::ImageView *) _nodeMap["imgBgBox"];
	_pnTitle = (cocos2d::ui::Widget *) _nodeMap["pnTitle"];
	_pn3DDisplay = (cocos2d::ui::Widget *) _nodeMap["pn3DDisplay"];

	_nodeMap["fake"]->setVisible(false);
	this->setCameraMask((int)CAM_FLAG::FLAG_BELOW);

	init3DView();
	initCamera();
	initBars();

	_rtQuantity->setTextHorizontalAlignment(TextHAlignment::CENTER);
	_lbBtnWorkShop->setString(languageMgr->localizedText("text_workshop"));
	_lbBtnEquip->setString(languageMgr->localizedText("text_equip"));
	_lbBtnCharge->setString(languageMgr->localizedText("text_charge"));
	_lbAuto->setString(languageMgr->localizedText("text_charge_auto"));
	_ogContentSizeMap.insert(pair<Node*, Size>(_btnEquip, _btnEquip->getContentSize()));

	_lbTitle->ignoreContentAdaptWithSize(true);
	_bg->setCameraMask((int)CameraFlag::DEFAULT);
	_bg->setGlobalZOrder(3);
	_pnBelow->setCameraMask((int)CueInspectUI::CAM_FLAG::FLAG_BELOW);

	auto batteryBox = _nodeMap["imgBattery"];
	auto batteryImg = (ImageView*)getControl("battery", batteryBox);
	_battery = Battery::create(batteryBox, batteryImg, _lbBattery);

	initShowAnimations();
}
void CueInspectUI::show()
{
	BaseLayer::show();
}

void CueInspectUI::initShowAnimations()
{
	quickAnimationUIMove(_btnBack, 0, 100, 1);
	// quickAnimationUIMove(_imgBackDock, 0, 100, 1);
	quickAnimationUIMove(_imgRightSlot, 0, 100, 1);
	quickAnimationUIMove(_pnTitle, 0, 100, 1);

	auto sprite = Sprite::create();
	sprite->retain();
	auto delay = DelayTime::create(0.5);
	delay->retain();
	setUpUIShowAnimation(sprite, delay, NULL, 2, NULL, NULL);
	quickAnimationUIMove(_imgBgBox, 0, -170, 3);
	setUpUIShowAnimation(sprite, delay, NULL, 4, NULL, NULL);
}

void CueInspectUI::update(float dt)
{
	_cueLerp->update(dt);
	_zoomLerp->update(dt);
}

void CueInspectUI::initBars()
{
	initBar(_pnForce);
	initBar(_pnSpin);
	initBar(_pnAim);
	initBar(_pnAccuracy);
	initBar(_pnAngle);
}

void CueInspectUI::initBar(cocos2d::ui::Widget *panel)
{
	auto bar = (Widget *)getControl("bar", panel);
	auto lb = (Text *)getControl("lb", panel);
	auto lbValue = (Text *)getControl("lbValue", panel);
	auto barSize = bar->getContentSize();
	lb->setPositionX(bar->getPositionX() - 90);
	lbValue->setPositionX(bar->getPositionX() + barSize.width + 10);

	auto stencil = DrawNode::create();
	stencil->setPosition(bar->getPosition());
	auto originalX = stencil->getPositionX() - barSize.width;
	_originalXMap.insert(pair<Node*, float>(stencil, originalX));
	// stencil.setRotationX(10);
	stencil->drawSolidRect(Vec2(0, 0), Vec2(barSize.width, barSize.height), Color4F(Color3B(255, 255, 255)));
	auto clippingNode = ClippingNode::create(stencil);
	_stencilMap.insert(pair<Widget*, DrawNode*>(bar, stencil));
	bar->removeFromParent();
	panel->addChild(clippingNode);
	clippingNode->addChild(bar);
}

void CueInspectUI::initCamera()
{
	auto winSize = Director::getInstance()->getWinSize();
	auto ca = Camera::createOrthographic(
		winSize.width,
		winSize.height,
		0,
		1);
	ca->setDepth(CueInspectUI::CAM_DEPTH::DEPTH_BELOW);
	ca->setCameraFlag(CueInspectUI::CAM_FLAG::FLAG_BELOW);
	addChild(ca);
	_cameras.push_back(ca);

	auto ca2 = Camera::createOrthographic(
		winSize.width,
		winSize.height,
		0,
		1);
	ca2->setDepth(CueInspectUI::CAM_DEPTH::DEPTH_ABOVE);
	ca2->setCameraFlag(CueInspectUI::CAM_FLAG::FLAG_ABOVE);
	addChild(ca2);
	_cameras.push_back(ca2);

	auto ca3 = Camera::createOrthographic(
		winSize.width,
		winSize.height,
		0,
		1
	);
	ca3->setDepth(CueInspectUI::CAM_DEPTH::DEPTH_ABOVE + 1);
	ca3->setCameraFlag(GameCamera::FLAG_UI_ALWAYS_TOP);
	addChild(ca3);
}

void CueInspectUI::init3DView()
{
	auto sceneCue = Node::create();
	addChild(sceneCue);

	auto winSize = Director::getInstance()->getWinSize();
	auto camera = Camera::createPerspective(30, winSize.width / winSize.height, 1, 1000);
	camera->setCameraFlag(CueInspectUI::CAM_FLAG::FLAG_3D);
	camera->setDepth(CueInspectUI::CAM_DEPTH::DEPTH_3D);
	camera->setPosition3D(Vec3(0, 0, -100));
	camera->lookAt(Vec3(0, 0, 0));

	sceneCue->addChild(camera);
	_camera = camera;

	_cueNode = Sprite3D::create();
	addCue3D(Res::CUE_VANG_1, Res::CUE_TEXTURE_SET_1, Res::CUE_TEXTURE_SET_1_MRAO, _enableLights);

	_cueNode->setPosition3D(Vec3(20, 3, 0));
	_cueNode->setScale(0.9);
	sceneCue->addChild(_cueNode);
	_cueNode->setCameraMask((int)CAM_FLAG::FLAG_3D);

	auto tmp = Sprite3D::create("res/3d/cue/cue_design/Sticklevel2_2.c3b");
	tmp->setCameraMask((int)CAM_FLAG::FLAG_3D);
	tmp->setRotation3D(Vec3(0, 90, 0));
	tmp->setPosition3D(Vec3(0,0,0));
	tmp->setScale(10);
	addChild(tmp);

	// cue = cue;
	// cueNode.setRotation3D(cc.math.vec3(0, 90, 4))

	init3DControl();
}

void CueInspectUI::addCue3D(std::string model, std::string tex, std::string mraoPath, bool isLighting)
{
	//if (_cue) _cue->setVisible(false);
	map<string, Sprite3D*> cues = _cues;
	if (cues.find(model) != cues.end())
	{
		cues[model]->setVisible(true);
	}
	else
	{
		auto cue = Sprite3D::create(model);
		cue->setCameraMask((int)CueInspectUI::CAM_FLAG::FLAG_3D);
		cue->setScale(0.35);
		cues.insert(pair<string, Sprite3D*>(model, cue));
		_cueNode->addChild(cue);

		//setUpShader(cue, tex, mraoPath);
	}
	_cue = cues[model];
	_cue->setTexture(tex);
	auto albedo = TextureCache::getInstance()->addImage(tex);
	//_stateMap[_cue]->setUniformTexture("u_albedo", albedo);
	_cue->setPosition3D(Vec3(0, 0, modelOffset->getModelWidthByPath(model)/2 *0.35));

	//resetRotation();
}

void CueInspectUI::setUpShader(cocos2d::Sprite3D * cue, std::string texPath, std::string mraoPath)
{
	// const program = cc.shaderCache.getProgram("CueShader_"+gv.settingMgr.Quality.MEDIUM);
		// // const program = new cc.GLProgram("res/shaders/3d_shader.vert", "res/shaders/IOS/3d_color_tex_metal_map.frag");
		// auto state = cc.GLProgramState.create(program);

		// cue.setGLProgramState(state);

	auto state = Utils::setUpCue3DShader(cue, texPath, mraoPath);
	_stateMap.insert(pair<Sprite3D*, GLProgramState*>(cue, state));
	// _state.setUniformTexture("u_albedo", cc.textureCache.addImage(res.CUE_TEXTURE_SET_1));
	// _state.setUniformTexture("u_mrao_map", cc.textureCache.addImage(res.CUE_TEXTURE_SET_1_MRAO));
	setLights(Vec3(1, 1, 1));
}

void CueInspectUI::setLights(cocos2d::Vec3 lightColor)
{
	for (auto ittr = _cues.begin(); ittr != _cues.end(); ittr++)
	{
		auto cue = ittr->second;
		auto state = _stateMap[cue];

		if (_lightPos.size() == 0)
		{
			_lightPos.push_back(Vec3(-160, 0, -180));
			_lightPos.push_back(Vec3(160, 0, -180));
		}

		state->setUniformVec3("u_light_color", lightColor);
		state->setUniformInt("u_light_count", _lightPos.size());

		for (int i = 0; i < _lightPos.size(); i++) {
			state->setUniformVec3("u_light_pos_" + i, _lightPos[i]);
		}
	}
}

void CueInspectUI::init3DControl()
{
	if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		|| CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	{
		auto listener = EventListenerTouchAllAtOnce::create();
		listener->onTouchesMoved = CC_CALLBACK_2(CueInspectUI::onTouchesMoved, this);

		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, _pn3DDisplay);
	}
	else
	{
		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = CC_CALLBACK_2(CueInspectUI::onTouchBegan, this);
		listener->onTouchMoved = CC_CALLBACK_2(CueInspectUI::onTouchMoved, this);
		listener->onTouchEnded = [this](cocos2d::Touch* touch, cocos2d::Event* event) {
		};

		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, _pn3DDisplay);
	}

	if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	{
		auto listener = EventListenerMouse::create();
		listener->onMouseScroll = CC_CALLBACK_1(CueInspectUI::onMouseScroll, this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, _pn3DDisplay);
	}

	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = [this](EventKeyboard::KeyCode key, Event* event)
	{
		auto pos = this->_cue->getPosition3D();

		switch (key) {
		case EventKeyboard::KeyCode::KEY_KP_LEFT:
			pos.x -= 1;
			this->_cue->setPosition3D(pos);
			break;
		case EventKeyboard::KeyCode::KEY_KP_UP:
			pos.y += 1;
			this->_cue->setPosition3D(pos);
			break;
		case EventKeyboard::KeyCode::KEY_KP_RIGHT:
			pos.x += 1;
			this->_cue->setPosition3D(pos);
			break;
		case EventKeyboard::KeyCode::KEY_KP_DOWN:
			pos.y -= 1;
			this->_cue->setPosition3D(pos);
			break;
		case EventKeyboard::KeyCode::KEY_A:
			pos.z -= 1;
			this->_cue->setPosition3D(pos);
			break;
		case EventKeyboard::KeyCode::KEY_D:
			pos.z += 1;
			this->_cue->setPosition3D(pos);
			break;
		case EventKeyboard::KeyCode::KEY_ENTER:
			//this->addChild(gv.takeScreenShot());
			break;

		}
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, _pn3DDisplay);
}

void CueInspectUI::onMouseScroll(cocos2d::Event *event)
{
	auto mouseEvent = static_cast<EventMouse*>(event);
	auto scrollY = mouseEvent->getScrollY();
	// _camera.focus(scrollY * 10);

	_zoom -= 0.1 * scrollY;

	// _zoom = min(4, Math.max(0.3, _zoom));
	// // cueNode.setScale(_zoom);
	// _rpt.r = ORIGINAL_R * (1 / _zoom);
	//
	// updateRotation();
	updateZoom();
}

bool CueInspectUI::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	//check in side
	auto pnSize = _pn3DDisplay->getContentSize();
	auto location = _pn3DDisplay->convertToNodeSpace(touch->getLocation());
	if (location.x < 0 || location.x > pnSize.width || location.y <  0 || location.y > pnSize.height)
		return false;
	return true;
}

void CueInspectUI::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	auto PI = M_PI;
	auto location = touch->getLocation();
	auto previousLocation = touch->getPreviousLocation();
	auto d = Vec2(previousLocation.x - location.x, previousLocation.y - location.y);
	// Touch move by y => rotate on x axis
	_rpt.p = (_rpt.p - d.x / 600);

	if (_rpt.p < PI / 2 || _rpt.p > 3 * PI / 2) {
		_rpt.t = (_rpt.t - d.y / 500);
	}
	else {
		_rpt.t = (_rpt.t + d.y / 500);
	}
	_rpt.t = max(-20 * PI / 180, min(30 * PI / 180, _rpt.t));

	updateRotation();
}

void CueInspectUI::updateZoom()
{
	_zoom = min(4.f, max(0.3f, _zoom));

	//Move the node along y axis while zooming to keep it in the center
	_zoomLerp->setTarget(_zoom);
}

void CueInspectUI::updateCueZoom(float zoom)
{
	auto x = zoom;
	auto x2 = x * x;
	auto x3 = x2 * x;
	auto x4 = x3 * x;
	auto x5 = x4 * x;

	auto y = -0.302613*x5 + 3.17586*x4 - 11.8388*x3 + 18.6212*x2 - 3.24919*x - 3.40649;
	_cueNode->setPositionY(y);
	_cueNode->setScale(zoom);
}

cocos2d::Color3B CueInspectUI::getColorObjByType(int type)
{
	Color3B color[] = {
		Color3B(255, 211, 75),
		Color3B(255, 102, 102),
		Color3B(232, 82, 230),
		Color3B(85, 204, 245)
	};
	return color[type - 1];
}

std::string CueInspectUI::getIconName(int type)
{
	switch (type) {
	case 1: return "Yellow_Icon";
	case 2: return "Red_Icon";
	case 3: return "Purple_Icon";
	case 4: return "Blue_Icon";
	default: return "Yellow_Icon";
	}
}

void CueInspectUI::resetRotation()
{
	_rpt.p = 0.8;
	_rpt.t = -0.3490658503988659;
	auto rpt = Rpt(0, _rpt.p * 180 / M_PI, _rpt.t * 180 / M_PI);
	_cueLerp->setTarget(rpt);
	_cueLerp->fastForward();
	_cueLerp->setEnabled(true);
	updateCueRotation(rpt);
	_zoomLerp->setTarget(_zoom);
	_zoomLerp->fastForward();
	_zoomLerp->setEnabled(true);
	updateCueZoom(_zoom);

	for (auto const &ittr: _cues) {
		auto cue = ittr.second;
		//cue->setOpacity(0);
	}
}

void CueInspectUI::updateRotation()
{
	auto rpt = Rpt(0, _rpt.p * 180 / M_PI, _rpt.t * 180 / M_PI);
	_cueLerp->setTarget(rpt);
}

void CueInspectUI::updateCueRotation(ps::ExtMath::Rpt rpt)
{
	_cueNode->setRotation3D(Vec3(rpt.t, rpt.p, 0));
}

void CueInspectUI::setInfo(CueInfo *cueInfo, bool isOwn, bool isGacha, bool isFusion)
{
	CCLOG("GACHA ", isGacha);
	_data = cueInfo;
	_isOwn = isOwn;
	_isGacha = isGacha;
	_isFusion = isFusion;

	// utils.log("cue info data", cueInfo);
	_force = cueInfo->_force;
	_angular = cueInfo->_angular;
	_angle[0] = cueInfo->_angle[0]; _angle[1] = cueInfo->_angle[1];
	_aim = cueInfo->_aim || 125;
	_accuracy = cueInfo->_accuracy || 76;
	_generalPoint = cueInfo->_generalPoint;
	_lbLevel->setString(to_string(cueInfo->_level));

	runInfoAnimation();
	auto batteryPercent = cueInfo->getBatteryPercentage();
	auto maxBattery = cueInfo->getMaxBattery();
	_battery->setPercent(batteryPercent);

	auto isInBag = cueInfo->isInBag();
	auto buttonEquip = _btnEquip;
	if (isInBag) {
		UIFactory::loadButtonTextures(buttonEquip, "Button_Red.png");
		buttonEquip->setContentSize(_ogContentSizeMap[buttonEquip]);
		_lbBtnEquip->setString(languageMgr->localizedText("text_unequip"));
	}
	else {
		auto isFull = inventoryMgr->isCueBagFull();
		if (isFull) {
			_lbBtnEquip->setString(languageMgr->localizedText("text_full"));
			UIFactory::loadButtonTextures(buttonEquip, "Button_Gray.png");
		}
		else {
			_lbBtnEquip->setString(languageMgr->localizedText("text_equip"));
			UIFactory::loadButtonTextures(buttonEquip, "Button_Green.png");
		}
		buttonEquip->setContentSize(_ogContentSizeMap[buttonEquip]);
	}

	_btnCharge->setVisible(!!isOwn);
	if (isOwn) {
		_lbBtnCharge->setString(isGacha ? languageMgr->localizedText("text_confirm") : languageMgr->localizedText("text_charge"));
	}
	auto cueName = cueInfo->_name;
	auto nameWidth = (Text::create(cueName, _lbTitle->getFontName(), 20))->getContentSize().width;
	_lbTitle->setString(cueName);
	_lbTitle->setFontSize(min(30, (int)(_imgTitleBar->getContentSize().width * 0.75 / nameWidth * 20)));

	auto type = cueInfo->_type - 1;
	_imgType->setSpriteFrame(InventoryElementUI::TYPE_FRAMES[type].icon);
	_imgType1->setSpriteFrame(InventoryElementUI::TYPE_FRAMES[type].icon);
	_imgType->setScale(1.2);
	_imgType1->setScale(1.2);
	setNumStar(cueInfo->_tier);

	set3DModel(cueInfo);
	auto goldText = "<image=#Gold.png>0.6</image><color=#FFB027>\ \ \ " + to_string(_generalPoint) + "</color>";
	auto imgText = "<image=#" + InventoryElementUI::TYPE_FRAMES[type].icon + ">1</image>\ \ ";

	// set Upgrade and Charge info
	setChargeCriteriaInfo(cueInfo);
}

void CueInspectUI::setChargeCriteriaInfo(CueInfo *data)
{
	auto type = data->_type;
	auto pnGold = _pnGold;
	auto textGold = _rtGold;
	auto iconGold = _imgPnGold;
	//auto color = CueInspectUI::getColorByType(type);
	auto color3B = CueInspectUI::getColorObjByType(type);

	auto curItemLb = _rtQuantity;
	auto curItemImg =_imgImgRightSlot;
	curItemImg->loadTexture("Icon_Battery_0" + to_string(type) + ".png", TextureResType::PLIST);
	auto itemQuantity = inventoryMgr->getInventory()->getItemQuantity(ItemConstant::CHALLENGE_ITEM_SONG_TOAN - 1 + type);
	curItemLb->setString(StringUtility::standartNumber(itemQuantity));
	curItemLb->setTextColor(Color4B(color3B));
	// textcc.setVisible(data->tier < 6);
	// textGold.setVisible(!data->isFull());
	//auto isFull = data->isFull ? data->isFull() : false;
	auto isFull = false;
	auto batteryPercentage = data->getBatteryPercentage();
	iconGold->setVisible(!isFull);
	if (isFull) {
		textGold->setString(languageMgr->localizedText("text_full_charged"));
		textGold->setTextColor(Color4B(color3B));
		textGold->setPositionX((textGold->getParent()->getContentSize().width - UIFactory::getTextWidth(textGold)) / 2);
	}
	else {
		iconGold->loadTexture("Icon_Battery_0" + to_string(type) + ".png", TextureResType::PLIST);
		textGold->setString(StringUtility::standartNumber((1 - batteryPercentage / 100.f) * 5000));
		textGold->setTextColor(Color4B(color3B));
		auto goldWidth = iconGold->getContentSize().width + 3 + UIFactory::getTextWidth(textGold);
		textGold->setPositionX((textGold->getParent()->getContentSize().width - goldWidth) / 2 + iconGold->getContentSize().width + 3);;
	}
}

void CueInspectUI::set3DModel(CueInfo *data)
{
	auto paths = data->getModelPath();
	addCue3D(paths.model, paths.tex, paths.mrao, _enableLights);
}

void CueInspectUI::setNumStar(int num)
{
	num = min(7, max(1, num));
	for (int i = 0; i < 7; i++) {
		auto star = (ImageView*)_nodeStars->getChildByName("star_" +to_string(i));
		if (i < num) {
			star->loadTexture("inven_Star_02.png", TextureResType::PLIST);
		}
		else {
			star->loadTexture("inven_Star_01.png", TextureResType::PLIST);
		}
	}
}

void CueInspectUI::refresh()
{
	setInfo(_data, _isOwn, _isGacha, _isFusion);
}

void CueInspectUI::setUIInfo(const double & force, const double & angular, double angle[], const double & aim, const double & accuracy, const double & generalPoint)
{
	((Text*)_pnForce->getChildByName("lbValue"))->setString(to_string((int)force));
	((Text*)_pnSpin->getChildByName("lbValue"))->setString(to_string((int)angular));
	((Text*)_pnAim->getChildByName("lbValue"))->setString(to_string((int)aim));
	((Text*)_pnAccuracy->getChildByName("lbValue"))->setString(to_string((int)accuracy) + "%");
	((Text*)_pnAngle->getChildByName("lbValue"))->setString(to_string((int)angle[0]) + " - " + to_string((int)angle[1]));
	// this._bg._lbGeneralScore.setString(to_string((int)generalPoint));

	setPercent(_barPnForce, force / CueInspectUI::MAX_FORCE);
	setPercent(_barPnSpin, angular / CueInspectUI::MAX_ANGULAR);
	setPercent(_barPnAim, aim / CueInspectUI::MAX_AIM);
	setPercent(_barPnAccuracy, accuracy / CueInspectUI::MAX_ACCURACY);
	setPercent(_barPnAngle, (angle[1] - angle[0]) / CueInspectUI::MAX_ANGLE);
}

void CueInspectUI::setPercent(Widget *bar, float percentOnOne)
{
	auto stencil = _stencilMap[bar];
	auto x = _originalXMap[stencil] + bar->getContentSize().width * percentOnOne;
	_stencilMap[bar]->setPositionX(x);
}

void CueInspectUI::runInfoAnimation()
{
	if (_updateInfo) 
	{
		unschedule(schedule_selector(CueInspectUI::updateInfo));
		_updateInfo = false;
	}

	double angle[] = { 0, 0 };
	setUIInfo(0, 0, angle, 0, 0, 0);
	_animationTime = 0;

	schedule(schedule_selector(CueInspectUI::updateInfo));
}

void CueInspectUI::updateInfo(float dt)
{
	_animationTime += dt;
	if (_animationTime > ANIMATION_SPEED) {
		setUIInfo(_force, _angular, _angle, _aim, _accuracy, _generalPoint);
		unschedule(schedule_selector(CueInspectUI::updateInfo));
		_updateInfo = false;
		return;
	}

	auto ratio = animationTime / ANIMATION_SPEED;
	auto force = min(_force, ratio * CueInspectUI::MAX_FORCE);
	auto angular = min(_angular, ratio * CueInspectUI::MAX_ANGULAR);
	auto angle = min(_angle[1] - _angle[0], ratio * CueInspectUI::MAX_ANGLE);
	auto angleRatio = min(1., ratio * CueInspectUI::MAX_ANGLE / (_angle[1] - _angle[0]));
	auto aim = min(_aim, ratio * CueInspectUI::MAX_AIM);
	auto accuracy = min(_accuracy, ratio * CueInspectUI::MAX_ACCURACY);
	auto point = min(_generalPoint, ratio * CueInspectUI::MAX_POINT);
	double angles[] = { angleRatio * _angle[0], angleRatio * _angle[0] + angle };
	setUIInfo(force, angular, angles, aim, accuracy, point);
}

void CueInspectUI::onButtonRelease(cocos2d::ui::Button* button, int id)
{

}

void CueInspectUI::onTouchesMoved(const std::vector<Touch *>& touches, cocos2d::Event *event)
{
	if (touches.size() == 1) {
		onTouchMoved(touches[0], event);
	}
	else {
		Vec2 locations[] = { touches[0]->getLocation(), touches[1]->getLocation() };
		Vec2 previousLocations[] = { touches[0]->getPreviousLocation(), touches[1]->getPreviousLocation() };
		// utils.log("Locations", locations, "PrevLocation", previousLocations);

		auto l0 = Vec2(locations[1].x - locations[0].x, locations[1].y - locations[0].y );
		auto l1 = Vec2(previousLocations[1].x - previousLocations[0].x,  previousLocations[1].y - previousLocations[0].y );

		auto distSqr0 = l0.x * l0.x + l0.y * l0.y;
		auto distSqr1 = l1.x * l1.x + l1.y * l1.y;

		// NewZoom/Zoom = newDistance/distance
		_zoom = _zoom * sqrt(distSqr0 / distSqr1);
		// _zoom = min(4, Math.max(0.3, _zoom));
		updateZoom();
	}
}

const double CueInspectUI::ANIMATION_SPEED = 1;

const double CueInspectUI::ORIGINAL_R = 300;

const double CueInspectUI::animationTime = 1.1;

const double CueInspectUI::MAX_FORCE = 1500;

const double CueInspectUI::MAX_ACCURACY = 100;

const double CueInspectUI::MAX_AIM = 250;

const double CueInspectUI::MAX_POINT = 10000;

const double CueInspectUI::MAX_ANGLE = 90;

const double CueInspectUI::MAX_ANGULAR = 100;