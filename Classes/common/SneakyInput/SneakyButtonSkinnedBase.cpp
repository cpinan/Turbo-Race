//
//  SneakyButtonSkinnedBase.cpp
//  TurboRace-mobile
//
//  Created by Carlos Eduardo Pinan Indacochea on 9/1/19.
//

#include "SneakyButtonSkinnedBase.hpp"

USING_NS_CC;

SneakyButtonSkinnedBase::~SneakyButtonSkinnedBase() {
}

bool SneakyButtonSkinnedBase::init() {
    if(Layer::init()) {
        schedule(CC_SCHEDULE_SELECTOR(SneakyButtonSkinnedBase::watchSelf));
    }
    return false;
}

void SneakyButtonSkinnedBase::watchSelf(float delta) {
    if(button != nullptr) {
        if(!button->getEnabled()) {
            if(disabledSprite) {
                disabledSprite->setVisible(true);
            }
        } else {
            if(button->getIsActive()) {
                if(pressSprite) {
                    pressSprite->setVisible(false);
                }
                if(!button->getValue()) {
                    if(activatedSprite) {
                        activatedSprite->setVisible(false);
                    }
                    if(defaultSprite) {
                        defaultSprite->setVisible(true);
                    }
                } else {
                    if(activatedSprite) {
                        activatedSprite->setVisible(true);
                    }
                }
            } else {
                if(defaultSprite) {
                    defaultSprite->setVisible(false);
                    if(pressSprite) {
                        pressSprite->setVisible(true);
                    }
                }
            }
        }
    }
}

void SneakyButtonSkinnedBase::setContentSize(cocos2d::Size size) {
    Layer::setContentSize(size);
    defaultSprite->setContentSize(size);
}

void SneakyButtonSkinnedBase::setDefaultSprite(cocos2d::Sprite *sprite) {
    if(defaultSprite) {
        if(defaultSprite->getParent()) {
            defaultSprite->getParent()->removeChild(defaultSprite);
        }
        defaultSprite->release();
    }
    if(sprite) {
        defaultSprite = sprite;
        addChild(sprite, 0);
        setContentSize(sprite->getContentSize());
    }
}

void SneakyButtonSkinnedBase::setActivatedSprite(cocos2d::Sprite *sprite) {
    if(activatedSprite) {
        if(activatedSprite->getParent()) {
            activatedSprite->getParent()->removeChild(activatedSprite);
        }
        activatedSprite->release();
    }
    if(sprite) {
        activatedSprite = sprite;
        addChild(sprite, 1);
        setContentSize(sprite->getContentSize());
    }
}

void SneakyButtonSkinnedBase::setDisabledSprite(cocos2d::Sprite *sprite) {
    if(disabledSprite) {
        if(disabledSprite->getParent()) {
            disabledSprite->getParent()->removeChild(disabledSprite);
        }
        disabledSprite->release();
    }
    if(sprite) {
        disabledSprite = sprite;
        addChild(sprite, 2);
        setContentSize(sprite->getContentSize());
    }
}

void SneakyButtonSkinnedBase::setPressSprite(cocos2d::Sprite *sprite) {
    if(pressSprite) {
        if(pressSprite->getParent()) {
            pressSprite->getParent()->removeChild(pressSprite);
        }
        pressSprite->release();
    }
    if(sprite) {
        pressSprite = sprite;
        addChild(sprite, 3);
        setContentSize(sprite->getContentSize());
    }
}

void SneakyButtonSkinnedBase::setButton(SneakyButton *button) {
    if(this->button->getParent()) {
        this->button->getParent()->removeChild(this->button);
    }
    if(button) {
        this->button = button;
        addChild(button, 4);
        if(defaultSprite) {
            button->setRadius(defaultSprite->getBoundingBox().size.width / 2.0f);
        }
    }
}
