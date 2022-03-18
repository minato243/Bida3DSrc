#ifndef ResourceMgr_h__
#define ResourceMgr_h__

#define resourceMgr ResourceMgr::getInstance()
class ResourceMgr {
public:
	ResourceMgr();
	~ResourceMgr();
	static ResourceMgr* getInstance();
public:
	void init();
	void preloadResource();
	void preloadAnimation();
	void preloadSound();
public:
	static ResourceMgr* _instance;
};
#endif // ResourceMgr_h__
