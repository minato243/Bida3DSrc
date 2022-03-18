#ifndef SceneMgr_h__
#define SceneMgr_h__

#include "BaseScene.h"
#include "Dialog.h"
#include "LoadingGui.h"

#define sceneMgr SceneMgr::getInstance()

enum SceneTag
{
	MAIN_SCENE = 0,
	INSPECT_SCENE = 1,
	SHOP_SCENE = 2,
	SPONSORSHIP_CLAIM = 3,
	WORKSHOP_SCENE = 4,
	ACHIEVEMENT_SCENE = 5, 
	LOADING_SCENE = 6,
};


class SceneMgr {
public:
	SceneMgr();
	~SceneMgr();
	static SceneMgr* getInstance();
public:
	BaseScene* getRunningScene();
	cocos2d::Node* getMainLayer();
	template <class T>
	bool checkMainLayer()
	{
		cocos2d::Node* node = getMainLayer();
		return (dynamic_cast<T*>(node) != nullptr);
	}
	LoadingGui* addLoading(std::string text = "", bool fog = true);
	void clearLoading();
	void addWaiting();
	void clearWaiting();
	void updateCurrentGUI();
	void addQueueScene(std::string layer);
	std::string getQueueScene(bool direct = false);
	BaseLayer* openScene(std::string layerName, std::string callback = "", bool direct = false, bool cache = true);
	BaseLayer* openGUI(std::string slayer, int zoder = 1, int tag = 1, bool isCache = true);
	std::string getClassGUI(std::string cName = "");
	void preloadScene(std::string layer);
	cocos2d::Node* getGUI(int tag);
	void initialLayer();
	void updateScene(float dt);
	bool checkBackAvailable(std::vector<std::string> ignores = {});
	void showOkCancelDialog(std::string message, const DialogListener& callback);
	void showOkDialogWithAction(std::string message, const DialogListener& callback);
	void showOKDialog(std::string message);
	void showChangeGoldDialog(std::string message, const DialogListener& callback);

	void restartCacheGui();

	void showScene(cocos2d::Scene *scene, int tag);

	BaseScene *getOrCreateScene(int tag);

	BaseScene *loadSceneByTag(int tag);

	void setUpDebugNode();
private:
	BaseLayer* createLayerFromName(std::string className);
public:
	static SceneMgr* _instance;
	cocos2d::Color3B ccWhite;
	cocos2d::Color3B ccYellow;
	cocos2d::Color3B ccGreen;
	cocos2d::Color3B ccBlue;

	std::string curGui;
	std::map<std::string, BaseLayer*> arGuis;
	std::map<std::string, BaseLayer*> arPopups;

	cocos2d::Node* layerGUI;
	cocos2d::Node* layerSystem;
	std::vector<std::string> backScenes;
	std::vector<std::string> ignoreGuis;

	std::map<int, BaseScene*> _sceneMap;

	cocos2d::Node *_debugNode = NULL;

public:
	int _curSceneTag;

	static const std::string FONT_NORMAL;
	static const std::string FONT_BOLD;
	static const int FONT_SIZE_DEFAULT;
};



class Toast :
	public cocos2d::Layer {
public:
	Toast();
	~Toast();
public:
	bool init(float time, std::string message);
	void onEnter();

	static Toast* makeToast(float time, std::string message);
public:
	float _time;
	std::string _message;
	cocos2d::LayerColor* _layerColor;
	cocos2d::ui::Text* _label;

	static const float SHORT;
	static const float LONG;
};

class ToastFloat :
	public cocos2d::Node {
public:
	CREATE_FUNC(ToastFloat);
	ToastFloat();
	~ToastFloat();
public:
	bool init();
	void onEnter();
	void finishEffect();
	void setToast(std::string txt, float time = 1.0f);
	void clearToast();
	void update(float dt);
	static void makeToast(float time, std::string text);

public:
	float timeDelay;
	bool isRunningDelay;
	cocos2d::ui::Text* lb;
	cocos2d::ui::Scale9Sprite* bg;
	float _scale;

	static const float SHORT;
	static const float LONG;
	static const float MEDIUM;

	static const float POSITION_Y;
	static const float DELTA_WIDTH;
	static const int PAD_SIZE;
};

#define effectMgr EffectMgr::getInstance()

class EffectMgr {
public:
	struct LabelPoint {
		cocos2d::ui::Text* label;
		long long cur;
		long long des;
		int delta;
		int type;
		float delay;
	};
public:
	EffectMgr();
	~EffectMgr();
	static EffectMgr* getInstance();
public:
	void runLabelPoint(cocos2d::ui::Text* label, long long cur, long long des, float delayTime = 0, int numChange = 50, int type = 1);
	void updateEffect(float dt);
public:
	std::vector<LabelPoint*> arLbPoints;
	static EffectMgr* _instance;
};

#endif // SceneMgr_h__
