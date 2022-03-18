#include "IngameUI.h"
#include "BallTypeNotify.h"
#include <data/Resource.h>
#include <base/UIFactory.h>
#include <modules/core/interaction/InteractionManager.h>
#include <modules/core/ps/internal/PhysicsConstants.hpp>
#include <modules/core/ps/internal/ExtensionMath.hpp>
#include "../elements/PowerUI.h"
#include "PlacingBallControlUI.h"
#include <core/GameMgr.h>
#include <modules/utils/MShaderCache.h>

USING_NS_CC;
using namespace cocos2d::ui;
using namespace ps;


IngameUINew::IngameUINew(){
	
}

IngameUINew::~IngameUINew() {

}

bool IngameUINew::init() {
	if (!BaseLayer::init("IngameUINew")) {
		return false;
	}

	std::string filePath = Res::GUI::getCsbPathFromJson(Res::GUI::Ingame_UI_V4);
	this->initWithBinaryFile(filePath);

	initComponent();
	this->_showAngle = false;
	this->initTurnStates();
	//this->_listeners = {};
	this->updateTurnLayout(true);
	return true;
}

void IngameUINew::initGUI() {
	//todo
	getControl("fake")->setVisible(false);
	_bg = (ImageView *)_layout->getChildByName("bg");
	_btnBall = customButton("btnBall", IngameUINew::TAG_BTN_BALL, _bg);
	_btnBall->addTouchEventListener(CC_CALLBACK_2(IngameUINew::onCustomTouchEventHandler, this));
	_ballIndicator = _btnBall;
	_ballIndicatorTarget = (ImageView*) getControl("ballIndicator",_btnBall);

	_btnAngle = customButton("btnAngle", IngameUINew::TAG_BTN_ANGLE, _bg);
	_btnAngle->addTouchEventListener(CC_CALLBACK_2(IngameUINew::onCustomTouchEventHandler, this));
	_imgAngleCue = (ImageView*)getControl("imgCue", _btnAngle);

	_btnMoveBall = customButton("btnMoveBall", IngameUINew::TAG_BTN_MOVE_BALL, _bg);

	_btnSelectPocket = customButton("btnSelectPocket", IngameUINew::TAG_BTN_MOVE_BALL, _bg);
	
	_pnAvatars = (Widget*)getControl("pnAvatars", _bg);
	_imgAvatarBox = (ImageView *)getControl("imgAvatarBox", _pnAvatars);
	_pnHud = (Widget*)getControl("pnHud", _pnAvatars);
	_nodeP2 = getControl("nodeP2", _pnAvatars);
	_nodeP1 = getControl("nodeP1", _pnAvatars);

	_imgBgBtnCue = (ImageView*)getControl("imgBgBtnCue",_bg);
	_imgBallCueGroup = (ImageView*)getControl("imgBallCueGroup", _bg);

	_imgCollideBall = (ImageView *)getControl("imgCollideBall", _bg);
	_nodePlaceBall = getControl("nodePlaceBall", _bg);
	_imgPlaceBall = (ImageView *)getControl("imgPlaceBall", _nodePlaceBall);

	getControl("pnAvatars")->setVisible(false);
	getControl("imgCollideBall")->setVisible(false);
	getControl("imgMenuChat")->setVisible(false);
	getControl("btnSelectPocket")->setVisible(false);
	getControl("btnConfirm")->setVisible(false);
	getControl("imgTimerBg")->setVisible(false);
	//nodePlaceBall
}

void IngameUINew::initComponent(){
	//this->autoInitComponent(); todo

	this->initRulerAndPowerBar();
	//// initPocketedBalls();
	this->initAvatarNodes();
	this->initTimers();
	//this->initTimeBar();
	//// initGrayscaleShader();
	this->initPlacingBall();
	this->initHUD();
	//this->initScoreTicks();
	this->initAngleNode();
	this->initBallTypeNotify();
	//// _bg._imgControlsLeft._btnCamera.endScale = _bg._imgControlsLeft._btnCamera.getScale();
	//// _bg._imgControlsRight._btnCue.endScale = _bg._imgControlsRight._btnCue.getScale();
	//this->_ballIndicator = this->_bg._btnBall;
	//this->_ballIndicator.disableScale = true;
	//this->_ballIndicator._ballIndicator.offset = Vec2(0,0);
	//this->_fake.setVisible(false);
	//this->_bg._imgCollideBall.setVisible(false);
}

void IngameUINew::initBallTypeNotify() {
	Size winSize = Director::getInstance()->getVisibleSize();
	this->_ballTypeNotify = BallTypeNotify::create();
	this->addChild(this->_ballTypeNotify);
	this->_ballTypeNotify->setPosition(winSize.width / 2, winSize.height / 2);
	this->_ballTypeNotify->setVisible(false);
}

void IngameUINew::notifyBallType(int isSolid){
	this->_ballTypeNotify->showWithType(isSolid);
}

void IngameUINew::notifySelectPocket()
{
	_ballTypeNotify->showSelectEightBallPocket();
}



//
//void IngameUINew::notifySelectPocket(){
//	this->_ballTypeNotify.showSelectEightBallPocket();
//}
//
//void IngameUINew::initAngleNode(){
//	auto btnAngle = this->_bg._btnAngle;
//	auto btnBall = this->_bg._btnBall;
//	btnAngle.disableScale = true;
//	auto distance = btnAngle.y - (btnBall.y + btnBall.height*btnBall.getScaleY());
//	btnAngle.scheduleUpdate();
//	void IngameUINew::btnAngle.update (double dt){
//		btnAngle.y = btnBall.y + btnBall.height * btnBall.getScaleY() + distance;
//		btnAngle.setAngle(gv.gameMgr.interactionMgr._cueRPT.t / ExtMath::PI * 180);
//	}
//	void IngameUINew::btnAngle.setAngle (int angle){
//		btnAngle._imgCue.setRotation(- angle);
//	}
//
//	auto spriteMask = Sprite::create('#Mask.v4.png');
//	spriteMask.setAnchorPoint(0,0);
//	spriteMask.setPosition(0,0);
//	gv.scaleToSize(spriteMask, btnAngle.width, btnAngle.height);
//	// btnAngle.addChild(spriteMask);
//	auto cn = new cc.ClippingNode();
//	btnAngle.addChild(cn);
//	cn.setStencil(spriteMask);
//	btnAngle._imgCue.removeFromParent();
//	cn.addChild(btnAngle._imgCue);
//}
//
//void IngameUINew::initScoreTicks(){
//	auto pn = this->_bg._pnAvatars._pnHud;
//	auto nodes = [pn._imgGameModeRT3, pn._imgGameModeRT2];
//	for(auto i= 0;i<nodes.length;i++){
//		for(auto j=0;j<nodes[i].getChildren().length;j++){
//			auto box = nodes[i].getChildren()[j];
//			auto tick = Sprite::create('#Star.v3.png');
//			tick.setPosition(box.width/2, box.height/2);
//			tick.setVisible(false);
//			box.addChild(tick);
//			box.tick = tick;
//		}
//	}
//}
//
void IngameUINew::initHUD(){
	_hud = IngameHUD::create();
	Node* playerNodes[2] = { _nodeP1 , _nodeP2 };
	_hud->setIngameUI(this);
	_hud->initComponent(_pnHud, _imgAvatarBox, playerNodes);
	addChild(_hud);
}

//void IngameUINew::initGrayscaleShader(){
//	for(auto i=0;i<2;i++){
//		auto node = this->_bg._pnAvatars["_nodeP"+(i+1)]._imgBtnExt;
//		node.setLocalZOrder(3);
//
//		auto program = new cc.GLProgram("res/shaders/sprite_util.vert", "res/shaders/sprite_util.frag");
//		program.addAttribute(cc.ATTRIBUTE_NAME_POSITION, cc.VERTEX_ATTRIB_POSITION);
//		program.addAttribute(cc.ATTRIBUTE_NAME_TEX_COORD, cc.VERTEX_ATTRIB_TEX_COORDS);
//		program.link();
//		program.updateUniforms();
//		program.retain();
//		auto state = cc.GLProgramstate->create(program);
//		node.setGLProgramState(state)
//		state->setUniformFloat("u_grayscale", 1.0);
//		state->setUniformFloat("u_opacity", 1.0);
//		node._state = state;
//	}
//}
//
//void IngameUINew::setInteractionBall(int ballid){
//	if(ballid >=1 && ballid <= 15){
//		this->_bg._imgCollideBall.setVisible(true);
//		this->_bg._imgCollideBall.loadTexture('big_balls_'+(ballid)+'.png', ccui.Widget.PLIST_TEXTURE);
//	}
//	else{
//		this->_bg._imgCollideBall.setVisible(false);
//	}
//}
//
void IngameUINew::createCircleAvatarFromImgView(ImageView* imgView, int index){
	auto node = imgView;
	auto center = node->getPosition();
	node->setVisible(false);
	std::string fakeAvatarPath;
	switch(index){
		case 0: fakeAvatarPath = Res::INGAME_AVATAR_FAKE_1; break;
		case 1: fakeAvatarPath = Res::INGAME_AVATAR_FAKE_2; break;
		case 2: fakeAvatarPath = Res::INGAME_AVATAR_FAKE_3; break;
		case 3: fakeAvatarPath = Res::INGAME_AVATAR_FAKE_4; break;
	}
	auto realAvatar = Sprite::create(fakeAvatarPath);
	realAvatar->setScale(node->getContentSize().width / realAvatar->getContentSize().width);
	node->getParent()->addChild(realAvatar);
	realAvatar->setPosition(center);
	realAvatar->setLocalZOrder(0);
	realAvatar->setColor(Color3B(255,0,0));
	while (_avatarSprite.size() <= index) {
		_avatarSprite.push_back(nullptr);
	}
	this->_avatarSprite[index] = realAvatar;

	auto program = mShaderCache->_spriteUtils;
	auto state = GLProgramState::create(program);
	realAvatar->setGLProgramState(state);
	state->setUniformFloat("u_grayscale", 0);
	state->setUniformFloat("u_opacity", 1);
	state->setUniformFloat("u_circle", 1);
	state->setUniformFloat("u_width", realAvatar->getContentSize().width);
	state->setUniformFloat("u_height", realAvatar->getContentSize().height);
	state->setUniformFloat("u_radius", realAvatar->getContentSize().width*0.95/2);
	realAvatar->setGLProgramState(state);
}

void IngameUINew::initAvatarNodes(){
	_avatarSprite.clear();
	for(auto i=0;i<2;i++){
		Node *node = _pnAvatars->getChildByName("nodeP"+std::to_string(i+1));
		this->createCircleAvatarFromImgView((ImageView*)node->getChildByName("imgAvatar"), i);
		this->createCircleAvatarFromImgView((ImageView*)(node->getChildByName("nodeSub")->getChildByName("imgAvatar")), i + 2);
		node->getChildByName("nodeSub")->setLocalZOrder(3);
	}
	auto nodeP2 = _nodeP2;
	_nodeP2ShowPosition = nodeP2->getPosition();
	_nodeP2HidePosition = _nodeP1->getPosition();
	nodeP2->setPosition(_nodeP1->getPosition());
	nodeP2->setScaleX(1);
	nodeP2->getChildByName("imgBtnExt")->setScaleX(1);
}
//
//void IngameUINew::initPocketedBalls(){
//	this->pocketedBall = [[],[]];
//	auto size = this->_bg._EightBallPoolMode._pnSizeBallSlot.getContentSize();
//	for(auto i=0;i<2;i++){
//		auto arr = this->pocketedBall[i];
//		for(auto j=0;j<7;j++){
//			auto s = this->_bg._EightBallPoolMode._playerInfo["_nodeP"+(i+1)]._pocketedBall["_ball_"+j];
//			s.setTexture(res.BALLNUM_14);
//			gv.scaleToSize(s, size.width, size.height);
//			s.setVisible(false);
//			arr.push_back(s);
//		}
//	}
//}
//
//void IngameUINew::initTimeBar(){
//	this->timeBar = this->_bg._imgTimerBg._img;
//	this->timeBar.getParent().setPosition(cc.winSize.width/2, -10);
//}

void IngameUINew::initTimers(){
	for(auto i=0;i<2;i++){
		auto node = this->_pnAvatars->getChildByName("nodeP" + std::to_string(i + 1));
		node->getChildByName("border")->setLocalZOrder(1);
		Sprite *timerDummy = (Sprite *)node->getChildByName("timerDummy");
		timerDummy->setVisible(false);
		timerDummy->setFlippedX(true);
		timerDummy->setFlippedY(true);
		auto timer = ProgressTimer::create(timerDummy);
		timer->cleanup();
		timer->setMidpoint(Vec2(0.5, 0.5));
		timer->setType(ProgressTimer::Type::RADIAL);
		// timer.setReverseDirection(true);
		//_expProgress.setPosition(nodeExp.getPosition());
		node->addChild(timer, timerDummy->getLocalZOrder() +1, "timer");
		timer->setPosition(timerDummy->getPosition());
		// timer.flippedX =true;
		// timer.setRotation(180);
		timer->setPercentage(0);
		node->getChildByName("timeAlert")->setLocalZOrder(timer->getLocalZOrder()+1);
		this->_timers[i] = timer;
	}
}


//void IngameUINew::setDelegate(int delegate) {
//	this->_delegate = delegate;
//}
//
//void IngameUINew::onRulerMove(int delta) {
//	if (!this->_isMyTurn) return;
//	auto listener = this->_listeners.changeCueAngle;
//	if (listener) {
//		listener.handler.call(listener.context, delta);
//	}
//	// gv.gameMgr.interactionMgr.onChangeAngle(delta);
//}
//


//void IngameUINew::setCurMatchPlayerCount(int count){
//	this->_curMatchPlayerCount = count;
//	this->hud.onGameStart(count);
//}
//
//void IngameUINew::setPlayerInfo(double idx, int data){
//	// idx = matchIsndexToUIIndex(idx);
//	// utils.log("IDX", idx);
//	// const group = _bg._EightBallPoolMode._playerInfo["_nodeP"+(idx+1)];
//	// group._levelBox._lbLevel.setString(data.level || 20);
//	// group._lbName.setString(data.name);
//}
//
//void IngameUINew::getGoldLb(){
//	// return _bg._EightBallPoolMode._playerInfo._goldBox._lbGold;
//}
//
//void IngameUINew::getGoldPot(){
//	return this->_bg._EightBallPoolMode._playerInfo._goldBox._imgGold;
//}
//
//void IngameUINew::getPlayerAvatar(double idx){
//	// return _avatarSprite[idx];
//}
//
//void IngameUINew::setTimePercentRemain(double playerIdx, int percent){
//	playerIdx = this->matchIndexToTeamIndex(playerIdx);
//	this->_timer[playerIdx].setPercentage(percent);
//}
//
//void IngameUINew::setTimeRemain(double playerIdx, int time){
//	auto info = {realIndex: playerIdx};
//	this->hud.setActivePlayer(playerIdx, info);
//	playerIdx = this->matchIndexToTeamIndex(playerIdx);
//
//	auto percent = time/gv.GameConst.TURN_TIME*100;
//	this->_timer[playerIdx].setPercentage(percent);
//	if(time < 10 && this->timeInLastUpdate > 10){
//		cc.Device.vibrate(0.3);
//		this->timeBar.getParent().runAction(cc.moveTo(0.3, cc.winSize.width/2, 0));
//	}
//	else if(time > 10 && this->timeInLastUpdate < 10){
//		this->timeBar.getParent().runAction(cc.moveTo(0.3, cc.winSize.width/2, -10));
//	}
//	time = max(time, 0);
//	this->_timer[1-playerIdx].timeAlert.setVisible(false);
//	this->_timer[playerIdx].timeAlert.setVisible(time < 10);
//	this->_timer[playerIdx].timeAlert.setOpacity((time - floor(time))*(255-70) + 70);
//	this->_timer[playerIdx].timeAlert.setString(Math.ceil(time));
//	this->timeInLastUpdate = time;
//
//	this->timeBar.setScaleX(min(1,time / 10));
//	if(time > 5){
//		this->timeBar.setColor(utils.mix(Color3B(166, 160, 8), Color3B(8, 166, 33), (time-5)/5));
//	}
//	else{
//		this->timeBar.setColor(utils.mix(Color3B(166, 8, 8), Color3B(166, 160, 8), time/5));
//	}
//}
//
//void IngameUINew::setExtActivating(double playerIdx, int val, int isInExt){
//	if(val) utils.log("Set ext activating [Deprecated]"); return;
//	playerIdx = this->matchIndexToTeamIndex(playerIdx);
//	auto btn = this->_bg._pnAvatars["_nodeP"+(playerIdx + 1)]._imgBtnExt;
//	// utils.log("condition", val && !isInExt);
//	if(val && !isInExt){
//		btn._state->setUniformFloat('u_grayscale', 0);
//	}
//	else{
//		btn._state->setUniformFloat('u_grayscale', 1);
//	}
//}
//
//void IngameUINew::resetPocketedBall(){
//	// for(let i=0;i<2;i++){
//	// 	const arr = pocketedBall[i];
//	// 	for(let j=0;j<arr.length;j++)
//	// 		arr[j].setVisible(false);
//	// }
//}
//
//void IngameUINew::updateBallCollisionPoint(int percentX, int percentY){
//	auto btnBall = this->_ballIndicator;
//	btnBall._ballIndicator.setPosition(btnBall.width*percentX/100, btnBall.height*percentY/100);
//}
//
//void IngameUINew::setPocketedBall(int pId, int balls){
//	// const arr = pocketedBall[pId];
//	// for(let j=0;j<arr.length;j++){
//	// 	if(j >= balls.length) {
//	// 		arr[j].setVisible(false); continue;
//	// 	}
//	// 	arr[j].setVisible(true);
//	// 	arr[j].setTexture(res['BALLNUM_'+(balls[j].number ? balls[j].number : balls[j])]);
//	// }
//}
//
//
//void IngameUINew::setScoreVisible(int val){
//	// _bg._EightBallPoolMode._playerInfo._lbScore.setVisible(val);
//	// _bg._EightBallPoolMode._playerInfo._imgRound._lbRound.setVisible(val);
//}
//
//void IngameUINew::setRewardVisible(int val){
//	// _bg._EightBallPoolMode._playerInfo._goldBox.setVisible(val);
//}
//
//void IngameUINew::setScore(int p0Score, int p1Score){
//	utils.log("SET SCORE", p0Score, p1Score, this->hud.gameMode);
//	auto node;
//	auto half;
//	switch(this->hud.gameMode){
//		case gv.GameConst.GameMode.RT1:
//		case gv.GameConst.GameMode.S2:
//			return;
//		case gv.GameConst.GameMode.RT2:
//			node = this->_bg._pnAvatars._pnHud._imgGameModeRT2;
//			half = 2;
//			break;
//		case gv.GameConst.GameMode.RT3:
//			node = this->_bg._pnAvatars._pnHud._imgGameModeRT3;
//			half = 3;
//			break;
//		default: return;
//	}
//
//	this->hideAllTicks(node);
//	for(auto i=0;i<p0Score;i++){
//		node['_box'+i].tick.setVisible(true);
//	}
//	for(auto i=0;i<p1Score;i++){
//		node['_box'+(i+half)].tick.setVisible(true);
//	}
//	// _bg._EightBallPoolMode._playerInfo._lbScore.setString(p0Score + " : " + p1Score);
//	// _bg._EightBallPoolMode._playerInfo._imgRound._lbRound.setString("Round " + (p0Score + p1Score + 1));
//}
//
//void IngameUINew::hideAllTicks(int node){
//	for(auto i=0;i<node.getChildren().length;i++){
//		node.getChildren()[i].tick.setVisible(false);
//	}
//}
//
//void IngameUINew::setModeCompetitive(){
//	// setRewardVisible(false);
//	// setScoreVisible(true);
//}
//
//void IngameUINew::setModeGamble(){
//	// setRewardVisible(false);
//	// setScoreVisible(false);
//}
//
//void IngameUINew::setAngle(int angle){
//	// _bg._imgControlsLeft._lbAngle.setString(angle);
//}
//
//void IngameUINew::setModeAfterSelect(int pocket){
//	this->_bg._btnSelectPocket.setVisible(pocket);
//	this->_bg._btnConfirm.setVisible(false);
//
//	// _bg._imgControlsLeft.setVisible(true);
//	this->_bg._imgBallCueGroup.setVisible(true);
//	this->_bg._imgBgBtnCue.setVisible(true);
//	this->_bg._pnAvatars.setVisible(true);
//}
//
//void IngameUINew::setModeSelecting(int pocket){
//	// _bg._btnMoveBall.setVisible(false);
//	// _bg._btnSelectPocket.setVisible(false);
//	// _bg._btnConfirm.setVisible(true);
//
//	// // _bg._imgControlsLeft.setVisible(false);
//	// _bg._imgBallCueGroup.setVisible(false);
//	// _bg._imgBgBtnCue.setVisible(false);
//	// _bg._pnAvatars.setVisible(false);
//
//
//}
//
//// Chuyển từ idx trong trận: 12 chung team, 34 chung team => idx gui: 1 3 chung team, 2 4 chung team
//void IngameUINew::matchIndexToUIIndex(double idx){
//	auto team = floor(idx/this->_curMatchPlayerCount);
//	auto i = idx%this->_curMatchPlayerCount;
//	return this->_curMatchPlayerCount*i + team;
//}
//
//void IngameUINew::matchIndexToTeamIndex(double idx){
//	auto match = gv.gameMgr.ebpCtrl._curMatch;
//	if(!match) return 0;
//	return match.getCurGroupIdx(idx);
//}
//
//void IngameUINew::activePlayer(double idx){
//	auto info = {realIndex: idx};
//	utils.log("Active player", info);
//	idx = this->matchIndexToTeamIndex(idx);
//	this->hud.setActivePlayer(idx, info);
//	// _avatarSprite.forEach((e, i) => {
//	// 	e.getParent().setVisible(i === idx)
//	// });
//	// const nodeActive = _bg._EightBallPoolMode._playerInfo["_nodeP"+(idx+1)];
//	// nodeActive._imgBtnExt.setVisible(true);
//	// nodeActive._imgBtnExt._state->setUniformFloat("u_grayscale", 1);
//	// _timer[idx].setVisible(true);
//	// nodeActive.setOpacity(255);
//	// _avatarSprite[idx]._state->setUniformFloat("u_opacity", 1);
//	//
//	//
//	// for(let i=0;i<_avatarSprite.length;i++){
//	// 	if(i === idx) continue;
//	// 	const nodeDeactive = _bg._EightBallPoolMode._playerInfo["_nodeP"+(i+1)];
//	// 	nodeDeactive._imgBtnExt.setVisible(false);
//	// 	nodeDeactive._imgBtnExt._state->setUniformFloat("u_grayscale", 1);
//	// 	_timer[i].setVisible(false);
//	// 	nodeDeactive.setOpacity(190);
//	// 	_avatarSprite[i]._state->setUniformFloat("u_opacity", 0.01);
//	// }
//
//}
//
//void IngameUINew::onButtonTouchBegan(int sender, int tag){
//	
//}
//
//void IngameUINew::onButtonTouchMoved(int sender, int tag){
//	switch(tag){
//		case this->tagMap.btnBall:
//			this->submitBallTarget(sender.convertToNodeSpace(sender.getTouchMovePosition()), true);
//			// utils.log("touch moved", sender.convertToNodeSpace(sender.getTouchMovePosition()));
//			break;
//		case this->tagMap.btnAngle: {
//			auto listener = this->_listeners.changeCueAngle;
//			if (listener) {
//				auto y = sender.getTouchMovePosition().y;
//				listener.handler.call(listener.context, y - sender.lastY);
//				sender.lastY = y;
//			}
//			break;
//		}
//	}
//}
//
//void IngameUINew::onButtonTouchEnded(int sender, int tag){
//	switch(tag){
//		case this->tagMap.btnConfirm:
//			gv.gameMgr.interactionMgr.onSubmitBallOrPocket();
//			break;
//		case this->tagMap.btnMoveBall:
//			// gv.gameMgr.interactionMgr.enablePlacingMode();
//			break;
//		case this->tagMap.btnSelectPocket:
//			gv.gameMgr.interactionMgr.enableCallPocketMode();
//			break;
//		// case tagMap.btnCamera:
//
//		case this->tagMap.btnExpand:
//			this->hud.setModeFull(true);
//			break;
//		case this->tagMap.btnCue:
//			gv.inventoryMgr.useNextCue();
//			// gv.GuiMgr.getGui(Popup.COMPETITION_BAG_UI, CompetitionBagUI).show();
//			utils.log(gv.gameMgr.cue._cueInfo);
//			break;
//		case this->tagMap.btnAngle:
//			// _match.activeExt(sender);
//			sender.runAction(cc.scaleTo(0.2, 1));
//			this->toggleShowAngle();
//			break;
//		case this->tagMap.btnExt:
//			this->_match.activeExt(sender);
//			break;
//		case this->tagMap.btnMenu:
//			gv.GuiMgr.getGui(Popup.SETTING_UI, SettingUI).show();
//			break;
//		case this->tagMap.btnBall:
//			// if(!_ballTargetLayer){
//			// 	_ballTargetLayer = new MoveBallTargetLayer();
//			// 	addChild(_ballTargetLayer);
//			// }
//			// _ballTargetLayer.setShow(true);
//
//			// Update pos
//			if(gv.gameMgr.timeUtils.current() - sender.touchBeganTime < 500){
//				this->submitBallTarget(sender.convertToNodeSpace(sender.getTouchEndPosition()));
//			}
//			gv.gameMgr.interactionMgr.stopChangingCueOffset();
//			sender.runAction(cc.scaleTo(0.2, 1));
//			break;
//	}
//}
//
void IngameUINew::submitBallTarget(cocos2d::Vec2 pos, bool moving){
	auto ballIndicatorSize = this->_ballIndicator->getContentSize();
	auto center = Vec2(ballIndicatorSize.width/2, ballIndicatorSize.height/2);
	if(moving){
		auto d = Vec2(pos.x - _ballIndicatorData.beforePos.x, pos.y - _ballIndicatorData.beforePos.y);
		_ballIndicatorData.offset.x += d.x;
		_ballIndicatorData.offset.y += d.y;
		_ballIndicatorData.beforePos = pos;
	}
	else{
		_ballIndicatorData.offset = Vec2(pos.x - center.x, pos.y - center.y);
	}
	// Clamp
	// const p = _ballIndicator._ballIndicator.getPosition();
	auto r = _ballIndicatorData.offset;
	auto length = sqrt(r.x * r.x + r.y * r.y);
	auto maxWidth = ballIndicatorSize.width * 0.75 * 0.5;
	if(length > maxWidth){
		auto angle = acos(r.x/length);
		if(r.y < 0) angle = -angle;
		r.x = maxWidth * cos(angle);
		r.y = maxWidth * sin(angle);
	}

	auto h = r.x/maxWidth, v = r.y/maxWidth;
	//setBallIndicatorOffset(r, false);
	InteractionManager::getInstance()->setCueOffset(
		ps::ExtMath::vector(h * PhysicsConstants::BALL_RADIUS * 0.8, v * PhysicsConstants::BALL_RADIUS * 0.8)
	);
}

void IngameUINew::setBallIndicatorOffset(cocos2d::Vec2 offset, bool percent){
	auto ballIndicatorSize = _ballIndicator->getContentSize();
	auto center = Vec2(ballIndicatorSize.width/2, ballIndicatorSize.height/2);
	auto maxWidth = ballIndicatorSize.width * 0.75 * 0.5;
	if(percent){
		offset.x *= maxWidth * 1.0 / 0.8;
		offset.y *= maxWidth * 1.0 / 0.8;
	}
	_ballIndicatorData.offset = offset;
	this->_ballIndicatorTarget->setPosition(Vec2(center.x + offset.x, center.y + offset.y));
}
//
//void IngameUINew::addCameraViewCircle() {
//	// const angle = _bg._imgControlsLeft._lbAngle;
//	// angle.removeFromParent();
//	// const cameraCircle = new CameraCircle();
//	// cameraCircle.setPosition(105, 102);
//	// cameraCircle.setLbAngle(angle);
//	// angle.x = 40;
//	// angle.y = 20;
//	// addChild(cameraCircle);
//}
//
void IngameUINew::initPlacingBall() {
	this->_placingBallControl = PlacingBallControlUI::create();
	_placingBallControl->initNode(_imgPlaceBall);
	this->addChild(this->_placingBallControl);

	_btnMoveBall->setVisible(false);
	_btnSelectPocket->setVisible(false);
}

//void IngameUINew::enablePlacingBaenablePlacingBallll(int startPosition, int listener) {
//	this->_placingBallControl.(startPosition, listener);
//}
//
//void IngameUINew::setPlacingPosition(cocos2d::Vec3 position) {
//	this->_placingBallControl.setPlacingPosition(position);
//}
//

void IngameUINew::initTurnStates() 
{
	_stateObjects.push_back(_powerBar);
	_stateObjects.push_back(_imgBgBtnCue);
	_stateObjects.push_back(_imgBallCueGroup);
	_stateObjects.push_back(_btnBall);
	_stateObjects.push_back(_btnAngle);

	_stateOriginal.push_back(_powerBar->getPositionX());
	_stateOriginal.push_back(_imgBgBtnCue->getPositionX());
	_stateOriginal.push_back(_imgBallCueGroup->getPositionX());
	_stateOriginal.push_back(_btnBall->getPositionX());
	_stateOriginal.push_back(_btnAngle->getPositionX());

	_stateTransform.push_back(_powerBar->getPositionX() + 100);
	_stateTransform.push_back(_imgBgBtnCue->getPositionX() + 160);
	_stateTransform.push_back(_imgBallCueGroup->getPositionX() + 120);
	_stateTransform.push_back(_btnBall->getPositionX() - 120);
	_stateTransform.push_back(_btnAngle->getPositionX() - 100);

	this->_isMyTurn = false;
	for (int i = 0; i < _stateObjects.size(); i++) 
	{
		_stateObjects[i]->setPositionX(_stateTransform[i]);
	}
}

//void IngameUINew::toggleShowAngle() {
//	// if (!_isMyTurn) return;
//	// _showAngle = !_showAngle;
//	// const targetX = _showAngle ? states.original.ruler : states.transform.ruler;
//	// if (_showAngle) {
//	// 	states.objects.ruler.runAction(
//	// 		cc.moveTo(0.5, Vec2(targetX, states.objects.ruler.y)).easing(cc.easeOut(2)));
//	// } else {
//	// 	states.objects.ruler.runAction(
//	// 		cc.moveTo(0.5, Vec2(targetX, states.objects.ruler.y)).easing(cc.easeIn(2)));
//	// }
//}
//
//// Cam Modes
//void IngameUINew::setCamMode3DLayout() {
//	// utils.log("set cam mode 3d");
//	this->updateTurnLayout(true);
//}
//
//void IngameUINew::setCamMode2DLayout() {
//	// utils.log("set cam mode 2d");
//	this->updateTurnLayout(false);
//}
//
//void IngameUINew::setCamModeFreeLayout() {
//	// utils.log("set cam mode free");
//	this->updateTurnLayout(false);
//}
//
//// Register listener
//void IngameUINew::subscribeToggleCamMode(int context, int handler) {
//	this->_listeners.toggleCamMode = { context: context, handler: handler };
//}
//
//void IngameUINew::subscribeReleasePower(int context, int handler) {
//	this->_listeners.releasePower = { context: context, handler: handler };
//}
//
//void IngameUINew::subscribeChangeCueAngle(int context, int handler) {
//	this->_listeners.changeCueAngle = { context: context, handler: handler };
//}
//});
//
//auto PlacingBallControl = cc.Node.extend({
//IngameUINew::IngameUINew(int control) {
//	this->_super();
//	this->_control = control;
//	this->initControl();
//	this->initDrawNode();
//	this->colorize(false);
//	this->disablePlacingBall();
//	this->_lockExternal = false;
//	// _lerp = new gv.LerpUtils.VectorLerp(0.125, this, handleChange);
//	this->scheduleUpdate();
//}
//
//void IngameUINew::initDrawNode(){
//	this->dn = new cc.DrawNode();
//	this->addChild(this->dn);
//}
//
//void IngameUINew::update(double dt) {
//	// _lerp.update(dt);
//	if(this->_position && gv.gameMgr.timeUtils.current() - this->_lastUpdate > 500){
//		this->move(this->_position);
//	}
//
//	this->dn.clear();
//	if(this->_listener && this->_control.getParent().isVisible()){
//		auto pos = gv.ExtensionMath.vector.clone(gv.gameMgr.cueBall.body().position);
//		pos.z += gv.PhysicsConstants.BALL_RADIUS;
//		auto cueBallPos = gv.gameMgr.interactionMgr._camera.worldPositionToWindowLocation(pos);
//		auto color = this->_position ? Color3B(0, 255, 243) : Color3B(177, 177, 177);
//
//		auto ctrlPos = this->_control.getPosition();
//		auto vector = Vec2(cueBallPos.x - ctrlPos.x, cueBallPos.y - ctrlPos.y);
//		auto multiplier = (this->_control.width/4)/sqrt(vector.x*vector.x + vector.y*vector.y);
//		this->dn.drawLine(Vec2(ctrlPos.x + vector.x * multiplier, ctrlPos.y + vector.y * multiplier), cueBallPos, color);
//	}
//}
//
//void IngameUINew::initControl() {
//	this->_control.addTouchEventListener(this->touchListener, this);
//
//	utils.grayNode(this->_control);
//}
//
//void IngameUINew::touchListener(int sender, int type) {
//	auto position = null;
//	switch (type) {
//		case ccui.Widget.TOUCH_BEGAN:
//			this->_lockExternal = true;
//			position = sender.getTouchBeganPosition();
//			this->colorize(true);
//			this->_position = position;
//			this->_lastUpdate = gv.gameMgr.timeUtils.current();
//			break;
//		case ccui.Widget.TOUCH_MOVED:
//			position = sender.getTouchMovePosition();
//			this->move(position);
//			break;
//		case ccui.Widget.TOUCH_ENDED:
//		case ccui.Widget.TOUCH_CANCELED:
//			position = sender.getTouchEndPosition();
//			this->_position = null;
//			this->_lockExternal = false;
//			this->end();
//			this->colorize(false);
//			break;
//	}
//}
//
//void IngameUINew::colorize(int val){
//	// _control.setColor(val ? cc.color.WHITE: cc.color.RED);
//	// _control.getChildren().forEach(e => e.setColor(val ? cc.color.WHITE:cc.color.RED));
//	// _control.setOpacity(val ? 200 : 100);
//	// _control.getChildren().forEach(e => e.setOpacity(val ? 200 : 100));
//
//	if(val){
//		utils.normalNode(this->_control);
//	}
//	else{
//		utils.grayNode(this->_control);
//	}
//}
//
//void IngameUINew::move(cocos2d::Vec3 position) {
//	if (!this->_position || !position) return;
//
//	auto delta = Vec2(position.x - this->_position.x, position.y - this->_position.y);
//	this->_control.x += delta.x;
//	this->_control.y += delta.y;
//	this->_position = position;
//
//	if (this->_listener) {
//		this->_listener.call(null, position);
//	}
//	this->_lastUpdate = gv.gameMgr.timeUtils.current();
//}
//
//void IngameUINew::end() {
//	if (this->_listener) {
//		this->_listener.call(null, null);
//	}
//}
//
//void IngameUINew::enablePlacingBall(int startPosition, int listener) {
//	this->_listener = listener;
//	this->_control.setPosition(startPosition);
//	this->_control.setVisible(true);
//}
//
//void IngameUINew::setPlacingPosition(cocos2d::Vec3 position) {
//	if (this->_lockExternal) return;
//	this->_control.setPosition(position);
//}
//
//void IngameUINew::disablePlacingBall() {
//	this->_control.setVisible(false);
//	this->_listener = null;
//}
//});
//

void IngameUINew::initAngleNode()
{
	auto btnAngle = _btnAngle;
	auto btnBall = _btnBall;
	auto btnBallSize = btnBall->getContentSize();
	auto distance = btnAngle->getPositionY() - 
		(btnBall->getPositionY() + btnBallSize.height * btnBall->getScaleY());
	btnAngle->setPositionY(btnBall->getPositionY() + btnBallSize.height * btnBall->getScaleY() + distance);

	auto spriteMask = Sprite::createWithSpriteFrameName("Mask.v4.png");
	spriteMask->setAnchorPoint(Vec2(0, 0));
	spriteMask->setPosition(0, 0);
	auto btnAngleSize = btnAngle->getContentSize();
	UIFactory::scaleToSize(spriteMask, btnAngleSize.width, btnAngleSize.height);
	
	auto cn = ClippingNode::create();
	btnAngle->addChild(cn);
	cn->setStencil(spriteMask);
	_imgAngleCue->removeFromParent();
	cn->addChild(_imgAngleCue);
}

void IngameUINew::onEnter()
{
	BaseLayer::onEnter();
	
	schedule(schedule_selector(IngameUINew::updateAngle), 0.1f);
}

void IngameUINew::updateAngle(float dt)
{
	auto angle = interactionMgr->_cueRPT.t / M_PI * 180;
	_imgAngleCue->setRotation(-angle);
}

void IngameUINew::onButtonTouched(cocos2d::ui::Button* button, int id)
{
	switch (id) {
		//		case this->tagMap.btnCamera:
		//			if (this->_listeners.toggleCamMode) {
		//				auto listener = this->_listeners.toggleCamMode;
		//				auto mode = listener.handler.call(listener.context, this->_isMyTurn);
		//				this->updateTurnLayout(mode == gv.core.InteractionManager.CamModes.MODE_3D);
		//				// gv.gameMgr.interactionMgr.toggleCamMode();
		//			}
		//			break;
		//			// _delegate.dispatchEvent(300);
		//			// break;
	case IngameUINew::TAG_BTN_BALL:			
		button->runAction(ScaleTo::create(0.2, 1.99));
		break;
	case IngameUINew::TAG_BTN_ANGLE:
		button->runAction(ScaleTo::create(0.2, 1.99));
		break;
			}
}

void IngameUINew::onButtonTouchedMove(cocos2d::ui::Button * button, int id)
{
	switch (id)
	{
		case IngameUINew::TAG_BTN_BALL: 
		{
			Vec2 pos = button->getTouchMovePosition();
			Vec2 pos2 = button->convertToNodeSpace(pos);
			submitBallTarget(pos2, true);
			break;
		}
		case IngameUINew::TAG_BTN_ANGLE:
		{
			auto y = button->getTouchMovePosition().y;
			_changeCueAngleCallBack(y - _changeCueAngleData.lastY);
			_changeCueAngleData.lastY = y;
			break;
		}
			
	default:
		break;
	}
}

void IngameUINew::onButtonRelease(cocos2d::ui::Button * button, int id)
{
	switch(id){
		case IngameUINew::TAG_BTN_BALL:
			_ballIndicatorData.beforePos = button->getTouchStartPos();
			button->runAction(ScaleTo::create(0.2, 1));
			break;
		case IngameUINew::TAG_BTN_ANGLE:
		{
			button->runAction(ScaleTo::create(0.2, 1));
			break;
		}
	}
	
}

void IngameUINew::onButtonCanceled(cocos2d::ui::Button * button, int id)
{
	switch (id) {
	case IngameUINew::TAG_BTN_BALL:
		_ballIndicatorData.beforePos = button->getTouchStartPos();
		button->runAction(ScaleTo::create(0.2, 1));
		break;
	case IngameUINew::TAG_BTN_ANGLE:
	{
		button->runAction(ScaleTo::create(0.2, 1));
		break;
	}
	}
}

void IngameUINew::onCustomTouchEventHandler(cocos2d::Ref * pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Button* sender = (Button*)pSender;
	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		onButtonTouched(sender, sender->getTag());
	case ui::Widget::TouchEventType::MOVED:
		onButtonTouchedMove(sender, sender->getTag());
		break;
	case ui::Widget::TouchEventType::ENDED:
		onButtonRelease(sender, sender->getTag());
		break;
	case ui::Widget::TouchEventType::CANCELED:
		onButtonCanceled(sender, sender->getTag());
		break;
	}
}

void IngameUINew::initRulerAndPowerBar(){
	CCLOG("Init Power Bar");
	auto winSize = Director::getInstance()->getVisibleSize();

	auto powerBar = PowerUI::create();
	powerBar->setPosition(Vec2(winSize.width - 21, winSize.height / 2 + 43));
	// powerBar.subscribe(onPowerBarRelease.bind(this));
	PowerUIListener *listener = new PowerUIListener();
	listener->onChange = CC_CALLBACK_1(IngameUINew::onPowerBarChange, this);
	listener->onRelease = CC_CALLBACK_1(IngameUINew::onPowerBarRelease, this);
	powerBar->setListener(listener);
	powerBar->setLocalZOrder(-1);
	this->addChild(powerBar);
	_powerBar = powerBar;
}

void IngameUINew::onPowerBarChange(float percent) {
	/*if (!this->_isMyTurn) return;
	if (!this->_match.canShoot()) return;*/
	auto force = gameMgr->onChangePower(percent);
	_powerBar->setForceValue(force);
}

void IngameUINew::subscribeChangeCueAngle(const ChangeCueAngleCallBack & callback)
{
	_changeCueAngleCallBack = callback;
}

IngameHUD * IngameUINew::getHUD()
{
	return _hud;
}

PowerUI * IngameUINew::getPowerUI()
{
	return _powerBar;
}

cocos2d::Vec2 IngameUINew::getNodeP2ShowPosition()
{
	return _nodeP2ShowPosition;
}

cocos2d::Vec2 IngameUINew::getNodeP2HidePosition()
{
	return _nodeP2HidePosition;
}

void IngameUINew::disablePlacingBall()
{
	_placingBallControl->disablePlacingBall();
}

void IngameUINew::setMyTurnLayout()
{
	if (_isMyTurn) return;
	_isMyTurn = true;
	updateTurnLayout(_isMyTurn);
}

void IngameUINew::setEnemyTurnLayout()
{
	if (!_isMyTurn) return;
	_isMyTurn = false;
	updateTurnLayout(_isMyTurn);
}

void IngameUINew::updateTurnLayout(bool isMyTurn)
{
	bool myTurn = isMyTurn;
	for (int i = 0; i < _stateObjects.size(); i ++) {
		auto object = _stateObjects[i];
		auto targetY = object->getPositionY();
		auto targetX = myTurn ? _stateOriginal[i] : _stateTransform[i];
		object->stopAllActions();
		if (myTurn) {
			object->runAction(
				EaseOut::create(
					MoveTo::create(0.5, Vec2(targetX, targetY)),
					2)
				);
		}
		else {
			object->runAction(EaseIn::create(
				MoveTo::create(0.5, Vec2(targetX, targetY)), 
				2)
			);
		}
	}

	if (myTurn) {
		_nodePlaceBall->setVisible(true);
		_imgCollideBall->setOpacity(255);
	}
	else {
		_nodePlaceBall->setVisible(false);
		_imgCollideBall->setOpacity(0);
	}
}

void IngameUINew::enablePlacingBall(Vec2 startPosition, const PlacingBallControlUI::PlaceBallCallBack & listener)
{
	_placingBallControl->enablePlacingBall(startPosition, listener);
}

void IngameUINew::setPlacingPosition(cocos2d::Vec2 position)
{
	_placingBallControl->setPlacingPosition(position);
}

void IngameUINew::setCamMode3DLayout()
{
	updateTurnLayout(true);
}

void IngameUINew::setCamMode2DLayout()
{
	updateTurnLayout(false);
}

void IngameUINew::setCamModeFreeLayout()
{
	updateTurnLayout(false);
}

void IngameUINew::onPowerBarRelease(float percent) {
	interactionMgr->handleShoot(percent);
}

