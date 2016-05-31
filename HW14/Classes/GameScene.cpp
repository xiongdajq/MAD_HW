#include "GameScene.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <regex>
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace rapidjson;

USING_NS_CC;

cocos2d::Scene* GameScene::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool GameScene::init() {
    if (!Layer::init())
    {
        return false;
    }
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Size size = Director::getInstance()->getVisibleSize();
    visibleHeight = size.height;
    visibleWidth = size.width;

	auto sprite = Sprite::create("game.jpg");
	sprite->setScale(1.5);
	sprite->setPosition(Vec2(origin.x + visibleWidth / 2, origin.y + visibleHeight / 2));
	this->addChild(sprite);

    score_field = TextField::create("Score", "Arial", 30);
    score_field->setPosition(Size(visibleWidth / 4, visibleHeight / 4 * 3));
    this->addChild(score_field, 2);

    submit_button = Button::create();
    submit_button->setTitleText("Submit");
    submit_button->setTitleFontSize(30);
    submit_button->setPosition(Size(visibleWidth / 4, visibleHeight / 4));
    this->addChild(submit_button, 2);
	submit_button->addClickEventListener(CC_CALLBACK_1(GameScene::submit, this));

    rank_field = TextField::create("", "Arial", 30);
    rank_field->setPosition(Size(visibleWidth / 4 * 3, visibleHeight / 4 * 3));
    this->addChild(rank_field, 2);

    rank_button = Button::create();
    rank_button->setTitleText("Rank");
    rank_button->setTitleFontSize(30);
    rank_button->setPosition(Size(visibleWidth / 4 * 3, visibleHeight / 4));
	rank_button->addClickEventListener(CC_CALLBACK_1(GameScene::rank, this));
    this->addChild(rank_button, 2);

    return true;
}

void GameScene::submit(Ref* pSender) {
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://localhost:8080/submit");
	request->setRequestType(HttpRequest::Type::POST);
	vector<string> v;
	//v.push_back("Content-Type: application/x-www-form-urlencoded; charset=UTF-8");
	v.push_back("Cookie:"+Global::gameSessionId);
	
	request->setHeaders(v);

	request->setRequestData(("score=" + score_field->getString()).c_str(), 6 + score_field->getStringLength());
	//request->setRequestData("score=100", 9);
	request->setTag("submit");
	request->setResponseCallback(CC_CALLBACK_2(GameScene::submit_response, this));
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void GameScene::submit_response(HttpClient* client, HttpResponse* response) {
	char* body = response->getResponseData()->data();
	body[string(body).size()] = '\0';
	string body1 = string(body);
	TTFConfig ttfConfig;
	ttfConfig.fontFilePath = "fonts/arial.ttf";
	ttfConfig.fontSize = 36;
	if (body1.find("true")) {
		auto success = Label::createWithTTF(ttfConfig, "Submit Succeed!");
		success->setPosition(Size(visibleWidth / 4, visibleHeight / 4 + 50));
		this->addChild(success, 3);
		auto fadeout = FadeOut::create(2.0f);
		success->runAction(fadeout);
	}
	else {
		int a = body1.find("info\":");
		int b = body1.find(".\"}");
		string error1 = body1.substr(a + 7, b - a - 6);
		auto error = Label::createWithTTF(ttfConfig, error1);
		error->setPosition(Size(visibleWidth / 4, visibleHeight / 4 + 50));
		this->addChild(error, 3);
		auto fadeout = FadeOut::create(2.0f);
		error->runAction(fadeout);
	}
}

void GameScene::rank(Ref* pSender) {
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://localhost:8080/rank?top=3");
	vector<string> v;
	v.push_back("Cookie:" + Global::gameSessionId);
	request->setHeaders(v);
	request->setRequestType(HttpRequest::Type::GET);
	request->setTag("rank");
	//char* a = "top=3";
	//request->setRequestData(a, 5);
	request->setResponseCallback(CC_CALLBACK_2(GameScene::rank_response, this));
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void GameScene::rank_response(HttpClient* client, HttpResponse* response) {
	/*char* body = response->getResponseData()->data();
	body[strlen(body) ] = '\0';
	string body1(body);*/
	//body1 = body1.substr(0, body1.size() - 2);

	string body1(response->getResponseData()->data(), response->getResponseData()->size());
	string top3, top2, top1;

	if (body1.find("\"result\":true") < body1.size()-1) {
		int a = body1.find('|');
		string s = body1.substr(a + 1, body1.size() - a - 2);
		a = s.find('|');
		top1 = s.substr(0, a);
		s = s.substr(a + 1);
		a = s.find('|');
		if (a == -1) top2 = "";
		else top2 = s.substr(0, a);
		s = s.substr(a + 1);
		a = s.find('|');
		if (a == -1) top3 = "";
		else top3 = s.substr(0, a);
		rank_field->setString(top1 + "\r\n" + top2 + "\r\n" + top3 + "\r\n");
	}
	else {
		int a = body1.find("info\":");
		int b = body1.find(".\"}");
		string error = body1.substr(a + 7, b-a-6);
		rank_field->setString(error);
	}
}
