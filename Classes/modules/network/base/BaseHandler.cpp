#include "BaseHandler.h"
#include <lobby/network/GameClient.h>

BaseHandler::BaseHandler()
{
	GameClient::getInstance()->addHandler(this);
}

BaseHandler::~BaseHandler()
{
}

void BaseHandler::send(fr::OutPacket * msg)
{
	GameClient::getInstance()->sendPacket(msg);
}
