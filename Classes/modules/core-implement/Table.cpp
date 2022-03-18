#include "Table.h"
#include <data/Resource.h>
#include <modules/core/ps/internal/PhysicsConstants.hpp>
#include <functional>
#include <modules/utils/CocosUtils.h>
#include <modules/core-implement/SettingMgr.h>
#include <core/GameMgr.h>
#include <modules/core/interaction/InteractionManager.h>
#include <modules/camera-view/GameCamera.h>
#include "effekseer/effekseer/Effekseer.h"
#include "../ui/ingame/PocketArrows.h"
#include "BallPocket3DEffect.h"
#include "../core/ps/internal/PhysicsWorldV2.h"
#include "../core/ps/internal/BallGenerator.hpp"
#include "../core/utils/Utils.h"
#include "../core/utils/TimerSprite.h"
#include "../core/ps/PhysicsFactory.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace ps;

Table::Table()
: _fakeCueBall(NULL), _fakeBallState(NULL), _highlightingBalls(NULL),
_eventCount(0), _ballForceEfx(NULL), _guildLineVisible(true),
_drawCircleNode(NULL)
{
}

Table::~Table()
{
	_state.clear();
}

bool Table::init()
{
	if (!Node::init())
		return false;

	this->initPhysicsWorld();
	this->initTable();
	

	for (auto i = 0; i < _parts.size(); i++) {
		_parts[i]->setLightMask(0);
	}

	return true;
}

void Table::initTable()
{
	auto table = Sprite3D::create(Res::TABLE_3D);
	table->setLightMask(0);
	table->setScale(100);
	this->_table = table;

	this->addChild(table);
	this->drawKitchen();
	this->initEffect();
	this->initBillBoardOnPockets();
	this->initPocketHighlights();

}

void Table::initPhysicsWorld()
{
	_world = new ps::PhysicsWorldV2(1, -1);
	_world->getDispatcher()->subscribeWorldPaused(CC_CALLBACK_1(Table::onWorldPause, this));
	_world->getDispatcher()->subscribeBallCollision(CC_CALLBACK_3(Table::handleBallCollision, this));
	_world->getDispatcher()->subscribeCushionCollision(CC_CALLBACK_3(Table::handleCushionCollision, this));
	_world->getDispatcher()->subscribeBallFallen(CC_CALLBACK_3(Table::handleBallFallen, this));
}

void Table::addBall(Ball* ball, ps::ExtMath::vector position) {
	this->_balls.push_back(ball);
	this->addChild(ball);
	ball->setBallPosition(position);
	this->_world->addBall(ball->_body, ball->body()->id() == Ball::WHITE_BALL_ID);
	if (ball->body()->id() == Ball::WHITE_BALL_ID) {
		_whiteBall = ball;
	}
}

void Table::addWhiteBall(Ball* ball)
{
	this->addBall(ball);
}

void Table::addCue(Cue* cue)
{
	this->addChild(cue);
	cue->setCuePosition(this->_whiteBall->body()->position());
	_world->_cue = cue;;
	this->_cue = cue;
}

void Table::initGuildLine()
{
	this->_guildLine = TableGuildLine::create();
	this->addChild(_guildLine);
	_guildLine->setCameraMask(this->getCameraMask(), true);
}

void Table::initShader(cocos2d::GLProgram *tableShader, cocos2d::GLProgram *matShader, int level)
{
	//todo
	CCLOG("Todo initShader");
	_qualityLevel = level;
	auto table = _table;
	std::vector<Sprite3D*> _state;
	_parts.clear();
	auto tb = table;
	
	std::vector<std::string> partName = { "Metal", "ETC", "Mat" };

	// Get the part
	for (auto i = 0; i < partName.size(); i++) {
		Sprite3D* part = (Sprite3D*)_table->getChildByName("Table_Bida3D")->getChildByName(partName[i]);
		part->setLightMask(0);
		_parts.push_back(part);
		auto mraoMap = TextureCache::getInstance()->addImage(Res::Table_MRAO[i]);
		// mraoMap.setTexParameters(gl.NEAREST, gl.NEAREST, gl.CLAMP_TO_EDGE, gl.CLAMP_TO_EDGE)

		auto state = setPartShaderState(tableShader, matShader, part, mraoMap, i);
	}
	setLights(std::vector<Vec3>(Ball::DefaultLights, Ball::DefaultLights+4), Ball::DefaultLightColor);

}

GLProgramState* Table::setPartShaderState(cocos2d::GLProgram *tableShader, cocos2d::GLProgram *matShader, cocos2d::Sprite3D* part, cocos2d::Texture2D*  mraoMap, int partIdx)
{
	//todo
	CCLOG("Set Part shader %d", partIdx);
	auto isMat = partIdx > 1;
	auto ios = CC_PLATFORM_IOS == CC_TARGET_PLATFORM ? "_IOS" : "";
	auto program = isMat ? matShader : tableShader;
	
	auto level = _qualityLevel;

	auto state = GLProgramState::create(program);
	
	if (partIdx== 0) {
		auto texture = Director::getInstance()->getTextureCache()->addImage("res/3d/table/Table_Metal copy.png");
		part->setTexture(texture);
	}
	else if (partIdx== 1) {
		part->setTexture("res/3d/table/Table_ETC copy.png");
	}
	else {
		part->setTexture("res/3d/table/Table_Mat copy.png");
	}
	state->setUniformVec3("u_light_color", Vec3(1, 1, 1));
	if (level >= 0 && !isMat) {
		state->setUniformVec3("u_view_pos", Vec3(0, 160, 0));
		state->setUniformTexture("u_mrao_map", mraoMap);
		state->setUniformTexture("u_env_map", settingMgr->_tableEnvTexture);
		state->setUniformFloat("u_metallic", -1);
		state->setUniformFloat("u_roughness", -1);
		state->setUniformTexture("u_irradiance_map", settingMgr->_tableIrradianceTexture);
		state->setUniformTexture("u_brdf_lut", settingMgr->_brdfTexture);
	}
	CCLOG("setGLProgramState");

	part->setGLProgramState(state);
	part->setLightMask(0);

	_state.push_back(state);
	return state;
}

void Table::setLights(std::vector<cocos2d::Vec3> lightPos, cocos2d::Vec3 lightColor)
{
	if (_state.size() >0) {
		// Metal
		_state[0]->setUniformVec3("u_light_color", lightColor);
		_state[0]->setUniformInt("u_light_count", lightPos.size());
		for (auto j = 0; j < lightPos.size(); j++) {
			_state[0]->setUniformVec3("u_light_pos_" + std::to_string(j), lightPos[j]);
		}

		// ETC
		auto etcLight = std::vector<Vec3>(lightPos.begin(), lightPos.end());
		etcLight.insert(etcLight.end(), Ball::TableETCLights, Ball::TableETCLights+2);
		_state[1]->setUniformVec3("u_light_color", lightColor);
		_state[1]->setUniformInt("u_light_count", etcLight.size());
		for (auto j = 0; j < etcLight.size(); j++) {
			_state[1]->setUniformVec3("u_light_pos_" +std::to_string(j), etcLight[j]);
		}
	}
}

void Table::drawKitchen()
{
	auto width = PhysicsConstants::TABLE_WIDTH;
	auto height = PhysicsConstants::TABLE_HEIGHT;
	auto pocketRadius = PhysicsConstants::CORNER_POCKET_RADIUS;
	auto CornerDepthSqrt2 = PhysicsConstants::CORNER_POCKET_DEPTH / sqrt(2);
	auto d = pocketRadius - CornerDepthSqrt2;

	auto draw3dNode = Sprite3D::create();
	draw3dNode->setPosition3D(Vec3(0, 0.04, 0));
	addChild(draw3dNode);

	Vec2 kitchen[] = { Vec2(-width / 2 + d, -height / 2), Vec2(-width / 2, -height / 2 + d),
		Vec2(-width / 2, height / 2 - d), Vec2(-width / 2 + d, height / 2),
		Vec2(-60, height / 2), Vec2(-60, -height / 2) };
	auto draw = DrawNode::create();
	draw->drawPoly(kitchen, 6, false, Color4F(Color3B(255, 255, 255)));
	draw->setPosition3D(Vec3(0, 0, 0));
	draw->setRotation3D(Vec3(90, 0, 0));
	draw3dNode->addChild(draw);
	_kitchenHighlight = draw;
	draw3dNode->setCameraMask((int)GameCamera::FLAG_3D);
	draw->setVisible(false);
}

void Table::initEffect()
{
	//_efkManager = efk.EffectManager.create(cc.winSize);
	//_pocketEffects = { -1, -1, -1, -1, -1, -1 };
	//
	auto pockets = _world->pockets();
	
	_pocketsPos[0] = ExtMath::vector(pockets->topRightPocket.position);
	_pocketsPos[1] = ExtMath::vector(pockets->topSidePocket.position);
	_pocketsPos[2] = ExtMath::vector(pockets->topLeftPocket.position);
	_pocketsPos[3] = ExtMath::vector(pockets->bottomLeftPocket.position);
	_pocketsPos[4] = ExtMath::vector(pockets->bottomSidePocket.position);
	_pocketsPos[5] = ExtMath::vector(pockets->bottomRightPocket.position);
}

void Table::initBillBoardOnPockets()
{
	_billboards.clear();
	for (auto i = 0; i < 6; i++) {
		auto pos = _pocketsPos[i];

		auto bb = BillBoard::create();
		bb->setPosition3D(Vec3(pos.x, 5, -pos.y));
		addChild(bb);
		_billboards.push_back(bb);
		auto staticSprite = Sprite3D::create();
		auto moveSprite = Sprite3D::create();
		moveSprite->setCameraMask((int)GameCamera::FLAG_3D);
		staticSprite->setCameraMask((int)GameCamera::FLAG_3D);
		_billboardStaticSprites.push_back(staticSprite);
		_billboardMoveSprites.push_back(moveSprite);
		bb->addChild(staticSprite);
		bb->addChild(moveSprite);

		bb->setCameraMask((int)GameCamera::FLAG_3D);

		moveSprite->runAction(RepeatForever::create(
			Sequence::create(
				MoveTo::create(1, Vec2(0, 5)),
				MoveTo::create(1, Vec2(0, -5)),
				nullptr
			)
		));
	}
}

void Table::initPocketHighlights()
{
	_pocketHighlights.clear();
	auto CORNER_POCKET_RADIUS = PhysicsConstants::CORNER_POCKET_RADIUS;
	auto SIDE_POCKET_RADIUS = PhysicsConstants::SIDE_POCKET_RADIUS;
	for (auto i = 0; i < 6; i++) {
		auto size = i % 3 == 1 ? SIDE_POCKET_RADIUS : CORNER_POCKET_RADIUS;
		auto draw = Sprite::create(Res::INGAME_POCKET_INDI);
		draw->setPosition3D(Vec3(_pocketsPos[i].x, 0, -_pocketsPos[i].y));
		draw->setRotation3D(Vec3(-90, 0, 0));
		draw->setScale(size / draw->getContentSize().width * 2);
		addChild(draw);
		draw->setVisible(false);
		_pocketHighlights.push_back(draw);
	}

	hideAllHighlightPockets();
	// highlightPocket();
}

void Table::initEfks()
{
	for (int i = 0; i < NUM_POCKET; i++)
	{
		auto isSidePocket = i % 3 == 1;
		
		auto pocketPos = Vec3(_pocketsPos[i].x, 0.1, -_pocketsPos[i].y);

		// init eight ball pocket effect
		auto efx8 = efk::Effect::create("res/effect/ingame/select_hole_2.efk", 0.2);
		auto emitter8 = efk::EffectEmitter::create(_efkManager);
		emitter8->setEffect(efx8);
		emitter8->setPlayOnEnter(true);
		emitter8->setSpeed(1);
		emitter8->play();
		emitter8->setIsLooping(true);
		emitter8->setPosition3D(pocketPos);
		emitter8->setScale(2);
		emitter8->setVisible(false);
		addChild(emitter8);
		_eightBallEffects.push_back(emitter8);

		float rotation = 0;
		if (i == 0 || i == 3) {
			rotation = -45;
		}
		else if (i == 2 || i == 5) {
			rotation = 45;
		}
		auto efx = efk::Effect::create("res/effect/ingame/ball_to_hole_Fx.efk", 0.2);
		auto emitter = efk::EffectEmitter::create(_efkManager);
		emitter->setEffect(efx);
		emitter->setSpeed(1);
		emitter->setRemoveOnStop(false);
		emitter->setPosition3D(pocketPos);
		emitter->setRotation3D(Vec3(0, rotation, 0));
		emitter->setScale(2);
		emitter->setVisible(false);
		addChild(emitter);
		_ballToHoleEffects.push_back(emitter);

		std::map<int, Node*> holeBallNumberEfk;
		for (int i = 1; i <= NUM_BALL; i++) {
			char effPath[50];
			if (i < 10) sprintf(effPath, "res/effect/ingame/num_0%d.efk",i);
			else sprintf(effPath, "res/effect/ingame/num_%d.efk", i);

			auto fx = efk::Effect::create(effPath, 0.2);
			auto emt = efk::EffectEmitter::create(_efkManager);
			emt->setEffect(fx);
			emt->setSpeed(1);
			emt->setRemoveOnStop(false);
			emt->setPosition3D(pocketPos);
			emt->setScale(2);
			addChild(emt);

			holeBallNumberEfk[i] = emt;
		}
		_ballNumberHoleEffects.push_back(holeBallNumberEfk);
	}
}

void Table::initTableFenceEfks()
{
	float width = PhysicsConstants::TABLE_WIDTH + 5.8f;
	float height = PhysicsConstants::TABLE_HEIGHT + 5;

	float cushionHeight = 2.28f;
	auto efxRight = createFenceEfk("res/effect/ingame/fence_vr_UtoD.efk");
	auto efxRight1 = createFenceEfk("res/effect/ingame/fence_vr_DtoU.efk");
	auto efxTopLeft1 = createFenceEfk("res/effect/ingame/fence_hl_top_LtoR.efk");
	auto efxTopLeft = createFenceEfk("res/effect/ingame/fence_hl_top_RtoL.efk");
	auto efxTopRight = createFenceEfk("res/effect/ingame/fence_hr_top_LtoR.efk");
	auto efxTopRight1 = createFenceEfk("res/effect/ingame/fence_hr_top_RtoL.efk");
	auto efxBottomLeft1 = createFenceEfk("res/effect/ingame/fence_hl_bot_LtoR.efk");
	auto efxBottomLeft = createFenceEfk("res/effect/ingame/fence_hl_bot_RtoL.efk");
	auto efxBottomRight = createFenceEfk("res/effect/ingame/fence_hr_bot_LtoR.efk");
	auto efxBottomRight1 = createFenceEfk("res/effect/ingame/fence_hr_bot_RtoL.efk");
	auto efxLeft1 = createFenceEfk("res/effect/ingame/fence_vl_UtoD.efk");
	auto efxLeft = createFenceEfk("res/effect/ingame/fence_vl_DtoU.efk");

	efxRight->setPosition3D(Vec3(width / 2, cushionHeight, 0));
	efxRight1->setPosition3D(Vec3(width / 2, cushionHeight, 0));
	efxTopLeft->setPosition3D(Vec3(-width / 4, cushionHeight, -height / 2));
	efxTopLeft1->setPosition3D(Vec3(-width / 4, cushionHeight, -height / 2));
	efxTopRight->setPosition3D(Vec3(width / 4, cushionHeight, -height / 2));
	efxTopRight1->setPosition3D(Vec3(width / 4, cushionHeight, -height / 2));

	efxBottomLeft->setPosition3D(Vec3(-width / 4, cushionHeight, height / 2));
	efxBottomLeft1->setPosition3D(Vec3(-width / 4, cushionHeight, height / 2));
	efxBottomRight->setPosition3D(Vec3(width / 4, cushionHeight, height / 2));
	efxBottomRight1->setPosition3D(Vec3(width / 4, cushionHeight, height / 2));
	efxLeft->setPosition3D(Vec3(-width / 2, cushionHeight, 0));
	efxLeft1->setPosition3D(Vec3(-width / 2, cushionHeight, 0));

	addChild(efxRight);
	addChild(efxRight1);
	addChild(efxTopLeft);
	addChild(efxTopLeft1);
	addChild(efxTopRight);
	addChild(efxTopRight1);
	addChild(efxBottomLeft);
	addChild(efxBottomLeft1);
	addChild(efxBottomRight);
	addChild(efxBottomRight1);
	addChild(efxLeft);
	addChild(efxLeft1);

	_allFenceEfxs.insert(_allFenceEfxs.begin(), {efxRight, efxRight1, efxLeft, efxLeft1, efxTopLeft1, efxTopLeft, 
		efxTopRight, efxTopRight1, efxBottomRight1, efxBottomRight, efxBottomLeft, efxBottomLeft1 });
	hideAllFenceEfx();

	_fenceEfx[0].push_back(efxRight1); _fenceEfx[0].push_back(efxTopRight);
	_fenceEfx[1].push_back(efxTopLeft1); _fenceEfx[1].push_back(efxTopRight1);
	_fenceEfx[2].push_back(efxTopLeft); _fenceEfx[2].push_back(efxLeft);
	_fenceEfx[3].push_back(efxLeft1); _fenceEfx[3].push_back(efxBottomLeft);
	_fenceEfx[4].push_back(efxBottomLeft1); _fenceEfx[4].push_back(efxBottomRight1);
	_fenceEfx[5].push_back(efxBottomRight); _fenceEfx[5].push_back(efxRight);
}

void Table::initBallForceEffect()
{
	efk::Effect *effect = efk::Effect::create("res/effect/ingame/power_cue_lv.efk");
	efk::EffectEmitter *emitter = efk::EffectEmitter::create(_efkManager);
	emitter->setEffect(effect);
	emitter->setPlayOnEnter(true);
	emitter->setIsLooping(true);
	emitter->play(0);
	emitter->setScale(0.125);
	emitter->setRotation3D(Vec3(0, 90, 0));
	emitter->setPosition3D(Vec3(-60, PhysicsConstants::BALL_RADIUS, 0));
	emitter->setScaleZ(0.1);
	emitter->setCameraMask((int)GameCamera::FLAG_3D);
	addChild(emitter);
	emitter->setVisible(false);

	_ballForceEfx = emitter;
}

efk::EffectEmitter * Table::createFenceEfk(const std::string & path, bool revert /*= false*/)
{
	efk::Effect *effect = efk::Effect::create(path, 0.2);
	efk::EffectEmitter *emitter = efk::EffectEmitter::create(_efkManager);
	emitter->setEffect(effect);
	emitter->setPlayOnEnter(true);
	emitter->setIsLooping(true);
	emitter->play(0);
	if (revert) {
		emitter->setSpeed(-0.5);
	}
	else {
		emitter->setSpeed(0.5);
	}
	emitter->setScale(2.5);

	emitter->setCameraMask((int)GameCamera::FLAG_3D);
	return emitter;
}

void Table::setEfkManager(efk::EffectManager * efkManager)
{
	_efkManager = efkManager;
	initEfks();
	initTableFenceEfks();
	initBallForceEffect();
}

int Table::subscribeOnPaused(const TableOnPauseCallBack & tableOnPauseCallBack)
{
	int id = _onPausedCallbacks.size();
	_onPausedCallbacks.push_back(tableOnPauseCallBack);
	return id;
}

int Table::subscribeTableEvents(const TableEventCallBack & tableEventCallBack)
{
	int id = _tableEventListener.size();
	_tableEventListener.push_back(tableEventCallBack);
	return id;
}

void Table::onEnter()
{
	Node::onEnter();
	this->schedule(CC_SCHEDULE_SELECTOR(Table::step));
	scheduleUpdate();
	//PhysicsFactory *pf = new PhysicsFactory();
	//pf->test();
}

void Table::step(float dt)
{
	this->_world->update(dt);
}

void Table::onWorldPause(double dt)
{
	this->updateCuePosition();
	this->setGuildLineVisible(true);
	this->setCueVisible(true);
	
	for (int i = 0; i < _onPausedCallbacks.size(); i++)
		_onPausedCallbacks[i]();

	gameMgr->updateTrajectory();
	interactionMgr->enablePlayingMode();
}

void Table::handleBallCollision(double time, ps::BallBody * ballBody1, ps::BallBody * ballBody2)
{
	TableEventType eventType = TableEventType::BALL_COLLIDE_BALL;
	double v1 = ExtMath::vector::length(ballBody1->linearVelocity());
	double v2 = ExtMath::vector::length(ballBody2->linearVelocity());
	double velocity = MAX(v1, v2);
	BallCollisionData *ballCollisionData = new BallCollisionData(ballBody1->id(), ballBody2->id(), velocity);

	for (auto i = 0; i < _tableEventListener.size(); i++) {
		_tableEventListener[i](eventType, ballCollisionData);
	}
	CC_SAFE_DELETE(ballCollisionData);
}

void Table::handleCushionCollision(double time, ps::BallBody * ballBody, ps::CushionSegment * cushion)
{
	TableEventType eventType = TableEventType::BALL_COLLIDE_CUSHION;
	CushionCollisionData *cushionCollisionData = new CushionCollisionData(ballBody->id(), cushion->id);

	for (auto i = 0; i < _tableEventListener.size(); i++) {
		_tableEventListener[i](eventType, cushionCollisionData);
	}
	CC_SAFE_DELETE(cushionCollisionData);
}

void Table::handleBallFallen(double time, BallBody * ballBody, Pocket * pocket)
{
	TableEventType eventType = TableEventType::BALL_FELL_HOLE;
	BallFellHoleData *ballFellHoleData = new BallFellHoleData(ballBody->id(), pocket->id);

	for (auto i = 0; i < _tableEventListener.size(); i++) {
		_tableEventListener[i](eventType, ballFellHoleData);
	}
	CC_SAFE_DELETE(ballFellHoleData);
}

void Table::updateCuePosition()
{
	this->_cue->setCuePosition(this->_whiteBall->body()->position());
}

void Table::reset()
{
	auto ballPositions = (new BallGenerator())->generateDefault();
	for (auto i = 0; i < _balls.size(); i++) {
		auto ball = _balls[i];
		ball->body()->reset();
		if (ball->_number == 0) ball->setBallPosition(ps::PhysicsConstants::CUE_BALL_POSITION);
		else ball->setBallPosition(ballPositions[i - 1]);
	}
	_cue->setCuePosition(this->_whiteBall->body()->position());
	setCueVisible(false);
	setGuildLineVisible(false);
}

void Table::setCueVisible(bool visible)
{
	_cue->setVisible(visible);
}

void Table::setGuildLineVisible(bool visible)
{
	if(_drawCircleNode)
		_drawCircleNode->setVisible(visible);
	_guildLine->setVisible(visible);
	getFakeCueBall()->setVisible(visible);
}

void Table::updateGuildLines(std::vector<cocos2d::Vec3> points)
{
	auto sub = _guildLine->sub;
	sub->clear();
	sub->drawLine(points[0], points[1], Table::GUILD_WHITE);
}

void Table::drawTrajectory(std::vector<TrajectoryElement*> trajectories, std::vector<ps::ExtMath::vector> points)
{
	auto reflectDir = trajectories;
	this->drawFloatingTrajectory(reflectDir, points);
}

void Table::drawFloatingTrajectory(std::vector<TrajectoryElement*> trajectories, std::vector<ps::ExtMath::vector> points)
{
	if (points.size() ==0) {
		_guildLine->clearAll();
		this->getFakeCueBall()->setVisible(false);
		return;
	}
	auto len = points.size();

	auto guild = this->_guildLine;
	guild->clearAll();

	auto i = 1;
	auto prevPoint = points[0];
	vector point;
	auto hasFlying = false;

	while (i < len) {
		point = points[i];
		guild->drawLine(
			CocosUtils::transVecToCCVec(prevPoint),
			CocosUtils::transVecToCCVec(point),
			Table::GUILD_GREEN
		);

		hasFlying |= (point.z > PhysicsConstants::BALL_RADIUS);

		// if (prevPoint.z > gv.Physicsautoants.BALL_RADIUS || point.z > gv.Physicsautoants.BALL_RADIUS) {
		guild->sub->drawLine(
			Vec3(prevPoint.x, 0.02, -prevPoint.y),
			Vec3(point.x, 0.02, -point.y),
			Table::GUILD_GRAY
		);
		// }

		prevPoint = point;
		i = i + 1;
	}
	auto fakeBall = this->getFakeCueBall();
	fakeBall->setVisible(true);
	fakeBall->setPosition3D(CocosUtils::transVecToCCVec(prevPoint));
	this->getFakeCueBall()->setColor(Color3B(255, 255, 255));

	/*if (reflectDir && reflectDir.seg1.s) {
		guild->drawLine(
			CocosUtils::transVecToCCVec(reflectDir.seg1.s),
			CocosUtils::transVecToCCVec(reflectDir.e1),
			Table::GUILD_GREEN
		);

		guild->sub->drawLine(
			Vec3(reflectDir.seg1.s.x, 0.02, -reflectDir.seg1.s.y),
			Vec3(reflectDir.e1.x, 0.02, -reflectDir.e1.y),
			Table::GUILD_GRAY
		);

		guild->sub->drawCircle(
			Vec3(reflectDir.seg1.s.x, 0.02, -reflectDir.seg1.s.y),
			PhysicsConstants::BALL_RADIUS,
			Vec3(0, 1, 0),
			50,
			Table::GUILD_GRAY
		);
	}
	if (reflectDir && reflectDir.seg2.s) {
		guild->drawLine(
			CocosUtils::transVecToCCVec(reflectDir.seg2.s),
			CocosUtils::transVecToCCVec(reflectDir.seg2.e),
			Table::GUILD_GREEN
		);

			guild->sub->drawLine(
				Vec3(reflectDir.seg2.s.x, 0.02, -reflectDir.seg2.s.y),
				Vec3(reflectDir.seg2.e.x, 0.02, -reflectDir.seg2.e.y),
				Table::GUILD_GRAY
			);

		guild->sub->drawCircle(
			Vec3(reflectDir.seg2.s.x, 0.02, -reflectDir.seg2.s.y),
			PhysicsConstants::BALL_RADIUS,
			Vec3(0, 1, 0),
			50,
			Table::GUILD_GRAY
		);
	}*/
}

void Table::drawFloatingTrajectory(TrajectoryReflectData trajectory, std::vector<ps::ExtMath::vector> points)
{
	if (points.size() == 0) {
		_guildLine->clearAll();
		this->getFakeCueBall()->setVisible(false);
		return;
	}
	auto len = points.size();

	auto guild = this->_guildLine;
	guild->clearAll();

	auto i = 1;
	auto prevPoint = points[0];
	vector point;
	auto hasFlying = false;

	while (i < len) {
		point = points[i];
		guild->drawLine(
			CocosUtils::transVecToCCVec(prevPoint),
			CocosUtils::transVecToCCVec(point),
			Table::GUILD_GREEN
		);

		hasFlying |= (point.z > PhysicsConstants::BALL_RADIUS);

		// if (prevPoint.z > gv.Physicsautoants.BALL_RADIUS || point.z > gv.Physicsautoants.BALL_RADIUS) {
		guild->sub->drawLine(
			Vec3(prevPoint.x, 0.02, -prevPoint.y),
			Vec3(point.x, 0.02, -point.y),
			Table::GUILD_GRAY
		);
		// }

		prevPoint = point;
		i = i + 1;
	}
	auto fakeBall = this->getFakeCueBall();
	fakeBall->setVisible(true);
	fakeBall->setPosition3D(CocosUtils::transVecToCCVec(prevPoint));
	this->getFakeCueBall()->setColor(Color3B(255, 255, 255));
	
	auto reflectDir = trajectory;
	//TODO
	//auto match = gv.gameMgr.ebpCtrl._curMatch;
	auto ballIdx = trajectory.ballId;
	if (ballIdx != -1) {
		//auto suggestedBalls = match->getSuggestedBall();
		//if (suggestedBalls.indexOf(ballIdx) >= 0)
		//	getFakeCueBall()->setColor(Color3B(255, 255, 255));
		//else
		//	getFakeCueBall()->setColor(Color3B(255, 150, 150));
		//gameMgr.gameUI.getIngameUI().setInteractionBall(ballIdx);
	}
	else {
		//gameMgr.gameUI.getIngameUI().setInteractionBall(-1);
	}

	guild->drawLine(
		CocosUtils::transVecToCCVec(reflectDir.seg1.s),
		CocosUtils::transVecToCCVec(reflectDir.seg1.e),
		Table::GUILD_GREEN
	);

	guild->sub->drawLine(
		Vec3(reflectDir.seg1.s.x, 0.02, -reflectDir.seg1.s.y),
		Vec3(reflectDir.seg1.e.x, 0.02, -reflectDir.seg1.e.y),
		Table::GUILD_GRAY
	);

	guild->sub->drawCircle(
		Vec3(reflectDir.seg1.s.x, 0.02, -reflectDir.seg1.s.y),
		PhysicsConstants::BALL_RADIUS,
		Vec3(0, 1, 0),
		50,
		Table::GUILD_GRAY
	);

	if (vector::equalsZero(reflectDir.seg2.s) && !vector::equalsZero(reflectDir.seg2.e)) {
		guild->drawLine(
			CocosUtils::transVecToCCVec(reflectDir.seg2.s),
			CocosUtils::transVecToCCVec(reflectDir.seg2.e),
			Table::GUILD_GREEN
		);

		guild->sub->drawLine(
			Vec3(reflectDir.seg2.s.x, 0.02, -reflectDir.seg2.s.y),
			Vec3(reflectDir.seg2.e.x, 0.02, -reflectDir.seg2.e.y),
			Table::GUILD_GRAY
		);

		guild->sub->drawCircle(
			Vec3(reflectDir.seg2.s.x, 0.02, -reflectDir.seg2.s.y),
			PhysicsConstants::BALL_RADIUS,
			Vec3(0, 1, 0),
			50,
			Table::GUILD_GRAY
		);
	}
}

void Table::drawTrajSegmentPoints(std::vector<std::list<vector>> segments, bool showGhostBall)
{
	std::list<Node*>& circles = _drawCircles;
	if (!_guildLineVisible) {
		for (auto ittr = circles.begin(); ittr != circles.end(); ittr++) {
			Utils::releaseEfkCircle(*ittr);
		}
		circles.clear();
		return;
	}

	auto width = PhysicsConstants::TABLE_WIDTH;
	auto height = PhysicsConstants::TABLE_HEIGHT;

	auto pointCounter = 0;
	
	
	double lastTimer = 0;
	if (circles.size() > 0) {
		Node *circle = circles.front();
		TimerSprite *sprite = (TimerSprite *)(circle->getChildByTag(TAG_COMMON));
		lastTimer = sprite->getTimer();
	}
	removeChildByTag(TAG_COMMON);
	Node *node = Node::create();
	node->setTag(TAG_COMMON);
	addChild(node);
	std::list<Node *>::iterator ittr = circles.begin();
	for (auto i = 0; i < segments.size(); ++i) {
		auto points = segments[i];
		int idx = 0;
		for (auto pointIttr = points.begin(); pointIttr != points.end(); pointIttr++){
			vector point = *pointIttr;
			Node* circle;
			if (point.x > width / 2 || point.x < -width / 2 || point.y < -height / 2 || point.y > height / 2) {
				continue;
			}
			pointCounter++;

			BillBoard *node3D = BillBoard::create();
			node->addChild(node3D);
			
			TimerSprite *sprite = TimerSprite::create("cricle_alpha.png");
			BlendFunc blendFunc; blendFunc.src = GL_ONE; blendFunc.dst = GL_ONE;
			sprite->setBlendFunc(blendFunc);
			sprite->setTimer(lastTimer - 0.03 * idx);
			node3D->addChild(sprite);
			auto ccPos = CocosUtils::transVecToCCVec(point);
			node3D->setPosition3D(ccPos);
			node3D->setCameraMask((int)GameCamera::FLAG_3D);
			sprite->setColor(Color3B(255, 255, 255));
			//node3D->setScale(0.01);
			++idx;
		}
	}
	_drawCircleNode = node;

	auto ghostBall = getFakeCueBall();
	ghostBall->setVisible(showGhostBall);
	if (showGhostBall && segments[0].size() > 0) {
		auto firstSegment = segments[0];
		auto lastPosition = firstSegment.back();

		ghostBall->setPosition3D(CocosUtils::transVecToCCVec(lastPosition));
	}
}

void Table::onChangeCameraPosition(cocos2d::Vec3 position)
{
	if (_state.size() > 0) {
		for (auto i = 0; i < _state.size() - 1; i++) {
			_state[i]->setUniformVec3("u_view_pos", position);
		}
	}

	if (_fakeBallState != nullptr) {
		_fakeBallState->setUniformVec3("u_view_pos", position);
	}
}

void Table::resume(bool notHideCue)
{
	//TODO
	//if(!notHideCue) {
	//	this->setCueVisible(false);
	//	this->setGuildLineVisible(false);
	//}
	//_world->setRun(true);
}

void Table::highlightBalls(std::vector<int> *nums)
{
	if (_highlightingBalls != NULL)
	{
		CC_SAFE_DELETE(_highlightingBalls);
	}
	_highlightingBalls = nums;

	for (auto i = 0; i < _balls.size(); i++) {
		auto ball = _balls[i];
		if (std::find(nums->begin(), nums->end(), ball->_number) != nums->end()) {
			ball->highlight(true);
		}
		else {
			ball->highlight(false);
		}
	}
}

void Table::highlightBalls()
{
	auto nums = _highlightingBalls;
	for (auto i = 0; i < _balls.size(); i++) {
		auto ball = _balls[i];
		if (std::find(nums->begin(), nums->end(), ball->_number) != nums->end()) {
			ball->highlight(true);
		}
		else {
			ball->highlight(false);
		}
	}
}

void Table::hightLightAllBalls()
{
	for (auto i = 0; i < _balls.size(); i++) {
		auto ball = _balls[i];
		ball->highlight(true);
	}
}

void Table::unHighlightBalls()
{
	for (auto i = 0; i < _balls.size(); i++) {
		auto ball = _balls[i];
		ball->highlight(false);
	}
}

void Table::resetEventCount()
{
	_eventCount = 0;
}

void Table::highlightPocket(int pocketId)
{
	if (pocketId == -1)
	{
		for (int i = 0; i < NUM_POCKET; i++)
		{
			showSelectPocket(i, SelectPocketState::SELECTING);
		}
		hideAllFenceEfx();
	}
	else
	{
		hideAllHighlightPockets();
		showHighlightPocket(pocketId, true);
		showSelectPocket(pocketId, SelectPocketState::AFTER);
	}
}

void Table::hideAllHighlightPockets()
{
	hideAllFenceEfx();
	for (int i = 0; i < _eightBallEffects.size(); i++)
	{
		_eightBallEffects[i]->setVisible(false);
	}

	for (int i = 0; i < NUM_POCKET; i++) 
	{
		removeSpriteOnPocket(i);
	}
}

void Table::hideAllFenceEfx()
{
	for (int i = 0; i < _allFenceEfxs.size(); i++)
	{
		_allFenceEfxs[i]->setVisible(false);
	}
}

void Table::showHighlightPocket(int pocketId, bool effect)
{
	_eightBallEffects[pocketId]->setVisible(true);

	auto fenceEfxs = _fenceEfx[pocketId];
	for (int i = 0; i < fenceEfxs.size(); i++)
	{
		fenceEfxs[i]->setVisible(true);
	}
}

void Table::showSelectPocket(int pocketId, SelectPocketState state)
{
	std::string resPath = "";
	bool moving = false;
	if (state == SelectPocketState::SELECTING)
	{
		resPath = Res::INGAME_FINGERPRINT;
		moving = false;
	}
	else if (state == SelectPocketState::AFTER)
	{
		resPath = "Tab_Button_Choose.png";
		moving = true;
	}

	
	cocos2d::Node *sprite;

	if (state == SelectPocketState::SELECTING)
	{
		auto btn = Button::create();
		btn->loadTextures(resPath, resPath, resPath, TextureResType::LOCAL);
		btn->setTag(TAG_SELECT);
		btn->setScale(0.08);
		btn->setPositionY(0);
		btn->setTouchEnabled(true);
		sprite = btn;
	}
	else if(state == SelectPocketState::AFTER)
	{
		sprite = PocketArrows::create();
		sprite->setTag(TAG_ARROW);
		sprite->setCameraMask((int)GameCamera::FLAG_3D);
		sprite->setScale(0.25);
		sprite->setPositionY(7);
		showSpriteOnPocket(pocketId, sprite, false);
		removeSpriteOnPocket(pocketId, TAG_SELECT);
		return;
	}

	removeSpriteOnPocket(pocketId, TAG_SELECT);
	showSpriteOnPocket(pocketId, sprite, moving);
}

void Table::showSpriteOnPocket(int pocketId, cocos2d::Node * sprite, bool isMoving)
{
	sprite->setCameraMask((int) GameCamera::FLAG_3D);
	auto billboard = _billboards[pocketId];
	if (isMoving)
	{
		_billboardMoveSprites[pocketId]->addChild(sprite);
	}
	else
	{
		_billboardMoveSprites[pocketId]->addChild(sprite);
	}
}

void Table::removeSpriteOnPocket(int pocketId, int tag)
{
	if (tag == -1)
	{
		_billboardMoveSprites[pocketId]->removeAllChildren();
		_billboardStaticSprites[pocketId]->removeAllChildren();
	}
	else {
		_billboardMoveSprites[pocketId]->removeChildByTag(tag);
		_billboardStaticSprites[pocketId]->removeChildByTag(tag);
	}
	
	
}

ps::ExtMath::vector Table::getCueBallDefaultPosForPlacing()
{
	auto pos = ExtMath::vector(-60, 0, 0);
	double width = PhysicsConstants::TABLE_WIDTH, height = PhysicsConstants::TABLE_HEIGHT;
	while (!isCueBallPosValid(pos)) {
		pos = vector((double)rand()/RAND_MAX *(width / 2 - 60) - width / 2, (double)rand() / RAND_MAX* height - height / 2, 0);
	}
	return pos;
}

bool Table::isCueBallPosValid(ps::ExtMath::vector pos)
{
	for (auto i = 0; i < _balls.size(); i++) {
		if (_balls[i] == _whiteBall || _balls[i]->body()->isDisabled()) continue;
		if (ExtMath::vector::distanceSquare(pos, _balls[i]->body()->position()) <= 4 * PhysicsConstants::BALL_RADIUS * PhysicsConstants::BALL_RADIUS)
			return false;
	}
	return true;
}

void Table::setBallPosition(int ballId, ps::ExtMath::vector pos)
{
	CCLOG("setBallPosition pos(%f %f %f)", pos.x, pos.y, pos.z);
	for (int i = 0; i < _balls.size(); i++)
	{
		auto ball = _balls[i];
		if (ball->_number == ballId)
		{
			ball->body()->setPosition(pos);
			ball->body()->dispatch();
			break;
		}
	}
	_world->resetStaticWorldBallPosition();
}


void Table::setBallEnable(int ballId)
{
	for (int i = 0; i < _balls.size(); i++)
	{
		auto ball = _balls[i];
		if (ball->_number == ballId)
		{
			ball->body()->enable();
			return;
		}
	}
}

void Table::setBallDisable(int ballId)
{
	for (int i = 0; i < _balls.size(); i++)
	{
		auto ball = _balls[i];
		if (ball->_number == ballId)
		{
			ball->setVisible(false);
			ball->body()->disable();
			ball->body()->cheatTotalInHole(true);
			return;
		}
	}
}

void Table::loadFakeData(std::map<int, ps::ExtMath::vector> balls)
{
	for (int i = 0; i < 16; i++)
	{
		if (balls.find(i) == balls.end())
		{
			setBallDisable(i);
		}
		else
		{
			setBallPosition(i, balls[i]);
			if (balls[i].z < 0) {
				setBallDisable(i);
			}
		}
	}
}

int Table::getPocketRegion(ps::ExtMath::vector p)
{
	auto pocket = PhysicsWorldV2::floorSurface()->findExtractPocket(p);
	if (pocket != NULL)
		return pocket->id;
	return -1;
}

void Table::update(float dt)
{
	auto camera = gameMgr->_camera->getCamera();
	Vec3 cameraPos = camera->getPosition3D();
	for (int i = 0; i < _billboards.size(); i++) {
		bool hasChildren = _billboardMoveSprites[i]->getChildrenCount() + _billboardStaticSprites[i]->getChildrenCount() > 0;
		if (!hasChildren) continue;
		auto bbPos = _billboards[i]->getPosition3D();
		Vec3 v = bbPos - cameraPos;
	
		auto dist = v.length();
		auto p = bbPos;
		auto d = Vec3(0, 0, 0);
		// camera.getViewProjectionMatrix().transformVector(p, d)
		// utils.log("POS", p, d);
		auto mat = camera->getViewProjectionMatrix();
		d.x = mat.m[0] * p.x + mat.m[4] * p.y + mat.m[8] * p.z + mat.m[12];
		d.y = mat.m[1] * p.x + mat.m[5] * p.y + mat.m[9] * p.z + mat.m[13];
		d.z = mat.m[2] * p.x + mat.m[6] * p.y + mat.m[10] * p.z + mat.m[14];

		if (d.x >= -200 && d.x <= 200 && d.y >= -200 && d.y <= 200) {
			auto camToBb = Vec3(cameraPos.x - bbPos.x, cameraPos.y - bbPos.y, cameraPos.z - bbPos.z);
			auto bbToCenter = Vec3(-bbPos.x, -bbPos.y, -bbPos.z);
			auto dotProduct = camToBb.x * bbToCenter.x + camToBb.y * bbToCenter.y + camToBb.z * bbToCenter.z;

			auto blockSight = dotProduct < 0;
			if (blockSight && dist < 250) {
				auto children = _billboardMoveSprites[i]->getChildren();
				for (int j = 0; j < children.size(); j++)
				{
					children.at(j)->setOpacity(255 * dist / 250 / 5);
				}

				auto staticChildren = _billboardStaticSprites[i]->getChildren();
				for (int j = 0; j < staticChildren.size(); j++)
				{
					staticChildren.at(j)->setOpacity(255 * dist / 250 / 5);
				}

			}
			else {
				auto children = _billboardMoveSprites[i]->getChildren();
				for (int j = 0; j < children.size(); j++)
				{
					children.at(j)->setOpacity(255);
				}

				auto staticChildren = _billboardStaticSprites[i]->getChildren();
				for (int j = 0; j < staticChildren.size(); j++)
				{
					staticChildren.at(j)->setOpacity(255);
				}
			}
		}
	}
	updateEfkSeer(dt);
}

void Table::updateEfkSeer(float dt)
{
	if (_efkManager)
	{
		for (int i = 0; i < NUM_POCKET; i++)
		{
			auto holeEfk = _ballToHoleEffects[i];
			if (holeEfk->isVisible())
			{
				_showTimes[holeEfk] += dt;
				if (_showTimes[holeEfk] > EFX_SHOW_TIME)
				{
					holeEfk->setVisible(false);
					_showTimes[holeEfk] = -1;
				}
			}
		}

	}
}

void Table::onPocketedBall(int pocketId, int ballId)
{
	if (_ballToHoleEffects.size() == 0)
		return;

	auto holeEfx = (efk::EffectEmitter *)_ballToHoleEffects[pocketId];
	holeEfx->setVisible(true);
	_showTimes[holeEfx] = 0;
	holeEfx->play(0);

	if (ballId > 0 && ballId <= NUM_BALL)
	{
		auto ball = BallPocket3DEffect::create(ballId);
		addChild(ball);
		ball->setPosition3D(holeEfx->getPosition3D());
		ball->play();
	}
}

void Table::highlightCueAndEightBall(bool val)
{
	for (int i = 0; i < _balls.size(); i++) {
		auto ball = _balls[i];
		if (ball->_number == 8 || ball == _whiteBall) {
			ball->setBallHighlightVisible(val);
		}
	}
}


void Table::drawForce(const ps::ExtMath::vector & direction, float forcePercent)
{
	_ballForceEfx->setVisible(forcePercent > 2);
	if (forcePercent < 2)
		return;
	forcePercent *= 3;

	float length = vector::length(direction);
	vector uDirection = vector::unit(direction);

	float theta = std::asin(uDirection.z);

	vector d = vector::unit(vector(uDirection.x, uDirection.y));

	float p = std::acos(d.y);
	if (d.x > 0) p = -p;

	float ballR = PhysicsConstants::BALL_RADIUS;
	vector cuePos = getWhiteBall()->body()->position();

	float distance = forcePercent / 100 * 25 - Cue::OFFSET_DISTANCE;
	float scaleZ = 0.1f * distance / 5.85f;
	_ballForceEfx->setScaleZ(scaleZ);
	Vec3 efxPos = Vec3(cuePos.x + (ballR / 2 + distance / 2) * sin(p) * cos(-theta),
		ballR + (ballR / 2 + distance / 2) * sin(-theta),
		-cuePos.y + (ballR / 2 + distance / 2) * cos(p) * cos(-theta));

	_ballForceEfx->setPosition3D(efxPos);
	_ballForceEfx->setCameraMask((int) GameCamera::FLAG_3D);

	p = p * 180 / M_PI;
	theta = theta * 180 / M_PI;

	_ballForceEfx->setRotation3D(Vec3(theta, p, 0));
}

void Table::setVisibleEffectForce(bool visible)
{
	if (_ballForceEfx)
	{
		_ballForceEfx->setVisible(visible);
	}
}

cocos2d::Sprite3D* Table::getFakeCueBall()
{
	if (!this->_fakeCueBall) {
		auto ball = Sprite3D::create(Res::BALL_HI_POLY_2);
		ball->setScale(1);
		ball->setCameraMask(this->getCameraMask());
		auto state = GLProgramState::create(GLProgram::createWithFilenames("res/shaders/3d_shader.vert", "res/shaders/IOS/3d_shader_ball_5.frag"));
		ball->setGLProgramState(state);
		ball->setTexture(Res::Ball_Textures[0]);
		state->setUniformTexture("u_env_map", settingMgr->_ballEnvTexture);
		state->setUniformTexture("u_irradiance_map", settingMgr->_ballIrradianceTexture);
		state->setUniformTexture("u_brdf_lut", settingMgr->_brdfTexture);
		state->setUniformFloat("u_roughness", 0.23);
		state->setUniformFloat("u_metallic", 0.02);
		state->setUniformFloat("u_ao", 1);
		auto lightPos = Ball::DefaultLights;
		int n = Ball::NUM_DEFAULT_LIGHT;
		state->setUniformInt("u_light_count", n);
		state->setUniformVec3("u_light_color", Ball::DefaultLightColor);
		for (auto i = 0; i < n; i++) {
			state->setUniformVec3("u_light_pos_" + std::to_string(i), lightPos[i]);
		}
		state->setUniformFloat("u_opacity", 0.5);
		state->setUniformFloat("u_grayscale", 0);
		_fakeBallState = state;
		ball->setLightMask(0);
		this->addChild(ball);
		this->_fakeCueBall = ball;
	}
	return this->_fakeCueBall;
}

TableGuildLine* Table::getGuildLine()
{
	return this->_guildLine;
}

Cue * Table::getCue()
{
	return _cue;
}

Ball * Table::getWhiteBall()
{
	return _whiteBall;
}

Ball * Table::getBallWithId(int id)
{
	for (int i = 0; i < _balls.size(); i++)
	{
		if (_balls[i]->_number == id)
			return _balls[i];
	}
	return NULL;
}

std::vector<Sprite3D*> Table::getParts()
{
	return _parts;
}

ps::PhysicsWorldV2* Table::getWorld()
{
	return this->_world;
}

std::vector<Ball*> Table::getAllBall()
{
	return this->_balls;
}

Color4B Table::GUILD_RED = Color4B(255, 50, 50, 150);
Color4B Table::GUILD_GREEN = Color4B(50, 255, 50, 255);
Color4B Table::GUILD_GRAY = Color4B(0, 0, 0, 40);
Color4B Table::GUILD_WHITE = Color4B(255, 255, 255, 40);

const float Table::EFX_SHOW_TIME = 2.5f;



// Class TableGuildLine
TableGuildLine::TableGuildLine()
{
}

bool TableGuildLine::init()
{
	if (!ThiccDrawNode3D::init()){
		return false;
	}
	sub = ThiccDrawNode3D::create();
	this->addChild(sub);
	sub2 = ThiccDrawNode3D::create();
	this->addChild(sub2);
	sub3 = ThiccDrawNode3D::create();
	this->addChild(sub3);
	sub4 = ThiccDrawNode3D::create();
	this->addChild(sub4);
	sub5 = ThiccDrawNode3D::create();
	this->addChild(sub5);
	sub6 = ThiccDrawNode3D::create();
	this->addChild(sub6);
	return true;
}

void TableGuildLine::clearAll()
{
	this->clear();
	sub->clear();
	sub2->clear();
	sub3->clear();
	sub4->clear();
	sub5->clear();
	sub6->clear();
}
