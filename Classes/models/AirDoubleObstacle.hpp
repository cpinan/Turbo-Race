//
//  AirDoubleObstacle.hpp
//  TurboRace-mobile
//
//  Created by Carlos Eduardo Pinan Indacochea on 9/1/19.
//

#ifndef AirDoubleObstacle_hpp
#define AirDoubleObstacle_hpp

#include "BaseObstacle.hpp"

class AirDoubleObstacle : public BaseObstacle
{
public:
    AirDoubleObstacle();
    ~AirDoubleObstacle();
    bool collision(BaseVehicle& vehicle);
    void doUpdate(float x, float speed);
    
private:
    cocos2d::Sprite* spriteShadow;
};

#endif /* AirDoubleObstacle_hpp */
