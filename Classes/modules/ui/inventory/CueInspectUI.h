#ifndef CueInspectUI_h_
#define CueInspectUI_h_

#include <cocos2d.h>
#include <core/gui/BaseLayer.h>
#include "../popup/FullScreenPopup.h"
#include <data/GameConstant.h>
#include "../../core/ps/internal/ExtensionMath.hpp"
#include "../../core/utils/LerpUtils.h"
#include "../../camera-view/GameCamera.h"
#include "../base/UIBaseLayer.h"
#include "InventoryElementUI.h"

class CueInspectUI
	:public UIBaseLayer
{
public:
	class CAM_FLAG
	{
	public:
		const static cocos2d::CameraFlag FLAG_BELOW = GameCamera::FLAG_BG_LOWEST;
		const static cocos2d::CameraFlag FLAG_3D = GameCamera::FLAG_3D;
		const static cocos2d::CameraFlag FLAG_ABOVE = GameCamera::FLAG_UI;
	};

	enum CAM_DEPTH
	{
		DEPTH_BELOW = 1,
		DEPTH_3D = 2,
		DEPTH_ABOVE = 3
	};

	CueInspectUI();
	~CueInspectUI();

	CREATE_FUNC(CueInspectUI);

	bool init();

	void initGUI();

	void show();

	void initShowAnimations();

	void getOnAnimationCallbackDuration(int priority);

	void onStartAnimationPriority(int priority);

	void update(float dt);

	void initBars();

	void initBar(cocos2d::ui::Widget *panel);

	void resetCueView();

	void initCamera();

	void init3DView();

	void addCue3D(std::string model, std::string tex, std::string mraoPath, bool isLighting);
	
	void setUpShader(cocos2d::Sprite3D * cue, std::string texPath, std::string mraoPath);

	void setLights(cocos2d::Vec3 light);

	void init3DControl();

	void onMouseScroll(cocos2d::Event *unused_event);

	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	void updateZoom();

	void updateCueZoom(float zoom);

	void resetRotation();

	void rotateCueSoftly();

	void updateRotation();

	void updateCueRotation(ps::ExtMath::Rpt rpt);

	void setInfo(CueInfo *data, bool isOwn, bool isGacha, bool isFusion);

	void setChargeCriteriaInfo(CueInfo *data);

	void set3DModel(CueInfo *data);

	void setNumStar(int num);

	void refresh();

	void setUIInfo(const double & force, const double &  angular, double angle[], const double & aim, const double & accuracy, const double & generalPoint);

	void setPercent(cocos2d::ui::Widget *bar, float percentOnOne);

	void runInfoAnimation();

	void updateInfo(float dt);

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	virtual void onTouchesMoved(const std::vector<cocos2d::Touch *>& touches, cocos2d::Event *unused_event) override;

private:
	ps::ExtMath::Rpt _rpt;
	float _zoom;
	SphericalLerp *_cueLerp;
	NumberLerp *_zoomLerp;
	bool _enableLights;

	cocos2d::ui::Text *_lbTitle;
	cocos2d::ui::ImageView *_imgTitleBar;
	cocos2d::Sprite *_imgType;
	cocos2d::Sprite *_imgType1;
	cocos2d::Node *_nodeStars;

	cocos2d::ui::Text *_rtQuantity;
	cocos2d::ui::Text *_lbLevel;
	cocos2d::ui::Text *_lbBattery;

	cocos2d::ui::Button *_btnBack;
	cocos2d::ui::Button *_btnWorkshop;
	cocos2d::ui::Button *_btnEquip;
	cocos2d::ui::Button *_btnCharge;
	cocos2d::ui::Text *_lbBtnWorkShop;
	cocos2d::ui::Text *_lbBtnEquip;
	cocos2d::ui::Text *_lbBtnCharge;
	cocos2d::ui::Text *_lbAuto;
	cocos2d::ui::ImageView *_imgTick;

	cocos2d::ui::ImageView *_imgRightSlot;
	cocos2d::ui::ImageView *_imgImgRightSlot;
	cocos2d::ui::ImageView *_imgBgBox;
	
	cocos2d::ui::Widget *_pnTitle;
	cocos2d::ui::Widget *_pnBelow;
	cocos2d::ui::Widget *_pnAura;

	cocos2d::ui::Widget *_pnForce;
	cocos2d::ui::Widget *_pnSpin;
	cocos2d::ui::Widget *_pnAim;
	cocos2d::ui::Widget *_pnAccuracy;
	cocos2d::ui::Widget *_pnAngle;
	
	cocos2d::ui::Widget *_barPnForce;
	cocos2d::ui::Widget *_barPnSpin;
	cocos2d::ui::Widget *_barPnAim;
	cocos2d::ui::Widget *_barPnAccuracy;
	cocos2d::ui::Widget *_barPnAngle;

	cocos2d::ui::Widget *_pnGold;
	cocos2d::ui::ImageView *_imgPnGold;
	cocos2d::ui::Text *_rtGold;

	cocos2d::ui::Widget *_pn3DDisplay;
	Battery *_battery;

	cocos2d::Camera *_camera;

	cocos2d::Sprite3D *_cueNode;
	cocos2d::Sprite3D *_cue;
	std::map<std::string, cocos2d::Sprite3D*> _cues;

	std::map<cocos2d::Sprite3D *, cocos2d::GLProgramState *> _stateMap;
	cocos2d::GLProgramState *_state;
	std::vector<cocos2d::Vec3> _lightPos;

	std::vector<cocos2d::Camera *> _cameras;

	std::map<cocos2d::ui::Widget *, cocos2d::DrawNode *> _stencilMap;
	std::map<cocos2d::Node *, float> _originalXMap;
	std::map<cocos2d::Node *, cocos2d::Size> _ogContentSizeMap;

	bool _isOwn;
	bool _isGacha;
	bool _isFusion;

	CueInfo *_data;
	double _force;
	double _angular;
	double _angle[2];
	double _generalPoint;
	double _aim;
	double _accuracy;

	bool _updateInfo;
	float _animationTime;

public:
	static const double ANIMATION_SPEED;
	static const double ORIGINAL_R;
	static const double animationTime;

	static const double MAX_FORCE;
	static const double MAX_ANGULAR;
	static const double MAX_ANGLE;
	static const double MAX_POINT;
	static const double MAX_AIM;
	static const double MAX_ACCURACY;

	static cocos2d::Color3B getColorObjByType(int type);

	static std::string getIconName(int type);
protected:
private:
};
#endif
