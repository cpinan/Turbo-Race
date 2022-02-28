//
//  VehicleFrog.cpp
//  TurboRace-mobile
//
//  Created by Carlos Eduardo Pinan Indacochea on 9/1/19.
//

#include "VehicleFrog.hpp"
#include "../common/Constants.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;

VehicleFrog::VehicleFrog() : BaseVehicle("bicho_0001.png")
{
    defaultTexture = getTexture();
    deadTexture = Sprite::create("bicho_0004.png")->getTexture();
    
    Animation* _jumpAnimation = Animation::create();
    _jumpAnimation->addSpriteFrameWithFile("bicho_0003.png");
    _jumpAnimation->setRestoreOriginalFrame(false);
    _jumpAnimation->setDelayPerUnit(1.0f / 10.0f);
    
    jumpAnimation = Animate::create(_jumpAnimation);
    jumpAnimation->retain();
    
    Animation* _idleAnimation = Animation::create();
    
    _idleAnimation->addSpriteFrameWithFile("bicho_0001.png");
    _idleAnimation->addSpriteFrameWithFile("bicho_0002.png");
    
    _idleAnimation->setRestoreOriginalFrame(false);
    _idleAnimation->setDelayPerUnit(1.0f / 12.0f);
    
    idleAnimation = RepeatForever::create(Animate::create(_idleAnimation));
    idleAnimation->retain();
    
    runAction(idleAnimation);
    
}

VehicleFrog::~VehicleFrog()
{
    CC_SAFE_RELEASE(jumpAnimation);
    CC_SAFE_RELEASE(idleAnimation);
}
