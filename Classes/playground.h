//
//  playground.h
//  
//
//  Created by iwamototateatsu on 2016/08/12.
//
//

#ifndef ____playground__
#define ____playground__

#include <stdio.h>
#include "SimpleAudioEngine.h"

class playground :public cocos2d::Layer
{
public:
    
    enum class GameState
    {
        Ready,
        Playing,
        Result
    };
    
    playground();
    virtual ~playground();
    bool init() override;
    void update(float dt) override;
    
    static cocos2d::Scene* createScene();
    CREATE_FUNC(playground);
    
    CC_SYNTHESIZE_RETAIN(cocos2d::Sprite *, _player, Player);
    CC_SYNTHESIZE_RETAIN(cocos2d::SpriteBatchNode *, _rotateTurbo, RotateTurbo);
    CC_SYNTHESIZE_RETAIN(cocos2d::Sprite *, _img, Img);
    CC_SYNTHESIZE_RETAIN(cocos2d::SpriteBatchNode *, _gold, Gold);
    
    CC_SYNTHESIZE(float, _degree, Degree);
    CC_SYNTHESIZE(float, _degree1, Degree1);
    
    CC_SYNTHESIZE(int, _x, X);
    CC_SYNTHESIZE(int, _y, Y);
    
    CC_SYNTHESIZE(int, _x1, X1);
    CC_SYNTHESIZE(int, _y1, Y1);
    
    CC_SYNTHESIZE(int, _score, Score);
    CC_SYNTHESIZE(float, _second, Second);
    
    CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _scoreLabel, ScoreLabel);
    CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _secondLabel, SecondLabel);
    
    CC_SYNTHESIZE(std::mt19937, _engine, Engine);
    CC_SYNTHESIZE(GameState, _state, State);

    CC_SYNTHESIZE(CocosDenshion::SimpleAudioEngine *, _audio, Audio);
    
private:
    void addGold(cocos2d::Size size,float dt);
    void circlePlayer(float dt);
    void delaychildremove(float dt);
    void timer(float dt);
    void hitGold();
    void onResult();
    
    float generateRandom(float min,float max);
};

#endif /* defined(____playground__) */
