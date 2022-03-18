#include "WorkshopSwapUI.h"
#include "../../../../data/Resource.h"

WorkshopSwapUI::WorkshopSwapUI()
{

}

WorkshopSwapUI::~WorkshopSwapUI()
{

}

bool WorkshopSwapUI::init()
{
	if (!BaseLayer::init())
		return false;
	
	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::WorkshopSwapUI);
	this->initWithBinaryFile(filePath);

	initGUI();
		
	return true;
}

void WorkshopSwapUI::initGUI()
{

}

