#include "LogUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

LogUI::LogUI()
{

}

LogUI::~LogUI()
{

}

bool LogUI::init()
{
	BaseLayer::init();

	initWithBinaryFile("LogUI.csb");
	return true;
}

void LogUI::initGUI()
{
	Size size = Director::getInstance()->getVisibleSize();
	list =  (ListView*)getControl("list");
	customButton("close", 0, _layout, true);
	_label = (Text*)getControl("lb", list);
	_label->ignoreContentAdaptWithSize(false);
	_label->setContentSize(Size(size.width*_scale - 20, 5000));
	list->setContentSize(size);
	list->setInnerContainerSize(Size(size.width*_scale, 5000));
}

void LogUI::onEnterFinish()
{
	strMsg = "";
	//TODO: add cheat
	//	for (var s in CheatCenter.LOGS) {
//		this.strMsg += (CheatCenter.LOGS[s]);
//	}
	_label->setString(strMsg);
	list->scrollToBottom(0.5, true);
}

void LogUI::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	onClose();
}

void LogUI::addLog(std::string msg)
{
	strMsg += msg + "\n";
	_label->setString(strMsg);
}

void LogUI::show(cocos2d::Node* parent)
{
	if (!logUI)
	{
		logUI = LogUI::create();
	}
	Director::getInstance()->getRunningScene()->addChild(logUI, 9999999);
}

void LogUI::log(std::string msg)
{
	if (logUI) {
		logUI->addLog(msg);
	}
}

LogUI* LogUI::logUI = nullptr;

