#ifndef PacketsFactory_h__
#define PacketsFactory_h__

#include "BaseFramework.h"

class PacketsFactory {
public:
	static fr::InPacket* getInPacket(fr::sPackage* pkg);
	static fr::OutPacket* getOutPacket(int msgId, int controllerId = 1);
	static void putInPacket(fr::InPacket* packet);
	static void putOutPacket(fr::OutPacket* packet);
	static void destroyInstance();
public:
	PacketsFactory();
	~PacketsFactory();
private:
	static PacketsFactory* _instance;
	static void initInstanceIfNull();
private:
	
};
#endif // PacketsFactory_h__
