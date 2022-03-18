#ifndef InventoryUI_h_
#define InventoryUI_h_

#include <core/gui/BaseLayer.h>
#include "../popup/FullScreenPopup.h"
#include <data/GameConstant.h>
#include "InventoryElementUI.h"

class InventoryUI: 
	public BaseLayer
{
public:
	enum TAG
	{
		btnTabMasse,
		btnTabBreak,
		btnTabJump,
		btnTabOther,
		btnBack,
		btnCueActive1,
		btnCueActive2,
		btnCueActive3
	};
	InventoryUI();
	~InventoryUI();

	CREATE_FUNC(InventoryUI);
	bool init();

	void initGUI();

	void initTag();

	void initTabs();

	void initCams();

	void initCues();

	void initRackDebugOffset();

	void show();

	void hide();

	int getTypeByTabIndex(int idx);

	int getTabIndexByType(int type);

	void refresh();

	void reloadTableViewData(bool keep);

	void reloadCue3DView();

	void addCueModelToRack(int index, const std::string & path, const std::string &texPath, const std::string &mraoPath);

	void selectActivatingCue(int cueIdx);

	void switchTab(int tabIdx);

	void onButtonRelease(cocos2d::ui::Button* button, int id);

	void onInitCueElement(InventoryElementUI *item, const cocos2d::Rect & rect);
public:
	float _drawNodeHeight;
	int _curTab;

private:
	cocos2d::Camera *_cam;

	cocos2d::ui::Button *_btnTabBreak;
	cocos2d::ui::Button *_btnTabJump;
	cocos2d::ui::Button *_btnTabMasse;
	cocos2d::ui::Button *_btnTabOther;

	cocos2d::ui::ImageView *_imgCueRackIconsBar;
	cocos2d::ui::ImageView *_imgCueRack;

	cocos2d::Sprite3D* _cues3d[GameConstant::CUE_IN_BAG];
	std::vector<std::map<std::string, cocos2d::Sprite3D*>> _cues3dCache;

	float dy;

	std::vector<cocos2d::ui::Button*> _btnTab;
	std::string _iconNameBtnTab[5];

	float _tabActivePosX;
	float _tabInactivePosX;

	cocos2d::Layer *_cueList;
	cocos2d::Layer *_itemList;
};
#endif // !InventoryUI_h_

