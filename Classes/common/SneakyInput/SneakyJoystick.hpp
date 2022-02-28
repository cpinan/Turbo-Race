//
//  SneakyJoystick.hpp
//  TurboRace-mobile
//
//  Created by Carlos Eduardo Pinan Indacochea on 9/1/19.
//

#ifndef SneakyJoystick_hpp
#define SneakyJoystick_hpp

#include "cocos2d.h"

class SneakyJoystick : public cocos2d::Node {
public:
    CC_SYNTHESIZE(bool, enabled, Enabled);
    
    CC_SYNTHESIZE_READONLY(cocos2d::Point, stickPosition, StickPosition);
    CC_SYNTHESIZE_READONLY(float, degrees, Degrees);
    CC_SYNTHESIZE_READONLY(cocos2d::Point, velocity, Velocity);
    CC_SYNTHESIZE(bool, autoCenter, AutoCenter);
    CC_SYNTHESIZE_READONLY(bool, isDPad, IsDPad);
    CC_SYNTHESIZE(bool, hasDeadzone, HasDeadzone);
    CC_SYNTHESIZE(int, numberOfDirections, NumberOfDirections);
    CC_SYNTHESIZE(cocos2d::Point, thumbStartPosition, ThumbStartPosition);
    
    CC_SYNTHESIZE_READONLY(float, joystickRadius, JoystickRadius);
    CC_SYNTHESIZE_READONLY(float, thumbRadius, ThumbRadius);
    CC_SYNTHESIZE_READONLY(float, deadRadius, DeadRadius);
        
    bool initWithRect(cocos2d::Rect rect);
    
    bool isTouchEnabled();
    void setTouchEnabled(bool status);
    
    virtual bool ccTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    virtual void ccTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    virtual void ccTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    virtual void ccTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event);
    
    void setIsDPad(bool state);
    void setJoystickRadius(float radius);
    void setThumbRadius(float radius);
    void setDeadRadius(float radius);
    
    void start();
    
private:
    cocos2d::Point _beganPoint;
    float _joystickRadiusSquare;
    float _thumbRadiusSquare;
    float _deadRadiusSquare;
    
    void _updateVelocity(cocos2d::Point point);
    
    cocos2d::EventListenerTouchOneByOne* _touchListener;
};

#endif /* SneakyJoystick_hpp */
