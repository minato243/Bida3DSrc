#include "CueInspectScene.h"
#include "../GuiMgr.h"
#include "../../camera-view/GameCamera.h"
#include "CueInspectUI.h"

CueInspectScene::CueInspectScene()
{
	_ui = NULL;
	_data = NULL;
}

CueInspectScene::~CueInspectScene()
{
	
}

bool CueInspectScene::init()
{
	BaseScene::init();

	return true;
}

void CueInspectScene::onSceneReload()
{
	if (_ui)
	{
		_ui->refresh();
	}
}

void CueInspectScene::onEnter()
{
	BaseScene::onEnter();

	if (!_ui) {
		_ui = guiMgr->getGui(Popup::CUE_INSPECT_UI);
		_ui->show();
		_ui->setCameraMask((int)GameCamera::FLAG_UI);
	}

	auto ui = (CueInspectUI*)_ui;
	if (_data)
	{
		ui->setInfo(_data, _isOwn, _isGacha, _isFusion);
		_data = NULL;
	}

	getDefaultCamera()->setDepth(CueInspectUI::CAM_DEPTH::DEPTH_ABOVE + 2);
}

void CueInspectScene::loadData(CueInfo *data, bool isOwn, bool isGacha /*= false*/, bool isFusion /*= false*/)
{
	_data = data;
	_isOwn = isOwn;
	_isGacha = isGacha;
	_isFusion = isFusion;
	if (_ui)
	{
		auto ui = (CueInspectUI*)_ui;
		ui->setInfo(data, isOwn, isGacha, isFusion);
	}
}

