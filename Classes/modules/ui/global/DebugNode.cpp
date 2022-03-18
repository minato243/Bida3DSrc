#include "DebugNode.h"
#include "../../../data/Resource.h"
#include "../GuiMgr.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;


DebugNode::DebugNode()
{

}

DebugNode::~DebugNode()
{

}

bool DebugNode::init()
{
	if (!BaseLayer::init("DebugNode"))
	{
		return false;
	}

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::DebugNode);
	this->initWithBinaryFile(filePath);

	this->_keep = true;

	return true;
}

void DebugNode::initGUI()
{
	initTag();
	autoInitComponent(_layout);
}

void DebugNode::initTag()
{
	_tagMap.insert(pair<string, int>("btnMain", btnMain));
}

void DebugNode::onButtonRelease(cocos2d::ui::Button* button, int id)
{
	switch (id)
	{
	case TAG::btnMain:
	{
		showDebugDialog();
		break;
	}
	default:
		break;
	}
}

void DebugNode::showDebugDialog()
{
	guiMgr->getGui(Popup::DEBUG_DIALOG)->show();
}

