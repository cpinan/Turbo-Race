//
//  SneakyButton.hpp
//  TurboRace-mobile
//
//  Created by Carlos Eduardo Pinan Indacochea on 9/1/19.
//

#ifndef SneakyButton_hpp
#define SneakyButton_hpp

#include "cocos2d.h"

class SneakyButton : public cocos2d::Node {
public:
    bool initWithRect(cocos2d::Rect rect);
    void limiter(float delta);
    void setRadius(float radius);
    
    bool isTouchEnabled();
    void setTouchEnabled(bool status);
    
    virtual bool ccTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    virtual void ccTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    virtual void ccTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    virtual void ccTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event);
    
    CC_SYNTHESIZE(bool, enabled, Enabled);
    CC_SYNTHESIZE_READONLY(bool, active, IsActive);
    CC_SYNTHESIZE_READONLY(bool, value, Value);
    CC_SYNTHESIZE(bool, isHoldable, IsHoldable);
    CC_SYNTHESIZE(bool, isToggleable, IsToggleable);
    CC_SYNTHESIZE(float, rateLimit, RateLimit);
    
    CC_SYNTHESIZE_READONLY(float, radius, Radius);
    
protected:
    cocos2d::Point center;
    cocos2d::Rect bounds;
    float radiusSquare;
    
private:
    cocos2d::EventListenerTouchOneByOne* _touchListener;
    
};

#endif /* SneakyButton_hpp */
