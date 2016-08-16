//
//  TitleScene.h
//  
//
//  Created by iwamototateatsu on 2016/08/15.
//
//

#ifndef ____TitleScene__
#define ____TitleScene__

#include <stdio.h>

class TitleScene:public cocos2d::Layer
{

protected:
    TitleScene();
    virtual ~TitleScene();
    bool init() override;
  
public:
    static cocos2d::Scene* createScene();
    CREATE_FUNC(TitleScene);
};

#endif /* defined(____TitleScene__) */
