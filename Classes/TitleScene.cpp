//
//  TitleScene.cpp
//  
//
//  Created by iwamototateatsu on 2016/08/15.
//
//

#include "TitleScene.h"
#include "playground.h"

USING_NS_CC;

Scene* TitleScene::createScene()
{
    auto scene = Scene::create();
    auto layer = TitleScene::create();
    scene->addChild(layer);
    return scene;
}

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
 
}


bool TitleScene::init()
{
    if(!Layer::init()){
        return false;
    }
    
    auto director = Director::getInstance();
    auto winSize = director->getWinSize();
    
    //背景
    auto background = Sprite::create("startBackground.png");
    background->setPosition(Vec2(winSize.width/2.0,winSize.height/2.0));
    this->addChild(background);
    
    //ロゴの追加
    auto startLogo = Sprite::create("Start.png");
    startLogo->setPosition(Vec2(winSize.width/2.0,60));
    this->addChild(startLogo);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](Touch* touch, Event* event){
        
        //何度も押せないように
        this->getEventDispatcher()->removeAllEventListeners();
        
        auto startGame = CallFunc::create([]{
            auto scene = playground::createScene();
            auto transition = TransitionFade::create(0.5,scene);
            Director::getInstance()->replaceScene(transition);
        });
        this->runAction(startGame);
        return true;
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}