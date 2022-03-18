#include "CueTypeSocket.h"
#include <data/GameConstant.h>
#include "base/UIFactory.h"


USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;


CueTypeSocket::CueTypeSocket(cocos2d::Node *container)
{
	_lockChangeCue = false;
	_timeFactor = 1;

	_container = container;
	init();
}

CueTypeSocket::~CueTypeSocket()
{

}

void CueTypeSocket::init()
{
	auto imgBox2 = (ImageView*)_container->getChildByName("imgBox2");
	auto imgBox3 = (ImageView*)_container->getChildByName("imgBox3");
	auto imgBox4 = (ImageView*)_container->getChildByName("imgBox4");
	_sockets.push_back(imgBox2); _sockets.push_back(imgBox4); _sockets.push_back(imgBox3);

	_typeMap[imgBox2] = 2;
	_typeMap[imgBox3] = 3;
	_typeMap[imgBox4] = 4;

	for (int i = 0; i < _sockets.size(); i++)
	{
		auto socket = _sockets[i];
		_socketPos.push_back(socket->getPosition());
		socket->ignoreContentAdaptWithSize(true);
		auto imgCueType = (ImageView*)socket->getChildByName("imgCueType");
		imgCueType->ignoreContentAdaptWithSize(true);
		_originalScaleMap[socket] = socket->getScale();
	}

	changeToType(3, NULL, false);
}

void CueTypeSocket::setTimeFactor(float f)
{
	_timeFactor = f;
}

void CueTypeSocket::changeToType(int type, CUSTOM_CALLBACK callback, bool anim /*= true*/)
{
	if (_lockChangeCue)
	{
		return;
	}

	if (type == _typeMap[_sockets[0]])
	{
		if (callback)
			callback();
		return;
	}

	if (type != _typeMap[_sockets[2]])
	{
		changeToNextType();
	}

	_lockChangeCue = true;
	_callback = callback;

	auto ANIMATE_TIME = 0.2 * _timeFactor;

	for (int i = 0; i < _sockets.size(); i++)
	{
		auto socket = _sockets[i];
		auto item = (ImageView*)socket->getChildByName("imgCueType");

		auto icons = getIcon(_typeMap[socket]);
		bool isGonnaActive = (i == 2);
		bool isCurActive = (i == 0);

		socket->loadTexture(isGonnaActive ? "IngameEBP_Change_Cue_Active.png" : "IngameEBP_Change_Cue_Inactive.png", TextureResType::PLIST);
		item->loadTexture(isGonnaActive ? icons.active : icons.inactive, TextureResType::PLIST);
		auto socketSize = socket->getContentSize();
		item->setPosition(Vec2(socketSize.width/2, socketSize.height/2));
		
		item->setScale(0.7 *socketSize.width/ item->getContentSize().height);

		auto pos = _socketPos[(i + 1) % 3];
		if (anim)
		{
			item->runAction(Sequence::create(
				DelayTime::create(ANIMATE_TIME),
				CallFunc::create([this]() {
					_lockChangeCue = false;
				}),
				NULL
			));

			socket->runAction(Spawn::create(
				ScaleTo::create(ANIMATE_TIME, _originalScaleMap[socket]),
				MoveTo::create(ANIMATE_TIME, pos), NULL
			));
		}
		else
		{
			_lockChangeCue = false;
			socket->setScale(_originalScaleMap[socket]);
			socket->setPosition(pos);
		}
	}

	if (anim)
	{
		if (callback)
		{
			_container->runAction(Sequence::create(
				DelayTime::create(ANIMATE_TIME),
				CallFunc::create([this, callback]() {
					callback();
				}),
				NULL
			));
		}
	} 
	else
	{
		if (callback) callback();
	}

	auto tmp = _sockets[2];
	_sockets[2] = _sockets[1];
	_sockets[1] = _sockets[0];
	_sockets[0] = tmp;
}

void CueTypeSocket::changeToNextType()
{
	for (int i = 0; i < _sockets.size(); i++)
	{
		auto socket = _sockets[i];
		auto item = (ImageView*)socket->getChildByName("imgCueType");

		auto icons = getIcon(_typeMap[socket]);
		bool isGonnaActive = (i == 2);
		bool isCurActive = (i == 0);

		socket->loadTexture(isGonnaActive ? "IngameEBP_Change_Cue_Active.png" : "IngameEBP_Change_Cue_Inactive.png", TextureResType::PLIST);
		item->loadTexture(isGonnaActive ? icons.active : icons.inactive, TextureResType::PLIST);
		auto socketSize = socket->getContentSize();
		item->setPosition(Vec2(socketSize.width / 2, socketSize.height / 2));

		item->setScale(0.7 *socketSize.width / item->getContentSize().height);

		auto pos = _socketPos[(i + 1) % 3];
	
		socket->setScale(_originalScaleMap[socket]);
		socket->setPosition(pos);
	}
	
	auto tmp = _sockets[2];
	_sockets[2] = _sockets[1];
	_sockets[1] = _sockets[0];
	_sockets[0] = tmp;
}

CueTypeIconData CueTypeSocket::getIcon(int type)
{
	switch (type)
	{
	case CueType::NORMAL:
		return normalIconData;
	case CueType::BREAK:
		return breakIconData;
	case CueType::JUMP:
		return jumpIconData;
	case CueType::MASSE:
		return masseIconData;
	default:
		break;
	}
	return normalIconData;
}

const CueTypeIconData CueTypeSocket::normalIconData = {"Button_Green.png", "Button_Green.png"};

const CueTypeIconData CueTypeSocket::breakIconData = { "Icon_Active_02.png", "Icon_Inactive_02.png" };

const CueTypeIconData CueTypeSocket::jumpIconData = { "Icon_Active_03.png", "Icon_Inactive_03.png" };

const CueTypeIconData CueTypeSocket::masseIconData = { "Icon_Active_04.png", "Icon_Inactive_04.png" };

