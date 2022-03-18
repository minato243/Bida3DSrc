#include "Utilities3D.h"
#include "core/Config.h"

USING_NS_CC;

cocos2d::Animate3D* Utilities3D::makeAnimation(std::string anim, int startFrame, int endFrame, float scale)
{
	///if (Config::ENABLE_CHEAT) scale = scale / CheatCenterScene.ANIMATION_SPEED;

	auto animation3D = Animation3D::create(anim);
	Animate3D* animation;
	if (startFrame != -1 && endFrame != -1) {
		animation = Animate3D::createWithFrames(animation3D, startFrame, endFrame);
	}
	else {
		animation = Animate3D::create(animation3D);
	}

	animation->retain();
	animation->setQuality(Animate3DQuality::QUALITY_HIGH);
	animation->setDuration(animation->getDuration() * scale);
	return animation;
}
