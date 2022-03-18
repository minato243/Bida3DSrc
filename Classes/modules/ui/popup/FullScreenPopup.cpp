#include "FullScreenPopup.h"
#include "../../../data/Resource.h"

USING_NS_CC;
using namespace cocos2d::ui;

FullScreenPopup::FullScreenPopup()
{

}

FullScreenPopup::~FullScreenPopup()
{

}

bool FullScreenPopup::init()
{
	if (!BaseLayer::init("FullScreenPopup"))
	{
		return false;
	}

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::FullScreenPopup);
	this->initWithBinaryFile(filePath);

	this->_keep = true;
	this->setTitleInventory(false);

	return true;
}

void FullScreenPopup::initGUI()
{
	auto bg = this->_layout->getChildByName("bg");
	this->customButton("btn_close", FullScreenPopup::BTN_CLOSE, bg);
	this->_txtTitle = (Text*)bg->getChildByName("txt_title");
	this->_imgTitle = (ImageView*)bg->getChildByName("img_title");
	this->_txtTitle->ignoreContentAdaptWithSize(true);
	this->_container = (Widget*)bg->getChildByName("container");
}

void FullScreenPopup::setTitle(const std::string & title)
{
	this->_txtTitle->setText(title);
	this->_txtTitle->setVisible(true);
	this->_imgTitle->setVisible(false);
}

void FullScreenPopup::setTitleInventory(bool val)
{
	this->_txtTitle->setVisible(!val);
	this->_imgTitle->setVisible(val);
}

cocos2d::Node * FullScreenPopup::getContainer()
{
	return this->_container;
}

void FullScreenPopup::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	switch (id) {
	case FullScreenPopup::BTN_CLOSE:
		this->onRequestClose();
		break;
	}
}

void FullScreenPopup::onRequestClose()
{

}

