//
//  playground.cpp
//  
//
//  Created by iwamototateatsu on 2016/08/12.
//
//

#include "playground.h"
#include "TitleScene.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"

USING_NS_CC;

const float TIME_LIMIT_SECOND = 60;

playground::playground()
:
_player(NULL),
_gold(NULL),
_img(NULL),
_degree(0),
_degree1(0),
_x(Director::getInstance()->getWinSize().width / 2.0  - (Director::getInstance()->getWinSize().width / 1.2)  * sinf(_degree / 4)),
_y(Director::getInstance()->getWinSize().height / 2.0  -  (Director::getInstance()->getWinSize().width / 2.2) * cosf(_degree / 4)),
_x1(Director::getInstance()->getWinSize().width / 2.0 - (Director::getInstance()->getWinSize().width / 1.2) * sinf(_degree / 12)),
_y1(Director::getInstance()->getWinSize().height / 2.0 - (Director::getInstance()->getWinSize().width / 2.2) * cosf(_degree / 12)),
_score(0),
_second(TIME_LIMIT_SECOND),
_scoreLabel(NULL),
_secondLabel(NULL),
_rotateTurbo(NULL),
_audio(NULL),
_state(GameState::Ready)
{
    std::random_device rdev;
    _engine.seed(rdev());
}

playground::~playground()
{
    CC_SAFE_RELEASE_NULL(_player);
    CC_SAFE_RELEASE_NULL(_gold);
    CC_SAFE_RELEASE_NULL(_rotateTurbo);
    CC_SAFE_RELEASE_NULL(_scoreLabel);
    CC_SAFE_RELEASE_NULL(_secondLabel);
    
    _img = NULL;
    _degree = 0;
    _degree1 = 0;
}

Scene* playground::createScene()
{
    auto scene = Scene::create();
    auto layer = playground::create();
    scene->addChild(layer);
    return scene;
}

bool playground::init()
{
    _audio = CocosDenshion::SimpleAudioEngine::getInstance();
    
    _audio->playBackgroundMusic("bgm_cyber.mp3");
    
    if(!Layer::init()){
        return false;
    }
    
    _state = GameState::Playing;
    
    //Directorを取り出す。
    auto director = Director::getInstance();
    //画面サイズを取り出す。
    auto size = director -> getWinSize();
    //背景のスプライド生成
    auto background = Sprite::create("background.png");

    //スプライドの表示位置
    background->setPosition(Vec2(size.width / 2.0,size.height/2.0));
    
    //親ノードに追加。
    this->addChild(background);
    
    SpriteBatchNode* batchnode = SpriteBatchNode::create("gold.png");
    this->setGold(batchnode);
    _gold->setName("gold");
    
    SpriteBatchNode* batchnodeTurbo = SpriteBatchNode::create("turbo.png");
    this->setRotateTurbo(batchnodeTurbo);
    _rotateTurbo->setName("turbo");
    
    auto scoreLabel = Label::createWithSystemFont(StringUtils::toString(_score), "Marker Felt", 8);
    
    this->setScoreLabel(scoreLabel);
    scoreLabel->enableShadow(Color4B::BLACK,Size(0.5,0.5),3);
    scoreLabel->enableOutline(Color4B::BLACK,1.5);
    scoreLabel->setPosition(Vec2(size.width/2.0 * 1.6, size.height - 16));
    this->addChild(scoreLabel);
    
    auto scoreLabelHeader = Label::createWithSystemFont("SCORE", "Marker Felt", 6);
    scoreLabelHeader->enableShadow(Color4B::BLACK,Size(0.5, 0.5),3);
    scoreLabelHeader->enableOutline(Color4B::BLACK,1.5);
    scoreLabelHeader->setPosition(Vec2(size.width/2.0*1.6,size.height-7));
    this->addChild(scoreLabelHeader);
    
    _second = TIME_LIMIT_SECOND;
    auto secondLabel = Label::createWithSystemFont(StringUtils::toString(_second), "Marker Felt", 8);
    this->setSecondLabel(secondLabel);
    secondLabel->enableShadow(Color4B::BLACK,Size(0.5,0.5),3);
    secondLabel->enableOutline(Color4B::BLACK,1.5);
    secondLabel->setPosition(Vec2(size.width/2.0,size.height-16));
    this->addChild(secondLabel);
    
    auto secondLabelHeader = Label::createWithSystemFont("TIME", "Marker Felt", 6);
    secondLabelHeader->enableShadow(Color4B::BLACK,Size(0.5,0.5),3);
    secondLabelHeader->enableOutline(Color4B::BLACK,1.5);
    secondLabelHeader->setPosition(Vec2(size.width / 2.0, size.height - 7));
    this->addChild(secondLabelHeader);
    
    this->setPlayer(Sprite::create("character.png"));
    _player->setPosition(Vec2(_x1,_y1));
    this->runAction(Sequence::create(Spawn::create(CallFunc::create([this]{
        this->scheduleUpdate();
    }) ,/*DelayTime::create(0.5133)*/DelayTime::create(0.1),NULL),CallFunc::create([this]{
        this->unscheduleUpdate();
    }),NULL));
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this,size](Touch* touch,Event* event){
        if(_state==GameState::Playing){
            this->schedule(schedule_selector(playground::circlePlayer));
        }
        return  true;
    };
    listener->onTouchMoved = [this,size](Touch* touch,Event* event){
    };
    listener->onTouchEnded = [this](Touch* touch,Event* event){
        this->unschedule(schedule_selector(playground::circlePlayer));
    };
    
    director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    this->addChild(_player);
    this->reorderChild(_player, 1);
    
    schedule(schedule_selector(playground::timer));
    
    return true;
}

void playground::update(float dt)
{
        //Directorを取り出す。
        auto director = Director::getInstance();
        //画面サイズを取り出す。
        auto size = director -> getWinSize();
        //circlePlayer(size, dt);
        addGold(size,dt);
}

void playground::addGold(Size size,float dt)
{
        if(rand() % 5 == 1){
            _img = Sprite::createWithTexture(_rotateTurbo->getTexture());
            _img->setName("turbo");
        }
        else{
            _img = Sprite::createWithTexture(_gold->getTexture());
            _img->setName("gold");
        }
    
        auto centerX = size.width/2.0;
        auto centerY = size.height/2.0;
        _img->setAnchorPoint(Vec2(0.5, 0.5));
        _img->setPosition(_x, _y);
        _degree += 1.8;
        _x = centerX - (centerX / 1.2) * sinf(_degree / 4);
        _y = centerY - (centerY / 1.8) * cosf(_degree / 4);
        this->addChild(_img, 0, dt);
}

void playground::circlePlayer(float dt)
{
    //Directorを取り出す。
    auto director = Director::getInstance();
    //画面サイズを取り出す。
    auto size = director -> getWinSize();
    auto centerX = size.width/2.0;
    auto centerY = size.height/2.0;
    _player->setAnchorPoint(Vec2(0.5,0.5));
    _player->setPosition(_x1, _y1);
    _degree1 += 0.8;
    _x1 = centerX - (centerX / 1.2) * sinf(_degree1 / 12);
    _y1 = centerY - (centerY / 1.8) * cosf(_degree1 / 12);
    
    Vec2 busketPosition = _player->getPosition();
    Rect boundingBox = this->getChildByTag(0)->getBoundingBox();
    bool isHit = boundingBox.containsPoint(busketPosition);
    
    if (isHit){
        if (this->getChildByTag(0)->getName() == _gold->getName()) {
            this->removeChildByTag(0);
            addGold(size, dt);
            _score += 1;
        }
        else if(this->getChildByTag(0)->getName() == _rotateTurbo->getName()){
            this->removeChildByTag(0);
            addGold(size, dt);
            _score += 5;
        }
    }
    _scoreLabel -> setString(StringUtils::toString(_score));
}

void playground::timer(float dt)
{
    if (_state == GameState::Playing) {
        _second -= dt;
        int second = static_cast<int>(_second);
        _secondLabel->setString(StringUtils::toString(second));
        
        if(_second < 0){
            _audio->stopBackgroundMusic();
            _second = 0;
            int second = static_cast<int>(_second);
            _secondLabel->setString(StringUtils::toString(second));
            
            this->unschedule(schedule_selector(playground::circlePlayer));
            this->runAction(Sequence::create(DelayTime::create(0.5),FadeOut::create(1.0),CallFunc::create([this]{
                this->onResult();
                
            }), NULL));
        }
    }
}

void playground::onResult()
{
    auto winSize = Director::getInstance()->getWinSize();
    _state = GameState::Result;
    
    //続きボタン
    auto continueButton = MenuItemImage::create("Continue.png","Continue.png",[](Ref* ref){
        auto scene = playground::createScene();
        auto transition = TransitionFade::create(0.5,scene);
        Director::getInstance()->replaceScene(transition);
    });
    
    //戻るボタン
    
    auto titleButton = MenuItemImage::create("GoToTitle.png","GoToTitle.png",[this](Ref*ref){
        auto scene = TitleScene::createScene();
        auto transition = TransitionCrossFade::create(1.0, scene);
        Director::getInstance()->replaceScene(transition);
    });
    
    //ボタン生成
    auto menu = Menu::create(continueButton,titleButton,NULL);
    
    menu->alignItemsVerticallyWithPadding(15);
    menu->setPosition(Vec2(winSize.width/2.0,winSize.height/2.0));
    this->addChild(menu);
}

float playground::generateRandom(float min, float max)
{
    std::uniform_real_distribution<float> dest(min,max);
    return dest(_engine);
}