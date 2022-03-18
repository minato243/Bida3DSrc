#include "CueGachaElementUI.h"
#include "../../../data/Resource.h"
#include "../../camera-view/GameCamera.h"
#include "../../core/inventory/InventoryMgr.h"
#include "../../../data/config/ConfigMgr.h"
#include "core/gui/SceneMgr.h"
#include "../inventory/CueInspectScene.h"
#include "../../core/ps/internal/PhysicsConstants.hpp"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;
using namespace ps;

CueGachaElementUI::CueGachaElementUI()
{
	_isInitRect = false;
	_cue3D = nullptr;
}

CueGachaElementUI::~CueGachaElementUI()
{

}

bool CueGachaElementUI::init()
{
	if (!BaseLayer::init("CueGachaElementUI"))
	{
		return false;
	}

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::CueGachaElementUI);
	this->initWithBinaryFile(filePath);

	_size = getOriginalSize();
	
	refresh();
	return true;
}

void CueGachaElementUI::initGUI()
{
	initTag();
	autoInitComponent(_layout);

	_bg = _nodeMap["bg"];
	_lbName = (cocos2d::ui::Text *) _nodeMap["lbName"];
	_lbGeneralPoint = (cocos2d::ui::Text *) _nodeMap["lbGeneralPoint"];
	_btnInspect = (cocos2d::ui::Button *) _nodeMap["btnInspect"];

	setCameraMask((int)GameCamera::FLAG_UI);
}

void CueGachaElementUI::initTag()
{
	_tagMap.insert(pair<string, int>("btnInspect", btnInspect));
}

void CueGachaElementUI::setContainerRect(cocos2d::Rect rect)
{
	_rect = rect;
}

cocos2d::Size CueGachaElementUI::getOriginalSize()
{
	return _bg->getContentSize();
}

void CueGachaElementUI::addCueModel(const std::string & model, const std::string & tex)
{
	auto winSize = Director::getInstance()->getWinSize();
	Sprite3D* cue;
	if (_cues.find(model) != _cues.end())
	{
		cue = _cues[model];
	}
	else 
	{
		cue = Sprite3D::create(model);
		cue->setTexture(tex);

		addChild(cue);
		cue->setCameraMask((int)GameCamera::FLAG_3D_ABOVE_UI);

		if (_isInitRect)
		{
			auto state = GLProgramState::create(GLProgram::createWithFilenames("res/shaders/basic_3d.vert", "res/shaders/basic_3d_rect_mask.frag"));
			cue->setGLProgramState(state);
			state->setUniformVec2("u_min", Vec2(_rect.origin.x, _rect.origin.y));
			state->setUniformVec2("u_max", Vec2(_rect.origin.x + _rect.size.width, _rect.origin.y + _rect.size.height));
		}

		cue->setRotation3D(Vec3(0, -90, 0));
		auto pos = Vec2(-_bg->getContentSize().width / 2 + 30, -20);
		cue->setPosition(pos);
		cue->setScale(2);
		cue->setScale(1.65 * winSize.width / 1136);
		_cueOriginalScale.insert(pair<Sprite3D*, float>(cue, 1.65 * winSize.width / 1136));
		Vec2 rat = Vec2((cue->getPositionX() + _size.width / 2) / _size.width, (cue->getPositionY() + _size.height / 2) / _size.height);
		_cueRatio.insert(pair<Sprite3D*, Vec2>(cue, rat));
		_cues.insert(pair<string, Sprite3D*>(model, cue));
	}

	if(_cue3D) _cue3D->setVisible(false);
	_cue3D = cue;
	_cue3D->setVisible(true);
}

void CueGachaElementUI::loadData(CueRateInBox *data)
{
	_data = data;

	auto cueInfo = inventoryMgr->convertCueIdToComponent(data->cueId);
	setNumStar(cueInfo.star);
	_lbName->setString(loadCueName(cueInfo.type));
	_lbGeneralPoint->setString(to_string(data->cueId));

	auto modelPath = CueInfo::getCueModel(cueInfo.configId, cueInfo.type, cueInfo.star);
	addCueModel(modelPath.model, modelPath.tex);
}
string CueGachaElementUI::loadCueName(int type)
{
	auto name = "";
	switch (type) {
	case 1: name = "Neutral Cue"; break;
	case 2: name = "Break Cue"; break;
	case 3: name = "Jump Cue"; break;
	default: name = "Masse Cue"; break;
	}
	return name;
}

void CueGachaElementUI::setNumStar(int num)
{
	num = min(5, max(1, num));
	for (auto i = 0; i < 7; i++) {
		auto star = (ImageView*)_nodeMap["star_" + to_string(i)];
		if (i < num)
		{
			star->loadTexture("inven_Star_02.png", Widget::TextureResType::PLIST);
		}
		else
		{
			star->loadTexture("inven_Star_01.png", Widget::TextureResType::PLIST);
		}
	}
}

void CueGachaElementUI::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	switch (id)
	{
	case TAG::btnInspect:
	{
		auto cueInfo = inventoryMgr->convertCueIdToComponent(_data->cueId);
		CueInfo *cue = new CueInfo(cueInfo);

		auto inspectScene = sceneMgr->loadSceneByTag(SceneTag::INSPECT_SCENE);
		((CueInspectScene*)inspectScene)->loadData(cue, false);
		break;
	}
	default:
		break;
	}
}

