#ifndef DebugNode_h_
#define DebugNode_h_

#include <core/gui/BaseLayer.h>

class DebugNode
	: public BaseLayer
{
public:

	enum TAG
	{
		btnMain = 101
	};
	DebugNode();
	~DebugNode();

	CREATE_FUNC(DebugNode);
	virtual bool init();

	void initGUI();

	void initTag();

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void showDebugDialog();

public:

};
#endif