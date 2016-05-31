#include "LoginScene.h"
#include "cocostudio/CocoStudio.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "Global.h"
#include "GameScene.h"
#include <regex>
using std::to_string;
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace rapidjson;
USING_NS_CC;

using namespace cocostudio::timeline;

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
using namespace  rapidjson;

Scene* LoginScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LoginScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}





// on "init" you need to initialize your instance
bool LoginScene::init()
{
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto sprite = Sprite::create("login.jpg");
	sprite->setPosition(Vec2(origin.x+visibleSize.width / 2, origin.y+visibleSize.height / 2));
	this->addChild(sprite);
    Size size = Director::getInstance()->getVisibleSize();
    visibleHeight = size.height;
    visibleWidth = size.width;

    textField = TextField::create("Player Name", "Arial", 30);
    textField->setPosition(Size(visibleWidth / 2, visibleHeight / 4 * 3));
	textField->setMaxLengthEnabled(true);
	textField->setMaxLength(10);

    this->addChild(textField, 2);

    auto button = Button::create();
    button->setTitleText("Login");
    button->setTitleFontSize(30);
    button->setPosition(Size(visibleWidth / 2, visibleHeight / 3));
	button->addClickEventListener(CC_CALLBACK_1(LoginScene::login, this));
    this->addChild(button, 2);

    return true;
}
void LoginScene::login(Ref* pSender) {

	// send requuest
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://localhost:8080/login");
	request->setRequestType(HttpRequest::Type::POST);
	request->setTag("login");
	request->setRequestData(("username="+textField->getString()).c_str(), textField->getStringLength()+9);
	request->setResponseCallback(CC_CALLBACK_2(LoginScene::login_response, this));
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
	
}
void LoginScene::login_response(HttpClient* client, HttpResponse* response) {
	char* body = response->getResponseData()->data();
	body[string(body).size()] = '\0';
	string body1 = string(body);
	if (body1.find("true") < body1.size()) {
		// jump next scene
		auto scene = GameScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(1.5, scene, Color3B(255, 255, 255)));
		char* header = response->getResponseHeader()->data();
		string session = string(header);
		int a = session.find(':');
		session = session.substr(a+1);
		a = session.find(':');
		session = session.substr(a + 1);
		a = session.find("\r\n");
		Global::gameSessionId = session.substr(0, a);
	}
	else {
		TTFConfig ttfConfig;
		ttfConfig.fontFilePath = "fonts/arial.ttf";
		ttfConfig.fontSize = 36;
		int a = body1.find("info\":");
		int b = body1.find(".\"}");
		string error1 = body1.substr(a + 7, b - a - 6);
		auto error = Label::createWithTTF(ttfConfig, error1);
		error->setPosition(Size(visibleWidth / 4, visibleHeight / 4 + 50));
		this->addChild(error, 3);
	}
}
