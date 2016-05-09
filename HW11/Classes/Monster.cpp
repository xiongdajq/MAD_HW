#include"Monster.h"
USING_NS_CC;

Factory* Factory::factory = NULL;
Factory::Factory() {
	initSpriteFrame();
}
Factory* Factory::getInstance() {
	if (factory == NULL) {
		factory = new Factory();
	}
	return factory;
}
void Factory::initSpriteFrame(){
	auto texture = Director::getInstance()->getTextureCache()->addImage("Monster.png");
	monsterDead.reserve(4);
	for (int i = 0; i < 4; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(258-48*i,0,42,42)));
		monsterDead.pushBack(frame);
	}
}

Sprite* Factory::createMonster() {
	Sprite* mons = Sprite::create("Monster.png", CC_RECT_PIXELS_TO_POINTS(Rect(364,0,42,42)));
	monster.pushBack(mons);
	return mons;
}

void Factory::removeMonster(Sprite* sp) {
	cocoa2d:Vector<Sprite*>::iterator it = monster.begin();
	for (; it != monster.end();it++) {
		if (sp == (*it)) {
			sp->getParent()->removeChild(sp, true);
			//monster.erase(it);
			*it = nullptr;
			return;
		}
			
		//else ;
	}
}
void Factory::moveMonster(Vec2 playerPos,float time){
	for (auto sp : monster) {
		auto moveto = MoveTo::create(time, playerPos);
		if (sp == nullptr) continue;
		//sp->stopAllActions();
		sp->runAction(moveto);
	}
}

Sprite* Factory::collider(Rect rect) {
	for (auto sp : monster) {
		if (sp == nullptr) continue;
		if (rect.containsPoint(sp->getPosition())) {
			return sp;
		}
	}
	return NULL;
}