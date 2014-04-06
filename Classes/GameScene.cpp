#include "string"
#include "GameScene.h"
#include <CCActionInterval.h>
#include <stack>
#define TIME_PAST 0
#define TIME_PRESENT 1
#define TIME_FUTURE 2

#define OBSTACLE_FLUID 0
#define OBSTACLE_ANIMAL 1
#define OBSTACLE_PIT 2

USING_NS_CC;

typedef std::stack<Sprite*> SpriteStack;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    obstacleKills[OBSTACLE_FLUID][TIME_PAST] = false;
    obstacleKills[OBSTACLE_FLUID][TIME_PRESENT] = true;
    obstacleKills[OBSTACLE_FLUID][TIME_FUTURE] = true;
    obstacleKills[OBSTACLE_ANIMAL][TIME_PAST] = true;
    obstacleKills[OBSTACLE_ANIMAL][TIME_PRESENT] = false;
    obstacleKills[OBSTACLE_ANIMAL][TIME_FUTURE] = true;
    obstacleKills[OBSTACLE_PIT][TIME_PAST] = true;
    obstacleKills[OBSTACLE_PIT][TIME_PRESENT] = true;
    obstacleKills[OBSTACLE_PIT][TIME_FUTURE] = false;
    
    currentTime = TIME_PAST;
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    
    backgroundSpeed = 250;
    
    Animation* animation = Animation::create();
    
    backgrounds = Vector<Sprite *>(3);
    Sprite* bg = Sprite::create("BackgroundIce.png");
    bg->setVisible(false);
    backgrounds.insert(TIME_PAST, bg);
    bg = Sprite::create("BackgroundCity.png");
    bg->setVisible(false);
    backgrounds.insert(TIME_PRESENT, bg);
    bg = Sprite::create("BackgroundFuture.png");
    bg->setVisible(false);
    backgrounds.insert(TIME_FUTURE, bg);
    
    backgrounds_next = Vector<Sprite *>(3);
    bg = Sprite::create("BackgroundIce.png");
    bg->setVisible(false);
    backgrounds_next.insert(TIME_PAST, bg);
    bg = Sprite::create("BackgroundCity.png");
    bg->setVisible(false);
    backgrounds_next.insert(TIME_PRESENT, bg);
    bg = Sprite::create("BackgroundFuture.png");
    bg->setVisible(false);
    backgrounds_next.insert(TIME_FUTURE, bg);
    
    currentBackground = backgrounds.at(currentTime);
    currentBackground->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y ));
    currentBackground->setVisible(true);
    this->addChild(currentBackground, 0);
    
    nextBackground = backgrounds_next.at(currentTime);
    setNextBackground();
    nextBackground->setScale(-1, 1);
    nextBackground->setVisible(true);
    this->addChild(nextBackground, 0);

    
    for (int i = 1; i <= 4; i++)
    {
        char name[100] = {0};
        sprintf(name, "run_%i.gif", i);
        animation->addSpriteFrameWithFile(name);
    }
    animation->setDelayPerUnit(1/10.0f);
    animation->setRestoreOriginalFrame(true);
    Action* playerAnimation = RepeatForever::create(Animate::create(animation));
    playerAnimation->retain();
    
    playerSprite = Sprite::create("run_1.gif");
    playerSprite->setPosition(Point(75, 50));
    playerSprite->setScale(2, 2);
    this->addChild(playerSprite);
    
    playerSprite->runAction(playerAnimation);
    
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    this->schedule(schedule_selector(GameScene::updateGame));
    
    return true;
}
SpriteStack* s = new SpriteStack();
void GameScene::spawnObstacle()
{
    int obstacleType = rand() % 3;
    Sprite* obstacle = Sprite::create(obstacleResource[obstacleType][currentTime]);
    
    s->push(obstacle);
}



void GameScene::menuCloseCallback(Object* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void GameScene::setNextBackground()
{
    nextBackground->setPosition(Point(currentBackground->getBoundingBox().origin.x + currentBackground->getBoundingBox().size.width/2 + currentBackground->getBoundingBox().size.width, visibleSize.height/2 + origin.y));
}

void GameScene::updateGame(float dt)
{
    float currentX = currentBackground->getBoundingBox().origin.x + currentBackground->getBoundingBox().size.width/2 - (dt * backgroundSpeed);
    currentBackground->setPosition(Point(currentX, visibleSize.height/2 + origin.y));
    setNextBackground();
    if (currentBackground->getBoundingBox().origin.x + currentBackground->getBoundingBox().size.width <= 0)
    {
        Sprite* temp = nextBackground;
        nextBackground = currentBackground;
        currentBackground = temp;
        setNextBackground();
    }
    Sprite* s1 = s->top();
    if(s1->getPosition().x == playerSprite->getPosition().x)
        commitJump(100) ;
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    //log("Key with keycode %d pressed", keyCode);
    if(keyCode == cocos2d::EventKeyboard::KeyCode::KEY_SPACE)
        	commitJump(100);
    
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode) {
        case cocos2d::EventKeyboard::KeyCode::KEY_Z:
            log("Key Z");
            changeTime(TIME_PAST);
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_X:
            log("Key X");
            changeTime(TIME_PRESENT);
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_C:
            log("Key C");
            changeTime(TIME_FUTURE);
            break;
        default:
            break;
    }
}

void GameScene::commitJump(int height)
{
    inJump = true;
    JumpTo* jump = JumpTo::create(2, Point(300, 0), 50, 4);
    jump->startWithTarget(playerSprite);
    log("jumped");
    inJump = false;
}


void GameScene::changeTime(int newTime)
{
    currentTime = newTime;
    
    Sprite* newCurrent = backgrounds.at(newTime);
    Sprite* newNext = backgrounds_next.at(newTime);
    
    newCurrent->setPosition(currentBackground->getPosition());
    newNext->setPosition(nextBackground->getPosition()); //XXX
    
    //currentBackground->setVisible(false);
    nextBackground->setVisible(false);
    
    currentBackground = newCurrent;
    nextBackground = newNext;
    
    currentBackground->setVisible(true);
    nextBackground->setVisible(true);
    
//    FadeOut* currentBgFadeOut = FadeOut::create(0.7);
//    currentBackground->runAction(currentBgFadeOut);
//    FadeOut* nextBgFadeOut = FadeOut::create(0.7);
////    nextBackground->runAction(nextBgFadeOust);
//    Point currentBgPosition = currentBackground->getBoundingBox().origin;
//    Point nextBgPosition = nextBackground->getBoundingBox().origin;
//    
//    currentBackground = (Sprite* )backgrounds.at(newTime);
//    currentBackground->setVisible(true);
//    currentBackground->setPosition(currentBgPosition);
//    currentBackground->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
//    
//    nextBackground = (Sprite *)backgrounds_next.at(newTime);
//    nextBackground->setPosition(nextBgPosition);
//    nextBackground->setVisible(true);
}