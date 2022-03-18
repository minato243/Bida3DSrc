#ifndef FullScreenPopup_h_
#define FullScreenPopup_h_

#include <core/gui/BaseLayer.h>

class FullScreenPopup
	: public BaseLayer
{
public:
	FullScreenPopup();
	~FullScreenPopup();

	CREATE_FUNC(FullScreenPopup);
	virtual bool init();

	void initGUI();

	void setTitle(const std::string & title);

	void setTitleInventory(bool val);

	cocos2d::Node *getContainer();

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void onRequestClose();

public:
	cocos2d::ui::Text *_txtTitle;
	cocos2d::ui::ImageView *_imgTitle;
	cocos2d::ui::Widget *_container;

	bool _keep;

public:
	static const int BTN_CLOSE = 10001;

};
#endif