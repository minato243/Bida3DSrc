#ifndef CueCollectionCueItem_h_
#define CueCollectionCueItem_h_

#include <cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <data/DataStruct.h>
#include "../../../utils/MoveDetector.h"

class CueCollectionCueItem
	:public cocos2d::Node
{
public:
	CueCollectionCueItem();
	~CueCollectionCueItem();

	void initUI();

	bool init();

	bool init(cocos2d::Size size);

	void init3D();

	void autoResize();

	void onButtonTouched(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type);

	void onClick();

	void loadData(CueInfoData *data);

	void showCue3D(std::string path, std::string tex);

	void updateCueState();

	void setIdx(int idx);

	cocos2d::Size getOriginalSize();
	
	void setModeDetail(bool modeDetail);

	void setActive(bool isActive);
private:

	bool _active;
	bool _modeDetail;
	cocos2d::Size _size;

	cocos2d::ui::Layout *_bg;
	cocos2d::ui::Button *_itemBtn;

	cocos2d::Node *_cueNode;
	std::map<std::string, cocos2d::Sprite3D*> _cues;
	cocos2d::Sprite3D *_curCue;
	std::map<cocos2d::Sprite3D*, cocos2d::GLProgramState*> _states;

	CueInfoData *_data;
	MoveDetector *_moveDetector;
	
	int _idx;

public:
	static cocos2d::Size defaultSize;
	static cocos2d::Vec2 fadeBL;
	static cocos2d::Vec2 fadeTR;
	static cocos2d::Vec2 opaqueBL;
	static cocos2d::Vec2 opaqueTR;
};

#endif