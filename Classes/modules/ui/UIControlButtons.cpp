#include "UIControlButtons.h"

//	 UIControlButtons::UIControlButtons() {
//		this->_uiVisible = true;
//		this->_show = false;
//
//		this->_ingameLayer = new IngameUINew();
//		this->_ingameLayer.activeExt = this->activeExt.bind(this);
//		this->addChild(this->_ingameLayer);
//		this->initButtons();
//		// this.initSliders();
//		// this.initMatch();
//		// this.toggleUI();
//
//
//		// Uncomment ở chỗ này
//		this->scheduleUpdate();
//	}
//
//	void UIControlButtons::initMatch(int match){
//		cc.log("new match")
//		this->_match = match;
//		this->_ingameLayer._match = match;
//
//		auto playerCount = this->_match.getPlayersCount();
//		for(auto i=0;i<this->_ingameLayer._avatarSprite.length;i++){
//			this->_ingameLayer._avatarSprite[i].getParent().setVisible(i < playerCount);
//		}
//		// this._ingameLayer.setCurMatchPlayerCount(playerCount/2);
//	}
//
//	void UIControlButtons::show(){
//		this->_show = true;
//		this->setVisible(true);
//	}
//
//	void UIControlButtons::hide(){
//		this->_show = false;
//		this->setVisible(false);
//	}
//
//	void UIControlButtons::setDelegate(int delegate) {
//		this->_delegate = delegate;
//		this->_ingameLayer.setDelegate(delegate);
//	}
//
//	void UIControlButtons::initButtons() {
//		// const left = gv.commonButton(100, 50, 75, 50, "Left");
//		// const right = gv.commonButton(100, 50, 185, 50, "Right");
//		// const up = gv.commonButton(100, 50, 75, 110, "Up");
//		// const down = gv.commonButton(100, 50, 185, 110, "Down");
//
//		// const shoot = gv.commonButton(100, 50, cc.winSize.width - 220, 50, "End!");
//		// const hide = gv.commonButton(100, 50, shoot.x - 120, 50, "Hide");
//		// const submitBallPos = gv.commonButton(100, 50, cc.winSize.width - 220, 50, "SET");
//		// const debug = gv.commonButton(100, 50, submitBallPos.x - 120, 50, "debug");
//		// const setBall = gv.commonButton(120, 50, debug.x - 120, 50, "set ball");
//		// const choosePocket = gv.commonButton(100, 50, submitBallPos.x, 120, "POCKET");
//
//		// const RotateLeft = gv.commonButton(50, 50, 50, cc.winSize.height - 50, "←");
//		// const RotateRight = gv.commonButton(50, 50, 110, cc.winSize.height - 50, "→");
//		// const Up = gv.commonButton(50, 50, 50, cc.winSize.height - 110, "↑");
//		// const Down = gv.commonButton(50, 50, 110, cc.winSize.height - 110, "↓");
//		// const ZoomIn = gv.commonButton(50, 50, 50, cc.winSize.height - 170, "+");
//		// const ZoomOut = gv.commonButton(50, 50, 110, cc.winSize.height - 170, "-");
//		// const showDebugNotify = gv.commonButton(100, 50, 110, cc.winSize.height - 110, 'log');
//		//
//		// const addL = gv.commonButton(100, 50, 195, cc.winSize.height - 50, "addL");
//		// const rmL = gv.commonButton(100, 50, 195, cc.winSize.height - 110, "rmL");
//		//
//		//
//		// const reload = gv.commonButton(100, 50, cc.winSize.width - 160, cc.winSize.height - 135, "Reload");
//		// const toggleAccel = gv.commonButton(100, 50, cc.winSize.width - 160, cc.winSize.height - 80, "Accel");
//		// const restore = gv.commonButton(100, 50, cc.winSize.width - 480, cc.winSize.height - 25, "Restore");
//		// const run = gv.commonButton(100, 50, cc.winSize.width - 370, cc.winSize.height - 25, "Run");
//		// const save = gv.commonButton(100, 50, cc.winSize.width - 155, cc.winSize.height - 25, "Save");
//		// const load = gv.commonButton(100, 50, cc.winSize.width - 260, cc.winSize.height - 25, "Load");
//		// const pause = gv.commonButton(100, 50, cc.winSize.width - 595, cc.winSize.height - 25, "Pause");
//		// const top = gv.commonButton(100, 50, pause.x - 120, pause.y, "Top");
//		//
//		// const tableShaderPart = new ccui.TextField();
//		// tableShaderPart.setPosition(cc.winSize.width * 3/5 - 150, cc.winSize.height - 120);
//		// tableShaderPart.setContentSize(cc.size(100,50));
//		// tableShaderPart.setTag(300);
//		// tableShaderPart.addEventListener(this.onTextFieldChange.bind(this), this);
//		//
//		// const tableShaderUniformName = new ccui.TextField();
//		// tableShaderUniformName.setPosition(cc.winSize.width * 3/5, cc.winSize.height - 120);
//		// tableShaderUniformName.setContentSize(cc.size(100,50));
//		// tableShaderUniformName.setTag(301);
//		// tableShaderUniformName.addEventListener(this.onTextFieldChange.bind(this), this);
//		//
//		// const tableShaderVal = new ccui.TextField();
//		// tableShaderVal.setPosition(cc.winSize.width * 3/5 + 150, cc.winSize.height - 120);
//		// tableShaderVal.setContentSize(cc.size(100,50));
//		// tableShaderVal.setTag(302);
//		// tableShaderVal.addEventListener(this.onTextFieldChange.bind(this), this);
//
//		this->_ballSelectButtons = [];
//		for(auto i = 0;i<=15;i++){
//			auto b = gv.commonButton(40, 40, 70 + i*50, 120, i);
//			this->addChild(b);
//			b.setTag(3000 + i);
//			b.setVisible(false);
//			this->_ballSelectButtons.push_back(b);
//			b.addTouchEventListener(this->handleEvent, this);
//		}
//
//		//
//		// left.setTag(1);
//		// right.setTag(2);
//		// up.setTag(10);
//		// down.setTag(11);
//		// shoot.setTag(3);
//		// throwRandomBall.setTag(21);
//		// RotateLeft.setTag(4);
//		// RotateRight.setTag(5);
//		// ZoomIn.setTag(6);
//		// ZoomOut.setTag(7);
//		// Up.setTag(8);
//		// Down.setTag(9);
//		// hide.setTag(12);
//		// reload.setTag(13);
//		// toggleAccel.setTag(22);
//		// restore.setTag(14);
//		// save.setTag(15);
//		// load.setTag(16);
//		// run.setTag(17);
//		// pause.setTag(18);
//		// top.setTag(19);
//		// submitBallPos.setTag(23);
//		// addL.setTag(30);
//		// rmL.setTag(31);
//		// debug.setTag(32);
//		// setBall.setTag(33);
//		// choosePocket.setTag(34);
//		// showDebugNotify.setTag(35);
//
//		// left.addTouchEventListener(this.handleEvent, this);
//		// right.addTouchEventListener(this.handleEvent, this);
//		// up.addTouchEventListener(this.handleEvent, this);
//		// down.addTouchEventListener(this.handleEvent, this);
//		// shoot.addTouchEventListener(this.handleEvent, this);
//		// throwRandomBall.addTouchEventListener(this.handleEvent, this);
//		// RotateLeft.addTouchEventListener(this.handleEvent, this);
//		// RotateRight.addTouchEventListener(this.handleEvent, this);
//		// ZoomIn.addTouchEventListener(this.handleEvent, this);
//		// ZoomOut.addTouchEventListener(this.handleEvent, this);
//		// Up.addTouchEventListener(this.handleEvent, this);
//		// Down.addTouchEventListener(this.handleEvent, this);
//		// hide.addTouchEventListener(this.handleEvent, this);
//		// reload.addTouchEventListener(this.handleEvent, this);
//		// restore.addTouchEventListener(this.handleEvent, this);
//		// toggleAccel.addTouchEventListener(this.handleEvent, this);
//		// save.addTouchEventListener(this.handleEvent, this);
//		// load.addTouchEventListener(this.handleEvent, this);
//		// run.addTouchEventListener(this.handleEvent, this);
//		// pause.addTouchEventListener(this.handleEvent, this);
//		// top.addTouchEventListener(this.handleEvent, this);
//		// submitBallPos.addTouchEventListener(this.handleEvent, this);
//		// addL.addTouchEventListener(this.handleEvent, this);
//		// rmL.addTouchEventListener(this.handleEvent, this);
//		// debug.addTouchEventListener(this.handleEvent, this);
//		// setBall.addTouchEventListener(this.handleEvent, this);
//		// choosePocket.addTouchEventListener(this.handleEvent, this);
//		// showDebugNotify.addTouchEventListener(this.handleEvent, this);
//
//		// this._hide = hide;
//		// this._uis = [
//		// 	ZoomIn, ZoomOut, reload, restore, save, load, run, pause, top, toggleAccel, showDebugNotify
//		// ];
//
//		// this.addChild(left);
//		// this.addChild(right);
//		// this.addChild(up);
//		// this.addChild(down);
//		// this.addChild(shoot);
//		// this.addChild(throwRandomBall);
//		// this.addChild(RotateLeft);
//		// this.addChild(RotateRight);
//		// this.addChild(ZoomIn);
//		// this.addChild(ZoomOut);
//		// this.addChild(Up);
//		// this.addChild(Down);
//		// this.addChild(hide);
//		// this.addChild(reload);
//		// this.addChild(toggleAccel);
//		// this.addChild(restore);
//		// this.addChild(save);
//		// this.addChild(load);
//		// this.addChild(run);
//		// this.addChild(pause);
//		// this.addChild(top);
//		// this.addChild(addL);
//		// this.addChild(rmL);
//		// this.addChild(submitBallPos);
//		// this.addChild(debug);
//		// this.addChild(setBall);
//		// this.addChild(choosePocket);
//		// this.addChild(showDebugNotify);
//
//		// this._choosePocket = choosePocket;
//		// choosePocket.setVisible(false);
//		// this._debugBallButton = setBall;
//		// this.addChild(tableShaderPart);
//		// this.addChild(tableShaderUniformName);
//		// this.addChild(tableShaderVal);
//
//		// submitBallPos.setVisible(false);
//		// this._submitPosBtn = submitBallPos;
//
//		// this.initDebugDialog();
//	}
//
//	void UIControlButtons::initDebugDialog(){
//		this->_debugDialog = new IngameDebugUI();
//		this->addChild(this->_debugDialog);
//
//		this->_debugDialog.setDelegate(this);
//		this->_debugDialog.setPosition(cc.winSize.width/2, cc.winSize.height/2);
//		this->_debugDialog.setVisible(false);
//
//		auto show = gv.commonButton(100, 50, cc.winSize.width - 595, cc.winSize.height - 75, "dialog");
//		this->addChild(show);
//		show.setVisible(false);
//		this->_uis.push_back(show);
//		void UIControlButtons::(int function(sender, int type){
//			 if(type == ccui.Widget.TOUCH_BEGAN) {
//				 this->_debugDialog.setVisible(!this->_debugDialog.isVisible());
//				 if(this->_debugDialog.isVisible()){
//				 	gv.gameMgr.setDebugMode(true)
//				 }
//			 }
//		}.bind(this), this);
//	}
//
//	void UIControlButtons::shootDebug(int force, int direction, int offset){
//		this->endDebug();
//		if(this->_match.canShoot()){
//			gv.gameMgr.shootDebug(force, direction, offset);
//		}
//		else if(this->_match.canSetBallPosition()){
//			gv.gameMgr.debugModule.setBallPositionDebugByDialog(direction.x, direction.y);
//		}
//	}
//
//	void UIControlButtons::endDebug(){
//		gv.gameMgr.setDebugMode(false);
//	}
//
//	void UIControlButtons::setChoosePocketMode(int val){
//		this->_choosePocket.setVisible(val);
//	}
//
//	void UIControlButtons::onPowerBarRelease(int percent) {
//		// this._delegate.percent = percent;
//		// this._delegate.shoot();
//		if(this->_match.canShoot()){
//			gv.gameMgr.shoot(percent);
//		}
//	}
//
//	void UIControlButtons::stopCharging(){
//		return this->getPowerBarValue();
//	}
//
//	void UIControlButtons::getPowerBarValue(){
//		if(this->_uiVisible)
//			return this->_powerSlider.getPercent()/100;
//		return this->_powerBar.val;
//	}
//
//	// Reset timer
//	// Show/hide timer views of players
//	// Set callback after the end of a player turn
//
//	void UIControlButtons::setPocketedBall(int pId, int balls){
//		this->_ingameLayer.setPocketedBall(pId, balls);
//	}
//
//	void UIControlButtons::resetPocketedBall(){
//		this->_ingameLayer.resetPocketedBall();
//	}
//
//	void UIControlButtons::setScore(int p1, int p2){
//		this->_ingameLayer.setScore(p1, p2);
//	}
//
//	void UIControlButtons::setBallPosition(){
//		// this._submitPosBtn.setVisible(true);
//		// this.pauseTimer();
//	}
//
//	void UIControlButtons::finishSetBallPosition(){
//		// this._submitPosBtn.setVisible(false);
//		// this.resumeTimer();
//	}
//
//	void UIControlButtons::setBtnSubmitPlaceBallVisible(int visibility){
//		// this._submitPosBtn.setVisible(visibility);
//		// this._ingameLayer.setModeAfterSelect(true, false);
//	}
//
//	void UIControlButtons::resumeTimer(){
//		this->_pauseTimer = false;
//	}
//
//	void UIControlButtons::pauseTimer(){
//		this->_pauseTimer = true;
//	}
//
//	void UIControlButtons::startMatch(int match){
//		this->initMatch(match);
//		this->resetPocketedBall();
//		this->totalTime = 0;
//
//		auto mode = gv.core.MatchWrapper.Mode;
//		// TODO: add more flexible code like setMode()... or similar
//		utils.log("match mode", this->_match._matchMode & mode.STREAK_2, mode.checkMode(this->_match._matchMode, mode.STREAK_2), this->_match._matchMode & mode.GAMBLE, mode.checkMode(this->_match._matchMode, mode.GAMBLE));
//		if(mode.checkMode(this->_match._matchMode, mode.GAMBLE) || mode.checkMode(this->_match._matchMode, mode.STREAK_2)){
//			// Gamble
//			this->_ingameLayer.setModeGamble();
//		}
//		else{
//			//Competitive
//			this->_ingameLayer.setModeCompetitive();
//		}
//	}
//
//	// _debugStart: function(playerIdx){
//	// 	this._match.start();
//	// 	this._match.startTurnPlayer(playerIdx);
//	// },
//
//	void UIControlButtons::update(double dt){
//		// return;
//		if(!this->_match || !this->_show) return;
//
//		if(this->_turn != this->_match.getTurn()){
//			this->_turn = this->_match.getTurn();
//			this->_curPlayerIdx = this->_match.getCurPlayerIdx();
//			// utils.log("CUR PLAYER IDX", this._curPlayerIdx);
//			// this.deactivePlayer(1-this._curPlayerIdx);
//			this->activePlayer(this->_curPlayerIdx);
//		}
//
//		if(this->_delegate._table._cue._pitch){
//			auto degree = Math.round(this->_delegate._table._cue._pitch * 180 / ExtMath::PI);
//			this->_ingameLayer.setAngle(degree);
//		}
//
//		if(this->_pauseTimer) return;
//
//		this->totalTime += dt;
//		this->_match.update(dt);
//
//		// utils.log("set time remain", this._curPlayerIdx, this._match.getTimeRemain());
//		this->setTimeRemain(this->_curPlayerIdx, this->_match.getTimeRemain());
//		// this.setTimePercentRemain(this._curPlayerIdx, this._match.getTimePercent());
//		this->_ingameLayer.setExtActivating(this->_curPlayerIdx, this->_match.isExtActivating(), this->_match.isInExt() && (this->totalTime % 1 < 0.5));
//	}
//
//	void UIControlButtons::deactivePlayer(double idx){
//		if(idx == 0){
//			this->_leftExt.setVisible(false);
//			this->_timerLeft.setVisible(false);
//			this->_avatarLeft.setTexture(res.INGAME_AVATAR_3);
//			this->_levelLeft.setTexture(res.INGAME_LEVEL_BOX_02);
//		}
//		else{
//			this->_rightExt.setVisible(false);
//			this->_timerRight.setVisible(false);
//			this->_avatarRight.setTexture(res.INGAME_AVATAR_3);
//			this->_levelRight.setTexture(res.INGAME_LEVEL_BOX_02);
//		}
//	}
//
//	void UIControlButtons::activePlayer(double idx){
//		this->_ingameLayer.activePlayer(idx);
//	}
//
//	void UIControlButtons::setTimePercentRemain(double playerIdx, int percent){
//		this->_ingameLayer.setTimePercentRemain(playerIdx, percent);
//	}
//
//
//	void UIControlButtons::setTimeRemain(double playerIdx, int timeRemain){
//		this->_ingameLayer.setTimeRemain(playerIdx, timeRemain);
//	}
//
//	void UIControlButtons::activeExt(int btn){
//		if (this->_match.activeExt()){
//			// btn.setBright(true);
//		} else {
//			cc.log("Ext already activated");
//		}
//	}
//	void UIControlButtons::initSliders() {
//		auto slider = this->createSlider();
//		slider.setRotation(0);
//		slider.setTag(1000);
//		slider.setPercent(50);
//		slider.x = cc.winSize.width / 2;
//		slider.y = cc.winSize.height - 30;
//		slider.addEventListener(this->sliderEvent, this);
//		this->addChild(slider);
//
//		this->fovText = gv.commonText("45", slider.x, slider.y - 30);
//		this->addChild(this->fovText);
//
//		slider = this->createSlider();
//		slider.setRotation(0);
//		slider.setTag(1001);
//		slider.setPercent(0);
//		slider.x = cc.winSize.width / 2;
//		slider.y = cc.winSize.height - 50;
//		slider.addEventListener(this->sliderEvent, this);
//		this->addChild(slider);
//
//		// this._uis.push(slider);
//	//
//	// 	let slider = this.createSlider();
//	// 	slider.setTag(1001);
//	// 	slider.x = 30;
//	// 	slider.y = cc.winSize.height / 2 - 20;
//	// 	slider.setPercent(50);
//	// 	slider.addEventListener(this.sliderEvent, this);
//	// 	this.addChild(slider);
//	// 	this._uis.push(slider);
//	//
//	// 	slider = this.createSlider();
//	// 	slider.setTag(1002);
//	// 	slider.setPercent(50);
//	// 	slider.x = 60;
//	// 	slider.y = cc.winSize.height / 2 - 20;
//	// 	slider.addEventListener(this.sliderEvent, this);
//	// 	this.addChild(slider);
//	// 	this._uis.push(slider);
//	//
//	// 	slider = this.createSlider();
//	// 	slider.setTag(1005);
//	// 	slider.setPercent(10);
//	// 	slider.x = 90;
//	// 	slider.y = cc.winSize.height / 2 - 20;
//	// 	slider.addEventListener(this.sliderEvent, this);
//	// 	this.addChild(slider);
//	// 	this._uis.push(slider);
//	//
//	// 	// slider = this.createSlider();
//	// 	// slider.setTag(1003);
//	// 	// slider.x = cc.winSize.width - 60;
//	// 	// slider.y = cc.winSize.height / 2;
//	// 	// slider.addEventListener(this.sliderEvent, this);
//	// 	// slider.setPercent(100);
//	// 	// this.addChild(slider);
//	// 	//
//	// 	// slider = this.createSlider();
//	// 	// slider.setTag(1004);
//	// 	// slider.x = cc.winSize.width - 100;
//	// 	// slider.y = cc.winSize.height / 2;
//	// 	// slider.addEventListener(this.sliderEvent, this);
//	// 	// slider.setPercent(100);
//	// 	// this.addChild(slider);
//	}
//
//	void UIControlButtons::createSlider() {
//		auto slider = new ccui.Slider();
//		slider.setRotation(-90);
//		slider.setTouchEnabled(true);
//		slider.loadBarTexture("res/ui/sliderTrack.png");
//		slider.loadSlidBallTextures("res/ui/sliderThumb.png", "res/ui/sliderThumb.png", "");
//		slider.loadProgressBarTexture("res/ui/sliderProgress.png");
//		return slider;
//	}
//
//	void UIControlButtons::initUIBugList() {
//		auto uiBugList = new UIBugList();
//		this->addChild(uiBugList);
//	}
//
//	void UIControlButtons::setPlayerInfo(double idx, int data){
//		this->_ingameLayer.setPlayerInfo(idx, data);
//	}
//
//	void UIControlButtons::setBallIndicatorOffset(int offset, int percent){
//		this->_ingameLayer.setBallIndicatorOffset(offset, percent);
//	}
//
//	void UIControlButtons::toggleUI() {
//		this->_uiVisible = !this->_uiVisible;
//		this->_uis.forEach(ui => {
//			if (cc.sys.isObjectValid(ui)) {
//				ui.setVisible(this->_uiVisible)
//			}
//		});
//		this->_hide.setTitleText(this->_uiVisible ? "Hide" : "Show");
//	}
//
//	void UIControlButtons::onTextFieldChange(int sender, int type){
//		// cc.log("sender.getTag: " + sender.getTag());
//		switch(sender.getTag()){
//			case 300:
//				cc.log("part name: " + sender.getString());
//				break;
//			case 301:
//				cc.log("uniform name: " + sender.getString());
//				break;
//			case 302:
//				cc.log("val: " + sender.getString() + " " + parseFloat(sender.getString()));
//				auto part = this->getChildByTag(300).getString();
//				auto uniform = this->getChildByTag(301).getString();
//				MainLayer.instance._table.setUniformPart(part, uniform, parseFloat(sender.getString()));
//				break;
//		}
//	}
//
//	void UIControlButtons::handleEvent(int sender, int type) {
//		if (sender.getTag() == 12 && type == ccui.Widget.TOUCH_BEGAN) {
//			return this->toggleUI();
//		}
//		if (sender.getTag() == 32 && type == ccui.Widget.TOUCH_BEGAN) {
//			return gv.gameMgr.debugMatch();
//		}
//		if(sender.getTag() == 33 && type == ccui.Widget.TOUCH_BEGAN){
//			auto val = !gv.gameMgr.isDebugMode();
//			this->_debugBallButton.setTitleText(val ? "stop" : "set ball");
//			return gv.gameMgr.setDebugMode(val);
//		}
//		if(sender.getTag() == 35 && type == ccui.Widget.TOUCH_BEGAN){
//			gv.ENABLE_SHOW_DEBUG_NOTIFY = !gv.ENABLE_SHOW_DEBUG_NOTIFY;
//			return;
//		}
//		if(sender.getTag() >= 3000 && sender.getTag() <= 3015 && type == ccui.Widget.TOUCH_BEGAN){
//			return gv.gameMgr.debugModule.setBallPositionDebugById(sender.getTag() - 3000);
//		}
//		// if (sender.getTag() === 3){
//		// 	// this._delegate.percent = this.getPowerBarValue() * 100;
//		// }
//		if(sender.getTag() == 200 && type == ccui.Widget.TOUCH_BEGAN){
//			this->activeExt(sender);
//			return;
//		}
//		switch (type) {
//			case ccui.Widget.TOUCH_BEGAN:
//				this->_delegate.dispatchEvent(sender.getTag());
//				break;
//			case ccui.Widget.TOUCH_MOVED:
//				break;
//			case ccui.Widget.TOUCH_ENDED:
//			case ccui.Widget.TOUCH_CANCELED:
//				this->_delegate.cancelEvent(sender.getTag());
//				break;
//		}
//	}
//
//	void UIControlButtons::onRulerMove(int delta) {
//		this->_delegate._interactionManager.moveX(delta / 10, this->_match.canShoot());
//	}
//
//	void UIControlButtons::setDebugButtonsVisible(int val){
//		this->_ballSelectButtons.forEach((e) => {
//			e.setVisible(val);
//		})
//	}
//
//	void UIControlButtons::sliderEvent(int sender, int type) {
//		switch (type) {
//			case ccui.Slider.EVENT_PERCENT_CHANGED:
//				var percent = sender.getPercent();
//				if (sender.getTag() == 1000) {
//					this->processChangeFoV(percent);
//				} else if (sender.getTag() == 1001) {
//					this->processChangeRBase(percent);
//				}
//				// cc.log("Percent: " + percent);
//				// if (sender.getTag() === 1000) {
//				// 	this.percent.setString(percent.toFixed(0) + "%");
//				// }
//				// this._delegate.dispatchEvent(sender.getTag(), percent);
//			break;
//		}
//	}
//
//	void UIControlButtons::processChangeFoV(int percent) {
//		auto fov = 45 + (percent - 50) / 100;
//		this->fovText.setString(fov);
//		gv.gameMgr.interactionMgr.changeFoV(fov);
//	}
//
//	void UIControlButtons::processChangeRBase(int percent) {
//		gv.gameMgr.interactionMgr.changeRBase(percent);
//	}
//});
//
//auto IngameUI = UIBaseLayer.extend({
//	 UIControlButtons::UIControlButtons(){
//		this->_super({isLayer: true});
//		this->initWithJsonFile(res.GUI.Ingame_UI);
//		this->addCameraViewCircle();
//	}
//
//	void UIControlButtons::initComponent(){
//		this->autoInitComponent();
//
//		this->initRulerAndPowerBar();
//		this->initTimers();
//		this->initPocketedBalls();
//		this->initAvatarNodes();
//		this->initGrayscaleShader();
//		this->_bg._btnCamera.endScale = this->_bg._btnCamera.getScale();
//		this->_bg._btnCue.endScale = this->_bg._btnCue.getScale();
//		this->_ballIndicator = this->_bg._btnBall;
//		this->_bg._Fake_Ingame_01_i5_1.setVisible(false);
//	}
//
//	void UIControlButtons::initGrayscaleShader(){
//		for(auto i=0;i<4;i++){
//			auto node = this->_bg._EightBallPoolMode._playerInfo["_nodeP"+(i+1)]._imgBtnExt;
//			node.setLocalZOrder(1);
//
//			auto program = new cc.GLProgram("res/shaders/sprite_util.vert", "res/shaders/sprite_util.frag");
//			program.addAttribute(cc.ATTRIBUTE_NAME_POSITION, cc.VERTEX_ATTRIB_POSITION);
//			program.addAttribute(cc.ATTRIBUTE_NAME_TEX_COORD, cc.VERTEX_ATTRIB_TEX_COORDS);
//			program.link();
//			program.updateUniforms();
//			program.retain();
//			auto state = cc.GLProgramState.create(program);
//			node.setGLProgramState(state)
//			state.setUniformFloat("u_grayscale", 1.0);
//			state.setUniformFloat("u_opacity", 1.0);
//			node._state = state;
//		}
//	}
//
//	void UIControlButtons::initAvatarNodes(){
//		this->_avatarSprite = [];
//		for(auto i=0;i<4;i++){
//			auto node = this->_bg._EightBallPoolMode._playerInfo["_nodeP"+(i+1)]._avatarBorder;
//			auto center = node._avatarMask.getPosition();
//			node._avatarMask.setVisible(false);
//			auto realAvatar = new cc.Sprite(res.INGAME_AVATAR_FAKE_2);
//			node.addChild(realAvatar);
//			realAvatar.setPosition(center);
//			this->_avatarSprite[i] = realAvatar;
//
//			auto program = shaderCache.spriteUtils;
//			// program.addAttribute(cc.ATTRIBUTE_NAME_POSITION, cc.VERTEX_ATTRIB_POSITION);
//			// program.addAttribute(cc.ATTRIBUTE_NAME_TEX_COORD, cc.VERTEX_ATTRIB_TEX_COORDS);
//			// program.link();
//			// program.updateUniforms();
//			// program.retain();
//			auto state = cc.GLProgramState.create(program);
//			realAvatar.setGLProgramState(state)
//			state.setUniformFloat("u_grayscale", 0.0);
//			state.setUniformFloat("u_opacity", 1.0);
//			state.setUniformFloat("u_circle", 1.0)
//			state.setUniformFloat("u_width", realAvatar.width);
//			state.setUniformFloat("u_height", realAvatar.height);
//			state.setUniformFloat("u_radius", node._avatarMask.width);
//			realAvatar._state = state;
//		}
//	}
//
//	void UIControlButtons::initPocketedBalls(){
//		this->pocketedBall = [[],[]];
//		auto size = this->_bg._EightBallPoolMode._pnSizeBallSlot.getContentSize();
//		for(auto i=0;i<2;i++){
//			auto arr = this->pocketedBall[i];
//			for(auto j=0;j<7;j++){
//				auto s = this->_bg._EightBallPoolMode._playerInfo["_nodeP"+(i+1)]._pocketedBall["_ball_"+j];
//				s.setTexture(res.BALLNUM_14);
//				gv.scaleToSize(s, size.width, size.height);
//				s.setVisible(false);
//				arr.push_back(s);
//			}
//		}
//	}
//
//	void UIControlButtons::initTimers(){
//		this->_timer = [];
//		for(auto i=0;i<4;i++){
//			auto node = this->_bg._EightBallPoolMode._playerInfo['_nodeP'+(i+1)];
//			node._countDownDummy.setVisible(false);
//			node._countDownDummy.setFlippedX(true);
//			auto timer = new cc.ProgressTimer(node._countDownDummy);
//			timer.cleanup();
//			timer.midPoint = cc.p(0.5, 0.5);
//			timer.setType(cc.ProgressTimer.TYPE_RADIAL);
//			timer.setReverseDirection(true);
//			//this._expProgress.setPosition(nodeExp.getPosition());
//			node.addChild(timer);
//			timer.setPosition(node._countDownDummy.getPosition());
//			timer.setLocalZOrder(0);
//			// timer.flippedX =true;
//			timer.setRotation(180);
//			timer.setPercentage(0);
//			this->_timer[i] = timer;
//		}
//	}
//
//	void UIControlButtons::initRulerAndPowerBar(){
//		auto ruler = new gv.ui_elements.Ruler();
//		ruler.setPosition(0, cc.winSize.height / 2);
//		ruler.subscribe(this->onRulerMove.bind(this));
//		this->addChild(ruler);
//
//		auto powerBar = new gv.ui_elements.Power();
//		powerBar.setPosition(cc.winSize.width, cc.winSize.height / 2);
//		// powerBar.subscribe(this.onPowerBarRelease.bind(this));
//		auto self = this;
//		powerBar.setListener({
//			void UIControlButtons::onRelease(int percent) {
//				self.onPowerBarRelease(percent);
//			}
//
//			void UIControlButtons::onChange(int percent) {
//				self.onPowerBarChange(percent);
//			}
//		})
//		this->addChild(powerBar);
//	}
//
//	void UIControlButtons::setDelegate(int delegate) {
//		this->_delegate = delegate;
//	}
//
//	void UIControlButtons::onRulerMove(int delta) {
//		this->_delegate._interactionManager.moveX(delta / 10, this->_match.canShoot());
//	}
//
//	void UIControlButtons::onPowerBarRelease(int percent) {
//		if(this->_match.canShoot()){
//			gv.gameMgr.shoot(percent);
//		}
//	}
//
//	void UIControlButtons::onPowerBarChange(int percent) {
//		gv.gameMgr.onChangePower(percent);
//	}
//
//	void UIControlButtons::setCurMatchPlayerCount(int count){
//		this->_curMatchPlayerCount = count;
//	}
//
//	void UIControlButtons::setPlayerInfo(double idx, int data){
//		idx = this->matchIndexToUIIndex(idx);
//		utils.log("IDX", idx);
//		auto group = this->_bg._EightBallPoolMode._playerInfo["_nodeP"+(idx+1)];
//		group._levelBox._lbLevel.setString(data.level || 20);
//		group._lbName.setString(data.name);
//	}
//
//	void UIControlButtons::getGoldLb(){
//		return this->_bg._EightBallPoolMode._playerInfo._goldBox._lbGold;
//	}
//
//	void UIControlButtons::getGoldPot(){
//		return this->_bg._EightBallPoolMode._playerInfo._goldBox._imgGold;
//	}
//
//	void UIControlButtons::getPlayerAvatar(double idx){
//		return this->_avatarSprite[idx];
//	}
//
//	void UIControlButtons::setTimePercentRemain(double playerIdx, int percent){
//		// const node = this._bg._EightBallPoolMode._playerInfo["_nodeP"+(playerIdx+1)]._timerBar._bar;
//		// node.setPercent(percent);
//		playerIdx = this->matchIndexToUIIndex(playerIdx);
//		this->_timer[playerIdx].setPercentage(percent * 0.95 + 5);
//	}
//
//	void UIControlButtons::setExtActivating(double playerIdx, int val, int isInExt){
//		// if(this._curPlayerExtChanged || !val) return;
//		playerIdx = this->matchIndexToUIIndex(playerIdx);
//		auto btn = this->_bg._EightBallPoolMode._playerInfo["_nodeP"+(playerIdx+1)]._imgBtnExt;
//		// utils.log("condition", val && !isInExt);
//		if(val && !isInExt){
//			btn._state.setUniformFloat('u_grayscale', 0);
//		}
//		else{
//			btn._state.setUniformFloat('u_grayscale', 1);
//		}
//		// this._curPlayerExtChanged = true;
//	}
//
//	void UIControlButtons::resetPocketedBall(){
//		for(auto i=0;i<2;i++){
//			auto arr = this->pocketedBall[i];
//			for(auto j=0;j<arr.length;j++)
//				arr[j].setVisible(false);
//		}
//	}
//
//	void UIControlButtons::updateBallCollisionPoint(int percentX, int percentY){
//		auto btnBall = this->_ballIndicator;
//		btnBall._ballIndicator.setPosition(btnBall.width*percentX/100, btnBall.height*percentY/100);
//	}
//
//	void UIControlButtons::setPocketedBall(int pId, int balls){
//		auto arr = this->pocketedBall[pId];
//		for(auto j=0;j<arr.length;j++){
//			if(j >= balls.length) {
//				arr[j].setVisible(false); continue;
//			}
//			arr[j].setVisible(true);
//			arr[j].setTexture(res['BALLNUM_'+(balls[j].number ? balls[j].number : balls[j])]);
//		}
//	}
//
//
//	void UIControlButtons::setScoreVisible(int val){
//		this->_bg._EightBallPoolMode._playerInfo._lbScore.setVisible(val);
//		this->_bg._EightBallPoolMode._playerInfo._imgRound._lbRound.setVisible(val);
//	}
//
//	void UIControlButtons::setRewardVisible(int val){
//		this->_bg._EightBallPoolMode._playerInfo._goldBox.setVisible(val);
//	}
//
//	void UIControlButtons::setScore(int p0Score, int p1Score){
//		this->_bg._EightBallPoolMode._playerInfo._lbScore.setString(p0Score + " : " + p1Score);
//		this->_bg._EightBallPoolMode._playerInfo._imgRound._lbRound.setString("Round " + (p0Score + p1Score + 1));
//	}
//
//	void UIControlButtons::setModeCompetitive(){
//		this->setRewardVisible(false);
//		this->setScoreVisible(true);
//	}
//
//	void UIControlButtons::setModeGamble(){
//		this->setRewardVisible(false);
//		this->setScoreVisible(false);
//	}
//
//	void UIControlButtons::setAngle(int angle){
//		this->_bg._imgAngle._lbAngle.setString(angle + "°");
//	}
//
//	// Chuyển từ idx trong trận: 12 chung team, 34 chung team => idx gui: 1 3 chung team, 2 4 chung team
//	void UIControlButtons::matchIndexToUIIndex(double idx){
//		auto team = floor(idx/this->_curMatchPlayerCount);
//		auto i = idx%this->_curMatchPlayerCount;
//		return this->_curMatchPlayerCount*i + team;
//	}
//
//	void UIControlButtons::activePlayer(double idx){
//		// const deactiveIdx = 1-idx;
//
//		idx = this->matchIndexToUIIndex(idx);
//		auto nodeActive = this->_bg._EightBallPoolMode._playerInfo["_nodeP"+(idx+1)];
//		nodeActive._imgBtnExt.setVisible(true);
//		nodeActive._imgBtnExt._state.setUniformFloat("u_grayscale", 1);
//		this->_timer[idx].setVisible(true);
//		// nodeActive._timerBar.setVisible(true);
//		// nodeActive._btExt._program.setUniformLocationWith1f(nodeActive._btExt._program.getUniformLocation("u_grayscale"), 1);
//		// nodeActive._btExt.setBright(false);
//		nodeActive.setOpacity(255);
//		this->_avatarSprite[idx]._state.setUniformFloat("u_opacity", 1);
//
//
//		for(auto i=0;i<this->_avatarSprite.length;i++){
//			if(i == idx) continue;
//			auto nodeDeactive = this->_bg._EightBallPoolMode._playerInfo["_nodeP"+(i+1)];
//			nodeDeactive._imgBtnExt.setVisible(false);
//			nodeDeactive._imgBtnExt._state.setUniformFloat("u_grayscale", 1);
//			// nodeDeactive._timerBar.setVisible(false);
//			this->_timer[i].setVisible(false);
//			nodeDeactive.setOpacity(190);
//			this->_avatarSprite[i]._state.setUniformFloat("u_opacity", 0.01);
//		}
//
//		// this._curPlayerExtChanged = false;
//	}
//
//	void UIControlButtons::onButtonTouchBegan(int sender, int tag){
//		switch(tag){
//			case this->tagMap.btnCamera:
//				this->_delegate.dispatchEvent(300);
//				break;
//		}
//	}
//
//	void UIControlButtons::onButtonTouchEnded(int sender, int tag){
//		switch(tag){
//			case this->tagMap.btnCamera:
//				this->_delegate.cancelEvent(300);
//				break;
//			case this->tagMap.btnCue:
//				gv.GuiMgr.getGui(Popup.COMPETITION_BAG_UI, CompetitionBagUI).show();
//				utils.log(gv.gameMgr.cue._cueInfo);
//				break;
//			case this->tagMap.btnExtReal:
//				this->activeExt(sender);
//				break;
//			case this->tagMap.btnMenu:
//				gv.GuiMgr.getGui(Popup.SETTING_UI, SettingUI).show();
//				break;
//			case this->tagMap.btnBall:
//				if(!this->_ballTargetLayer){
//					this->_ballTargetLayer = new MoveBallTargetLayer();
//					this->addChild(this->_ballTargetLayer);
//				}
//				this->_ballTargetLayer.setShow(true);
//				break;
//		}
//	}
//
//	void UIControlButtons::addCameraViewCircle() {
//		auto cameraCircle = new CameraCircle();
//		cameraCircle.setPosition(140, 180);
//		this->addChild(cameraCircle);
//	}
//});
