#include "CueCollectionCueItem.h"
#include "../../../../data/Resource.h"
#include "../../../utils/MoveDetector.h"
#include "../../../core/inventory/CueInfo.h"
#include "../../../camera-view/GameCamera.h"
#include "../ModelOffset.h"
#include "../../GuiMgr.h"
#include "CueCollectionUIDetail.h"

USING_NS_CC;
using namespace cocos2d::ui;

CueCollectionCueItem::CueCollectionCueItem()
{
	_modeDetail = false;
	_moveDetector = nullptr;

	auto frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
	fadeTR = Vec2(frameSize.width - 50, frameSize.height);
	opaqueTR = Vec2(frameSize.width - 150, frameSize.height);
}

CueCollectionCueItem::~CueCollectionCueItem()
{

}

void CueCollectionCueItem::initUI()
{
	auto layout = Layout::create();
	layout->setBackGroundColorType(Layout::BackGroundColorType::NONE);
	layout->setContentSize(defaultSize);
	layout->setAnchorPoint(Vec2(0.5, 0.5));
	layout->setPosition(Vec2(0, 0));
	_bg = layout;
	addChild(layout);

	auto button = Button::create(Res::Base::img_btn_normal, Res::Base::img_btn_normal, Res::Base::img_btn_normal);
	button->setScale9Enabled(true);
	button->setUnifySizeEnabled(false);
	button->ignoreContentAdaptWithSize(false);
	auto capInsets = Rect(15, 15, 15, 15);
	button->setCapInsets(capInsets);
	button->setContentSize(Size(200, 500));
	button->setAnchorPoint(Vec2(0.5, 0.5));
	button->setPosition(Vec2(0, 0));
	button->setSwallowTouches(false);
	button->setOpacity(0);
	button->addTouchEventListener(CC_CALLBACK_2(CueCollectionCueItem::onButtonTouched, this));
	_itemBtn = button;
	addChild(button, true);
}

bool CueCollectionCueItem::init()
{
	Node::init();
	initUI();
	init3D();
	return true;
}

bool CueCollectionCueItem::init(cocos2d::Size size)
{
	init();
	_size = Size(size.width *0.95, size.height * 0.95);
	autoResize();
	return true;
}

void CueCollectionCueItem::init3D()
{
	_curCue = nullptr;
	_cueNode = Node::create();
	_cueNode->setRotation(110);
	addChild(_cueNode);
}

void CueCollectionCueItem::autoResize()
{
	auto ratio = _size.width / _size.height;
	auto orgRatio = defaultSize.width / defaultSize.height;

	if (ratio < orgRatio) {
		_bg->setScale(_size.width / defaultSize.width);
	}
	else {
		_bg->setScale(_size.height / defaultSize.height);
	}

	_itemBtn->setContentSize(_size);
}

void CueCollectionCueItem::onButtonTouched(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (sender ==_itemBtn) {
		Button *btn = (Button*)sender;
		if (type == Widget::TouchEventType::BEGAN) {
			if(_moveDetector == nullptr)
				_moveDetector = new MoveDetector();
			_moveDetector->init(btn->getTouchBeganPosition());
		}
		if (type == Widget::TouchEventType::MOVED) {
			_moveDetector->setPoint(btn->getTouchMovePosition());
		}
		if (type == Widget::TouchEventType::ENDED && !_moveDetector->isMoved() &&_data != nullptr) {
			onClick();
		}
	}
}

void CueCollectionCueItem::onClick()
{
	CCLOG("CueCollectionCueItem::onClick");
	if (_modeDetail) {
		if (_data == nullptr) return;
		auto detailUI = guiMgr->getGui(Popup::Cue_COLLECTION_DETAIL);
		((CueCollectionUIDetail*)detailUI)->scrollToData(_data);
	}
	else {
		auto detailUI = guiMgr->getGui(Popup::Cue_COLLECTION_DETAIL);
		detailUI->show();
		((CueCollectionUIDetail*)detailUI)->loadData(_data->type, _idx -1);
	}
}

void CueCollectionCueItem::loadData(CueInfoData *data)
{
	_data = data;
	if (data == nullptr) {
		if (_curCue != nullptr)
			_curCue->setVisible(false);
		return;
	}
	auto paths = CueInfo::getCueModel(data->configId, data->type, data->star, data->level);
	showCue3D(paths.model, paths.tex);
	updateCueState();
}

void CueCollectionCueItem::showCue3D(std::string model, std::string texture)
{
	if (_curCue != nullptr) {
		_curCue->setVisible(false);
		_curCue = nullptr;
	}
	if (_cues.find(model) == _cues.end()) {
		auto cue = Sprite3D::create(model);
		cue->setTexture(texture);
		cue->setCameraMask((int)GameCamera::FLAG_3D_ABOVE_UI);
		auto state = GLProgramState::create(GLProgram::createWithFilenames("res/shaders/basic_3d.vert", "res/shaders/basic_3d_fade.frag"));
		auto scale = model.find("Stick") > 0 ? 2 : 1.65;
		scale *= Director::getInstance()->getWinSize().width / 1136;
		cue->setGLProgramState(state);
		state->setUniformVec2("fadeBottomLeft", fadeBL);
		state->setUniformVec2("opaqueBottomLeft", opaqueBL);
		state->setUniformVec2("fadeTopRight", fadeTR);
		state->setUniformVec2("opaqueTopRight", opaqueTR);
		_states.insert(std::pair<Sprite3D*, GLProgramState*>(cue, state));
		cue->setScale(scale);
		cue->setRotation3D(Vec3(0, 270, 0));
		cue->setPosition(Vec2(-modelOffset->getModelWidthByPath(model) / 2 * scale, 0));

		_cueNode->addChild(cue);

		// set Rotation
		_cues.insert(std::pair<std::string, Sprite3D*>(model, cue));
	}
	_curCue = _cues[model];
	_curCue->setTexture(texture);
	_curCue->setVisible(true);
}

void CueCollectionCueItem::updateCueState()
{
	if (_data == nullptr || !_curCue) return;
	auto state = _states[_curCue];
	state->setUniformFloat("u_disabled", _data->owned ? 0 : _active ? 2 : 1);
}

void CueCollectionCueItem::setIdx(int idx)
{
	_idx = idx;
}

cocos2d::Size CueCollectionCueItem::getOriginalSize()
{
	return defaultSize;
}

void CueCollectionCueItem::setModeDetail(bool modeDetail)
{
	_modeDetail = modeDetail;
}

void CueCollectionCueItem::setActive(bool isActive)
{
	_active = isActive;
	setScale(_active ? 1.3 : 1);
	updateCueState();
}

cocos2d::Size CueCollectionCueItem::defaultSize = Size(200, 500);

cocos2d::Vec2 CueCollectionCueItem::fadeBL = Vec2(50, 0);

cocos2d::Vec2 CueCollectionCueItem::fadeTR;

cocos2d::Vec2 CueCollectionCueItem::opaqueBL = Vec2(150, 0);

cocos2d::Vec2 CueCollectionCueItem::opaqueTR;

