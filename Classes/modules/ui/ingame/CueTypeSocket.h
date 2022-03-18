#ifndef CueTypeSocket_h_
#define CueTypeSocket_h_

#include <cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "data/GameConstant.h"

struct CueTypeIconData
{
	std::string active;
	std::string inactive;
};

class CueTypeSocket
{
public:
	CueTypeSocket(cocos2d::Node *container);
	~CueTypeSocket();

	void init();

	void setTimeFactor(float f);

	void changeToType(int type, CUSTOM_CALLBACK callback, bool anim = true);

	void changeToNextType();

	CueTypeIconData getIcon(int type);

private:
	cocos2d::Node *_container;
	std::vector<cocos2d::ui::ImageView *> _sockets;
	std::map<cocos2d::Node*, int> _typeMap;
	std::map<cocos2d::Node *, float> _originalScaleMap;
	std::vector<cocos2d::Vec2> _socketPos;

	float _timeFactor;

	bool _lockChangeCue;
	
	CUSTOM_CALLBACK _callback;

	static const CueTypeIconData normalIconData;
	static const CueTypeIconData breakIconData;
	static const CueTypeIconData jumpIconData;
	static const CueTypeIconData masseIconData;
};

#endif
