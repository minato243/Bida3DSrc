#ifndef CueHandler_h_
#define CueHandler_h_

#include "BaseFramework.h"
#include <data/DataStruct.h>
#include "../base/BaseHandler.h"

#define cueHandler CueHandler::getInstance()

class CueHandler
	:BaseHandler
{
public:
	CueHandler();
	~CueHandler();

	bool onReceived(fr::InPacket* pk);

	void requestCreateCue(int type);

	void requestRenameCue(int id, std::string name);

	void requestChargeCue(int id);

	void requestDeleteCue(int id);

	void requestAccumulateDust(int cueId, int dust);

	void requestBuyCueBox(int type, int quantity);

	void requestCheatInventoryItem(int type, int quantity);

	void requestUpgradeCue(int cueId);

	void requestWorkshopUpgradeCue(int cueId, const std::vector<MatData> & mats);

	void requestWorkshopFusionCues(const int* cueIds);

	void requestWorkshopSwapCues(int src, int dst);

private:

	static CueHandler *_instance;

public:
	static CueHandler * getInstance();
};

#endif // !GAME_HANDLER_H_

