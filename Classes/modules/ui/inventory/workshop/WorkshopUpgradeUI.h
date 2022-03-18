#ifndef WorkshopUpgradeUI_h_
#define WorkshopUpgradeUI_h_

#include <core/gui/BaseLayer.h>
#include "../../elements/StatBarPanel.h"
#include "../../../core/inventory/CueInfo.h"

class WorkshopUpgradeUI
	:public BaseLayer
{
public:
	enum TAG
	{
		BTN_CUE_LEVEL,
		BTN_MAT1,
		BTN_MAT2,
		BTN_MAT3,
		BTN_MAT4,
		BTN_GEN1,
		BTN_GEN2,
		BTN_GEN3,
		BTN_GEN4,
		BTN_GEN5,
		BTN_UPGRADE
	};
	WorkshopUpgradeUI();
	~WorkshopUpgradeUI();

	CREATE_FUNC(WorkshopUpgradeUI);
	bool init();

	void initGUI();

	void initTag();

	void initMatSlots();

	void initBar();

	void setUIInfo(const double & force, const double & angular, 
		double angle[], const int & generalPoint, 
		const int &aim, const double &accuracy);

	void setUIInfoDefault();

	void initModelWidthPath();

	void initCueView();

	void loadText();

	void onSelectCue(CueInfo *cue);

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void showCue3D(const std::string & path, const std::string texPath, const std::string & mraoPath);

	float getModelWidthPath(const std::string & path);

protected:
private:
	cocos2d::Node *_cueNode;

	cocos2d::ui::Button *_btnCueLevel;
	cocos2d::ui::ImageView *_imgMachine;

	cocos2d::ui::Widget *_pnStats;
	cocos2d::ui::ImageView *_imgLevel;
	cocos2d::ui::Widget *_pnForce;
	cocos2d::ui::Widget *_pnSpin;
	cocos2d::ui::Widget *_pnAngle;
	cocos2d::ui::Widget *_pnAim;
	cocos2d::ui::Widget *_pnAccuracy;

	StatBarPanel *_pnForceWrapper;
	StatBarPanel *_pnSpinWrapper;
	StatBarPanelAngle *_pnAngleWrapper;
	StatBarPanel *_pnAimWrapper;
	StatBarPanel *_pnAccuracyWrapper;

	cocos2d::Sprite3D *_cue;
	std::map<std::string, cocos2d::Sprite3D*> _cues;
	cocos2d::Sprite3D *_curCue;
	std::map<std::string, float> _modelWidthByPath;
};

#endif