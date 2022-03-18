#include "WorkshopFusionUI.h"
#include "../../../../data/Resource.h"

WorkshopFusionUI::WorkshopFusionUI()
{

}

WorkshopFusionUI::~WorkshopFusionUI()
{

}

bool WorkshopFusionUI::init()
{
	if (!BaseLayer::init())
		return false;

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::WorkshopFusionUI);
	this->initWithBinaryFile(filePath);

	initGUI();

	return true;
}

void WorkshopFusionUI::initGUI()
{

}

