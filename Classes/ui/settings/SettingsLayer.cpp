//
//  SettingsLayer.cpp
//  TurboRace
//
//  Created by Carlos Eduardo Pinan Indacochea on 21/02/22.
//

#include "SettingsLayer.hpp"
#include "../../common/Constants.h"
#include "../../common/LocalStorageManager.hpp"
#include "audio/include/AudioEngine.h"
#define CONTROL_ALPHA 100

USING_NS_CC;

Scene* SettingsLayer::createScene() {
    Scene* scene = Scene::create();
    
    SettingsLayer* layer = new SettingsLayer();
    layer->autorelease();
    scene->addChild(layer);
    
    return scene;
}

SettingsLayer::SettingsLayer() {
    if(initWithColor(Color4B(0, 0, 0, 128))) {
        bool joypad = LocalStorageManager::isUsingJoypad();
        
        Size visibleSize = Director::getInstance()->getVisibleSize();
        Point visibleOrigin = Director::getInstance()->getVisibleOrigin();
        
        Sprite* background = Sprite::create("controls_options.png");
        background->setPosition(visibleOrigin.x + visibleSize.width * 0.5f, visibleOrigin.y + visibleSize.height * 0.5f);
        addChild(background);
        
        Point center = Vec2(background->getContentSize().width * 0.5f, background->getContentSize().height * 0.5f);
        
        Menu* menu = Menu::create();
        menu->setPosition(0, 0);
        menu->setAnchorPoint(Vec2(0, 0));
        
        // Joypad Mode
        itemJoypad = MenuItemImage::create("control_joystick.png", "control_joystick.png", CC_CALLBACK_1(SettingsLayer::onOptionsTapped, this));
        itemJoypad->setTag(kTagSettingJoypadMode);
        itemJoypad->setPosition(center.x - itemJoypad->getContentSize().width * 0.5f, center.y + itemJoypad->getContentSize().height * 0.05f);
        menu->addChild(itemJoypad);
        if(!joypad) {
            itemJoypad->setOpacity(CONTROL_ALPHA);
        }
        
        // Acceleration Mode
        itemAcceleration = MenuItemImage::create("control_tilt.png", "control_tilt.png", CC_CALLBACK_1(SettingsLayer::onOptionsTapped, this));
        itemAcceleration->setTag(kTagSettingAccelMode);
        itemAcceleration->setPosition(center.x + itemAcceleration->getContentSize().width * 0.6f, itemJoypad->getPositionY());
        menu->addChild(itemAcceleration);
        if(joypad) {
            itemAcceleration->setOpacity(CONTROL_ALPHA);
        }
        
        // Home button
        MenuItemImage* itemHome = MenuItemImage::create("pause_home_off.png", "pause_home.png", CC_CALLBACK_1(SettingsLayer::onOptionsTapped, this));
        itemHome->setTag(kTagSettingGoHome);
        itemHome->setPosition(center.x, center.y - itemHome->getContentSize().height * 1.6f);
        menu->addChild(itemHome);
        
        // Menu
        background->addChild(menu);
    }
}

void SettingsLayer::onOptionsTapped(Ref* sender) {
    MenuItem* item = (MenuItem*) sender;
    AudioEngine::play2d(SFX_BUTTON);
    
    switch(item->getTag()) {
        case kTagSettingGoHome:
            getEventDispatcher()->dispatchCustomEvent(NOTIFICATION_ENABLE_BUTTONS);
            getEventDispatcher()->dispatchCustomEvent(NOTIFICATION_HOW_TO_PLAY);
            this->setVisible(false);
            break;
            
        case kTagSettingJoypadMode:
            itemJoypad->setOpacity(255);
            itemAcceleration->setOpacity(CONTROL_ALPHA);
            LocalStorageManager::setControlType(true);
            break;
            
        case kTagSettingAccelMode:
            itemJoypad->setOpacity(CONTROL_ALPHA);
            itemAcceleration->setOpacity(255);
            LocalStorageManager::setControlType(false);
            break;
    }
}
