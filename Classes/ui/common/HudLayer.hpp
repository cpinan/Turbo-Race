//
//  HudLayer.hpp
//  TurboRace
//
//  Created by Carlos Eduardo Pinan Indacochea on 21/02/22.
//

#ifndef HudLayer_hpp
#define HudLayer_hpp

#include "cocos2d.h"
#include "../../common/SneakyInput/SneakyJoystick.hpp"
#include "../../models/BaseVehicle.hpp"

class HudLayer : public cocos2d::Layer
{
public:
    SneakyJoystick* joypad;
    HudLayer();
    
    void updateControl(BaseVehicle& player, float dt);
    void runTutorialJoypad();
    void stopTutorialJoypad();
    
private:
    cocos2d::Sprite* joypadBG;
    cocos2d::Sprite* joypadThumb;
    
};


#endif /* HudLayer_hpp */
