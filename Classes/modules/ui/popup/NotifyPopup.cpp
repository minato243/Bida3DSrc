#include "NotifyPopup.h"
#include "../../../data/Resource.h"
#include "../../../core/utils/LanguageMgr.h"

USING_NS_CC;
using namespace cocos2d::ui;

NotifyPopup::NotifyPopup()
{
	_keep = true;
}

NotifyPopup::~NotifyPopup()
{

}

bool NotifyPopup::init()
{
	if (!UIBaseLayer::init())
		return false;

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::PopupConfirmOneChoice);
	this->initWithBinaryFile(filePath);

	this->_keep = true;

	return true;
}

void NotifyPopup::initGUI()
{
	initTag();
	autoInitComponent(_layout);

	_bg = _nodeMap["bg"];

	_lbTitle = (Text*)_nodeMap["lbTitle"];
	_rtNotify = (Text *)_nodeMap["rtNotify"];
	_btnConfirm = (Button *)_nodeMap["btnConfirm"];
	_lbConfirm = (Text *)getControl("lb", _btnConfirm);
}

void NotifyPopup::initTag()
{
	_tagMap["btnConfirm"] = btnConfirm;
}

void NotifyPopup::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	if (_callback) {
		_callback();
	}
	hide();
}

void NotifyPopup::setMessage(const std::string& title, const std::string& message, const std::string& confirmText, CUSTOM_CALLBACK callback)
{
	_lbTitle->setString(title);
	_rtNotify->setString(message);
	_lbConfirm->setString(confirmText);
	_callback = callback;
}

void NotifyPopup::setMessage(const std::string& title, const std::string& message, CUSTOM_CALLBACK callback /*= NULL*/)
{
	setMessage(title, message, languageMgr->localizedText("text_Accept"), callback);
}

