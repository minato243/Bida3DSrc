#ifndef MiniGameHandler_h_
#define MiniGamehandler_h_

#include "../base/BaseHandler.h"

class MiniGameHandler
	:public BaseHandler
{
public:
	MiniGameHandler();
	~MiniGameHandler();

	bool onReceived(fr::InPacket* pk);

	void sendSpinWheel();

	void sendCheatSpin();

private:
	static MiniGameHandler *_instance;

public:
	static MiniGameHandler *getInstance();
};

#define miniGameHandler MiniGameHandler::getInstance()
#endif
