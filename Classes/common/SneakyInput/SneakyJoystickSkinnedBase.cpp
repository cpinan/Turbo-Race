//
//  SneakyJoystickSkinnedBase.cpp
//  TurboRace-mobile
//
//  Created by Carlos Eduardo Pinan Indacochea on 9/1/19.
//

#include "SneakyJoystickSkinnedBase.hpp"

USING_NS_CC;

SneakyJoystickSkinnedBase::~SneakyJoystickSkinnedBase() {
}

bool SneakyJoystickSkinnedBase::init() {
    if(Layer::init()) {
        schedule(CC_SCHEDULE_SELECTOR(SneakyJoystickSkinnedBase::updatePositions));
        return true;
    }
    return false;
}

void SneakyJoystickSkinnedBase::updatePositions(float delta) {
    if(joystick && thumbSprite) {
        thumbSprite->setPosition(joystick->getStickPosition());
    }
}

void SneakyJoystickSkinnedBase::setContentSize(cocos2d::Size size) {
    Layer::setContentSize(size);
    if(backgroundSprite) {
        backgroundSprite->setContentSize(size);
    }
}

void SneakyJoystickSkinnedBase::setBackgroundSprite(cocos2d::Sprite *sprite) {
    if(backgroundSprite){
        if(backgroundSprite->getParent())
            backgroundSprite->getParent()->removeChild(backgroundSprite, true);
    }
    backgroundSprite = sprite;
    if(sprite){
        this->addChild(backgroundSprite, 0);
        this->setContentSize(backgroundSprite->getContentSize());
    }
}

void SneakyJoystickSkinnedBase::setThumbSprite(cocos2d::Sprite *sprite) {
    if(thumbSprite){
        if(thumbSprite->getParent())
            thumbSprite->getParent()->removeChild(thumbSprite, true);
    }
    thumbSprite = sprite;
    if(sprite){
        thumbStartPosition.x = backgroundSprite->getContentSize().width * 0.5f;
        thumbStartPosition.y = backgroundSprite->getContentSize().height * 0.5f;
        thumbSprite->setPosition(thumbStartPosition);
        backgroundSprite->addChild(thumbSprite, 1);
        
        //joystick->setThumbRadius(thumbSprite->getContentSize().width/2);
    }
}

void SneakyJoystickSkinnedBase::setJoystick(SneakyJoystick *joystick) {
    this->joystick = joystick;
    if(joystick){
        this->addChild(joystick, 2);
        if(thumbSprite) {
            joystick->setThumbRadius(thumbSprite->getBoundingBox().size.width/2);
        }
        else
            joystick->setThumbRadius(0);
        
        if(backgroundSprite)
            joystick->setJoystickRadius(backgroundSprite->getBoundingBox().size.width/2);
    }
}
