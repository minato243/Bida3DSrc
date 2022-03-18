#include "WorkshopUI_new.h"

{
    WorkshopScene::WorkshopScene(){
        this->_super();
        this->_trackingCue = null;
    }

    void WorkshopScene::onEnter(){
        this->_super();
        this->ui = gv.GuiMgr.getGui(Popup.WORKSHOP_UI, WorkshopUI);
        this->ui.setParentScene(this);
        this->ui.show();
        if(this->_trackingCue != null){
            this->ui.setTrackingCue(this->_trackingCue);
        }
        // if(this.data){
        //     this.ui.setInfo(this.data, this.isOwn, this.isGacha);
        //     this.data = null;
        // }
        this->getDefaultCamera().setDepth(4);
        this->initCameras();
    }

    void WorkshopScene::initCameras(){
        auto bgCamera = new cc.Camera(
            cc.Camera.Mode.ORTHOGRAPHIC,
            cc.winSize.width,
            cc.winSize.height,
            0,
            1);
        bgCamera.setDepth(0);
        bgCamera.setCameraFlag(GameCamera.FLAG_BG_LOWEST);
        this->addChild(bgCamera);

        auto camera = new cc.Camera(
            cc.Camera.Mode.PERSPECTIVE,
            45,
            cc.winSize.width/cc.winSize.height,
            0.01,
            100);
        camera.setDepth(1);
        camera.setCameraFlag(GameCamera.FLAG_3D);
        camera.setPosition3D(Vec3(0, 0, -10));
        camera.lookAt(Vec3(0,0,0));
        this->addChild(camera);

        auto cameraUI = new cc.Camera(
            cc.Camera.Mode.ORTHOGRAPHIC,
            cc.winSize.width,
            cc.winSize.height,
            -1,
            10
        )
        cameraUI.setDepth(2);
        cameraUI.setCameraFlag(GameCamera.FLAG_UI);
        this->addChild(cameraUI);

        auto camera3DOrtho = new cc.Camera(
            cc.Camera.Mode.ORTHOGRAPHIC,
            cc.winSize.width,
            cc.winSize.height,
            -10,
            100
        );
        // camera3DOrtho.setPosition3D(cc.math.vec3(0,0,-10));
        // camera3DOrtho.lookAt(cc.math.vec3(0,0,0));
        camera3DOrtho.setDepth(3);
        camera3DOrtho.setCameraFlag(GameCamera.FLAG_3D_ABOVE_UI);
        this->addChild(camera3DOrtho);
        // this._cameras.push(camera);
    }

    void WorkshopScene::loadData(int data, int isOwn, int isGacha){
        // this.data = data;
        // this.isOwn = isOwn;
        // this.isGacha = isGacha;
        // if(this.ui) this.ui.setInfo(data, isOwn, isGacha);
    }

    void WorkshopScene::setTrackingCue(int data){
        if(this->ui){
            this->ui.setTrackingCue(data);
        }
        else{
            this->_trackingCue = data;
        }
    }

    void WorkshopScene::onSceneReload(){
        // if(this.ui) this.ui.resetCueView();
        if(this->ui) {
            this->ui.refresh();
            this->ui.tabs[2].scheduleUpdate();
        }
    }
});

auto WorkshopUI = UIBaseLayer.extend({
    WorkshopScene::WorkshopScene(){
        this->_super({tag: Popup.WORKSHOP_UI});
        this->keep = true;
        this->_trackingCue = null;
        this->initWithJsonFile(res.GUI.WorkshopUI);
    }

    void WorkshopScene::initComponent(){
        this->autoInitComponent();
        this->setCameraMask(GameCamera.FLAG_UI, true);
        this->_fake.setVisible(false);

        this->_bg._imgBg.setCameraMask(GameCamera.FLAG_BG_LOWEST);
        this->initTabs();
    }

    void WorkshopScene::initTabs(){
        auto upgradeTab = new WorkshopUpgradeUI();
        this->addChild(upgradeTab);

        auto fusionTab = new WorkshopFusionUI();
        this->addChild(fusionTab);

        auto swapTab = new WorkshopSwapUI();
        this->addChild(swapTab);

        this->btnTabs = [this->_bg._imgTabs._btnTabUpgrade, this->_bg._imgTabs._btnTabFusion, this->_bg._imgTabs._btnTabSwap];
        this->btnTabs[0].texNameActive = 'workshop_Upgrade_Tab_01.png';
        this->btnTabs[1].texNameActive = 'workshop_Fusion_Tab_01.png';
        this->btnTabs[2].texNameActive = 'workshop_Swap_Tab_01.png';
        this->btnTabs[0].texNameInactive = 'workshop_Upgrade_Tab_02.png';
        this->btnTabs[1].texNameInactive = 'workshop_Fusion_Tab_02.png';
        this->btnTabs[2].texNameInactive = 'workshop_Swap_Tab_02.png';
        this->tabs = [upgradeTab, fusionTab, swapTab];
        this->curTab = -1;
        this->switchTab(WorkshopUI.UPGRADE_TAB);
    }

    void WorkshopScene::refresh(){
        this->tabs[this->curTab].refresh();
    }

    void WorkshopScene::setTrackingCue(int cue){
        this->_trackingCue = cue;
    }

    void WorkshopScene::switchTab(double tabIdx){
        if(this->curTab == tabIdx) return;
        this->curTab = tabIdx;
        void WorkshopScene::(int function(tab, double idx){
            auto active = idx == tabIdx;
            tab.setVisible(active);
            if(active) {
                tab.refresh();
                tab.onSwitched && tab.onSwitched();
            }
            this->_bg._imgTabs['_imgTabActive' + (idx + 1)].setVisible(active);
            utils.loadButtonTextures(this->btnTabs[idx], active ? this->btnTabs[idx].texNameActive : this->btnTabs[idx].texNameInactive);
        }.bind(this));
    }

    void WorkshopScene::setParentScene(int scene){
        this->_parentScene = scene;
    }

    void WorkshopScene::onButtonTouchEnded(int sender, int tag){
        switch(tag){
            case this->tagMap.btnTabUpgrade:
                this->switchTab(WorkshopUI.UPGRADE_TAB); break;
            case this->tagMap.btnTabFusion:
                this->switchTab(WorkshopUI.FUSION_TAB); break;
            case this->tagMap.btnTabSwap:
                this->switchTab(WorkshopUI.SWAP_TAB); break;
            case this->tagMap.btnBack:
                if(gv.gameMgr.lastSceneTag == Scenes.INSPECT_SCENE && this->_trackingCue != null && this->_trackingCue.isDisappeared()){
                    gv.gameMgr.loadSceneByTag(Scenes.MAIN_SCENE);
                }
                else{
                    // gv.gameMgr.loadSceneByTag(gv.gameMgr.lastSceneTag);
                    gv.gameMgr.loadLastScene();
                }
                this->_trackingCue = null;
                this->_parentScene._trackingCue = null;
                break;
            case this->tagMap.btnInventory:
                gv.GuiMgr.showGui(Popup.INVENTORY_UI, InventoryUIv2);
                break;
        }
    }
});

WorkshopUI.UPGRADE_TAB = 0;
WorkshopUI.FUSION_TAB = 1;
WorkshopUI.SWAP_TAB = 2;
