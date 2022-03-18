#include "Dialog.h"
#include "../utils/LanguageMgr.h"

USING_NS_CC;
using namespace cocos2d::ui;

Dialog::Dialog()
{
	_bg = nullptr;
	_btnOK = nullptr;
	_btnCancel = nullptr;
	_lb_message = nullptr;

	_btnId = -1;
	_callback = nullptr;
}

Dialog::~Dialog()
{

}

bool Dialog::init()
{
	BaseLayer::init(Dialog::className);

	initWithBinaryFile("Dialog.csb");
	return true;
}

void Dialog::initGUI()
{
	Node* bg = getControl("bg");
	_bg = bg;

	_btnOK = customButton("btnOK", Dialog::BTN_OK, bg);
	LocalizedSprite::buttonLocalized(_btnOK, LocalizedSprite::getDir("Lobby_Button_btn_ok"));
	_btnCancel = customButton("btnCancel", Dialog::BTN_CANCEL, bg);
	LocalizedSprite::buttonLocalized(_btnCancel, LocalizedSprite::getDir("Lobby_Button_btn_cancel"));
	_btnClose = customButton("btnQuit", Dialog::BTN_QUIT, bg);

	_lb_message = (Text*)getControl("lb_message", bg);

	_pLeft = _btnOK->getPosition();
	_pRight = _btnCancel->getPosition();
	_pCenter = Helper::seekWidgetByName(Helper::seekWidgetByName((Widget*)_layout, "bg"), "btnCenter")->getPosition();
}

void Dialog::onEnterFinish()
{
	setShowHideAnimate(_bg, true);
	setFog(true);
}

void Dialog::resetButton()
{
	_btnOK->setVisible(false);
	_btnCancel->setVisible(false);

	_btnOK->setPosition(_pLeft);
	_btnCancel->setPosition(_pRight);

	_callback = nullptr;
	_btnId = -1;
}

void Dialog::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	_btnId = id;
	onClose();
}

void Dialog::onCloseDone()
{
	BaseLayer::onCloseDone();

	if (_callback)
		_callback(_btnId);
}

void Dialog::setOkCancel(std::string message, const DialogListener& listener)
{
	setMessage(message);
	_callback = listener;

	_btnOK->loadTextures("Button/btn_ok.png", "Button/btn_ok.png", "");

	_btnOK->setPosition(_pLeft);
	_btnCancel->setPosition(_pRight);
	_btnOK->setVisible(true);
	_btnCancel->setVisible(true);
}

void Dialog::setOkWithAction(std::string message, const DialogListener& listener)
{
	setMessage(message);
	_callback = listener;

	_btnOK->loadTextures("Button/btn_ok.png", "Button/btn_ok.png", "");
	_btnOK->setVisible(true);
	_btnOK->setPosition(_pCenter);

	_btnCancel->setVisible(false);
	_btnClose->setVisible(false);
}

void Dialog::setOKNotify(std::string message)
{
	setMessage(message);

	_btnOK->loadTextures("Button/btn_ok.png", "Button/btn_ok.png", "");
	_btnOK->setVisible(true);
	_btnOK->setPosition(_pCenter);

	_btnCancel->setVisible(false);
}

void Dialog::setChangeGold(std::string message, const DialogListener& listener)
{
	setMessage(message);
	_callback = listener;
	_btnOK->loadTextures(LocalizedSprite::getDir("Lobby_Button_btnGold"), LocalizedSprite::getDir("Lobby_Button_btnGold"), "");
	_btnOK->setVisible(true);
	_btnOK->setPosition(_pLeft);

	_btnCancel->setVisible(true);
	_btnCancel->setPosition(_pRight);
}

void Dialog::setAddG(std::string message, const DialogListener& listener)
{
	setMessage(message);
	_callback = listener;

	_btnOK->loadTextures("Button/btnG.png", "Button/btnG.png", "");
	_btnOK->setVisible(true);
	_btnOK->setPosition(_pLeft);

	_btnCancel->setVisible(true);
	_btnCancel->setPosition(_pRight);
}

void Dialog::setMessage(std::string message)
{
	resetButton();
	setLabelText(message, _lb_message);
}

std::string Dialog::className = "Dialog";

const int Dialog::BTN_OK = 0;

const int Dialog::BTN_CANCEL = 1;

const int Dialog::BTN_QUIT = 2;

const int Dialog::ZODER = 10000;

const int Dialog::TAG = 10000;

const int Dialog::SUPPORT = 800;

