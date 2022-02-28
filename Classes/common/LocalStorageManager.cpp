//
//  LocalStorageManager.cpp
//  TurboRace-mobile
//
//  Created by Carlos Eduardo Pinan Indacochea on 9/1/19.
//

#include "LocalStorageManager.hpp"
#include "Constants.h"
#include "Globals.hpp"

USING_NS_CC;


LocalStorageManager::LocalStorageManager()
{
}

UserDefault* LocalStorageManager::defaults()
{
    return UserDefault::getInstance();
}

bool LocalStorageManager::isUsingJoypad()
{
    return defaults()->getBoolForKey(USER_CONTROL_IS_JOYPAD, true);
}

void LocalStorageManager::setControlType(bool joypad)
{
    defaults()->setBoolForKey(USER_CONTROL_IS_JOYPAD, joypad);
    defaults()->flush();
}

int LocalStorageManager::getSelectedPlayer()
{
    return defaults()->getIntegerForKey(USER_PLAYER_SELECTED, kStoragePlayerFrog);
}

void LocalStorageManager::setPlayer(StorageValues value)
{
    defaults()->setIntegerForKey(USER_PLAYER_SELECTED, (int) value);
    defaults()->flush();
}

float LocalStorageManager::getMaxScore()
{
    std::string key = StringUtils::format("%s%s", USER_MAX_SCORE, user_data_id.c_str());
    return defaults()->getFloatForKey(key.c_str(), 0);
}

void LocalStorageManager::setMaxScore(float score)
{
    std::string key = StringUtils::format("%s%s", USER_MAX_SCORE, user_data_id.c_str());
    defaults()->setFloatForKey(key.c_str(), score);
    defaults()->flush();
}

void LocalStorageManager::incrementTotalGamesPlayed()
{
    std::string key = StringUtils::format("%s%s", USER_TOTAL_GAMES_PLAYED, user_data_id.c_str());
    defaults()->setIntegerForKey(key.c_str(), getTotalGamesPlayed() + 1);
    defaults()->flush();
}

int LocalStorageManager::getTotalGamesPlayed()
{
    std::string key = StringUtils::format("%s%s", USER_TOTAL_GAMES_PLAYED, user_data_id.c_str());
    return defaults()->getIntegerForKey(key.c_str(), 0);
}

void LocalStorageManager::setTotalScore(float score)
{
    std::string key = StringUtils::format("%s%s", USER_TOTAL_SCORE, user_data_id.c_str());
    defaults()->setFloatForKey(key.c_str(), getTotalScore() + score);
    defaults()->flush();
    
    incrementTotalGamesPlayed();
}

float LocalStorageManager::getTotalScore()
{
    std::string key = StringUtils::format("%s%s", USER_TOTAL_SCORE, user_data_id.c_str());
    return defaults()->getFloatForKey(key.c_str(), 0);
}

float LocalStorageManager::getAverageScore()
{
    int totalGamesPlayed = getTotalGamesPlayed();
    
    if(totalGamesPlayed == 0)
        totalGamesPlayed = 1;
    
    float totalScore = getTotalScore();
    
    return totalScore / totalGamesPlayed;
}

void LocalStorageManager::setScore(float score)
{
    if(score > getMaxScore())
        setMaxScore(score);
    
    setTotalScore(score);
}

bool LocalStorageManager::isAchievementUnlocked(const char *achievementID)
{
    std::string key = StringUtils::format("%s%s", achievementID, user_data_id.c_str());
    return defaults()->getBoolForKey(key.c_str(), false);
}

void LocalStorageManager::unlockAchievement(const char *achievementID)
{
    std::string key = StringUtils::format("%s%s", achievementID, user_data_id.c_str());
    defaults()->setBoolForKey(key.c_str(), true);
    defaults()->flush();
}

void LocalStorageManager::setScoreInLevel(float score, int level)
{
    std::string userScoreKey = StringUtils::format("%s%s", USER_SCORE_IN_LEVEL, user_data_id.c_str());
    
    std::string key = StringUtils::format(userScoreKey.c_str(), level);

    defaults()->setFloatForKey(key.c_str(), score);
    defaults()->flush();
}

float LocalStorageManager::getScoreInLevel(int level)
{
    std::string userScoreKey = StringUtils::format("%s%s", USER_SCORE_IN_LEVEL, user_data_id.c_str());
    
    std::string key = StringUtils::format(userScoreKey.c_str(), level);
    return defaults()->getFloatForKey(key.c_str(), 0);
}

void LocalStorageManager::updateObstaclesJumped(int num)
{
    std::string key = StringUtils::format("%s%s", USER_OBSTACLES_JUMPED, user_data_id.c_str());
    defaults()->setIntegerForKey(key.c_str(), num);
    defaults()->flush();
}

int LocalStorageManager::getObstaclesJumped()
{
    std::string key = StringUtils::format("%s%s", USER_OBSTACLES_JUMPED, user_data_id.c_str());
    return defaults()->getIntegerForKey(key.c_str(), 0);
}

void LocalStorageManager::setMute(bool state)
{
    defaults()->setBoolForKey(USER_MANAGE_MUSIC, state);
    defaults()->flush();
}

bool LocalStorageManager::isMute()
{
    return defaults()->getBoolForKey(USER_MANAGE_MUSIC, false);
}

bool LocalStorageManager::showTutorial()
{
    int usingJoypad = 0;
    if(isUsingJoypad())
        usingJoypad = 1;
    std::string key = StringUtils::format(USER_TUTORIAL, usingJoypad);
    return defaults()->getBoolForKey(key.c_str(), true);
}

void LocalStorageManager::isTutorialOn(bool state, int mode)
{
    int usingJoypad = mode;
    if(usingJoypad == -1)
    {
        if(isUsingJoypad())
            usingJoypad = 1;
        else
            usingJoypad = 0;
    }
    
    std::string key = StringUtils::format(USER_TUTORIAL, usingJoypad);
    defaults()->setBoolForKey(key.c_str(), state);
    defaults()->flush();
}
