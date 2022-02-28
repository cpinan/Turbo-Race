//
//  BaseVehicle.cpp
//  TurboRace-mobile
//
//  Created by Carlos Eduardo Pinan Indacochea on 9/1/19.
//

#include "BaseVehicle.hpp"
#include "../common/Constants.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;

BaseVehicle::BaseVehicle(std::string filename)
{
    if(initWithFile(filename.c_str()))
    {
        limitBottomY = 0;
        limitTopY = 0;
        
        defaultTexture = NULL;
        jumpAnimation = NULL;
        idleAnimation = NULL;
        deadTexture = NULL;
        speed = 11.0f;
        state = kStateIdle;
        
        playerY = getPositionY() - getContentSize().height * 0.5f;
        
        jumpByAction = JumpBy::create(0.6f, Vec2(0, 0), MAX_PLAYER_JUMP, 1);
        jumpByAction->setTag(kActionJumpTag);
        jumpByAction->retain();
        
        spriteShadow = Sprite::create(SP_SHADOW);
        spriteShadow->setPositionX(getContentSize().width * 0.5f);
        addChild(spriteShadow, -1);
        updateShadow();
        
    }
}

BaseVehicle::~BaseVehicle()
{
    CC_SAFE_RELEASE(jumpByAction);
}

void BaseVehicle::doJump()
{
    int y = getPositionY() - getPlayerY() - getContentSize().height * 0.5f;
    if(y <= 1 && state != kStateJump/* && getActionByTag(kActionJumpTag) == NULL*/)
    {
        state = kStateJump;
        if(jumpAnimation != NULL && idleAnimation != NULL)
        {
            stopAction(idleAnimation);
            runAction(jumpAnimation);
        }
        runAction(jumpByAction);
        AudioEngine::play2d(SFX_JUMP);
    }
}

void BaseVehicle::dead()
{
    if(deadTexture != NULL)
    {
        stopAllActions();
        setTexture(deadTexture);
        AudioEngine::play2d(SFX_SMASH);
        
        //float x = getPositionX() + WIN_SIZE.width * 1.5f;
        //runAction(CCMoveTo::create(1.5f, ccp(x, getPositionY())));
        runAction(Blink::create(1.5f, 8));
        if(state == kStateJump)
        {
            float time = 1.0f;
            float times = 30;
            float y = spriteShadow->getPositionY() + getPositionY();
            runAction(MoveTo::create(time, Vec2(getPositionX() + getContentSize().width * 0.15f, y)));
            schedule(CC_SCHEDULE_SELECTOR(BaseVehicle::updateEnd), time / times, times, 0);
        }
    }
}

void BaseVehicle::updateEnd(float delta)
{
    updateShadow();
}

void BaseVehicle::doMove(Point velocity)
{
    
    if(getActionByTag(kActionJumpTag) == NULL)
    {
        if(state == kStateJump)
        {
            if(defaultTexture != NULL)
            {
                setTexture(defaultTexture);
                if(idleAnimation != NULL)
                    runAction(idleAnimation);
            }
            state = kStateIdle;
        }
        playerY = getPositionY() - getContentSize().height * 0.5f;
    }
    else
    {
        if(playerY + velocity.y > limitTopY)
            velocity.y = 0;
        else if(playerY + velocity.y < limitBottomY)
            velocity.y = 0;
    }
    
    Point newPosition = getPosition() + velocity;
    
    playerY += velocity.y;
    
    if(playerY > limitTopY)
        playerY = limitTopY;
    else if(playerY < limitBottomY)
        playerY = limitBottomY;
    
    if(newPosition.x < getContentSize().width * 0.5f)
        newPosition.x = getContentSize().width * 0.5f;
    
    if(newPosition.x > WIN_SIZE.width * 0.8f)
        newPosition.x = WIN_SIZE.width * 0.8f;
    
    if(getActionByTag(kActionJumpTag) == NULL)
        newPosition.y = playerY + getContentSize().height * 0.5f;
    
    setPosition(newPosition);
    updateShadow();

}

Rect BaseVehicle::getGroundCollision()
{
    float x = getBoundingBox().getMinX() + getContentSize().width * 0.3f;
    float y = playerY;
    
    float width = getContentSize().width * 0.55f;
    float height = getContentSize().height * 0.3f;
    
    Rect rect = Rect(x, y, width, height);
    
    return rect;
}

Rect BaseVehicle::getAirCollision()
{
    
    float x = getBoundingBox().getMinX() + getContentSize().width * 0.3f;
    float y = getBoundingBox().getMinY() + getContentSize().height * 0.16f;
    float w = getContentSize().width * 0.55f;
    float h = getContentSize().width * 0.2f;
    
    Rect rect;
    rect.setRect(x, y, w, h);
    
    return rect;
}

void BaseVehicle::updateShadow()
{
    spriteShadow->setPositionY(playerY - getPositionY() + getContentSize().height * 0.55f);
}

void BaseVehicle::setLimits(float limitBotY, float height)
{
    limitBottomY = limitBotY;
    limitTopY = limitBottomY + height;
}
