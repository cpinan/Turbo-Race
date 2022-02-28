//
//  AirDoubleObstacle.cpp
//  TurboRace-mobile
//
//  Created by Carlos Eduardo Pinan Indacochea on 9/1/19.
//

#include "AirDoubleObstacle.hpp"
#include "../common/Constants.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;


AirDoubleObstacle::~AirDoubleObstacle()
{
    removeAllChildren();
}

AirDoubleObstacle::AirDoubleObstacle() : BaseObstacle("obstaculo_1_c.png")
{
    sameCollisionArea = false;
    
    Rect collideArea;
    
    collideArea = Rect(getContentSize().width * 0.05f, getContentSize().height * 0.65f, getContentSize().width * 0.2f, getContentSize().height * 0.25f);
    vCollision.push_back(collideArea);

    collideArea = Rect(getContentSize().width * 0.2f, getContentSize().height * 0.5f, getContentSize().width * 0.2f, getContentSize().height * 0.25f);
    vCollision.push_back(collideArea);
    
    collideArea = Rect(getContentSize().width * 0.3f, getContentSize().height * 0.35f, getContentSize().width * 0.2f, getContentSize().height * 0.25f);
    vCollision.push_back(collideArea);
    
    collideArea = Rect(getContentSize().width * 0.4f, getContentSize().height * 0.25f, getContentSize().width * 0.2f, getContentSize().height * 0.25f);
    vCollision.push_back(collideArea);
    
    collideArea = Rect(getContentSize().width * 0.5f, getContentSize().height * 0.1f, getContentSize().width * 0.2f, getContentSize().height * 0.25f);
    vCollision.push_back(collideArea);
    
    spriteShadow = Sprite::create("sombra_obstaculo_1c.png");
    spriteShadow->setPosition(Vec2(spriteShadow->getContentSize().width * 0.6f, -spriteShadow->getContentSize().height * 0.4f));
    addChild(spriteShadow, -1);
    

}

bool AirDoubleObstacle::collision(BaseVehicle &vehicle)
{
    if(vehicle.getState() != kStateJump)
        return false;
    
    float y = vehicle.getPositionY() - vehicle.getPlayerY() - vehicle.getContentSize().height * 0.5f;
    if(y < MAX_PLAYER_JUMP * 0.45f)
        return false;
    
    Rect rectAir = vehicle.getAirCollision();
    
    int i;
    Rect area;
    
    for(i = 0; i < vCollision.size(); i++)
    {
        area = currentCollisionArea(vCollision[i]);
        if(area.intersectsRect(rectAir))
        {
            return true;
        }
    }
    
    return false;
}

void AirDoubleObstacle::doUpdate(float x, float speed)
{
    BaseObstacle::doUpdate(x, speed);
    spriteShadow->setPositionX(spriteShadow->getPositionX() - speed);
}
