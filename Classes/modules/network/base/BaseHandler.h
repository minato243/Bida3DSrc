#ifndef BASE_HANDLER_H_
#define BASE_HANDLER_H_

#include <BaseFramework.h>

class BaseHandler
{
public:
	BaseHandler();
	~BaseHandler();

	virtual bool onReceived(fr::InPacket* pk) = 0;

public:
	static void send(fr::OutPacket *msg);
};
#endif // !BASE_HANDLER_H_



