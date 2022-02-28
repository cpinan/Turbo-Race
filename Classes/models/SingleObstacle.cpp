//
//  SingleObstacle.cpp
//  TurboRace-mobile
//
//  Created by Carlos Eduardo Pinan Indacochea on 9/1/19.
//

#include "SingleObstacle.hpp"
#include "../common/Constants.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;

SingleObstacle::SingleObstacle() : BaseObstacle("muro_2b.png")
{
    sameCollisionArea = true;
    obstacType = kSimpleObstacle;
    
    Rect collideArea = Rect(getContentSize().width * 0.25f, getContentSize().height * 0.1f,getContentSize().width * 0.6f, getContentSize().height * 0.8f);
    
    vCollision.push_back(collideArea);
}

bool SingleObstacle::collision(BaseVehicle &vehicle)
{
    float top, bottom;
    Rect area = getBoundingBox();
    
    top = area.getMinY() + getContentSize().height * 0.0f;
    bottom = top + getContentSize().height * 0.37f;
    
    float y = vehicle.getGroundCollision().getMinY() + (vehicle.getGroundCollision().getMaxY() - vehicle.getGroundCollision().getMinY()) * 0.25f;
    
    y = vehicle.getPlayerY() + vehicle.getContentSize().height * 0.3f * 0.5f;
    
    if(y < top || y > bottom)
        return false;
    
    // CCLog("Pass %f ; %f ; %f", y, top, bottom);
    
    return BaseObstacle::collision(vehicle);
}
