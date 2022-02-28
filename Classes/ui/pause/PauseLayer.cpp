//
//  PauseLayer.cpp
//  TurboRace
//
//  Created by Carlos Eduardo Pinan Indacochea on 21/02/22.
//

#include "PauseLayer.hpp"
#include "../../common/Constants.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;

PauseLayer::PauseLayer() {
    if(initWithColor(Color4B(0, 0, 0, 100))) {
        Size visibleSize = Director::getInstance()->getVisibleSize();
        Point visibleOrigin = Director::getInstance()->getVisibleOrigin();
        Point origin = Vec2(visibleOrigin.x + visibleSize.width * 0.5f, visibleOrigin.y + visibleSize.height * 0.5f);
        
        // BGWhite
        Sprite* background = Sprite::create("pause_screen.png");
        background->setPosition(origin);
        addChild(background);
        
        float width = background->getContentSize().width;
        float height = background->getContentSize().height;
        Point backgroundOrigin = Vec2(width * 0.5f, height * 0.5f);
        
        // Play again button
        MenuItemImage* itemPlayAgain = MenuItemImage::create("pause_replay_off.png", "pause_replay.png", CC_CALLBACK_1(PauseLayer::onOptionsTapped, this));
        itemPlayAgain->setTag(kTagPausePlayAgain);
        itemPlayAgain->setPositionX(backgroundOrigin.x - itemPlayAgain->getContentSize().width);
        itemPlayAgain->setPositionY(backgroundOrigin.y - itemPlayAgain->getContentSize().height * 0.9f);
        
        // Resume button
        MenuItemImage* itemResume = MenuItemImage::create("pause_play_off.png", "pause_play.png", CC_CALLBACK_1(PauseLayer::onOptionsTapped, this));
        itemResume->setTag(kTagPauseResumeGame);
        itemResume->setPositionX(itemPlayAgain->getPositionX() + itemResume->getContentSize().width * 1.1f);
        itemResume->setPositionY(itemPlayAgain->getPositionY());
        
        // Home button
        MenuItemImage* itemHome = MenuItemImage::create("pause_home_off.png", "pause_home.png", CC_CALLBACK_1(PauseLayer::onOptionsTapped, this));
        itemHome->setTag(kTagPauseGoHome);
        itemHome->setPositionX(itemResume->getPositionX() + itemResume->getContentSize().width * 1.1f);
        itemHome->setPositionY(itemResume->getPositionY());
        
        itemPlayAgain->setPositionY(itemPlayAgain->getPositionY() - itemPlayAgain->getContentSize().height * 0.3f);
        
        itemResume->setPositionY(itemResume->getPositionY() - itemResume->getContentSize().height * 0.15f);
        
        // Menu
        Menu* menu = Menu::create();
        menu->setAnchorPoint(Vec2(0, 0));
        menu->setPosition(0, 0);
        
        menu->addChild(itemResume);
        menu->addChild(itemHome);
        menu->addChild(itemPlayAgain);
        
        background->addChild(menu);
    }
}

void PauseLayer::onOptionsTapped(Ref* sender) {
    MenuItem* item = (MenuItem*) sender;
    AudioEngine::play2d(SFX_BUTTON);
    
    switch (item->getTag()) {
        case kTagPauseResumeGame:
            getEventDispatcher()->dispatchCustomEvent(NOTIFICATION_RESUME_GAME);
            break;
            
        case kTagPausePlayAgain:
            getEventDispatcher()->dispatchCustomEvent(NOTIFICATION_PLAY_AGAIN);
            break;
            
        case kTagPauseGoHome:
            getEventDispatcher()->dispatchCustomEvent(NOTIFICATION_GO_HOME);
            break;
            
    }
}
