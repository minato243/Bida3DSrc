#include "MessagePopup.h"
#include "../../../data/Resource.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

MessagePoup::MessagePoup()
{
	_callback = NULL;
}

MessagePoup::~MessagePoup()
{

}

bool MessagePoup::init()
{
	if (!BaseLayer::init("MessagePoup"))
	{
		return false;
	}
	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::PopupTwoChoice);
	this->initWithBinaryFile(filePath);

	this->_keep = true;

	return true;
}

void MessagePoup::initGUI()
{
	initTag();
	autoInitComponent(_layout);

	_bg = _nodeMap["bg"];

	_lbTitle = (Text*)_nodeMap["lbTitle"];
	_rtDesc = (Text*)_nodeMap["rtDesc"];
	_btnPositive = (Button*)_nodeMap["btnPositive"];
	_btnNegative = (Button*)_nodeMap["btnNegative"];

	_lbBtnPositive = (Text*)getControl("lb", _btnPositive);
	_lbBtnNegative = (Text*)getControl("lb", _btnNegative);

	_centerBtnPos = Vec2(_bg->getContentSize().width / 2, _btnPositive->getPositionY());
}

void MessagePoup::initTag()
{
	_tagMap["btnNegative"] = TAG::btnNegative;
	_tagMap["btnPositive"] = TAG::btnPositive;
}

void MessagePoup::setOkDialog(const std::string & message, const PopupListener & callback)
{
	setOkDialog("NOTIFICATION", message, callback);
}

void MessagePoup::setOkDialog(const std::string &title, const std::string &message, const PopupListener & callback)
{
	_lbTitle->setString(title);
	_rtDesc->setString(message);
	_callback = callback;

	_btnPositive->setVisible(true);
	_btnNegative->setVisible(false);
	_btnPositive->setPosition(_centerBtnPos);
}

void MessagePoup::setCancelDialog(const std::string & message, const PopupListener & callback)
{
	_rtDesc->setString(message);
	_callback = callback;

	_btnPositive->setVisible(false);
	_btnNegative->setVisible(true);
	_btnNegative->setPosition(_centerBtnPos);
}

void MessagePoup::setOkCancelDialog(const std::string & message, const PopupListener & callback)
{
	setOkCancelDialog("NOTIFICATION", message, callback);
}

void MessagePoup::setOkCancelDialog(const std::string & title, const std::string & message, const PopupListener & callback)
{
	_lbTitle->setString(title);
	_rtDesc->setString(message);
	_callback = callback;

	_btnPositive->setVisible(true);
	_btnNegative->setVisible(true);
	_btnPositive->setPosition(_defaultPos[_btnPositive]);
	_btnNegative->setPosition(_defaultPos[_btnNegative]);
}

void MessagePoup::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	hide();
	if (_callback)
	{
		_callback(id);
	}
}

void MessagePoup::setBtnText(std::string positiveText, std::string negativeText)
{
	setOkBtnText(positiveText);
	setCancelBtnText(negativeText);
}

void MessagePoup::setOkBtnText(const std::string & text)
{
	_lbBtnPositive->setString(text);
}

void MessagePoup::setCancelBtnText(const std::string & text)
{
	_lbBtnNegative->setString(text);
}

