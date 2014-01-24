#include "GameScene.h"

USING_NS_CC;

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
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    currentBackground = Sprite::create("BackgroundIce.png");
    currentBackground->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(currentBackground, 0);

    nextBackground = Sprite::create("BackgroundIce.png");
    setNextBackground();
    this->addChild(nextBackground, 0);
    
    backgroundSpeed = 200;
    
    this->schedule(schedule_selector(GameScene::updateGame));
    
    return true;
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
}
