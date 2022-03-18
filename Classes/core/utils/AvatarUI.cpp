#include "AvatarUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

static const int AVATAR_DOWNLOAD = 0;
static const int AVATAR_DEFAULT = -1;
static const int AVATAR_DOWNLOADING = 1;

AvatarUI::AvatarUI()
{

}

AvatarUI::~AvatarUI()
{

}

AvatarUI* AvatarUI::create(std::string defaultImg, std::string maskAvatar)
{
	AvatarUI* avatar = new AvatarUI();
	avatar->init(defaultImg, maskAvatar);
	return avatar;
}

bool AvatarUI::init(std::string defaultImg, std::string maskAvatar)
{
	Node::init();

	_defaultImg = defaultImg;
	_downloading = AVATAR_DEFAULT;
	clipping = ClippingNode::create();
	clipping->setAlphaThreshold(0.5f);
	addChild(clipping);
	image = ImageView::create(defaultImg);
	image->ignoreContentAdaptWithSize(false);
	avatar = nullptr;
	clipping->addChild(image);
	clipping->setCascadeOpacityEnabled(true);
	setCascadeOpacityEnabled(true);
	setMaskSprite(maskAvatar);

	return true;
}

void AvatarUI::setImage(std::string path)
{
	if (path.empty())
		path = _defaultImg;
	image->loadTexture(path);
}

void AvatarUI::setDefaultImage()
{
	image->loadTexture(_defaultImg);
}

void AvatarUI::setMaskSprite(std::string maskAvatar)
{
	if (!maskAvatar.empty()) {
		Sprite* stencil = Sprite::create(maskAvatar);
		clipping->setStencil(stencil);
	}
}

void AvatarUI::asyncExecuteWithUrl(std::string path, std::string url)
{
	_urlImg = url;
	_pathImg = path;
	if (!avatar) {
		avatar = Avatar::create(_defaultImg, url);
		image->addChild(avatar);
		avatar->setPosition(image->getContentSize().width / 2, image->getContentSize().height / 2);
	}
	else {
		avatar->updateAvatar(url);
	}
}

void AvatarUI::asyncExecute()
{
	asyncExecuteWithUrl(_pathImg, _urlImg);
}

