#ifndef LogUI_h__
#define LogUI_h__
#include "core/gui/BaseLayer.h"

class LogUI :
	public BaseLayer {
public:
	CREATE_FUNC(LogUI);
	LogUI();
	~LogUI();
public:
	bool init();
	void initGUI();
	void onEnterFinish();
	void onButtonRelease(cocos2d::ui::Button* button, int id);
	void addLog(std::string msg);
	static void show(cocos2d::Node* parent);
	static void log(std::string msg);
public:
	cocos2d::ui::ListView* list;
	cocos2d::ui::Text* _label;
	std::string strMsg;

	static LogUI* logUI;
};
#endif // LogUI_h__
