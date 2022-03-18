#include "Network.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
USING_NS_CC;
using namespace network;

void Network::addRequestPost(std::string url, std::string data, NetworkRequestCallback callback)
{
	HttpRequest* cRequest = new HttpRequest();
	cRequest->setUrl(url.c_str());
	cRequest->setRequestType(HttpRequest::Type::POST);
	cRequest->setRequestData(data.c_str(), data.size());
	cRequest->setResponseCallback(
		[callback](HttpClient* client, HttpResponse* response) {
		if (!response || !response->isSucceed() || response->getResponseCode() == 404)
		{
			callback(false, "");
		}
		else {
			std::vector<char> *buffer = response->getResponseData();
			std::string str = std::string(buffer->begin(), buffer->end());
			callback(true, str);
		}

	}
	);
	HttpClient::getInstance()->setTimeoutForConnect(15);
	HttpClient::getInstance()->setTimeoutForRead(15);
	HttpClient::getInstance()->send(cRequest);
	cRequest->release();
}
