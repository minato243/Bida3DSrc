#ifndef MiniGameInPacket_h_
#define MiniGameInPacket_h_

#include "../base/BasePacket.h"

class SpinDataMsg
	:public BaseInPacket
{
public:
	SpinDataMsg(fr::InPacket *pk);
	~SpinDataMsg();

	void readData();

public:
	int cellId;
};

class SpinInfoMsg
	:public BaseInPacket
{
public:
	SpinInfoMsg(fr::InPacket *pk);
	~SpinInfoMsg();

	void readData();

public:
	int numberOfSpin;
};
#endif // !MiniGameInPacket_h_
