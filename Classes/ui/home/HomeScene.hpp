//
//  HomeScene.hpp
//  TurboRace
//
//  Created by Carlos Eduardo Pinan Indacochea on 21/02/22.
//

#ifndef HomeScene_hpp
#define HomeScene_hpp

#include "cocos2d.h"
#include "../../common/Constants.h"
#include "../game/GameLayer.hpp"

enum
{
    kZHomeLayer = 99999,
    kZHudLayer = 9999,
    kZGameLayer = 999
};

class HomeScene : public cocos2d::Scene {
private:
    HomeScene(GameMode gameMode, GameLevel gameLevel = kGameLevelNone);
    
public:
    static HomeScene* scene(GameMode gameMode, GameLevel gameLevel = kGameLevelNone);
};


#endif /* HomeScene_hpp */
