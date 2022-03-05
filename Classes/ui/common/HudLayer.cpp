//
//  HudLayer.cpp
//  TurboRace
//
//  Created by Carlos Eduardo Pinan Indacochea on 21/02/22.
//

#include "HudLayer.hpp"
#include "../../common/Constants.h"
#include "../../common/SneakyInput/SneakyJoystickSkinnedBase.hpp"
#include "../../common/LocalStorageManager.hpp"
#include "audio/include/AudioEngine.h"

#define JOYPAD_BG "joystick.png"
#define JOYPAD_THUMB "joy_L.png"
#define JOYPAD_OPACITY 255

USING_NS_CC;

HudLayer::HudLayer() {
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    joypadBG = Sprite::create(JOYPAD_BG);
    joypadBG->setOpacity(JOYPAD_OPACITY);
    joypadThumb = Sprite::create(JOYPAD_THUMB);
    joypadThumb->setOpacity(JOYPAD_OPACITY);
    
    Point joypadPosition = Vec2(origin.x + joypadBG->getContentSize().width * 0.9f, origin.y + joypadBG->getContentSize().height * 0.6f);
    
    SneakyJoystickSkinnedBase* baseStick = SneakyJoystickSkinnedBase::create();
    
    joypad = new SneakyJoystick();
    joypad->initWithRect(Rect(0, 0, joypadBG->getContentSize().width, joypadBG->getContentSize().height));
    joypad->autorelease();
    
    baseStick->setBackgroundSprite(joypadBG);
    baseStick->setThumbSprite(joypadThumb);
    baseStick->setJoystick(joypad);
    baseStick->setPosition(joypadPosition);
    
    joypad = baseStick->getJoystick();
    joypad->setEnabled(true);
    joypad->setThumbStartPosition(baseStick->thumbStartPosition);
    joypad->start();
    
    addChild(baseStick);
}

void HudLayer::runTutorialJoypad() {
    joypadBG->setOpacity(255);
    joypadThumb->setOpacity(255);
    
    joypadBG->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5f, 1.2f), ScaleTo::create(0.5f, 1.0f), NULL)));
    
    joypadThumb->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5f, 1.2f), ScaleTo::create(0.5f, 1.0f), NULL)));
    
}

void HudLayer::stopTutorialJoypad() {
    joypadBG->stopAllActions();
    joypadBG->setScale(1);
    joypadBG->setOpacity(JOYPAD_OPACITY);
    
    joypadThumb->stopAllActions();
    joypadThumb->setScale(1);
    joypadThumb->setOpacity(JOYPAD_OPACITY);
    
    joypad->setEnabled(true);
}

void HudLayer::updateControl(BaseVehicle& player, float dt) {
    Point velocity = joypad->getVelocity();
    velocity = joypad->getVelocity() * player.getSpeed();
    player.doMove(velocity);
}

void HudLayer::updateVisibility() {
    if(!LocalStorageManager::isUsingJoypad()) {
        joypad->setVisible(false);
    } else {
        joypad->setVisible(true);
    }
}
