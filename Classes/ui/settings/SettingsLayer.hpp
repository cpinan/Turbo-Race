//
//  SettingsLayer.hpp
//  TurboRace
//
//  Created by Carlos Eduardo Pinan Indacochea on 21/02/22.
//

#ifndef SettingsLayer_hpp
#define SettingsLayer_hpp

#include "cocos2d.h"

enum SettingsTags
{
    kTagSettingJoypadMode = 0,
    kTagSettingAccelMode = 1,
    kTagSettingGoHome = 2
};

class SettingsLayer : public cocos2d::LayerColor {
public:
    SettingsLayer();
    static cocos2d::Scene* createScene();
private:
    
    void onOptionsTapped(cocos2d::Ref* sender);
    cocos2d::MenuItemImage* itemJoypad;
    cocos2d::MenuItemImage* itemAcceleration;
};

#endif /* SettingsLayer_hpp */
