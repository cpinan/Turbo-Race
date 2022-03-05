//
//  PopUpLoseLayer.cpp
//  TurboRace
//
//  Created by Carlos Eduardo Pinan Indacochea on 21/02/22.
//

#include "PopUpLoseLayer.hpp"
#include "../settings/SettingsLayer.hpp"
#include "../home/HomeScene.hpp"
#include "../../common/Constants.h"
#include "../../common/LocalStorageManager.hpp"
#include "audio/include/AudioEngine.h"

USING_NS_CC;

PopUpLoseLayer::PopUpLoseLayer() {
    if(initWithColor(Color4B(0, 0, 0, 128))) {
        leaderboardID = "";
        
        score = 0;
        disable = false;
        
        Size visibleSize = Director::getInstance()->getVisibleSize();
        Point visibleOrigin = Director::getInstance()->getVisibleOrigin();
        Point origin = Vec2(visibleOrigin.x + visibleSize.width * 0.5f, visibleOrigin.y + visibleSize.height* 0.5f);
        
        // BGWhite
        Sprite* bg = Sprite::create("gameover_screen.png");
        bg->setPosition(origin);
        addChild(bg);
        
        float w = bg->getContentSize().width;
        float h = bg->getContentSize().height;
        Point o = Vec2(w * 0.5f, h * 0.5f);
        
        // Badge and record
        badgeSprite = Sprite::create("bicho_0004.png");
        badgeSprite->setPosition(Vec2(o.x, o.y + badgeSprite->getContentSize().height * 0.5f));
        bg->addChild(badgeSprite, 10);
        
        scoreLabel = Label::createWithBMFont(FONT_GAME_BMP, "");
        scoreLabel->setScale(0.5f);
        scoreLabel->setPosition(Vec2(o.x + badgeSprite->getContentSize().width * 0.5f, o.y - badgeSprite->getContentSize().height * 0.1f));
        scoreLabel->setColor(Color3B(255, 255, 255));
        scoreLabel->setRotation(-3);
        bg->addChild(scoreLabel);

        maxScoreLabel = Label::createWithTTF("", FONT_GAME, SIZE_RATE_END);
        maxScoreLabel->setPosition(Vec2(scoreLabel->getPositionX(), scoreLabel->getPositionY() - badgeSprite->getContentSize().height * 0.28f));
        maxScoreLabel->setColor(Color3B(255, 255, 255));
        maxScoreLabel->setRotation(scoreLabel->getRotation());
        bg->addChild(maxScoreLabel);
        
        // Home button
        MenuItemImage* itemHome = MenuItemImage::create("pause_home_off.png", "pause_home.png", CC_CALLBACK_1(PopUpLoseLayer::onOptionTapped, this));
        itemHome->setTag(kTagGoHome);
        itemHome->setPositionX(o.x - itemHome->getContentSize().width * 0.5f);
        itemHome->setPositionY(o.y - itemHome->getContentSize().height * 1.7f);
        
        // Play again button
        MenuItemImage* itemPlayAgain = MenuItemImage::create("pause_replay_off.png", "pause_replay.png", CC_CALLBACK_1(PopUpLoseLayer::onOptionTapped, this));
        itemPlayAgain->setTag(kTagPlayAgain);
        itemPlayAgain->setPositionX(itemHome->getPositionX() + itemHome->getContentSize().width * 1.4f);
        itemPlayAgain->setPositionY(itemHome->getPositionY());
        
        itemPlayAgain->setPositionY(itemPlayAgain->getPositionY() + itemHome->getContentSize().height * 0.2f);
        itemHome->setPositionY(itemHome->getPositionY() + itemHome->getContentSize().height * 0.1f);
        
        // Menu
        Menu* menu = Menu::create();
        menu->setAnchorPoint(Vec2(0, 0));
        menu->setPosition(Vec2(0, 0));
        
        menu->addChild(itemHome);
        menu->addChild(itemPlayAgain);
        
        bg->addChild(menu);
    }
}

void PopUpLoseLayer::updateScore(int level, float score, int obstaclesAvoided)
{
    std::string scoreText = StringUtils::format("%d = %d x %d", (int) score, obstaclesAvoided, (int) kScoreFactor);
    scoreLabel->setString(scoreText);
    
    long longScore = (long) score;
    
    this->level = level;
    this->score = longScore;
    this->obstaclesAvoided = obstaclesAvoided;
    
    int totalGamesPlayed = LocalStorageManager::getTotalGamesPlayed();
    
    LocalStorageManager::setScore(score);
    
    long totalScore = (long) LocalStorageManager::getTotalScore();
    if(!LocalStorageManager::isAchievementUnlocked(ACH_AVERAGE_1000_OR_MORE_IN_50_GAMES_OR_MORE) &&  LocalStorageManager::getTotalGamesPlayed() >= 50 && LocalStorageManager::getAverageScore() >= 1000) {
        LocalStorageManager::unlockAchievement(ACH_AVERAGE_1000_OR_MORE_IN_50_GAMES_OR_MORE);
    }
    
    if(!LocalStorageManager::isAchievementUnlocked(ACH_GET_TOTAL_SCORE_100000_OR_MORE) && totalScore >= 100000) {     LocalStorageManager::unlockAchievement(ACH_GET_TOTAL_SCORE_100000_OR_MORE);
    }
    
    long scoreInLevel = (long) LocalStorageManager::getScoreInLevel(level);
    
    if(longScore > scoreInLevel) {
        LocalStorageManager::setScoreInLevel(score, level);
        scoreInLevel = score;
        
        badgeSprite->setTexture(Sprite::create("bicho_0003.png")->getTexture());
    }
    
    std::string maxScoreText = StringUtils::format("%lu", scoreInLevel);
    maxScoreLabel->setString(maxScoreText);
}

void PopUpLoseLayer::onOptionTapped(Ref *pSender)
{
    if(disable)
        return;
    
    MenuItem* item = (MenuItem*) pSender;
    AudioEngine::play2d(SFX_BUTTON);
    
    switch (item->getTag()) {
        case kTagPlayAgain:
            getEventDispatcher()->dispatchCustomEvent(NOTIFICATION_PLAY_AGAIN);
            break;
        case kTagGoHome:
            AudioEngine::stopAll();
            Scene* scene = HomeScene::scene(kGameModeHome);
            Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
            break;

    }
}
