//
//  BaseVehicle.hpp
//  TurboRace-mobile
//
//  Created by Carlos Eduardo Pinan Indacochea on 9/1/19.
//

#ifndef BaseVehicle_hpp
#define BaseVehicle_hpp

#include "cocos2d.h"

#define MAX_PLAYER_JUMP 140.0f

enum
{
    kActionJumpTag = 1000,
    kActionJumpAnimationTag = 1001,
    kActionIdleAnimationTag = 1002
};

enum ActorStates
{
    kStateNothing = 0,
    kStateIdle = 1,
    kStateJump = 2,
    kStateRun = 3,
    kStateBack = 4
};

class BaseVehicle : public cocos2d::Sprite {
public:
    BaseVehicle(std::string filename);
    virtual ~BaseVehicle();
    
    virtual void dead();
    virtual void doJump();
    virtual void doMove(cocos2d::Point velocity);
    virtual void setLimits(float limitBotY, float height);
    
    virtual cocos2d::Rect getGroundCollision();
    virtual cocos2d::Rect getAirCollision();
    
    CC_SYNTHESIZE(ActorStates, state, State);
    CC_SYNTHESIZE(float, playerY, PlayerY);
    CC_SYNTHESIZE(cocos2d::Texture2D*, defaultTexture, DefaultTexture);
    CC_SYNTHESIZE(cocos2d::Texture2D*, deadTexture, DeadTexture);
    CC_SYNTHESIZE(cocos2d::Action*, jumpAnimation, JumpAnimation);
    CC_SYNTHESIZE(cocos2d::Action*, idleAnimation, IdleAnimation);
    CC_SYNTHESIZE(float, speed, Speed);
    
private:
    void updateShadow();
    void updateEnd(float delta);
    
private:
    cocos2d::JumpBy* jumpByAction;
    cocos2d::Sprite* spriteShadow;
    
    float limitBottomY;
    float limitTopY;
};

#endif /* BaseVehicle_hpp */
