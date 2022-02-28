//
//  SingleObstacle.hpp
//  TurboRace-mobile
//
//  Created by Carlos Eduardo Pinan Indacochea on 9/1/19.
//

#ifndef SingleObstacle_hpp
#define SingleObstacle_hpp

#include "BaseObstacle.hpp"

class SingleObstacle : public BaseObstacle
{
public:
    SingleObstacle();
    bool collision(BaseVehicle& vehicle);
};

#endif /* SingleObstacle_hpp */
