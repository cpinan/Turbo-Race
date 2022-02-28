//
//  SneakyButtonSkinnedBase.hpp
//  TurboRace-mobile
//
//  Created by Carlos Eduardo Pinan Indacochea on 9/1/19.
//

#ifndef SneakyButtonSkinnedBase_hpp
#define SneakyButtonSkinnedBase_hpp

#include "SneakyButton.hpp"

class SneakyButtonSkinnedBase : public cocos2d::Layer {
public:
    CC_SYNTHESIZE_READONLY(cocos2d::Sprite *, defaultSprite, DefaultSprite);
    CC_SYNTHESIZE_READONLY(cocos2d::Sprite *, activatedSprite, ActivatedSprite);
    CC_SYNTHESIZE_READONLY(cocos2d::Sprite *, disabledSprite, DisabledSprite);
    CC_SYNTHESIZE_READONLY(cocos2d::Sprite *, pressSprite, PressSprite);
    CC_SYNTHESIZE_READONLY(SneakyButton *, button, Button);
    
    virtual ~SneakyButtonSkinnedBase();
    bool init();
    void watchSelf(float delta);
    void setContentSize(cocos2d::Size size);
    void setDefaultSprite(cocos2d::Sprite *sprite);
    void setActivatedSprite(cocos2d::Sprite *sprite);
    void setDisabledSprite(cocos2d::Sprite *sprite);
    void setPressSprite(cocos2d::Sprite *sprite);
    void setButton(SneakyButton *button);
    
    CREATE_FUNC(SneakyButtonSkinnedBase);
    
};

#endif /* SneakyButtonSkinnedBase_hpp */
