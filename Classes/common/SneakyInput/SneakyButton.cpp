//
//  SneakyButton.cpp
//  TurboRace-mobile
//
//  Created by Carlos Eduardo Pinan Indacochea on 9/1/19.
//

#include "SneakyButton.hpp"

#define DEFAULT_SNEAKY_BUTTON_RADIUS 32.0f
#define DEFAULT_SNEAKY_BUTTON_RATE_LIMIT 1.0f / 120.f

USING_NS_CC;

bool SneakyButton::initWithRect(Rect rect) {
    bounds = Rect(0, 0, rect.size.width, rect.size.height);
    center = Point(rect.size.width / 2, rect.size.height / 2);
    enabled = true;
    active = false;
    value = false;
    isHoldable = false;
    isToggleable = false;
    radius = DEFAULT_SNEAKY_BUTTON_RADIUS;
    rateLimit = DEFAULT_SNEAKY_BUTTON_RATE_LIMIT;
    setPosition(rect.origin);
    return true;
}

void SneakyButton::limiter(float delta) {
    value = false;
    unschedule(CC_SCHEDULE_SELECTOR(SneakyButton::limiter));
    active = false;
}

void SneakyButton::setRadius(float radius) {
    this->radius = radius;
    radiusSquare = radius * radius;
}

bool SneakyButton::isTouchEnabled() {
    return _touchListener != nullptr;
}

void SneakyButton::setTouchEnabled(bool state) {
    if(_touchListener != nullptr) {
        getEventDispatcher()->removeEventListener(_touchListener);
        _touchListener = nullptr;
    }
    if(state) {
        _touchListener = EventListenerTouchOneByOne::create();
        _touchListener->setSwallowTouches(true);
        
        _touchListener->onTouchBegan = CC_CALLBACK_2(SneakyButton::ccTouchBegan, this);
        _touchListener->onTouchMoved = CC_CALLBACK_2(SneakyButton::ccTouchMoved, this);
        _touchListener->onTouchEnded = CC_CALLBACK_2(SneakyButton::ccTouchEnded, this);
        _touchListener->onTouchCancelled = CC_CALLBACK_2(SneakyButton::ccTouchCancelled, this);
        
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(_touchListener, this);
    }
}

bool SneakyButton::ccTouchBegan(Touch *touch, Event *event) {
    if(active) {
        return false;
    }
    Vec2 location = convertToNodeSpace(Director::getInstance()->convertToGL(touch->getLocation()));
    if(location.x < -radius || location.x > radius || location.y < -radius || location.y > radius) {
        return false;
    } else {
        float delta = location.x * location.x + location.y + location.y;
        if(radiusSquare > delta) {
            active = true;
            if(!isHoldable && !isToggleable) {
                value = true;
                schedule(CC_SCHEDULE_SELECTOR(SneakyButton::limiter), rateLimit);
            }
            if(isHoldable) {
                value = true;
            }
            if(isToggleable) {
                value = !value;
            }
            return true;
        }
    }
    return false;
}

void SneakyButton::ccTouchMoved(Touch *touch, Event *event) {
    if(active) {
        Vec2 location = convertToNodeSpace(Director::getInstance()->convertToGL(touch->getLocation()));
        if(!(location.x < -radius || location.x > radius || location.y < -radius || location.y > radius)) {
            float delta = location.x * location.x + location.y + location.y;
            if(radiusSquare > delta) {
                if(isHoldable) {
                    value = true;
                }
            } else {
                if(isHoldable) {
                    value = false;
                    active = false;
                }
            }
        }
    }
}

void SneakyButton::ccTouchEnded(Touch *touch, Event *event) {
    if(!active) {
        return;
    }
    if(isHoldable) {
        value = false;
    }
    if(isHoldable || isToggleable) {
        active = false;
    }
}

void SneakyButton::ccTouchCancelled(Touch *touch, Event *event) {
    ccTouchEnded(touch, event);
}

