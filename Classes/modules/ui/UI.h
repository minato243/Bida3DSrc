#ifndef 	_UI_H_
#define 	_UI_H_


#include <cocos2d.h>

class UI: public cocos2d::Node{
public:

	UI();
	~UI();
	bool init();
	CREATE_FUNC(UI);
	void onEnter();
	void initUIControlButtions();
	void setVisible(int val);
	void getUI();
	void initUIBugList();
	void getIngameUI();

private:
	int _uiControlButtons;
};
#endif