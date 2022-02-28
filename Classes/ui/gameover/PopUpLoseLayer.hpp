//
//  PopUpLoseLayer.hpp
//  TurboRace
//
//  Created by Carlos Eduardo Pinan Indacochea on 21/02/22.
//

#ifndef PopUpLoseLayer_hpp
#define PopUpLoseLayer_hpp

#include "cocos2d.h"

enum PopUpLoseButtons
{
    kTagPlayAgain = 0,
    kTagGoHome = 1,
    kTagSendScore = 2
};

class PopUpLoseLayer : public cocos2d::LayerColor {
public:
    PopUpLoseLayer();
    void updateScore(int level, float score, int obstaclesAvoided);
    
private:
    void onOptionTapped(cocos2d::Ref* pSender);
    void finishHideLayer();
    
private:
    const char* leaderboardID;
    
    int obstaclesAvoided;
    int level;
    long score;
    
    cocos2d::Label* scoreLabel;
    cocos2d::Label* maxScoreLabel;
    cocos2d::Sprite* badgeSprite;
    bool disable;
};

#endif /* PopUpLoseLayer_hpp */
