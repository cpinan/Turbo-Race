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
        
        scoreLabel = Label::createWithTTF("", FONT_GAME, SIZE_RATE_END);
        scoreLabel->setPosition(Vec2(o.x + badgeSprite->getContentSize().width * 0.8f, o.y - badgeSprite->getContentSize().height * 0.1f));
        scoreLabel->setColor(Color3B(255, 255, 255));
        scoreLabel->setRotation(-3);
        bg->addChild(scoreLabel);

        maxScoreLabel = Label::createWithTTF("", FONT_GAME, scoreLabel->getSystemFontSize());
        maxScoreLabel->setPosition(Vec2(scoreLabel->getPositionX(), scoreLabel->getPositionY() - badgeSprite->getContentSize().height * 0.28f));
        maxScoreLabel->setColor(Color3B(255, 255, 255));
        maxScoreLabel->setRotation(scoreLabel->getRotation());
        bg->addChild(maxScoreLabel);
        
        // Send score button
        MenuItemImage* itemScore = MenuItemImage::create("fb-icon.png", "fb-icon.png", CC_CALLBACK_1(PopUpLoseLayer::onOptionTapped, this));
        itemScore->setTag(kTagSendScore);
        itemScore->setPositionX(o.x - itemScore->getContentSize().width * 1.5f);
        itemScore->setPositionY(o.y - itemScore->getContentSize().height * 1.7f);
        
        // Home button
        MenuItemImage* itemHome = MenuItemImage::create("pause_home_off.png", "pause_home.png", CC_CALLBACK_1(PopUpLoseLayer::onOptionTapped, this));
        itemHome->setTag(kTagGoHome);
        itemHome->setPositionX(itemScore->getPositionX() + itemScore->getContentSize().width * 1.5f);
        itemHome->setPositionY(itemScore->getPositionY());
        
        // Play again button
        MenuItemImage* itemPlayAgain = MenuItemImage::create("pause_replay_off.png", "pause_replay.png", CC_CALLBACK_1(PopUpLoseLayer::onOptionTapped, this));
        itemPlayAgain->setTag(kTagPlayAgain);
        itemPlayAgain->setPositionX(itemHome->getPositionX() + itemScore->getContentSize().width * 1.5f);
        itemPlayAgain->setPositionY(itemScore->getPositionY());
        
        itemPlayAgain->setPositionY(itemPlayAgain->getPositionY() + itemScore->getContentSize().height * 0.2f);
        itemHome->setPositionY(itemHome->getPositionY() + itemScore->getContentSize().height * 0.1f);
        
        // Menu
        Menu* menu = Menu::create();
        menu->setAnchorPoint(Vec2(0, 0));
        menu->setPosition(Vec2(0, 0));
        
        menu->addChild(itemScore);
        menu->addChild(itemHome);
        menu->addChild(itemPlayAgain);
        
        bg->addChild(menu);
    }
}

void PopUpLoseLayer::updateScore(int level, float score, int obstaclesAvoided)
{
    std::string scoreText = StringUtils::format("%d (%d x %d)", (int) score, obstaclesAvoided, (int) kScoreFactor);
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
