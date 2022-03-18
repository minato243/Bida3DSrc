#include "ShopScene.h"
#include "../../camera-view/GameCamera.h"
#include "ShopUI.h"
#include "../GuiMgr.h"

USING_NS_CC;

ShopScene::ShopScene()
{
	_ui = nullptr;
}

ShopScene::~ShopScene()
{

}

bool ShopScene::init()
{
	BaseScene::init();

	this->initCameras();
	return true;
}

void ShopScene::initCameras()
{
	auto winSize = Director::getInstance()->getWinSize();
	auto ca = Camera::createOrthographic(
		winSize.width,
		winSize.height,
		-10,
		50);
	ca->setDepth(0);
	ca->setPosition3D(Vec3(0, 0, 10));
	ca->lookAt(Vec3(0, 0, 0));
	ca->setCameraFlag(GameCamera::FLAG_BG_LOWEST);
	this->addChild(ca);
	this->_cameras.push_back(ca);

	auto ca2 = Camera::createOrthographic(
		winSize.width,
		winSize.height,
		-10,
		50);
	ca2->setDepth(2);
	ca2->setPosition3D(Vec3(0, 0, 10));
	ca2->lookAt(Vec3(0, 0, 0));
	ca2->setCameraFlag(GameCamera::FLAG_UI);
	this->addChild(ca2);
	this->_cameras.push_back(ca2);

	auto ca3 = Camera::createOrthographic(
		winSize.width,
		winSize.height,
		-10,
		50
	);
	ca3->setDepth(4);
	ca3->setPosition3D(Vec3(0, 0, 10));
	ca3->lookAt(Vec3(0, 0, 0));
	ca3->setCameraFlag(GameCamera::FLAG_UI_ALWAYS_TOP);
	this->addChild(ca3);

	auto camCueModel = Camera::createOrthographic(
		winSize.width,
		winSize.height,
		-10,
		50
	);
	camCueModel->setDepth(3);
	camCueModel->setPosition3D(Vec3(0, 0, 10));
	camCueModel->lookAt(Vec3(0, 0, 0));
	camCueModel->setCameraFlag(GameCamera::FLAG_3D_ABOVE_UI);
	this->addChild(camCueModel);

	this->getDefaultCamera()->setDepth(4);
}

void ShopScene::onSceneReload()
{
	if (_ui)
	{
		((ShopUI*)_ui)->onSceneReload();
	}
}

void ShopScene::onEnter()
{
	BaseScene::onEnter();

	if (!_ui) {
		_ui = guiMgr->getGui(Popup::SHOP_UI);
		_ui->show();
		_ui->setCameraMask((int)GameCamera::FLAG_UI);
	}
}

void ShopScene::toShopTab()
{

}

