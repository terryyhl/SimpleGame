
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "GameOverScene.h"

#include <android/log.h>
#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)


using namespace cocos2d;
using namespace CocosDenshion;

HelloWorld:: HelloWorld():
	_targets(NULL),
	_projectiles(NULL),
	_projectilesDestroyed(0){
}

HelloWorld::~HelloWorld(){
	if(_targets){
		_targets->release();
		_targets = NULL;
	}
	if(_projectiles){
		_projectiles->release();
		_projectiles = NULL;
	}
}

CCScene* HelloWorld::scene()
{
    CCScene *scene = NULL;
	do{
		scene = CCScene::create();
		CC_BREAK_IF(!scene); //if create faile break
		HelloWorld *layer = HelloWorld::create();
		CC_BREAK_IF(!layer);
		scene->addChild(layer); //add layer to scene
		
	}while(0);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    bool bRet = false;
	do{
		//set background color white if faile break
			CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("pew-pew-lei.wav");
		CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(255,255,255,255)));

		CCMenuItemImage *pCloseItem=CCMenuItemImage::create(
			"CloseNormal.png",
			"CloseSelected.png",
			this,
			menu_selector(HelloWorld::menuCloseCallback));
		CC_BREAK_IF(!pCloseItem);

        //add menu && set menu position
		CCSize winVisibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin=CCDirector::sharedDirector()->getVisibleOrigin();

		/*CCPoint test=ccp(
			origin.x+visibleSize.width-pCloseItem->getContentSize().width/2,
			origin.y+pCloseItem->getContentSize().height/2);
		LOGD("x value is %f && y value is %f",test.x,test.y);
		LOGD("origin.x value is %f && origin.y value is %f",origin.x,origin.y);
		LOGD("win.width value is %f && win.height value is %f",visibleSize.width,visibleSize.height);
            */
        //left 0 ,right winVisibleSize.x,top winVisibleSize.Y bottom 0
		pCloseItem->setPosition(ccp(
			winVisibleSize.width-pCloseItem->getContentSize().width/2,
			pCloseItem->getContentSize().height));
		
		CCMenu* pMenu=CCMenu::create(pCloseItem,NULL);
		pMenu->setPosition(CCPointZero);
		this->addChild(pMenu,1);
		//end 


		//add sprite
		LOGD("origin.x value %f,origin.y value %f",origin.x,origin.y);
		CCSprite *player=CCSprite::create("Player.png",CCRectMake(0,0,27,40));
		player->setPosition(ccp(
			origin.x+player->getContentSize().width/2, 
			winVisibleSize.height/2 + player->getContentSize().height/2)
			);
		this->addChild(player);

		//end

       //add 1 second add target
	 this->schedule(schedule_selector(HelloWorld::gameLogic),1.0);
      //end 
      
		this->setTouchEnabled(true);

		_targets=new cocos2d::CCArray;
		_projectiles=new cocos2d::CCArray;

      //every frame update game
	  this->schedule(schedule_selector(HelloWorld::updateGame));
	  //end

      //play background music
	  CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background-music-aac.wav",true);
	  

	  //end

	  bRet=true;
	}while(0);
    
    return bRet;
}


void HelloWorld::gameLogic(float dt){
	this->addTarget();
}

void HelloWorld::updateGame(float dt){
	CCArray *projectileToDelete =new CCArray;
	CCObject* it=NULL;
	CCObject* jt = NULL;
	CCARRAY_FOREACH(_projectiles,it){
		CCSprite *projectile=dynamic_cast<CCSprite*>(it);
		CCRect projectileRect =CCRectMake(
			projectile->getPosition().x - (projectile->getContentSize().width/2),
			projectile->getPosition().y-(projectile->getContentSize().height/2),
			projectile->getContentSize().width,
			projectile->getContentSize().height);

		CCArray* targetsToDelete=new CCArray;
		
		CCARRAY_FOREACH(_targets, jt)
		{
			CCSprite *target = dynamic_cast<CCSprite*>(jt);
			
			CCRect targetRect = CCRectMake(
			target->getPosition().x - (target->getContentSize().width/2),
			target->getPosition().y - (target->getContentSize().height/2),
			target->getContentSize().width,
			target->getContentSize().height);
			if (projectileRect.intersectsRect(targetRect))
			{
				targetsToDelete->addObject(target);
			}
		}

		 CCARRAY_FOREACH(targetsToDelete, jt)
         {
             CCSprite *target = dynamic_cast<CCSprite*>(jt);
             _targets->removeObject(target);
             this->removeChild(target, true);

              _projectilesDestroyed++;
              if (_projectilesDestroyed >= 10)
              {
                GameOverScene *gameOverScene=GameOverScene::create();
              	gameOverScene->getLayer()->getLabel()->setString("you win!!!");
				CCDirector::sharedDirector()->replaceScene(gameOverScene);
              }
          }
		 if(targetsToDelete->count() > 0){
			projectileToDelete->addObject(projectile);
		 }
		 targetsToDelete->release();
	}

    //remove projectile
	CCARRAY_FOREACH(projectileToDelete,it){
		CCSprite *projectile =dynamic_cast<CCSprite*>(it);
		_projectiles->removeObject(projectile);
		this->removeChild(projectile,true);
	}
	projectileToDelete->release();
	//end
 }
	




void HelloWorld::ccTouchesEnded(cocos2d::CCSet * touches,cocos2d::CCEvent * event){
	CCTouch* touch=(CCTouch*)(touches->anyObject());
	CCPoint location = touch ->getLocation();

	CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
	CCSprite *projectile = CCSprite::create("Projectile.png",CCRectMake(0,0,20,20));
	projectile ->setPosition(
		ccp(20,winSize.height/2+projectile->getContentSize().height/2)
		);

	float offX=location.x - projectile->getPosition().x;
	float offY=location.y - projectile->getPosition().y;

	if(offX <=0)return;

	this->addChild(projectile);
     
	float realX=winSize.width+(projectile->getContentSize().width/2);
	float ratio = offY/offX;
	float realY=(realX*ratio)+projectile->getPosition().y;
	CCPoint realDest =ccp(realX,realY);

	projectile->runAction(CCSequence::create(
		CCMoveTo::create(1.0,realDest),
		CCCallFuncN::create(this,callfuncN_selector(HelloWorld::spriteMoveFinished)),
		NULL
	));

	projectile->setTag(2);
	_projectiles->addObject(projectile);


	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pew-pew-lei.wav",false);
}


void HelloWorld::registerWithTouchDispatcher(){
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
}

void HelloWorld::addTarget(){
	//create target && set position Y random
	CCSprite *target=CCSprite::create("Target.png",CCRectMake(0,0,27,40));
	CCSize winSize=CCDirector::sharedDirector()->getVisibleSize();
	float minY=target->getContentSize().height/2;
	float maxY=winSize.height -target->getContentSize().height/2;

	int rangeY=(int)(maxY-minY);
	int actualy=(rand()%rangeY)+(int)minY;

	target->setPosition(ccp(
		winSize.width+(target->getContentSize().width/2),
		CCDirector::sharedDirector()->getVisibleOrigin().y+actualy
	));

	this->addChild(target);

	//end

	//set target move duration
	int minDuration=(int)2.0;
	int maxDuration=(int)4.0;
	int rangeDuration=maxDuration-minDuration;
	int actualDuration=(rand() % rangeDuration) +minDuration;

    CCFiniteTimeAction* actionMove = CCMoveTo::create((float)actualDuration,
		ccp(0-target->getContentSize().width/2,actualy));

	CCFiniteTimeAction* actionMoveDone=CCCallFuncN::create(
		this,callfuncN_selector(HelloWorld::spriteMoveFinished));
	target->runAction(CCSequence::create(actionMove,actionMoveDone,NULL));

	target->setTag(1);
	_targets->addObject(target);
	//end
	
}


void HelloWorld::spriteMoveFinished(cocos2d::CCNode* sender){
	CCSprite *sprite=(CCSprite *)sender;
	this->removeChild(sprite,true);

	if(sprite->getTag()==1){
		_targets->removeObject(sprite);
		//game over scene
		
	}
	else if(sprite ->getTag()==2){
		_projectiles->removeObject(sprite);
	}
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
