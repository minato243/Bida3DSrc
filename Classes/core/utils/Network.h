#ifndef Network_h__
#define Network_h__
#include <string>
#include <functional>

typedef std::function<void(bool isSuccess, std::string data)> NetworkRequestCallback;

class Network {

public:
	static void addRequestPost(std::string url, std::string data, NetworkRequestCallback callback);
};
#endif // Network
