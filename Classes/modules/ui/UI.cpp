#include "UI.h"

UI::UI() {
	/*this->initUIControlButtions();
	this->initUIBugList();*/
}

UI::~UI() {

}

bool UI::init() {
	return true;
}

void UI::onEnter() {
	//todo
	//this->_uiControlButtons.setDelegate(MainLayer.instance);
}
//
//void UI::initUIControlButtions() {
//	auto uiControlButtons = new UIControlButtons();
//	this->addChild(uiControlButtons);
//	this->_uiControlButtons = uiControlButtons;
//}
//
//void UI::setVisible(int val){
//	this->_super(val);
//	val?this->_uiControlButtons.show():this->_uiControlButtons.hide();
//}
//
//void UI::getUI(){
//	return this->_uiControlButtons;
//}
//
//void UI::initUIBugList() {
//
//}
//
//void UI::getIngameUI() {
//	return this->_uiControlButtons._ingameLayer;
//}
