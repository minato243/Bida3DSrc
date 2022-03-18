#include "InventoryElementUI.h"
#include <data/Resource.h>
#include "../../core/inventory/InventoryMgr.h"
#include "../../../base/UIFactory.h"
#include "../../../core/utils/LanguageMgr.h"
#include "../../camera-view/GameCamera.h"
#include "ModelOffset.h"
#include "../GuiMgr.h"
#include "../../network/inventory/InventoryHandler.h"
#include "CueInspectUI.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

InventoryElementUI::InventoryElementUI()
{
	_cue3d = nullptr;
}

InventoryElementUI::~InventoryElementUI()
{

}

bool InventoryElementUI::init()
{
	if (!BaseLayer::init("InventoryElementUI"))
	{
		return false;
	}

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::InventoryElementUIv2);
	this->initWithBinaryFile(filePath);

	return true;
}

InventoryElementUI * InventoryElementUI::create(const cocos2d::Size & size)
{
	InventoryElementUI *e = new InventoryElementUI();
	e->autorelease();
	e->_size = size;
	e->init();
	e->scaleToSize(size);
	
	return e;
}

cocos2d::Size InventoryElementUI::getOriginalSize()
{
	return _bg->getContentSize();
}

void InventoryElementUI::scaleToSize(const cocos2d::Size & size)
{
	_bg->setScale(size.width / _bg->getContentSize().width);
}

void InventoryElementUI::initGUI()
{
	initTagMap();
	autoInitComponent(_layout);
	_nodeMap["fake"]->setVisible(false);

	_bg = (cocos2d::ui::Widget *) _nodeMap["bg"];
	_btnItem = (Button*)_nodeMap["btnItem"];
	_lbConfigId = (cocos2d::ui::Text *)_nodeMap["lbConfigId"];

	_lbName = (Text*)_nodeMap["lbName"];
	_lbLevel = (Text*)_nodeMap["lbLevel"];
	_lbGeneralPoint = (Text*)_nodeMap["lbGeneralPoint"];

	_lbChargeFee = (Text*)_nodeMap["lbChargeFee"];
	_lbLowBattery = (Text*)_nodeMap["lbLowBattery"];
	_btnSwitch = (Button*)_nodeMap["btnSwitch"];
	_lbBtnSwitch = (Text*)_btnSwitch->getChildByName("lb");
	_imgGoldLbChargeFee = (ImageView*)_lbChargeFee->getChildByName("imgGold");

	_lbBattery = (Text*)_nodeMap["lbBattery"];
	_imgBattery = (ImageView*)_nodeMap["imgBattery"];

	_nodeStars = _nodeMap["nodeStars"];
	_btnAutoRecharge = (Button*)_nodeMap["btnAutoRecharge"];

	initBattery();
}

void InventoryElementUI::initTagMap()
{
	_tagMap.insert(std::pair<string, int>("btnItem", TAG::BTN_ITEM));
	_tagMap.insert(std::pair<string, int>("btnSwitch", TAG::BTN_SWITCH));
	_tagMap.insert(std::pair<string, int>("btnAutoRecharge", TAG::BTN_AUTO_CHARGE));
}

void InventoryElementUI::initBattery()
{
	ImageView *battery = (ImageView*)_imgBattery->getChildByName("battery");
	_battery = Battery::create(_imgBattery, battery, _lbBattery);
}

void InventoryElementUI::loadData(CueInfo *data)
{
	_data = data;
	_lbName->setString(data->_name);
	_lbLevel->setString(to_string(data->_level));
	setStars(data->_tier);
	// this.setBatteryPercent(data->getBatteryPercentage());
	_lbConfigId->setString("ConfigID: " + to_string(data->_configId));
	auto btnSwitch = _btnSwitch;
	auto bagFull = inventoryMgr->isCueBagFull();
	_lbLowBattery->setVisible(data->getBatteryPercentage() == 0);
	_lbChargeFee->setVisible(data->getBatteryPercentage() == 0);
	if (data->getBatteryPercentage() == 0) {
		UIFactory::loadButtonTextures(btnSwitch, "inventory.v2_Yellow_Button.png");
		_imgGoldLbChargeFee->loadTexture(CueInspectUI::getIconName(data->_type) + "_inven_2.2.png", Widget::TextureResType::PLIST);
		_lbChargeFee->setTextColor(Color4B(CueInspectUI::getColorObjByType(data->_type)));
		_lbBtnSwitch->setString(languageMgr->localizedText("text_charge"));
		_lbBtnSwitch->enableShadow(Color4B(58, 72, 54, 255), Size(0, -3), 0);
	}

	else if (data->isInBag()) {
		UIFactory::loadButtonTextures(btnSwitch, "Red_Button_Glass.png");
		_lbBtnSwitch->setText(languageMgr->localizedText("text_unequip"));
		_lbBtnSwitch->enableShadow(Color4B(41, 72, 74, 255), Size(0, -3), 0);
		//this._bg._states.forEach(s = > s.setUniformFloat("u_enabled", 1));
	}
	else {
		if (bagFull) {
			_lbBtnSwitch->setText(languageMgr->localizedText("text_full"));
			UIFactory::loadButtonTextures(btnSwitch, "inventory.v2_Gray_Button.png");
		}
		else {
			_lbBtnSwitch->setText(languageMgr->localizedText("text_equip"));
			UIFactory::loadButtonTextures(btnSwitch, "inventory.v2_Green_Button.png");
		}
		_lbBtnSwitch->enableShadow(Color4B(28, 87, 69, 255), Size(0, -3), 0);
		//this._bg._states.forEach(s = > {s.setUniformFloat("u_enabled", 0)});
	}

	auto paths = data->getModelPath();
	addCue3D(paths.model, paths.tex);
}

void InventoryElementUI::setAutoRecharge(bool isAuto)
{
	_btnAutoRecharge->getChildByName("imgTick")->setVisible(isAuto);
}

void InventoryElementUI::setContainerRect(cocos2d::Rect rect)
{
	_rect = rect;
}

void InventoryElementUI::addCue3D(string path, string texPath)
{
	auto winSize = Director::getInstance()->getWinSize();
	if (_cues.find(path) == _cues.end()) {
		auto cue3D = Sprite3D::create(path);
		cue3D->setCameraMask((int)GameCamera::FLAG_3D_ABOVE_UI);
		auto state = GLProgramState::create(GLProgram::createWithFilenames("res/shaders/basic_3d.vert", "res/shaders/basic_3d_rect_mask.frag"));
		cue3D->setGLProgramState(state);
		state->setUniformVec2("u_min", Vec2(this->_rect.origin.x, this->_rect.origin.y));
		state->setUniformVec2("u_max", Vec2(this->_rect.origin.x + this->_rect.size.width, this->_rect.origin.y + this->_rect.size.height));

		auto scale = path.find("Stick") >= 0 ? 1.8 : 1.3;
		scale *= winSize.width / 1136;
		auto cueWidth = scale * modelOffset->getModelWidthByPath(path);
		cue3D->setPosition3D(Vec3(this->_bg->getContentSize().width * 0.42 + cueWidth, this->_bg->getContentSize().height * 0.4, 0));
		cue3D->setScale(scale);
		cue3D->setRotation3D(Vec3(0, 90, 0));
		this->_cues.insert(pair<string, Sprite3D*>(path, cue3D));
		this->_bg->addChild(cue3D);
	}
	this->_cues[path]->setTexture(texPath);

	if (this->_cue3d)
	{
		this->_cue3d->setVisible(false);
	}
	this->_cue3d = this->_cues[path];
	this->_cue3d->setVisible(true);
}

void InventoryElementUI::setBattery(int value, int max)
{
	auto percent = MAX(0, MIN(100, ceil(value*1.f / max * 100)));
	_battery->setValue(value, max);
	_battery->setPercent(percent);
}

void InventoryElementUI::setBatteryPercent(float percent)
{
	_battery->setPercent(percent);
}

void InventoryElementUI::setStars(int star)
{
	int numberOfStar = min(7, max(1, star));
	for (auto i = 0; i < 7; i++) {
		auto img = (ImageView*)_nodeStars->getChildByName("star_" + to_string(i));
		if (i < numberOfStar) {
			img->loadTexture("inventory.v2_Star_02.png", Widget::TextureResType::PLIST);
		}
		else {
			img->loadTexture("inventory.v2_Star_01.png", Widget::TextureResType::PLIST);
		}
	}
}

void InventoryElementUI::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	switch (id) {
	case TAG::BTN_SWITCH:
		if (this->_data->getBatteryPercentage() == 0) {
			auto color = {
				"#FFD34B",
					"#FF6666",
					"#E852E6",
					"#55CCF5"
			};
			auto options = guiMgr->getGui(Popup::POPUP_CONFIRM_2_OPTION);
			/*options.showWithQuestion(
				languageMgr->localizedText("text_charge"),
				languageMgr->localizedText("text_charge_battery_confirm").replace("@1", "<color=" + color[this->_data.type - 1] + ">" + this->_data.name + "</color>").replace("@2", this->_data.getBatteryPercentage() + "\ \ "),
				languageMgr->localizedText("text_agree"),
				languageMgr->localizedText("text_cancel"),
				void InventoryElementUIv2::() {
				gv.inventoryMgr.chargeBatteryCue(this->_data.id);
			}.bind(this)
				)*/
		}
		else if (this->_data->isInBag()) {
			inventoryHandler->requestRemoveCueFromBag(this->_data->_id);
		}
		else {
			auto bagFull = inventoryMgr->isCueBagFull();
			if (!bagFull) {
				inventoryHandler->requestAddCueToBag(this->_data->_id);
			}
		}
		break;
	case TAG::BTN_ITEM:
		//gameMgr->loadSceneByTag(Scenes.INSPECT_SCENE).loadData(this->_data, true);
		break;
	case TAG::BTN_AUTO_CHARGE:
		if (this->_data->isInBag()) {
		}
		break;
	}
}

ElementTypeFrameData InventoryElementUI::TYPE_FRAMES[4] =
{
	ElementTypeFrameData("Icon_Color_02.png", "Yellow_Box.png", Color3B(235, 201, 160)),
	ElementTypeFrameData("Icon_Color_02.png", "Red_Box.png", Color3B(241, 116, 145)),
	ElementTypeFrameData("Icon_Color_03.png", "Purple_Box.png", Color3B(178, 140, 221)),
	ElementTypeFrameData("Icon_Color_04.png", "Blue_Box.png", Color3B(140, 186, 221))
};

//Battery-----------------------

Battery::Battery()
{

}

Battery::~Battery()
{

}

Battery * Battery::create(cocos2d::Node *batteryBox, cocos2d::ui::ImageView *battery, cocos2d::ui::Text *text)
{
	Battery *b = new Battery();
	b->init(batteryBox, battery, text);
	return b;
}

bool Battery::init(cocos2d::Node *batteryBox, cocos2d::ui::ImageView *battery, cocos2d::ui::Text *text)
{
	_batteryBox = (ImageView*)batteryBox;
	_battery = battery;
	_text = text;

	_battery->removeFromParent();

	auto p = _battery->getPosition();
	auto size = _battery->getContentSize();
	auto cn = ClippingNode::create();
	auto stencil = DrawNode::create();

	stencil->drawSolidRect(
		Vec2(p.x - size.width*_battery->getScaleX() / 2, p.y - size.height*_battery->getScaleY() / 2),
		Vec2(p.x + size.width*_battery->getScaleX() / 2, p.y + size.height*_battery->getScaleY() / 2),
		Color4F(Color3B(255, 255, 255))
	);
	_rectWidthStencil = size.width * _battery->getScaleX();
	cn->setStencil(stencil);
	_batteryBox->addChild(cn);
	cn->addChild(_battery);
	_stencil = stencil;
	
	return true;
}

void Battery::setPercent(float percent)
{
	auto x = -_rectWidthStencil + percent / 100 * _rectWidthStencil;
	_stencil->setPositionX(x);
}

void Battery::setValue(int val, int max)
{
	char text[20];
	sprintf(text, "%d/%d", val, max);
	_text->setText(text);
}
