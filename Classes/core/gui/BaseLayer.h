#ifndef BaseLayer_h__
#define BaseLayer_h__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"

class BaseLayer:
	public cocos2d::Layer
{
public:
	BaseLayer();
	~BaseLayer();
	virtual bool init(std::string id = "");
public:
	virtual void onEnter();
	virtual void onExit();
	virtual void initWithJsonFile(std::string json);
	virtual void doEffect() {};
	virtual void initWithBinaryFile(std::string json);
	virtual void setAsPopup(bool value, bool isCache);

	virtual cocos2d::ui::Button* customizeButton(std::string name, int tag, cocos2d::Node* parent = nullptr);
	virtual cocos2d::ui::Button* customButton(std::string name, int tag, cocos2d::Node* parent = nullptr, bool action = true);
	virtual void setLabelWithLength(std::string text, cocos2d::ui::Text* label);
	virtual void setLabelText(std::string text, cocos2d::ui::Text* control);
	virtual cocos2d::Node* getControl(std::string cName, cocos2d::Node* parent = nullptr, std::string sParent = "object");
	virtual cocos2d::Node* getControl(std::string cName, std::string parent);
	virtual void processScaleControl(cocos2d::Node* control, int direct = -1);
	virtual void analyzeCustomControl(cocos2d::Node* control);
	virtual void processListControl(std::string name, int num);
	virtual void setFog(bool isVisible, float alpha = 217);
	virtual void removeFog();
	virtual void enableFog();
	virtual void setDelayInit(float time = 0.35f);
	virtual void setShowHideAnimate(cocos2d::Node* parent = nullptr, bool customScale = false);
	virtual void onClose();
	virtual void onCloseDone();
	virtual void setBackEnable(bool enable);
	virtual void backKeyPress();
	virtual void resetDefaultPosition(cocos2d::Node* control);
	virtual void onTouchEventHandler(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	virtual void customizeGUI() {};
	virtual void onEnterFinish() {};
	virtual void onButtonRelease(cocos2d::ui::Button* button, int id) {};
	virtual void onButtonCanceled(cocos2d::ui::Button* button, int id) {};
	virtual void onButtonTouched(cocos2d::ui::Button* button, int id) {};
	virtual void onUpdateGUI() {};
	virtual void initGUI()=0;
	virtual void functionDelayInit() {};
	virtual void finishAnimate() {};
	virtual void onBack() {};
	cocos2d::Vec2 getDefaultPos(cocos2d::Node* control);
	void localizedText(cocos2d::Node* node);
	//void playSoundButton();
	
	void autoInitComponent(cocos2d::Node *node, bool autoInitButtons = true);
	cocos2d::Node *getNodeInMap(const std::string & nodeName);

	virtual void show();

	virtual void hide();

	virtual void refresh();
public:
	static cocos2d::ui::Text* createLabelText(std::string txt = "", cocos2d::Color3B color = cocos2d::Color3B::WHITE);
	static cocos2d::ui::EditBox* createEditBox(cocos2d::ui::TextField* tf);
	static void subLabelText(cocos2d::ui::Text* lb, std::string txt, std::string defaultString = "");
	static cocos2d::Vec2 outsidePostion(cocos2d::Vec2 point);
	void effectInOutside(cocos2d::Node* control, float time = 0);
	static float TIME_APPEAR_GUI;
public:
	bool _hasInit;
	std::string _id;
	cocos2d::Node* _layout;
	cocos2d::Node* _bg;
	std::string _layoutPath;
	float _scale;
	float _scaleRealX;
	float _scaleRealY;
	//cocos2d::Node* _layerGUI;
	bool _aaPopup;
	bool _showHideAnimate;
	cocos2d::Node* _bgShowHideAnimate;
	int _currentScaleBg;
	bool _enableBack;

	cocos2d::ui::Layout* _layerColor;
	cocos2d::Layer* _layerGUI;
	cocos2d::EventListenerKeyboard* _keyboardEvent;
	cocos2d::EventListenerTouchOneByOne* _listener;

	bool _cachePopup;

	std::map<cocos2d::Node*, int> _subText;
	std::map<cocos2d::Node*, int> _wrapText;
	std::map<cocos2d::Node*, cocos2d::Vec2> _defaultPos;
	float _layerAlpha;

	cocos2d::LayerColor* _fog;
	std::string lastGUI;

	std::map<std::string, cocos2d::Node*> _nodeMap;
	std::map<std::string, int> _tagMap;
	int _btnTagsCount;

	bool _isShow;
	bool _keep;
};
#endif // BaseLayer_h__
