//
//  BaseObstacle.hpp
//  TurboRace-mobile
//
//  Created by Carlos Eduardo Pinan Indacochea on 9/1/19.
//

#ifndef BaseObstacle_hpp
#define BaseObstacle_hpp

#include "BaseVehicle.hpp"

enum ObstacleType
{
    kNormalObstacle = 0,
    kJumpObstacle = 1,
    kSimpleObstacle = 2
};

class BaseObstacle : public cocos2d::Sprite
{
public:
    BaseObstacle(std::string file);
    virtual ~BaseObstacle();
    
    CC_SYNTHESIZE_READONLY(bool, sameCollisionArea, SameCollisionArea);
    CC_SYNTHESIZE(bool, passPlayerSFX, PassPlayerSFX);
    
    CC_SYNTHESIZE(std::vector<cocos2d::Rect>, vCollision, VCollision);
    CC_SYNTHESIZE(bool, isObjectAlerted, IsObjectAlerted);
    
    CC_SYNTHESIZE(int, numObjects, NumObjects);
    CC_SYNTHESIZE(float, distanceObjects, DistanceObjects);
    
    CC_SYNTHESIZE(ObstacleType, obstacType, ObstacType);
    
    virtual bool collision(BaseVehicle& vehicle);
    virtual cocos2d::Rect currentCollisionArea(cocos2d::Rect area);
    
    void doUpdate(float x, float speed);
};


#endif /* BaseObstacle_hpp */
