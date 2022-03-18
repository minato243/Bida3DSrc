#ifndef DisconnectPopup_h_
#define DisconnectPopup_h_

#include <modules/ui/base/UIBaseLayer.h>
#include "../../network/game/IngameInPacket.h"

class DisconnectPopup
	: public UIBaseLayer
{
public:
	DisconnectPopup();
	~DisconnectPopup();

	CREATE_FUNC(DisconnectPopup);
	bool init();

	void initGUI();

	void onPlayerAbandonMatch();

	void update(float dt);

	void setDisconnectState(std::vector<PlayerDisconnectData> players);

	void onPlayerReconnect(int uId);

public:
	cocos2d::ui::Text *_rtPlayersList;
	std::vector<PlayerDisconnectData> _players;
};

#endif
