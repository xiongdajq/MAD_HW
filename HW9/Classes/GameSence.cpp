#include "GameSence.h"

USING_NS_CC;

Scene* GameSence::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameSence::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameSence::init()
{

	if (!Layer::init())
	{
		return false;
	}
	isUp = true;
	//add touch listener
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameSence::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);


	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto background = Sprite::create("level-background-0.jpg");
	background->setPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height/2));
	this->addChild(background, 0);

	stone = Sprite::create("stone.png");
	stone->setPosition(Vec2(origin.x + visibleSize.width *3/ 5, origin.y + visibleSize.height * 3 / 4));
	this->addChild(stone, 1);

	 //add moving mouse
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("level-sheet.plist");
	char totalFrames = 8;
	char frameName[20];
	Animation* legAnimation = Animation::create();

	for (int i = 0; i < totalFrames; i++)
	{
		sprintf(frameName, "gem-mouse-%d.png", i);
		legAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
	}
	legAnimation->setDelayPerUnit(0.1);
	AnimationCache::getInstance()->addAnimation(legAnimation, "legAnimation");
	
    mouse = Sprite::createWithSpriteFrameName("gem-mouse-0.png");
	Animate* legAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("legAnimation"));
	mouse->runAction(RepeatForever::create(legAnimate));
	mouse->setPosition(origin.x+visibleSize.width/2, origin.y + visibleSize.height/2);
	this->addChild(mouse, 2);

	// about shoot
	auto shoot_label = Label::create("Shoot", "fonts/Marker Felt.ttf", 50);
	auto shoot = MenuItemLabel::create(shoot_label, CC_CALLBACK_1(GameSence::shoot_stone, this));
	shoot->setPosition(Vec2(origin.x + visibleSize.width * 4 / 5, origin.y + visibleSize.height * 3 / 4));
	auto menu = Menu::create(shoot, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 3);

	return true;
}

bool GameSence::onTouchBegan(Touch *touch, Event *unused_event) {
	// about cheese
	auto location = touch->getLocation();
	auto cheese = Sprite::create("cheese.png");
	cheese->setPosition(location);
	auto fadeIn = FadeIn::create(1.0f);
	cheese->runAction(fadeIn);
	auto fadeOut = FadeOut::create(4.0f);
	cheese->runAction(fadeOut);
	this->removeChildByName("cheese", true);
	this->addChild(cheese, 1);

	// about mouse
	if (location.x > mouse->getPositionX()) {
		if (!isUp) {
			auto rotateby = RotateBy::create(1.0f, -180.0f);
			mouse->runAction(rotateby);
		}
		auto moveto = MoveTo::create(2, location);
		mouse->runAction(moveto);
		isUp = true;
	}
	else if (location.x < mouse->getPositionX()) {
		if (isUp) {
			auto rotateby = RotateBy::create(1.0f, 180.0f);
			mouse->runAction(rotateby);
		}
		auto moveto = MoveTo::create(2, location);
		mouse->runAction(moveto);
		isUp = false;
	}


	
	return true;
}

void GameSence::shoot_stone(Ref* pSender) {
	auto stone1 = Sprite::create("stone.png");
	stone1->setPosition(stone->getPosition());
	this->addChild(stone1);
	auto moveto = MoveTo::create(0.5, mouse->getPosition());
	stone1->runAction(moveto);
	auto fadeout = FadeOut::create(1.5f);
	stone1->runAction(fadeout);
	this->removeChildByName("stone1");
	auto gem = Sprite::create("diamond.png");
	gem->setPosition(mouse->getPosition());
	this->addChild(gem, 1);
	if (!isUp) {
		auto rotate = RotateBy::create(1.0f, -180.0f);
		mouse->runAction(rotate);
		isUp = true;
	}
	
	auto moveto1 = MoveTo::create(2, Vec2(100, 370));
	mouse->runAction(moveto1);
}
