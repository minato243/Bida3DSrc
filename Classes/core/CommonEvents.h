#ifndef CommonEvents_h__
#define CommonEvents_h__

// input event: network, user input
struct EventSocketDisconnected {
	EventSocketDisconnected() {}
};

struct EventSocketFinished {
	EventSocketFinished(bool _isSuccess) :isSuccess(_isSuccess) {}
	bool isSuccess;
};


#endif // CommonEvents_h__
