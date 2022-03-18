#include "DisconnectPopup.h"
#include "../../../data/Resource.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

DisconnectPopup::DisconnectPopup()
{
	_keep = true;
}

DisconnectPopup::~DisconnectPopup()
{

}

bool DisconnectPopup::init()
{
	if (!UIBaseLayer::init())
	{
		return false;
	}

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::DisconnectPlayerPopup);
	initWithBinaryFile(filePath);

	return true;
}

void DisconnectPopup::initGUI()
{
	autoInitComponent(_layout);

	_rtPlayersList = (Text *)_nodeMap["rtPlayersList"];
}

void DisconnectPopup::onPlayerAbandonMatch()
{
	unscheduleUpdate();
	hide();
}

void DisconnectPopup::update(float dt)
{
	string str = "";
	for (auto i = 0; i < _players.size(); i++)
	{
		auto player = _players[i];
		player.timeRemain -= dt;
		str += player.name + ": " + to_string((int)player.timeRemain)+"\n";
	}
	_rtPlayersList->setString(str);
}

void DisconnectPopup::setDisconnectState(std::vector<PlayerDisconnectData> players)
{
	_players.clear();
	_players.insert(_players.begin(), players.begin(), players.end());
	unscheduleUpdate();
	scheduleUpdate();
}

void DisconnectPopup::onPlayerReconnect(int uId)
{
	for (auto i = 0; i < _players.size(); i++)
	{
		if (_players[i].id == uId)
		{
			unscheduleUpdate();
			hide();
			return;
		}
	}
}

