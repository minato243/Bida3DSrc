#ifndef CheatItemUI_h_
#define CheatItemUI_h_

#include <core/gui/BaseLayer.h>

class CheatItemUI
	: public BaseLayer
{
public:
	enum TAG
	{
		btnSubmit = 101
	};
	CheatItemUI();
	~CheatItemUI();

	CREATE_FUNC(CheatItemUI);
	virtual bool init();

	void initGUI();

	void initTag();

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	int getType();

	int getNum();

private:
	cocos2d::ui::TextField *_tfType;
	cocos2d::ui::TextField *_tfNum;

public:

};
#endif