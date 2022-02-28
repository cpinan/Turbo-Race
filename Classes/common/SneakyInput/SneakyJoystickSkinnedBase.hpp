//
//  SneakyJoystickSkinnedBase.hpp
//  TurboRace-mobile
//
//  Created by Carlos Eduardo Pinan Indacochea on 9/1/19.
//

#ifndef SneakyJoystickSkinnedBase_hpp
#define SneakyJoystickSkinnedBase_hpp

#include "cocos2d.h"
#include "SneakyJoystick.hpp"

class SneakyJoystickSkinnedBase : public cocos2d::Layer {
public:
    CC_SYNTHESIZE_READONLY(cocos2d::Sprite *, backgroundSprite, BackgroundSprite);
    CC_SYNTHESIZE_READONLY(cocos2d::Sprite *, thumbSprite, ThumbSprite);
    CC_SYNTHESIZE_READONLY(SneakyJoystick *, joystick, Joystick);
    
    CREATE_FUNC(SneakyJoystickSkinnedBase);
    virtual ~SneakyJoystickSkinnedBase();
    bool init();
    void updatePositions(float delta);
    void setContentSize(cocos2d::Size size);
    void setBackgroundSprite(cocos2d::Sprite *sprite);
    void setThumbSprite(cocos2d::Sprite *sprite);
    void setJoystick(SneakyJoystick *joystick);
    
    cocos2d::Point thumbStartPosition;
};

#endif /* SneakyJoystickSkinnedBase_hpp */
