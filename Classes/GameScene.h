#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class GameScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(Object* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
    
    void updateGame(float dt);
    
    void setNextBackground();
    
    cocos2d::Sprite* currentBackground;
    cocos2d::Sprite* nextBackground;
    float currentBackgroundXPosition;
    float backgroundSpeed;
    cocos2d::Size visibleSize;
    cocos2d::Point origin;
};

#endif // __HELLOWORLD_SCENE_H__