//
//  BaseObstacle.cpp
//  TurboRace-mobile
//
//  Created by Carlos Eduardo Pinan Indacochea on 9/1/19.
//

#include "BaseObstacle.hpp"
#include "../common/Constants.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;

BaseObstacle::BaseObstacle(std::string filename)
{
    if(initWithFile(filename.c_str()))
    {
        isObjectAlerted = false;
        passPlayerSFX = false;
        sameCollisionArea = false;
        distanceObjects = 0;
        numObjects = 1;
        obstacType = kNormalObstacle;
        setTag(1);
    }
}

BaseObstacle::~BaseObstacle()
{
    
}

bool BaseObstacle::collision(BaseVehicle& vehicle)
{
    
    Rect rectAir = vehicle.getAirCollision();
    Rect rectFloor = vehicle.getGroundCollision();
    
    int i;
    Rect area;
    
    for(i = 0; i < vCollision.size(); i++)
    {
        area = currentCollisionArea(vCollision[i]);
        if(area.intersectsRect(rectAir) && area.intersectsRect(rectFloor))
        {
            return true;
        }
    }
    
    
    return false;
}

Rect BaseObstacle::currentCollisionArea(Rect area)
{
    
    float left, top, right, bottom;
    float width, height;
    
    left = area.getMinX() + getPositionX() - getContentSize().width * 0.5f;
    top = area.getMinY() + getPositionY() - getContentSize().height * 0.5f;
    right = area.getMaxX() + getPositionX() - getContentSize().width * 0.5f;
    bottom = area.getMaxY() + getPositionY() - getContentSize().height * 0.5f;
    
    width = right - left;
    height = bottom - top;
    
    Rect newArea = Rect(left, top, width, height);

    
    return newArea;
}

void BaseObstacle::doUpdate(float x, float speed)
{
    setPositionX(x - speed);
}
