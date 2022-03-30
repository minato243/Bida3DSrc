#ifndef NotifyPopup_h_
#define NotifyPopup_h_

#include "../base/UIBaseLayer.h"
#include "../../../data/GameConstant.h"

class NotifyPopup 
: public UIBaseLayer
{
public:
	NotifyPopup();
	~NotifyPopup();

	enum TAG 
	{
		btnConfirm
	};

	bool init();
	CREATE_FUNC(NotifyPopup);

	void initGUI();

	void initTag();

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void setMessage(const std::string& title, const std::string& message, const std::string& confirmText, CUSTOM_CALLBACK callback = NULL);

	void setMessage(const std::string& title, const std::string& message, CUSTOM_CALLBACK callback = NULL);

private:
	CUSTOM_CALLBACK _callback;

	cocos2d::ui::Text *_lbTitle;
	cocos2d::ui::Text *_rtNotify;
	cocos2d::ui::Button *_btnConfirm;
	cocos2d::ui::Text *_lbConfirm;
};
#endif // !NotifyPopup_h_
