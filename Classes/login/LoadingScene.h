#ifndef LoadingScene_h__
#define LoadingScene_h__
#include "core/gui/BaseLayer.h"

class LoadingScene:
	public BaseLayer
{
public:
	CREATE_FUNC(LoadingScene);
	LoadingScene();
	~LoadingScene();
	bool init();
public:
	void initGUI();
	void onEnterFinish();

public:
	static const std::string className;
	
	cocos2d::ui::Text* lbVersion;
	cocos2d::ui::Text* lbUpdate;
};
#endif // LoadingScene_h__
