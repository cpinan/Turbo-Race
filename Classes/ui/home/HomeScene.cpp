//
//  HomeScene.cpp
//  TurboRace
//
//  Created by Carlos Eduardo Pinan Indacochea on 21/02/22.
//

#include "HomeScene.hpp"
#include "../common/HudLayer.hpp"
#include "../game/GameLayer.hpp"
#include "../home/HomeLayer.hpp"
#include "../settings/SettingsLayer.hpp"
#include "../../models/VehicleFrog.hpp"

USING_NS_CC;

HomeScene::HomeScene(GameMode gameMode, GameLevel gameLevel) {
    HudLayer* hudLayer = new HudLayer();
    hudLayer->setVisible(false);
    hudLayer->autorelease();
    addChild(hudLayer, kZHudLayer);
    
    GameLayer* gameLayer = new GameLayer(hudLayer, gameMode, gameLevel);
    gameLayer->autorelease();
    addChild(gameLayer, kZGameLayer);
    
    if(gameMode == kGameModeHome) {
        HomeLayer* homeLayer = new HomeLayer(gameLayer);
        // homeLayer->setVisible(!TEST_OBSTACLE);
        homeLayer->autorelease();
        addChild(homeLayer, kZHomeLayer);
    }
}

HomeScene* HomeScene::scene(GameMode gameMode, GameLevel gameLevel)
{
    HomeScene* scene = new HomeScene(gameMode, gameLevel);
    scene->autorelease();
    return scene;
}
