//
//  PauseLayer.hpp
//  TurboRace
//
//  Created by Carlos Eduardo Pinan Indacochea on 21/02/22.
//

#ifndef PauseLayer_hpp
#define PauseLayer_hpp

#include "cocos2d.h"

enum PauseButtons
{
    kTagPauseResumeGame = 0,
    kTagPauseGoHome = 1,
    kTagPausePlayAgain = 2
};

class PauseLayer : public cocos2d::LayerColor {
public:
    PauseLayer();
private:
    void onOptionsTapped(cocos2d::Ref* sender);
};


#endif /* PauseLayer_hpp */
