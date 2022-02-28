//
//  SneakyJoystick.cpp
//  TurboRace-mobile
//
//  Created by Carlos Eduardo Pinan Indacochea on 9/1/19.
//

#include "SneakyJoystick.hpp"

#define SJ_PI 3.14159265359f
#define SJ_PI_X_2 6.28318530718f
#define SJ_RAD2DEG 180.0f/SJ_PI
#define SJ_DEG2RAD SJ_PI/180.0f

#define DEFAULT_SNEAKY_JOYSTICK_THUMB_RADIUS 32.0f
#define DEFAULT_SNEAKY_JOYSTICK_DEAD_RADIUS 10.0f

USING_NS_CC;

float _round(float r) {
    return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}

bool SneakyJoystick::initWithRect(cocos2d::Rect rect) {
    _beganPoint = Vec2(0, 0);
    stickPosition = Vec2(0, 0);
    degrees = 0.0f;
    velocity = Vec2(0, 0);
    autoCenter = true;
    isDPad = false;
    hasDeadzone = false;
    numberOfDirections = 4;
    
    joystickRadius = rect.size.width / 2;
    thumbRadius = DEFAULT_SNEAKY_JOYSTICK_THUMB_RADIUS;
    deadRadius = 0.0f;
    setPosition(rect.origin);
    setTouchEnabled(true);
    return true;
}

bool SneakyJoystick::isTouchEnabled() {
    return _touchListener != nullptr;
}

void SneakyJoystick::setTouchEnabled(bool status) {
    if(_touchListener != nullptr) {
        getEventDispatcher()->removeEventListener(_touchListener);
        _touchListener = nullptr;
    }
    if(status) {
        _touchListener = EventListenerTouchOneByOne::create();
        _touchListener->setSwallowTouches(true);
        
        _touchListener->onTouchBegan = CC_CALLBACK_2(SneakyJoystick::ccTouchBegan, this);
        _touchListener->onTouchMoved = CC_CALLBACK_2(SneakyJoystick::ccTouchMoved, this);
        _touchListener->onTouchEnded = CC_CALLBACK_2(SneakyJoystick::ccTouchEnded, this);
        _touchListener->onTouchCancelled = CC_CALLBACK_2(SneakyJoystick::ccTouchCancelled, this);
        
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(_touchListener, this);
    }
}

bool SneakyJoystick::ccTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) {
    Vec2 location = convertToNodeSpace(Director::getInstance()->convertToGL(touch->getLocationInView()));
    
    if(location.x < -joystickRadius ||
       location.x > joystickRadius ||
       location.y < -joystickRadius ||
       location.y > joystickRadius){
        return false;
    }else{
        float delta = location.x * location.x + location.y * location.y;
        if(_joystickRadiusSquare > delta){
            _beganPoint = location;
            _updateVelocity(location);
            return true;
        }
    }
    return false;
}

void SneakyJoystick::ccTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) {
    _beganPoint = Vec2(0, 0);
    Vec2 location = Vec2(0, 0);
    if(!autoCenter) {
        location = convertToNodeSpace(Director::getInstance()->convertToGL(touch->getLocationInView()));
    }
    _updateVelocity(location);
}

void SneakyJoystick::ccTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event) {
    _updateVelocity(convertToNodeSpace(Director::getInstance()->convertToGL(touch->getLocationInView())));
}

void SneakyJoystick::ccTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event) {
    ccTouchEnded(touch, event);
}

void SneakyJoystick::setIsDPad(bool state) {
    isDPad = state;
    if(isDPad) {
        hasDeadzone = true;
        setDeadRadius(DEFAULT_SNEAKY_JOYSTICK_DEAD_RADIUS);
    }
}

void SneakyJoystick::setJoystickRadius(float radius) {
    joystickRadius = radius;
    _joystickRadiusSquare = radius * radius;
}

void SneakyJoystick::setThumbRadius(float radius) {
    thumbRadius = radius;
    _thumbRadiusSquare = radius * radius;
}

void SneakyJoystick::setDeadRadius(float radius) {
    deadRadius = radius;
    _deadRadiusSquare = radius * radius;
}

void SneakyJoystick::_updateVelocity(cocos2d::Point point) {
    float dx = point.x;// - _beganPoint.x;
    float dy = point.y;// - _beganPoint.y;
    float deltaSquare = dx * dx + dy * dy;
    
    if(deltaSquare <= _deadRadiusSquare){
        velocity = Vec2(0, 0);
        degrees = 0.0f;
        stickPosition = thumbStartPosition;
        return;
    }
    
    float angle = atan2f(dy, dx);
    if(angle < 0){
        angle += SJ_PI_X_2;
    }
    if(isDPad){
        float anglePerSector = 360.0f / numberOfDirections * SJ_DEG2RAD;
        angle = round(angle/anglePerSector) * anglePerSector;
    }
    
    float cosAngle = cosf(angle);
    float sinAngle = sinf(angle);
    
    // NOTE: Velocity goes from -1.0 to 1.0.
    if (deltaSquare > _joystickRadiusSquare || isDPad) {
        dx = cosAngle * joystickRadius;
        dy = sinAngle * joystickRadius;
    }
    
    velocity = Vec2(dx / joystickRadius, dy / joystickRadius);
    degrees = angle * SJ_RAD2DEG;
    
    // Update the thumb's position
    Point thumbPosition = thumbStartPosition;
    stickPosition = Vec2(dx + thumbPosition.x, dy + thumbPosition.y);
}

void SneakyJoystick::start() {
    stickPosition = thumbStartPosition;
}
