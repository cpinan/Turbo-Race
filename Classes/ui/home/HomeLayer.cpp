//
//  HomeLayer.cpp
//  TurboRace
//
//  Created by Carlos Eduardo Pinan Indacochea on 21/02/22.
//

#include "HomeLayer.hpp"
#include "../common/HudLayer.hpp"
#include "../game/GameLayer.hpp"
#include "../settings/SettingsLayer.hpp"
#include "../../models/VehicleFrog.hpp"
#include "../../common/LocalStorageManager.hpp"
#include "audio/include/AudioEngine.h"

#define HIDE_TIME 0.4f

USING_NS_CC;

HomeLayer::HomeLayer(GameLayer* gameLayer) : _gameLayer(gameLayer) {
    getEventDispatcher()->addCustomEventListener(NOTIFICATION_ENABLE_BUTTONS, [this](EventCustom* event) {
        _enableButtons();
      });
    
    getEventDispatcher()->addCustomEventListener(NOTIFICATION_HOW_TO_PLAY, [this](EventCustom* event) {
        _manageHowToPlay();
    });
    
    disable = false;
        
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point visibleOrigin = Director::getInstance()->getVisibleOrigin();
    
    // Tablero Izq.
    tablero = Sprite::create("tablero_title.png");
    tablero->setAnchorPoint(Vec2(0,0));
    tablero->setPositionX(visibleOrigin.x);
    tablero->setPositionY(visibleOrigin.y);
    addChild(tablero);
    
    // Crear logo
    logo = Sprite::create("logo.png");
    logo->setPositionX(-logo->getContentSize().width * 1.2f);
    logo->setPositionY(visibleOrigin.y + visibleSize.height * 0.55f);
    
    Point finalPos = Vec2(visibleOrigin.x + visibleSize.width * 0.65f, logo->getPositionY());
    MoveTo* move1 = MoveTo::create(0.9f, finalPos);
    
    logo->runAction(Sequence::create(DelayTime::create(0.25f), move1, NULL));
    
    addChild(logo , -1);
    
    // Achievements
    menuItemAchievements = MenuItemImage::create("achievement_off.png", "achievement.png", CC_CALLBACK_1(HomeLayer::_onOptionPressed, this));
    menuItemAchievements->setTag(kTagAchievements);
    menuItemAchievements->setPositionX(visibleOrigin.x + menuItemAchievements->getContentSize().width * 0.8f);
    menuItemAchievements->setPositionY(visibleOrigin.y + menuItemAchievements->getContentSize().height * 0.75f);
    menuItemAchievements->setEnabled(false);
    
    // Leaderboards
    menuItemLeaderboard = MenuItemImage::create("chart_off.png", "chart.png", CC_CALLBACK_1(HomeLayer::_onOptionPressed, this));
    menuItemLeaderboard->setTag(kTagLeaderboard);
    menuItemLeaderboard->setPositionX(menuItemAchievements->getPositionX() + menuItemAchievements->getContentSize().width * 1.2f);
    menuItemLeaderboard->setPositionY(menuItemAchievements->getPositionY());
    menuItemLeaderboard->setEnabled(false);
    
    float scale = 1.05f;
    float time_dt = 1.3f;
    
    // Hard Mode
    menuItemHard = MenuItemImage::create("hard_off.png", "hard.png", CC_CALLBACK_1(HomeLayer::_onOptionPressed, this));
    menuItemHard->setTag(kTagHardMode);
    menuItemHard->setAnchorPoint(Vec2(0, 0));
    menuItemHard->setPositionX(menuItemAchievements->getPositionX() - menuItemAchievements->getContentSize().width * 0.5f);
    menuItemHard->setPositionY(menuItemAchievements->getPositionY() + menuItemAchievements->getContentSize().height * 0.75f);
    menuItemHard->setRotation(-3);
    menuItemHard->runAction(RepeatForever::create(Sequence::create(DelayTime::create(2 * time_dt), ScaleTo::create(0.5f * time_dt, scale), ScaleTo::create(0.5f * time_dt, 1.0f), DelayTime::create(0), NULL)));
    
    float dtHeight = 1.35f;
    if(visibleSize.height >= 768)
        dtHeight = 1.7f;
    
    // Normal Mode
    menuItemNormal = MenuItemImage::create("medium_off.png", "medium.png", CC_CALLBACK_1(HomeLayer::_onOptionPressed, this));
    menuItemNormal->setTag(kTagNormalMode);
    menuItemNormal->setAnchorPoint(Vec2(0, 0));
    menuItemNormal->setPositionX(menuItemHard->getPositionX());
    menuItemNormal->setPositionY(menuItemHard->getPositionY() + menuItemAchievements->getContentSize().height * dtHeight);
    menuItemNormal->setRotation(-2);
    menuItemNormal->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1 * time_dt), ScaleTo::create(0.5f * time_dt, scale), ScaleTo::create(0.5f * time_dt, 1.0f), DelayTime::create(1 * time_dt), NULL)));
    
    // Easy Mode
    menuItemEasy = MenuItemImage::create("easy_off.png", "easy.png", CC_CALLBACK_1(HomeLayer::_onOptionPressed, this));
    menuItemEasy->setTag(kTagEasyMode);
    menuItemEasy->setAnchorPoint(Vec2(0, 0));
    menuItemEasy->setPositionX(menuItemNormal->getPositionX());
    menuItemEasy->setPositionY(menuItemNormal->getPositionY() + menuItemAchievements->getContentSize().height * dtHeight);
    menuItemEasy->setRotation(-2);
    menuItemEasy->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5f * time_dt, scale), ScaleTo::create(0.5f * time_dt, 1.0f), DelayTime::create(2 * time_dt), NULL)));
    
    // Settings
    menuItemSettings = MenuItemImage::create("ajustes_off.png", "ajustes.png", CC_CALLBACK_1(HomeLayer::_onOptionPressed, this));
    menuItemSettings->setTag(kTagSettings);
    menuItemSettings->setPosition(Vec2(visibleSize.width - menuItemSettings->getContentSize().width * 0.8f, visibleOrigin.y + menuItemSettings->getContentSize().height * 0.7f));
    
    // Rate App
    Label* lblRateApp = Label::createWithTTF("Rate this App!", FONT_GAME, SIZE_RATE_APP);
    lblRateApp->setColor(Color3B(255, 255, 255));
    
    menuRateApp = MenuItemLabel::create(lblRateApp, CC_CALLBACK_1(HomeLayer::_onOptionPressed, this));
    menuRateApp->setTag(kTagRateApp);
    menuRateApp->setPositionX(visibleOrigin.x + visibleSize.width * 0.75f);
    menuRateApp->setPositionY(visibleOrigin.y + menuRateApp->getContentSize().height * 2.2f);
    menuRateApp->runAction(RepeatForever::create(Sequence::create(RotateTo::create(0.5f, -2), RotateTo::create(0.5f, 2), NULL)));
    menuRateApp->setVisible(false);
    
    // How to Player
    Label* lblHowToPlay = Label::createWithTTF("How to Play", FONT_GAME, SIZE_RATE_HOW_TO_PLAY);
    lblHowToPlay->setColor(Color3B(255, 255, 255));
    
    menuHowToPlay = MenuItemLabel::create(lblHowToPlay, CC_CALLBACK_1(HomeLayer::_onOptionPressed, this));
    menuHowToPlay->setTag(kTagHowToPlay);
    menuHowToPlay->setPositionX(menuRateApp->getPositionX() - menuHowToPlay->getContentSize().width * 0.2f);
    menuHowToPlay->setPositionY(menuRateApp->getPositionY());
    menuHowToPlay->runAction(RepeatForever::create(Sequence::create(RotateTo::create(0.5f, -2), RotateTo::create(0.5f, 2), NULL)));
    
    if(LocalStorageManager::showTutorial())
        menuHowToPlay->setVisible(false);
    
    // Sound management
    MenuItemImage* menuSoundOn = MenuItemImage::create("sound_on_off.png", "sound_on.png");
    MenuItemImage* menuSoundOff = MenuItemImage::create("sound_off_off.png", "sound_off.png");
    
    menuSound = MenuItemToggle::createWithCallback(CC_CALLBACK_1(HomeLayer::_manageMusic, this), menuSoundOn, menuSoundOff, nullptr);
    menuSound->setPositionX(menuItemEasy->getPositionX() + menuSound->getContentSize().width * 0.6f);
    menuSound->setPositionY(menuItemLeaderboard->getPositionY());
    
    if(LocalStorageManager::isMute())
        menuSound->setSelectedIndex(1);
    
    // Menu
    menu = Menu::create();
    menu->setPosition(Vec2(0,0));
    menu->addChild(menuItemEasy);
    menu->addChild(menuItemNormal);
    menu->addChild(menuItemHard);
    //menu->addChild(menuItemLeaderboard);
    //menu->addChild(menuItemAchievements);
    menu->addChild(menuItemSettings);
    menu->addChild(menuSound);
    //menu->addChild(menuRateApp);
    menu->addChild(menuHowToPlay);
    
    addChild(menu);
    
    _settingsLayer = new SettingsLayer();
    _settingsLayer->autorelease();
    _settingsLayer->setVisible(false);
    addChild(_settingsLayer, 9999);
}

void HomeLayer::_manageMusic(cocos2d::Ref* pSender) {
    if(disable || _settingsLayer->isVisible())
        return;
    
    AudioEngine::play2d(SFX_BUTTON);
    bool state = LocalStorageManager::isMute();
    LocalStorageManager::setMute(!state);
}

HomeLayer::~HomeLayer() {
    getEventDispatcher()->removeCustomEventListeners(NOTIFICATION_ENABLE_BUTTONS);
    getEventDispatcher()->removeCustomEventListeners(NOTIFICATION_HOW_TO_PLAY);
}

void HomeLayer::_onOptionPressed(Ref *pSender) {
    if(disable || _settingsLayer->isVisible())
        return;
    
    AudioEngine::play2d(SFX_BUTTON);
    
    MenuItem* item = (MenuItem*) pSender;
    bool runGame = false;
    
    switch (item->getTag()) {
            
        case kTagHowToPlay:
            LocalStorageManager::isTutorialOn(true);
            runGame = true;
            _gameLayer->configureGame(kGameLevelEasy);
            break;
        
        case kTagEasyMode:
            runGame = true;
            _gameLayer->configureGame(kGameLevelEasy);
            break;
            
        case kTagNormalMode:
            runGame = true;
            _gameLayer->configureGame(kGameLevelNormal);
            break;
            
        case kTagHardMode:
            runGame = true;
            _gameLayer->configureGame(kGameLevelHard);
            break;
            
        case kTagSettings:
            _disableButtons();
            _settingsLayer->setVisible(true);
            break;
            
        case kTagLeaderboard:
            break;
            
        case kTagAchievements:
            break;
            
        case kTagRateApp:
            break;
            
        default:
            break;
    }
    
    if(runGame) {
        _disableButtons();
        disable = true;
        _hideToLeft();
        _hideToRight();
        scheduleOnce(CC_SCHEDULE_SELECTOR(HomeLayer::_finishHideLayer), HIDE_TIME + 0.1f);
    }
    
}

void HomeLayer::_hideToLeft() {
    MoveBy* move = MoveBy::create(HIDE_TIME, Vec2(-WIN_SIZE.width * 0.8f, 0));
    tablero->runAction(move->clone());
    menuItemEasy->runAction(move->clone());
    menuItemNormal->runAction(move->clone());
    menuItemHard->runAction(move->clone());
    //menuItemAchievements->runAction(move->clone());
    //menuItemLeaderboard->runAction(move->clone());
    menuSound->runAction(move->clone());
}

void HomeLayer::_hideToRight() {
    MoveBy* move = MoveBy::create(HIDE_TIME, Vec2(WIN_SIZE.width * 0.8f, 0));
    menuItemSettings->runAction(move->clone());
    //menuRateApp->runAction(move->clone());
    menuHowToPlay->runAction(move->clone());
    logo->runAction(move->clone());
}

void HomeLayer::_finishHideLayer(float dt) {
    this->setVisible(false);
    _gameLayer->playGame();
    this->removeFromParent();
}

void HomeLayer::_enableButtons() {
    auto children = menu->getChildren();
    for(int i = 0; i < children.size(); i++) {
        MenuItem* item = (MenuItem*) children.at(i);
        item->setEnabled(true);
    }
}

void HomeLayer::_disableButtons() {
    auto children = menu->getChildren();
    for(int i = 0; i < children.size(); i++) {
        MenuItem* item = (MenuItem*) children.at(i);
        item->setEnabled(false);
    }
}

void HomeLayer::_manageHowToPlay() {
    bool state = !LocalStorageManager::showTutorial();
    menuHowToPlay->setVisible(state);
}
