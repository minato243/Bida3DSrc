#ifndef 	_WORKSHOPSCENE_H_
#define 	_WORKSHOPSCENE_H_


#include <cocos2d.h>

class WorkshopScene{
public:

	WorkshopScene);
	void onEnter();
	void initCameras();
	void loadData(int data, int isOwn, int isGacha);
	void setTrackingCue(int data);
	void onSceneReload();
	WorkshopScene);
	void initComponent();
	void initTabs();
	void refresh();
	void setTrackingCue(int cue);
	void switchTab(double tabIdx);
	void (int function(tab, double idx);
	void setParentScene(int scene);
	void onButtonTouchEnded(int sender, int tag);


private:
	int _trackingCue;
	int ui;
	int keep;
	int btnTabs;
	int btnTabs[0];
	int btnTabs[1];
	int btnTabs[2];
	int tabs;
	int curTab;
	int _parentScene;
};
#endif