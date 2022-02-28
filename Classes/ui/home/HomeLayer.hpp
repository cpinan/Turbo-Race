//
//  HomeLayer.hpp
//  TurboRace
//
//  Created by Carlos Eduardo Pinan Indacochea on 21/02/22.
//

#ifndef HomeLayer_hpp
#define HomeLayer_hpp

#include "cocos2d.h"
#include "../../common/Constants.h"
#include "../game/GameLayer.hpp"
#include "../settings/SettingsLayer.hpp"

class HomeLayer : public cocos2d::Layer {
public:
    HomeLayer(GameLayer* gameLayer);
    virtual ~HomeLayer();
    
private:
    cocos2d::Sprite* tablero;
    cocos2d::Sprite* logo;
    cocos2d::MenuItemImage* menuItemEasy;
    cocos2d::MenuItemImage* menuItemNormal;
    cocos2d::MenuItemImage* menuItemHard;
    cocos2d::MenuItemImage* menuItemAchievements;
    cocos2d::MenuItemImage* menuItemLeaderboard;
    cocos2d::MenuItemImage* menuItemSettings;
    cocos2d::MenuItemLabel* menuRateApp;
    cocos2d::MenuItemLabel* menuHowToPlay;
    cocos2d::MenuItemToggle* menuSound;
    
private:
    void _onOptionPressed(cocos2d::Ref* pSender);
    void _finishHideLayer(float dt);
    void _manageMusic(cocos2d::Ref* pSender);
    
    void _hideToLeft();
    void _hideToRight();
    
    void _enableButtons();
    void _disableButtons();
    
    void _manageHowToPlay();
    
private:
    bool disable;
    GameLayer* _gameLayer;
    SettingsLayer* _settingsLayer;
    cocos2d::Menu* menu;
};

#endif /* HomeLayer_hpp */
