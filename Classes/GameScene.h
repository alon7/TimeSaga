#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "string"



typedef std::string String;


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
    void spawnObstacle();
    void commitJump(int height);
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void changeTime(int newTime);
    void updateSpriteImage(cocos2d::Sprite* sprite, std::string imageName);
    bool inJump = false;
    
    cocos2d::Sprite* currentBackground;
    cocos2d::Sprite* nextBackground;
    cocos2d::Sprite* playerSprite;
    float currentBackgroundXPosition;
    float backgroundSpeed;
    cocos2d::Size visibleSize;
    cocos2d::Point origin;
    int currentTime;
    
    
    bool obstacleKills[3][3];      //Defines if a certain obstacle in a certain time would kill. Syntax: [OBSTACLE][TIME]
    String obstacleResource[3][3];
    
    

    String background[3] = {"BackgroundIce.png","BackgroundCity.png","BackgroundFuture.png"};
    String human[3] = {"HumanIce.png","HumanCity.png","HumanIce.png"};
    
    cocos2d::Vector<cocos2d::Sprite *> backgrounds;
    cocos2d::Vector<cocos2d::Sprite *> backgrounds_next;
    
};

#endif // __HELLOWORLD_SCENE_H__
