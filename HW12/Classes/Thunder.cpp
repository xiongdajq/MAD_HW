#include "Thunder.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <algorithm>
#include <iterator>
#include <vector>
#include <cmath>
USING_NS_CC;

using namespace CocosDenshion;

Scene* Thunder::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Thunder::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool Thunder::init() {
    if ( !Layer::init() ) {
        return false;
    }
    visibleSize = Director::getInstance()->getVisibleSize();
	
	time = 0;
    auto bgsprite = Sprite::create("background.jpg");
    bgsprite->setPosition(visibleSize / 2);
    // bgsprite->setScale(visibleSize.width / bgsprite->getContentSize().width, \
    //     visibleSize.height / bgsprite->getContentSize().height);
    this->addChild(bgsprite, 0);

    player = Sprite::create("player.png");
    player->setPosition(visibleSize.width / 2, player->getContentSize().height + 5);
    player->setName("player");
    this->addChild(player, 1);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("x.plist");
	char totalFrames = 16;
	char frameName[20];
	Animation* xAnimation = Animation::create();
	for (int i = 1; i <= totalFrames; i++)
	{
		sprintf(frameName, "image %d.png", i);
		xAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
	}
	xAnimation->setDelayPerUnit(0.1);
	AnimationCache::getInstance()->addAnimation(xAnimation, "x");
	

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("y.plist");
	char totalFrames1 = 17;
	frameName[20];
	Animation* yAnimation;
	yAnimation = Animation::create();

	for (int i = 681; i <= totalFrames1 * 2 + 681; i = i + 2)
	{
		sprintf(frameName, "image %d.png", i);
		yAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
	}
	yAnimation->setDelayPerUnit(0.1);
	AnimationCache::getInstance()->addAnimation(yAnimation, "y");

	



    addEnemy(5);

   // preloadMusic();
    //playBgm();

    addTouchListener();
    addKeyboardListener();
    addCustomListener();

    // TODO
    // add schedule
	schedule(schedule_selector(Thunder::update), 0.1f, kRepeatForever, 0);
	schedule(schedule_selector(Thunder::down), 1.0f, kRepeatForever, 0);
    return true;
}
void Thunder::down(float dt) {
	time++;
	if (time % 3 == 0) {
		for (auto sp : enemys) {
			sp->setPosition(sp->getPosition() - Vec2(0, 5));
		}
	}
	else if (time % 3 == 2) {
		for (auto sp : enemys) {
			sp->setPositionX(sp->getPositionX() - 3);
		}
	}
	else if (time % 3 == 1) {
		for (auto sp : enemys)
			sp->setPositionX(sp->getPositionX() + 3);
	}
	
}

void Thunder::preloadMusic() {
    // TODO
	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("1-01 Main Theme.mp3");
	audio->preloadEffect("explore.wav");
	audio->preloadEffect("fire.wav");
}

void Thunder::playBgm() {
    // TODO
	auto audio = SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("1-01 Main Theme.mp3", true);

}

void Thunder::addEnemy(int n) {
    enemys.resize(n * 3);
    for(unsigned i = 0; i < 3; ++i) {
        char enemyPath[20];
        sprintf(enemyPath, "stone%d.png", 3 - i);
        double width  = (visibleSize.width - 20) / (n + 1.0),
               height = visibleSize.height - (50 * (i + 1));
        for(int j = 0; j < n; ++j) {
            auto enemy = Sprite::create(enemyPath);
            enemy->setAnchorPoint(Vec2(0.5, 0.5));
            enemy->setScale(0.5, 0.5);
            enemy->setPosition(width * (j + 1), height);
            enemys[i * n + j] = enemy;
			addChild(enemy);
        }
    }
}

void Thunder::addTouchListener(){
    // 
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto touch = EventListenerTouchOneByOne::create();
	
	touch->onTouchBegan = CC_CALLBACK_2(Thunder::onTouchBegan, this);
	touch->onTouchEnded = CC_CALLBACK_2(Thunder::onTouchEnded, this);
	touch->onTouchMoved = CC_CALLBACK_2(Thunder::onTouchMoved, this);
	dispatcher->addEventListenerWithSceneGraphPriority(touch, player);
}

void Thunder::addKeyboardListener() {
	auto dispatcher = Director::getInstance()->getEventDispatcher();

	auto eventlistenerKeyboard= EventListenerKeyboard::create();
	eventlistenerKeyboard->onKeyPressed = CC_CALLBACK_2(Thunder::onKeyPressed, this);
	eventlistenerKeyboard->onKeyReleased = CC_CALLBACK_2(Thunder::onKeyReleased, this);
	dispatcher->addEventListenerWithSceneGraphPriority(eventlistenerKeyboard, player);
}

void Thunder::update(float f) {
    // TODO
	

	auto dispatcher = Director::getInstance()->getEventDispatcher();
	
	int en = -1, fi = -1;
	for (en = 0; en < enemys.size(); en++) {


		// TODOS
		if (enemys[en] != NULL) {
			if (ccpDistance(enemys[en]->getPosition(), player->getPosition()) < 30) {
				Animate* yAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("y"));
				auto player2 = Sprite::createWithSpriteFrameName("image 681.png");
				player2->setName("player2");
				auto seq2 = Sequence::createWithTwoActions(yAnimate, CCCallFuncN::create([](Node*n) {
					n->getParent()->removeChild(n);
				}));
				player2->runAction(seq2);
				player2->setPosition(player->getPosition());
				this->addChild(player2, 1);
				auto fadeout = FadeOut::create(0.1f);
				player->runAction(fadeout);
				//this->removeChildByName("player");
				unschedule(schedule_selector(Thunder::update));
				//return;
				break;
			}

			for (fi = 0; fi < fires.size(); fi++) {
					if (ccpDistance(enemys[en]->getPosition(), fires[fi]->getPosition()) < 30) {
						EventCustom * event = new EventCustom("meet");

						data * data1 = new data();

						data1->en = en;
						data1->fi = fi;
						event->setUserData(data1);
						CCLOG("en fi");
						CCLOGERROR("%d %d", data1->en, data1->fi);
						QueEvent.push(event);
					}	
			}
		}
	}
	while (!QueEvent.empty()) {
		dispatcher->dispatchCustomEvent("meet", (void*)QueEvent.front()->getUserData());
		QueEvent.pop();
	}
	/*std::vector<Sprite*> upFire;
	for (fi = 0; fi < fires.size(); fi++) {
		if (fires[fi]->getPositionY() == visibleSize.height) {
			std::vector<Sprite*>::iterator it1 = fires.begin() + fi;
			this->removeChild(*it1, true);
			upFire.push_back(*it1);
		}
	}*/
	/*std::vector<Sprite*>::iterator it3 = upFire.begin();
	while (it3 != upFire.end()) {
		it3 = upFire.erase(it3);
	}*/
	

	if (player->getPositionX()+move < visibleSize.width-5 && player->getPositionX()+move > 5 )
		player->setPosition(player->getPosition() + Vec2(move, 0));
	if (player->getPositionY() + move1 < visibleSize.height - 5 && player->getPositionY() + move1 > 5)
		player->setPosition(player->getPosition() + Vec2(0, move1));
	if (istouch && pos.x > 5 && pos.x < visibleSize.width-5 && pos.y > 5 && pos.y < visibleSize.height-5)
	player->setPosition(pos);

	/*static double count = 0;
	static int dir = 1;
	count += f;
	if (count > 1) { count = 0.0; dir = -dir; }*/

	/*if (bullet != NULL) {
		bullet->setPosition(bullet->getPositionX(), bullet->getPositionY() + 5);
		if (bullet->getPositionY() > visibleSize.height - 10) {
			bullet->removeFromParentAndCleanup(true);
			bullet = NULL;
		}
	}

	for (unsigned i = 0; i < enemys.size(); i++) {
		if (enemys[i] != NULL) {
			enemys[i]->setPosition(enemys[i]->getPosition() + Vec2(dir, 0));
		}
    }*/
}

void Thunder::fire() {
    //if (bullet != NULL) return;
	auto audio = SimpleAudioEngine::getInstance();
	audio->playEffect("fire.wav", false);
	Thunder * t = this;
    bullet = Sprite::create("bullet.png");
	fires.push_back(bullet);
	//std::vector<Sprite*>::iterator it = std::find(fires.begin(), fires.end(), bullet);
	bullet->setPosition(player->getPosition());
	auto moveto = MoveTo::create(3.0f, Vec2(player->getPositionX(), visibleSize.height));

	//todos****************************************************************
	auto seq = Sequence::createWithTwoActions(moveto, CallFuncN::create([t](Node* n) {
		n->getParent()->removeChild(n);
		/*std::vector<Sprite*>::iterator it = t->fires.begin();
		while (*it != n && it != t->fires.end()) {
			++it;
		}
		t->fires.erase(it);*/

		std::vector<Sprite*>::iterator it = std::find(t->fires.begin(), t->fires.end(), n);

		t->fires.erase(it);
	}));
	fires.at(fires.size() -1) ->runAction(seq);
	addChild(bullet, 2);
}
void Thunder::meet(EventCustom* event) {
	auto audio = SimpleAudioEngine::getInstance();
    data * dd = (data*)(event->getUserData());
	audio->playEffect("explore.wav", false);

	
	
	//std::vector<Sprite*> * deleting = static_cast<std::vector<Sprite*> *>(event->getUserData());
	std::vector<Sprite*>::iterator it = enemys.begin() + dd->en;
 	std::vector<Sprite*>::iterator it2 = fires.begin() + dd->fi;

	
	Animate* xAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("x"));
	player1 = Sprite::createWithSpriteFrameName("image 1.png");
	player1->setName("player1");
	auto seq1 = Sequence::createWithTwoActions(xAnimate, CCCallFuncN::create([](Node*n) {
		n->getParent()->removeChild(n);
	}));
	player1->runAction(seq1);
	player1->setPosition((*it)->getPosition());

	this->addChild(player1, 3);
	
	this->removeChild(*it, true);
	this->removeChild(*it2, true);
	//*it = nullptr;
	//it2 = nullptr;
	it = enemys.erase(it);
	it2 = fires.erase(it2);
	//std::vector<Sprite*>::iterator it = (*deleting).begin();
	//while (it != (*deleting).end()) {
	//	this->removeChild(*it, true);
	//	//it = (*deleting).erase(it);
	//	it++;
	//}
	
}

void Thunder::addCustomListener() {
    // TODO
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto eventlistenerMeet = EventListenerCustom::create("meet", CC_CALLBACK_1(Thunder::meet, this));
	dispatcher->addEventListenerWithSceneGraphPriority(eventlistenerMeet, this);
	
}

bool Thunder::onTouchBegan(Touch *touch, Event *unused_event) {
    //
	player->setScale(1.1);
	istouch = true;
	return true;
	
}

void Thunder::onTouchMoved(Touch *touch, Event *unused_event) {
    //
	 pos = touch->getLocation();
	 CCLOG("move");
	 istouch = true;
}

void Thunder::onTouchEnded(Touch *touch, Event *unused_event) {
    //
	player->setScale(0.9);
	istouch = false;
}

void Thunder::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
    switch (code) {
        case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case cocos2d::EventKeyboard::KeyCode::KEY_A:
			if (player->getPositionX() > 5)
				player->setPosition(player->getPositionX() - 1, player->getPositionY());
			move -= 5;
			break;
        case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_D:
			if (player->getPositionX() < visibleSize.width-5)
			player->setPosition(player->getPositionX() + 1, player->getPositionY());
			move += 5;
            break;
		case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
		case cocos2d::EventKeyboard::KeyCode::KEY_W:
			if (player->getPositionY() < visibleSize.height-5)
			player->setPosition(player->getPositionX(), player->getPositionY()+1);
			move1 += 5;
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		case cocos2d::EventKeyboard::KeyCode::KEY_S:
			if (player->getPositionY() > 5)
			player->setPosition(player->getPositionX(), player->getPositionY() - 1);
			move1 -= 5;
			break;
        case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
            fire();
            break;
        default:
            break;
    }
}

void Thunder::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
	switch (code)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case cocos2d::EventKeyboard::KeyCode::KEY_A:
			if (player->getPositionX() > 5)
			player->setPosition(player->getPositionX() - 1, player->getPositionY());
			move += 5;
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case cocos2d::EventKeyboard::KeyCode::KEY_D:
			if (player->getPositionX() < visibleSize.width - 5)
			player->setPosition(player->getPositionX() + 1, player->getPositionY());
			move -= 5;
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
		case cocos2d::EventKeyboard::KeyCode::KEY_W:
			if (player->getPositionY() < visibleSize.height - 5)
			player->setPosition(player->getPositionX(), player->getPositionY() + 1);
			move1 -= 5;
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		case cocos2d::EventKeyboard::KeyCode::KEY_S:
			if (player->getPositionY() > 5)
			player->setPosition(player->getPositionX(), player->getPositionY() - 1);
			move1 += 5;
			break;
	default:
		break;
	}
}
