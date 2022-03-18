#include "LoadingScene.h"
#include "core/utils/LanguageMgr.h"
#include "core/utils/JNI.h"

USING_NS_CC;

LoadingScene::LoadingScene()
{

}

LoadingScene::~LoadingScene()
{

}

bool LoadingScene::init()
{
	BaseLayer::init();

	initWithBinaryFile("Lobby/LoadingScene.csb");

	return true;
}

void LoadingScene::initGUI()
{
	Size size = Director::getInstance()->getVisibleSize();

	Node* logo = getControl("logo");
	logo->setScale(_scale);
	logo->setPosition(Vec2(size.width / 2, size.height / 2 + logo->getContentSize().width / 2));

	Node* bar = getControl("bar");
	bar->setScaleX(size.width / bar->getContentSize().width);
	bar->setScaleY(_scale);
	bar->setPosition(size.width / 2, 0);

	lbVersion = (ui::Text*)getControl("lbVersion");
	lbUpdate = (ui::Text*)getControl("lbUpdate");

	lbVersion->setPosition(Vec2(size.width - 10, bar->getContentSize().height * _scale));
	lbUpdate->setPosition(Vec2(size.width / 2, bar->getContentSize().height * _scale));
}

void LoadingScene::onEnterFinish()
{
	lbVersion->setString(JNI::getVersionString());
	lbUpdate->setString(LocalizedString::to("DOWNLOAD_GAME_INFORMATION"));
}

const std::string LoadingScene::className = "LoadingScene";

