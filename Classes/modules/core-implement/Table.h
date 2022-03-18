#ifndef TABLE_H_
#define TABLE_H_

#include <cocos2d.h>
#include "Ball.h"
#include <modules/core-implement/Cue.h>
#include <modules/camera-view/utils/ThiccDrawNode3D.h>
#include <modules/core/ps/TrajectoryElement.h>
#include <modules/core/utils/TableEvent.h>
#include "effekseer/effekseer/Effekseer.h"
#include "../core/ps/internal/PhysicsWorldV2.h"

class TableGuildLine : public ThiccDrawNode3D {
public:
	TableGuildLine();
	bool init();
	CREATE_FUNC(TableGuildLine);
	void clearAll();

	ThiccDrawNode3D* sub;
	ThiccDrawNode3D* sub2;
	ThiccDrawNode3D* sub3;
	ThiccDrawNode3D* sub4;
	ThiccDrawNode3D* sub5;
	ThiccDrawNode3D* sub6;
};

class Table: public cocos2d::Node {
public:
	typedef std::function<void()> TableOnPauseCallBack;
	typedef std::function<void(TableEventType, TableEventData*)> TableEventCallBack;

	Table();
	~Table();

	CREATE_FUNC(Table);

	virtual bool init();

	void initTable();

	void initPhysicsWorld();

	void addBall(Ball *ball, ps::ExtMath::vector position = ps::ExtMath::vector(0,0));

	void addWhiteBall(Ball *ball);

	void addCue(Cue *cue);

	void initGuildLine();

	void initShader(cocos2d::GLProgram *tableShader, cocos2d::GLProgram *matShader, int level);

	cocos2d::GLProgramState* setPartShaderState(cocos2d::GLProgram *tableShader, cocos2d::GLProgram *matShader, cocos2d::Sprite3D* part,cocos2d::Texture2D*  mraoMap, int i);

	void setLights(std::vector<cocos2d::Vec3> lightPositions, cocos2d::Vec3 lightColor);
	
	void drawKitchen();

	void initEffect();

	void initBillBoardOnPockets();

	void initPocketHighlights();

	void initEfks();

	void initTableFenceEfks();

	void initBallForceEffect();

	efk::EffectEmitter *createFenceEfk(const std::string & path, bool revert = false);

	void setEfkManager(efk::EffectManager * efkManager);
	
	int subscribeOnPaused(const TableOnPauseCallBack & tableOnPauseCallBack);

	int subscribeTableEvents(const TableEventCallBack & tableEventCallBack);

	virtual void onEnter();

	void step(float dt);

	void onWorldPause(double dt);

	void handleBallCollision(double time, ps::BallBody *ballBody1, ps::BallBody *ballBody2);

	void handleCushionCollision(double time, ps::BallBody *ballBody, ps::CushionSegment *cushion);

	void handleBallFallen(double time, BallBody *ballBody, Pocket *pocket);

	void updateCuePosition();

	void reset();

	void setCueVisible(bool visible);

	void setGuildLineVisible(bool visible);

	void updateGuildLines(std::vector<cocos2d::Vec3> points);

	void drawTrajectory(std::vector<TrajectoryElement *> trajectories, std::vector<ps::ExtMath::vector> points);

	void drawFloatingTrajectory(std::vector<TrajectoryElement*> trajectories, std::vector<ps::ExtMath::vector> points);

	void drawFloatingTrajectory(TrajectoryReflectData trajectory, std::vector<ps::ExtMath::vector> points);

	void drawTrajSegmentPoints(std::vector<std::list<vector>> segments, bool showGhostBall);

	void onChangeCameraPosition(cocos2d::Vec3 position);

	void resume(bool notHideCue = false);

	void highlightBalls(std::vector<int> *nums);

	void highlightBalls();

	void hightLightAllBalls();

	void unHighlightBalls();

	void resetEventCount();

	void highlightPocket(int pocketId = -1);

	void hideAllHighlightPockets();

	void hideAllFenceEfx();

	void showHighlightPocket(int pocketId, bool effect);

	void showSelectPocket(int pocketId, SelectPocketState state);

	void showSpriteOnPocket(int pocketId, cocos2d::Node * sprite, bool isMoving);

	void removeSpriteOnPocket(int pocketId, int tag = -1);

	ps::ExtMath::vector getCueBallDefaultPosForPlacing();

	bool isCueBallPosValid(ps::ExtMath::vector pos);

	void setBallPosition(int ballId, ps::ExtMath::vector pos);

	void setBallEnable(int ballId);

	void setBallDisable(int ballId);

	void loadFakeData(std::map<int, ps::ExtMath::vector> balls);

	int getPocketRegion(ps::ExtMath::vector p);
	
	ps::PhysicsWorldV2* getWorld();


	std::vector<Ball *> getAllBall();

	cocos2d::Sprite3D* getFakeCueBall();

	TableGuildLine* getGuildLine();

	Cue *getCue();

	Ball *getWhiteBall();

	Ball *getBallWithId(int id);
	
	std::vector<cocos2d::Sprite3D*> getParts();
    
    void update(float dt);

	void updateEfkSeer(float dt);

	void onPocketedBall(int pocketId, int ballId);

	void highlightCueAndEightBall(bool value);

	void drawForce(const ps::ExtMath::vector & direction, float forcePercent);

	void setVisibleEffectForce(bool visible);

private:
	cocos2d::Sprite3D* _table;
	std::vector<cocos2d::Sprite3D*> _parts;
	std::vector<Ball*> _balls;
	Cue* _cue;
	Ball* _whiteBall;
	cocos2d::Sprite3D* _fakeCueBall;
	ps::PhysicsWorldV2* _world;
	TableGuildLine* _guildLine;
	int _qualityLevel;
	std::vector<cocos2d::GLProgramState *> _state;
	cocos2d::GLProgramState *_fakeBallState;
	std::vector<cocos2d::Sprite *> _pocketHighlights;
	std::vector<int> _pocketEffects;

	std::vector<int> *_highlightingBalls;

	cocos2d::DrawNode *_kitchenHighlight;

	std::vector<cocos2d::BillBoard *> _billboards;
	std::vector<cocos2d::Sprite3D *> _billboardStaticSprites;
	std::vector<cocos2d::Sprite3D *> _billboardMoveSprites;

	ps::ExtMath::vector _pocketsPos[6];

	std::vector<TableOnPauseCallBack> _onPausedCallbacks;
	std::vector<TableEventCallBack> _tableEventListener;

	bool _effectChargeVisible;

	int _eventCount;

	efk::EffectManager *_efkManager;
	std::vector<cocos2d::Node *> _eightBallEffects;
	std::vector<cocos2d::Node *> _ballToHoleEffects;
	std::vector<std::map<int, cocos2d::Node *>> _ballNumberHoleEffects;

	std::map<cocos2d::Node *, float> _showTimes;

	static const int NUM_FENCE_EFX = 12;
	std::vector<efk::EffectEmitter*> _allFenceEfxs;
	std::vector<efk::EffectEmitter*> _fenceEfx[NUM_POCKET];

	efk::EffectEmitter *_ballForceEfx;

	bool _guildLineVisible;
	std::list<cocos2d::Node *> _drawCircles;
	cocos2d::Node *_drawCircleNode;

public:
	static cocos2d::Color4B GUILD_RED;
	static cocos2d::Color4B GUILD_GREEN;
	static cocos2d::Color4B GUILD_GRAY;
	static cocos2d::Color4B GUILD_WHITE;

	static const int TAG_SELECT = 1234;
	static const int TAG_ARROW = 1235;

	static const float EFX_SHOW_TIME;
};
#endif // !TABLE_H_
