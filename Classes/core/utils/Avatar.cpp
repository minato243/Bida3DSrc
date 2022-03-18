#include "Avatar.h"
#include "CMD5Checksum.h"

USING_NS_CC;

Avatar::Avatar()
{

}

Avatar::~Avatar()
{

}

Avatar* Avatar::create(std::string defaultImg, std::string url /*= ""*/)
{
	Avatar* avatar = new Avatar();
	avatar->init(defaultImg, url);
	avatar->autorelease();
	return avatar;
}

bool Avatar::init(std::string defaultImg, std::string url /*= ""*/)
{
	setCascadeOpacityEnabled(true);

	defaultAvatar = Sprite::create(defaultImg);
	defaultAvatar->setCascadeOpacityEnabled(true);
	addChild(defaultAvatar);

	avatar = fr::AsyncSprite::create(defaultAvatar->getContentSize(), std::bind(&Avatar::onFinishLoad, this, std::placeholders::_1));
	addChild(avatar);

	updateAvatar(url);
	return true;
}

void Avatar::updateAvatar(std::string url)
{
	if (url.empty())
	{
		return;
	}
	defaultAvatar->setVisible(true);
	avatar->setVisible(false);

	avatar->updatePath(url, getStorePath(url));
}

void Avatar::onFinishLoad(bool result)
{
	if (result)
	{
		defaultAvatar->setVisible(false);
		avatar->setVisible(true);
	}
	else
	{
		defaultAvatar->setVisible(true);
		avatar->setVisible(false);
	}
}

std::string Avatar::getStorePath(std::string url)
{
	return StringUtils::format("%s/%s",FileUtils::getInstance()->getWritablePath().c_str(), CMD5Checksum::GetMD5OfString(url).c_str());
}

