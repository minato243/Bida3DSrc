#include "WorkshopUpgradeUI.h"
#include "../../../../data/Resource.h"
#include "../../../camera-view/GameCamera.h"
#include "../CueInspectUI.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

WorkshopUpgradeUI::WorkshopUpgradeUI()
{
	_cue = nullptr;
}

WorkshopUpgradeUI::~WorkshopUpgradeUI()
{

}

bool WorkshopUpgradeUI::init()
{
	if (!BaseLayer::init())
		return false;

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::WorkshopUpgradeUI);
	this->initWithBinaryFile(filePath);

	return true;
}

void WorkshopUpgradeUI::initGUI()
{
	initTag();
	autoInitComponent(_layout);

	_btnCueLevel = (cocos2d::ui::Button *)_nodeMap.find("btnCueLevel")->second;
	_imgMachine = (cocos2d::ui::ImageView *) _nodeMap.find("imgMachine")->second;

	_pnStats = (cocos2d::ui::Widget *) _nodeMap.find("pnStats")->second;
	_imgLevel = (cocos2d::ui::ImageView *) _nodeMap.find("imgLevel")->second;
	_imgLevel->setVisible(false);

	initBar();
	initCueView();
	//int mask = (int)GameCamera::FLAG_UI;
	//setCameraMask(mask);
	initMatSlots();
	loadText();
	//onSelectCue(nullptr);

	_nodeMap.find("fake")->second->setVisible(false);
	_nodeMap.find("fake1")->second->setVisible(false);
}

void WorkshopUpgradeUI::initTag()
{
	_tagMap.insert(std::pair<std::string, int>("btnGem1", TAG::BTN_GEN1));
	_tagMap.insert(std::pair<std::string, int>("btnGem2", TAG::BTN_GEN2));
	_tagMap.insert(std::pair<std::string, int>("btnGem3", TAG::BTN_GEN3));
	_tagMap.insert(std::pair<std::string, int>("btnGem4", TAG::BTN_GEN4));
	_tagMap.insert(std::pair<std::string, int>("btnGem5", TAG::BTN_GEN5));
	_tagMap.insert(std::pair<std::string, int>("btnMat1", TAG::BTN_MAT1));
	_tagMap.insert(std::pair<std::string, int>("btnMat2", TAG::BTN_MAT2));
	_tagMap.insert(std::pair<std::string, int>("btnMat3", TAG::BTN_MAT3));
	_tagMap.insert(std::pair<std::string, int>("btnMat4", TAG::BTN_MAT4));
	_tagMap.insert(std::pair<std::string, int>("btnCueLevel", TAG::BTN_CUE_LEVEL));
	_tagMap.insert(std::pair<std::string, int>("btnUpgrade", TAG::BTN_UPGRADE));
}

void WorkshopUpgradeUI::initMatSlots()
{

}

void WorkshopUpgradeUI::initBar()
{
	_pnForce = (Widget*)_nodeMap.find("pnForce")->second;
	_pnSpin = (Widget*)_nodeMap.find("pnSpin")->second;
	_pnAngle = (Widget*)_nodeMap.find("pnAngle")->second;
	_pnAim = (Widget*)_nodeMap.find("pnAim")->second;
	_pnAccuracy = (Widget*)_nodeMap.find("pnAccuracy")->second;

	_pnForceWrapper = new StatBarPanel(_pnForce);
	_pnSpinWrapper = new StatBarPanel(_pnSpin);
	_pnAimWrapper = new StatBarPanel(_pnAim);
	_pnAccuracyWrapper = new StatBarPanel(_pnAccuracy);

	_pnAngleWrapper = new StatBarPanelAngle(_pnAngle);

	_pnAccuracyWrapper->setPostfix("%%");
}

void WorkshopUpgradeUI::setUIInfo(const double & force, const double & angular,
	double angle[], const int & generalPoint,
	const int &aim, const double &accuracy)
{
	_pnForceWrapper->setInfo(force, force / CueInspectUI::MAX_FORCE);
	_pnSpinWrapper->setInfo(angular, angular / CueInspectUI::MAX_ANGULAR);
	_pnAngleWrapper->setInfo(floor(angle[0]), floor(angle[1]), (angle[1] - angle[0]) / CueInspectUI::MAX_ANGLE);
	_pnAimWrapper->setInfo(floor(aim), aim / CueInspectUI::MAX_AIM);
	_pnAccuracyWrapper->setInfo(floor(accuracy), accuracy / 100);
}

void WorkshopUpgradeUI::setUIInfoDefault()
{
	_pnForceWrapper->setDefaultValue();
	_pnSpinWrapper->setDefaultValue();
	_pnAngleWrapper->setDefaultValue();
	_pnAimWrapper->setDefaultValue();
	_pnAccuracyWrapper->setDefaultValue();
}

void WorkshopUpgradeUI::initModelWidthPath()
{
	_modelWidthByPath.insert(std::pair<std::string, float>(Res::CUE_VANG_1, 248));
	_modelWidthByPath.insert(std::pair<std::string, float>(Res::CUE_VANG_2, 254));
	_modelWidthByPath.insert(std::pair<std::string, float>(Res::CUE_DO_1, 260));
	_modelWidthByPath.insert(std::pair<std::string, float>(Res::CUE_DO_2, 268));
	_modelWidthByPath.insert(std::pair<std::string, float>(Res::CUE_TIM_1, 236));
	_modelWidthByPath.insert(std::pair<std::string, float>(Res::CUE_TIM_2, 247));
	_modelWidthByPath.insert(std::pair<std::string, float>(Res::CUE_XANH_1, 226));
	_modelWidthByPath.insert(std::pair<std::string, float>(Res::CUE_XANH_2, 238));
}

void WorkshopUpgradeUI::initCueView()
{
	auto imgMachine = getControl("imgMachine");
	_cueNode = getControl("nodeCue", imgMachine);
	_cueNode->setRotation3D(Vec3(0, 0, 161));
}

void WorkshopUpgradeUI::loadText()
{

}

void WorkshopUpgradeUI::onSelectCue(CueInfo * cue)
{
	auto hasCue = cue != nullptr;
	_btnCueLevel->getChildByName("imgBg")->setVisible(!hasCue);
	_imgLevel->setVisible(hasCue);
	_pnStats->getChildByName("nodeStar")->setVisible(hasCue);
	_pnStats->getChildByName("rtCueName")->setVisible(hasCue);
	_pnStats->getChildByName("lbNoCueSelected")->setVisible(!hasCue);
	_pnStats->getChildByName("nodeStar")->setVisible(hasCue);
	_imgMachine->getChildByName("imgLight")->setVisible(hasCue);
	if (hasCue) {
		for (auto i = 0; i < 5; i++) {
			if (i < cue->_tier) {
				((ImageView*)getNodeInMap("star" + to_string(i)))->loadTexture("Star_02.png", Widget::TextureResType::PLIST);
			}
			else {
				((ImageView*)getNodeInMap("star"+to_string(i)))->loadTexture("Star_01.png", Widget::TextureResType::PLIST);
			}
		}
		// _bg._pnStats._btnCueLevel._imgBtn.setVisible(false);
		/*auto typeImg = InventoryElementUI.TYPE_FRAMES[cue.type - 1].icon;
		_bg._pnStats._rtCueName.setString("<image=#" + typeImg + ">1</image>\ \ \ \ " + cue.name + "<image=#" + typeImg + ">1</image>");*/
		((Text*)_imgLevel->getChildByName("lbLevel"))->setText(to_string(cue->_level));
		setUIInfo(cue->_force, cue->_angular, cue->_angle, cue->_generalPoint, cue->_aim, cue->_accuracy);
		auto path = cue->getModelPath();
		showCue3D(path.model, path.tex, path.mrao);
		_imgMachine->loadTexture("workshop_Machine_01.png", Widget::TextureResType::PLIST);
	}
	else {
		_imgMachine->loadTexture("workshop_Machine_02.png", Widget::TextureResType::PLIST);
		if (_cue)
			_cue->setVisible(false);
		setUIInfoDefault();
	}
	_curCue = _cue;
}

void WorkshopUpgradeUI::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	switch (id)
	{
	case TAG::BTN_CUE_LEVEL:
		CCLOG("WorkshopUpgradeUI::onButtonRelease BTN_CUE_LEVEL");
		break;

	case TAG::BTN_MAT1:
		CCLOG("WorkshopUpgradeUI::onButtonRelease BTN_MAT1");
		break;
	case TAG::BTN_MAT2:
		CCLOG("WorkshopUpgradeUI::onButtonRelease BTN_MAT2");
		break;
	case TAG::BTN_MAT3:
		CCLOG("WorkshopUpgradeUI::onButtonRelease BTN_MAT3");
		break;
	case TAG::BTN_MAT4:
		CCLOG("WorkshopUpgradeUI::onButtonRelease BTN_MAT4");
		break;

	case TAG::BTN_GEN1:
		CCLOG("WorkshopUpgradeUI::onButtonRelease BTN_GEN1");
		break;
	case TAG::BTN_GEN2:
		CCLOG("WorkshopUpgradeUI::onButtonRelease BTN_GEN2");
		break;
	case TAG::BTN_GEN3:
		CCLOG("WorkshopUpgradeUI::onButtonRelease BTN_GEN3");
		break;
	case TAG::BTN_GEN4:
		CCLOG("WorkshopUpgradeUI::onButtonRelease BTN_GEN4");
		break;
	case TAG::BTN_GEN5:
		CCLOG("WorkshopUpgradeUI::onButtonRelease BTN_GEN5");
		break;
	case TAG::BTN_UPGRADE:
		CCLOG("WorkshopUpgradeUI::onButtonRelease BTN_UPGRADE");
		break;
	default:
		break;
	}
}

void WorkshopUpgradeUI::showCue3D(const std::string & path, const std::string texPath, const std::string & mraoPath)
{
	auto winSize = Director::getInstance()->getVisibleSize();
	if (_cue) _cue->setVisible(false);
	auto cues = _cues;
	if (cues.find(path) != cues.end()) {
		cues.find(path)->second->setVisible(true);
	}
	else {
		auto cue = Sprite3D::create(path);
		cue->setTexture(texPath);
		// cue.setRotation3D(cc.math.vec3(-0, 0, 0))

		auto scale = path.find("Stick") > 0 ? 2 : 1.65;
		scale *= winSize.width / 1136;
		auto cueOffset = scale * _modelWidthByPath.find(path)->second / 2;
		auto pos = Vec2(-cueOffset, 0);
		cue->setPosition(pos);
		cue->setRotation3D(Vec3(0, -90, 0));
			cue->setCameraMask((int)GameCamera::FLAG_3D_ABOVE_UI);
		cue->setScale(scale);
		cues[path] = cue;
		_cueNode->addChild(cue);

		// this.setUpShader(cue, texPath, mraoPath);
	}
	_cue = cues[path];
	// _cue.setPosition3D(cc.math.vec3(6,0,0));
}

float WorkshopUpgradeUI::getModelWidthPath(const std::string & path)
{
	if (_modelWidthByPath.find(path) != _modelWidthByPath.end())
		return _modelWidthByPath.find(path)->second;
	return 0;
}

