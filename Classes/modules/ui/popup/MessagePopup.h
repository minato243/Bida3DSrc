#ifndef MessagePopup_h_
#define MessagePopup_h_

#include "../../../core/gui/BaseLayer.h"

class MessagePoup
	:public BaseLayer
{
public:
	typedef std::function<void(int btnId)> PopupListener;

	enum TAG 
	{
		btnPositive,
		btnNegative,
	};

	MessagePoup();
	~MessagePoup();

	bool init();
	CREATE_FUNC(MessagePoup);

	void initGUI();

	void initTag();

	void setOkDialog(const std::string & message, const PopupListener & callback);

	void setOkDialog(const std::string & title, const std::string & message, const PopupListener & callback);

	void setCancelDialog(const std::string & message, const PopupListener & callback);

	void setOkCancelDialog(const std::string & message, const PopupListener & callback);

	void setOkCancelDialog(const std::string &title, const std::string & message, const PopupListener & callback);

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void setBtnText(std::string positiveText, std::string negativeText);

	void setOkBtnText(const std::string & text);

	void setCancelBtnText(const std::string & text);

protected:
private:

	cocos2d::ui::Text *_lbTitle;
	cocos2d::ui::Text *_rtDesc;

	cocos2d::ui::Button *_btnPositive;
	cocos2d::ui::Text *_lbBtnPositive;
	cocos2d::ui::Button *_btnNegative;
	cocos2d::ui::Text *_lbBtnNegative;

	cocos2d::Vec2 _centerBtnPos;

	PopupListener _callback;
};


#endif