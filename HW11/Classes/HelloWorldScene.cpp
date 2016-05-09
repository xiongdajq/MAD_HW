#include "HelloWorldScene.h"
#include "menuitemlabel_my.h"
#include "Monster.h"
#define database UserDefault::getInstance()

#pragma execution_character_set("utf-8")
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

	//创建一张贴图
	auto texture = Director::getInstance()->getTextureCache()->addImage("$lucia_2.png");
	//从贴图中以像素单位切割，创建关键帧
	auto frame0 = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 113, 113)));
	//使用第一帧创建精灵
	player = Sprite::createWithSpriteFrame(frame0);
	player->setPosition(Vec2(origin.x + visibleSize.width / 2,
							origin.y + visibleSize.height/2));
	addChild(player, 3);
	

	TTFConfig ttfConfig;
	ttfConfig.fontFilePath = "fonts/arial.ttf";
	ttfConfig.fontSize = 36;
	
	//倒计时
	time = Label::createWithTTF(ttfConfig, "0");
	//倒计时周期性调用调度器
	schedule(schedule_selector(HelloWorld::updateTime), 1.0f, kRepeatForever, 0);
	//倒计时的数字
	dtime = 180;
	count = 0;
	time->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - time->getContentSize().height));
	addChild(time,1);

	//hp条
	Sprite* sp0 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
	Sprite* sp = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(610, 362, 4, 16)));

	//使用hp条设置progressBar
	pT = ProgressTimer::create(sp);
	pT->setScaleX(90);
	pT->setAnchorPoint(Vec2(0, 0));
	pT->setType(ProgressTimerType::BAR);
	pT->setBarChangeRate(Point(1, 0));
	pT->setMidpoint(Point(0,1));
	pT->setPercentage(100);
	pT->setPosition(Vec2(origin.x+14*pT->getContentSize().width,origin.y + visibleSize.height - 2*pT->getContentSize().height));
	addChild(pT,2);
	sp0->setAnchorPoint(Vec2(0, 0));
	sp0->setPosition(Vec2(origin.x + pT->getContentSize().width, origin.y + visibleSize.height - sp0->getContentSize().height));
	addChild(sp0,1);


	//静态动画
	idle.reserve(1);
	idle.pushBack(frame0);
	//攻击动画
	Animation* attackAnimation = Animation::create();
	attack.reserve(17);
	for (int i = 0; i < 17; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(113*i,0,113,113)));
		attack.pushBack(frame);
		attackAnimation->addSpriteFrame(frame);
	}
	attackAnimation->setDelayPerUnit(0.1);
	AnimationCache::getInstance()->addAnimation(attackAnimation, "attack");
	//死亡动画
	Animation* deadAnimation = Animation::create();
	auto texture2 = Director::getInstance()->getTextureCache()->addImage("$lucia_dead.png");
	dead.reserve(22);
	for (int i = 0; i < 22; i++) {
		auto frame = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(79 * i, 0, 79, 90)));
		dead.pushBack(frame);
		deadAnimation->addSpriteFrame(frame);
	}
	deadAnimation->setDelayPerUnit(0.1);
	AnimationCache::getInstance()->addAnimation(deadAnimation, "dead");
	
	//运动动画
	Animation* moveAnimation = Animation::create();
	auto texture3 = Director::getInstance()->getTextureCache()->addImage("$lucia_forward.png");
	for (int i = 0; i < 8; i++) {
		auto frame = SpriteFrame::createWithTexture(texture3, CC_RECT_PIXELS_TO_POINTS(Rect(68 * i, 0, 68, 101)));
		run.pushBack(frame);
		moveAnimation->addSpriteFrame(frame);
	}
	moveAnimation->setDelayPerUnit(0.1);
	AnimationCache::getInstance()->addAnimation(moveAnimation, "move");


	//Label
	auto menuLabel1 = Label::createWithTTF(ttfConfig, "W");
	auto menuLabel2 = Label::createWithTTF(ttfConfig, "S");
	auto menuLabel3 = Label::createWithTTF(ttfConfig, "A");
	auto menuLabel4 = Label::createWithTTF(ttfConfig, "D");
	auto menuLabel5 = Label::createWithTTF(ttfConfig, "X");
	//auto menuLabel6 = Label::createWithTTF(ttfConfig, "Y");
	//menuItem
	auto item1 = menuitemlabel_my::createMy(menuLabel1, CC_CALLBACK_1(HelloWorld::stopEvent, this,'W'), CC_CALLBACK_1(HelloWorld::moveEvent, this, 'W'));
	auto item2 = menuitemlabel_my::createMy(menuLabel2, CC_CALLBACK_1(HelloWorld::stopEvent, this,'S'), CC_CALLBACK_1(HelloWorld::moveEvent, this, 'S'));
	auto item3 = menuitemlabel_my::createMy(menuLabel3, CC_CALLBACK_1(HelloWorld::stopEvent, this,'A'), CC_CALLBACK_1(HelloWorld::moveEvent, this, 'A'));
	auto item4 = menuitemlabel_my::createMy(menuLabel4, CC_CALLBACK_1(HelloWorld::stopEvent, this,'D'), CC_CALLBACK_1(HelloWorld::moveEvent, this, 'D'));
	/*item1->setCallback_my(CC_CALLBACK_1(HelloWorld::moveEvent, this, 'W'));
	item2->setCallback_my(CC_CALLBACK_1(HelloWorld::moveEvent, this, 'S'));
	item3->setCallback_my(CC_CALLBACK_1(HelloWorld::moveEvent, this, 'A'));
	item4->setCallback_my(CC_CALLBACK_1(HelloWorld::moveEvent, this, 'D'));*/
	auto item5 = menuitemlabel_my::create(menuLabel5, CC_CALLBACK_1(HelloWorld::actionEvent, this, 'X'));
	//auto item6 = menuitemlabel_my::create(menuLabel6, CC_CALLBACK_1(HelloWorld::actionEvent, this, 'Y'));

	item3->setPosition(Vec2(origin.x+item3->getContentSize().width,origin.y+item3->getContentSize().height));
	item4->setPosition(Vec2(item3->getPosition().x + 3 * item4->getContentSize().width, item3->getPosition().y));
	item2->setPosition(Vec2(item3->getPosition().x + 1.5*item2->getContentSize().width, item3->getPosition().y));
	item1->setPosition(Vec2(item2->getPosition().x, item2->getPosition().y + item1->getContentSize().height));
	item5->setPosition(Vec2(origin.x+visibleSize.width-item5->getContentSize().width,item1->getPosition().y));
	//item6->setPosition(Vec2(item5->getPosition().x-item6->getContentSize().width,item3->getPosition().y));
	
	auto menu = Menu::create(item1, item2, item3, item4, item5, /*item6,*/ NULL);
	menu->setPosition(Vec2(0, 0));
	addChild(menu, 1);
	// 检查碰撞
	schedule(schedule_selector(HelloWorld::hitByMonster), 1.0f, kRepeatForever, 0);
	schedule(schedule_selector(HelloWorld::moveMonster1), 3.0f, kRepeatForever, 0);
	isclick = 0;

	static cocos2d::Size designResolutionSize = cocos2d::Size(960, 640);
	TMXTiledMap* tmx = TMXTiledMap::create("map.tmx");
	tmx->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	tmx->setAnchorPoint(Vec2(0.5, 0.5));
	tmx->setScale(Director::getInstance()->getContentScaleFactor());
	this->addChild(tmx, 0);

	if (!database->getBoolForKey("isExist")) {
		database->setBoolForKey("isExist", true);
	}
	time->setString(std::to_string(database->getIntegerForKey("count")));
	return true;
}

void HelloWorld::stopEvent(Ref*, char cid) {
	switch (cid)
	{
	case 'W':
		player->stopAllActions();
		break;
	case 'A':
		player->stopAllActions();
		break;
	case 'S':
		player->stopAllActions();
		break;
	case 'D':
		player->stopAllActions();
		break;
	default:
		break;
	}
}
void HelloWorld::moveEvent(Ref*,char cid) {

	Animate* moveAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("move"));
	player->runAction(RepeatForever::create(moveAnimate));

	auto movetow = MoveTo::create(2, Vec2(player->getPosition().x, visibleSize.height));
	auto movetoa = MoveTo::create(2, Vec2(0, player->getPositionY()));
	auto movetos = MoveTo::create(2, Vec2(player->getPositionX(), 0));
	auto movetod = MoveTo::create(2, Vec2(visibleSize.width, player->getPositionY()));
	switch (cid) {
	case 'W':
		
		player->runAction(movetow);
		break;
	case 'A':
		player->runAction(movetoa);
		break;
	case 'S':
		player->runAction(movetos);
		break;
	case 'D':
		player->runAction(movetod);
		break;
	}
}


void HelloWorld::actionEvent(Ref*, char cid) {
	
	// about attack
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("x.plist");
	char totalFrames = 16;
	char frameName[20];
	Animation* xAnimation;
	xAnimation = Animation::create();

	for (int i = 1; i <= totalFrames; i++)
	{
		sprintf(frameName, "image %d.png", i);
		xAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
	}
	xAnimation->setDelayPerUnit(0.1);
	AnimationCache::getInstance()->addAnimation(xAnimation, "x");

	auto player1 = Sprite::createWithSpriteFrameName("image 1.png");
	player1->setPosition(player->getPosition());

	Animate* xAnimate;
	xAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("x"));
	auto seq = Sequence::create(Show::create(), xAnimate, Hide::create(), nullptr);

	//about dead
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("y.plist");
    char totalFrames1 = 17;
	frameName[20];
	Animation* yAnimation;
	yAnimation = Animation::create();

	for (int i = 681; i <= totalFrames1*2+681; i = i+2)
	{
		sprintf(frameName, "image %d.png", i);
		yAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
	}
	yAnimation->setDelayPerUnit(0.1);
	AnimationCache::getInstance()->addAnimation(yAnimation, "y");

	auto player2 = Sprite::createWithSpriteFrameName("image 681.png");
	player2->setPosition(player->getPosition());

	Animate* yAnimate;
	yAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("y"));
	auto seq1 = Sequence::create(Show::create(), yAnimate, Hide::create(), nullptr);
	switch (cid) {
	case 'X':
		isclick = 1;
		this->addChild(player1);
		
		player1->runAction(seq);

		Animate* attackAnimate;
		attackAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("attack"));
		player->runAction(attackAnimate);
		this->removeChildByName("player1", true);
		
		break;
	case 'Y':
		isclick = 2;
		pT->setPercentage(pT->getPercentage() - 10.0);
		this->addChild(player2);
		
		player2->runAction(seq1);

		Animate* deadAnimate;
		deadAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("dead"));
		player->runAction(deadAnimate);
		this->removeChildByName("player2", true);
		break;
	}
}

void HelloWorld::stopAc() {
}

void HelloWorld::moveMonster1(float dt) {
	auto fac = Factory::getInstance();
	auto m = fac->createMonster();
	float x = random(origin.x, visibleSize.width);
	float y = random(origin.y, visibleSize.height);
	m->setPosition(x, y);
	this->addChild(m, 3);
	fac->moveMonster(player->getPosition(), 5.0f);
}

void HelloWorld::updateTime(float dt) {
//	dtime--;
//	if (dtime < 0) return;
//	
//	
}
void HelloWorld::hitByMonster(float dt) {
	auto fac = Factory::getInstance();
	Sprite* col = fac->collider(player-> getBoundingBox());
	if (col != NULL) {
		fac->removeMonster(col);
		if (isclick == 0 || isclick == 2) {
			actionEvent(this, 'Y');
		}
		else if (isclick == 1) {
			pT->setPercentage(pT->getPercentage() + 10.0);
			count++;
			database->setIntegerForKey("count", count);
			time->setString(std::to_string(database->getIntegerForKey("count")));
		}
	}
	isclick = 0;
}