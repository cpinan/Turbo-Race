//
//  DoubleObstacle.cpp
//  TurboRace-mobile
//
//  Created by Carlos Eduardo Pinan Indacochea on 9/1/19.
//

#include "DoubleObstacle.hpp"
#include "../common/Constants.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;


DoubleObstacle::DoubleObstacle() : BaseObstacle("obstaculo_1.png")
{
    obstacType = kJumpObstacle;
    sameCollisionArea = false;
    
    Rect collideArea1 = Rect(getContentSize().width * 0.1f, getContentSize().height * 0.5f, getContentSize().width * 0.5f, getContentSize().height * 0.5f);
    
    Rect collideArea2 = Rect(getContentSize().width * 0.3f, 0, getContentSize().width * 0.5f, getContentSize().height * 0.5f);
    
    vCollision.push_back(collideArea1);
    vCollision.push_back(collideArea2);
    
}
