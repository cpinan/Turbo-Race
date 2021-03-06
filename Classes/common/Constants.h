//
//  Constants.h
//  TurboRace
//
//  Created by Carlos Eduardo Pinan Indacochea on 9/1/19.
//

#ifndef Constants_h
#define Constants_h

#include "cocos2d.h"

static cocos2d::Size designResolutionSize = cocos2d::Size(1024, 768);

#define IOS() (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#define kScoreFactor 100.0f

#define FONT_GAME "Carton_Six.ttf"
#define FONT_GAME_BMP "CartonSixBMP.fnt"
#define SIZE_SCORE_GAME 45.0f * CC_CONTENT_SCALE_FACTOR()
#define SIZE_RATE_APP 30.0f * CC_CONTENT_SCALE_FACTOR()
#define SIZE_RATE_HOW_TO_PLAY 45.0f * CC_CONTENT_SCALE_FACTOR()
#define SIZE_RATE_END 30.0f * CC_CONTENT_SCALE_FACTOR()
#define SIZE_TUT_INST 47.0f * CC_CONTENT_SCALE_FACTOR()
#define SIZE_TUT_TITLE 80.0f * CC_CONTENT_SCALE_FACTOR()

#define SP_SHADOW "shadow.png"
#define SHOW_FPS false
#define WIN_SIZE Director::getInstance()->getWinSize()

// Local Storage Keys

#define USER_CONTROL_IS_JOYPAD "KEY_CONTROL_IS_JOYPAD"
#define USER_SAVE_REPLAY "KEY_REPLAY_NUMBER_%d"
#define USER_PLAYER_SELECTED "KEY_PLAYER_SELECTED"
#define USER_MAX_SCORE "KEY_MAX_SCORE"
#define USER_TOTAL_GAMES_PLAYED "KEY_TOTAL_GAMES_PLAYED"
#define USER_TOTAL_SCORE "KEY_TOTAL_SCORE"
#define USER_SCORE_IN_LEVEL "KEY_SCORE_LEVEL_%d"
#define USER_OBSTACLES_JUMPED "KEY_OBSTACLES_JUMPED"
#define USER_MANAGE_MUSIC "KEY_MUSIC"
#define USER_TUTORIAL "KEY_TUTORIAL_%d"

// Notifications
#define NOTIFICATION_PAUSE_GAME "NOT_PAUSE_GAME"
#define NOTIFICATION_RESUME_GAME "NOT_RESUME_GAME"
#define NOTIFICATION_PLAY_AGAIN "NOT_PLAY_AGAIN"
#define NOTIFICATION_GO_HOME "NOT_GO_HOME"
#define NOTIFICATION_ENABLE_BUTTONS "NOTIFICATION_ENABLE_BUTTONS"
#define NOTIFICATION_HOW_TO_PLAY "NOTIFICATION_HOW_TO_PLAY"

// Musics and SFX
#define BG_MUSIC_01 "vg_bt_music.mp3"
#define BG_MUSIC_02 "diego_music.mp3"
#define BG_MUSIC_03 "POL-turtle-blues-short.mp3"

#define SFX_JUMP "jump.mp3"
#define SFX_SMASH "smash.mp3"
#define SFX_SWOOSH "swoosh.mp3"
#define SFX_BUTTON "button.mp3"
#define SFX_LIGHTNING "lightning.mp3"

// Game constants
#define MAX_PARALLAX 5
#define MAX_OBSTACLES 10

#define START_X_OBSTACLES designResolutionSize.width * 1.9f
#define START_WORLD_SPEED designResolutionSize.width * 0.5f
#define MIN_DISTANCE_OBSTACLES designResolutionSize.width / 1.8f

#define DRAW_COLLISIONS false
#define TEST_OBSTACLE false

// Music and Sound
#define BG_MUSIC_VOLUME 0.4f

// Achievements

#define ACH_MORE_THAN_3000 !IOS() ? "CgkIyb_B9_4ZEAIQBQ" : "ACH_MORE_THAN_3000"

#define ACH_PLAY_10_TIMES !IOS() ? "CgkIyb_B9_4ZEAIQCg" : "ACH_PLAY_10_TIMES"

#define ACH_PLAY_100_TIMES !IOS() ? "CgkIyb_B9_4ZEAIQCQ" : "ACH_PLAY_100_TIMES"

#define ACH_PLAY_1000_TIMES !IOS() ? "CgkIyb_B9_4ZEAIQCw" : "ACH_PLAY_1000_TIMES"

#define ACH_AVOID_25_OBSTACLES_IN_HARD_MODE !IOS() ? "CgkIyb_B9_4ZEAIQBg" : "ACH_AVOID_25_OBSTACLES_IN_HARD_MODE"

#define ACH_AVOID_50_OBSTACLES_IN_NORMAL_MODE !IOS() ? "CgkIyb_B9_4ZEAIQBw" : "ACH_AVOID_50_OBSTACLES_IN_NORMAL_MODE"

#define ACH_AVOID_100_OBSTACLES_IN_EASY_MODE !IOS() ? "CgkIyb_B9_4ZEAIQCA" : "ACH_AVOID_100_OBSTACLES_IN_EASY_MODE"

#define ACH_JUMP_50_OBSTACLES !IOS() ? "CgkIyb_B9_4ZEAIQDA" : "ACH_JUMP_50_OBSTACLES"

#define ACH_GET_10000_OR_MORE_IN_EASY_MODE !IOS() ? "CgkIyb_B9_4ZEAIQDQ" : "ACH_GET_10000_OR_MORE_IN_EASY_MODE"

#define ACH_GET_8000_OR_MORE_IN_NORMAL_MODE !IOS() ? "CgkIyb_B9_4ZEAIQDg" : "ACH_GET_8000_OR_MORE_IN_NORMAL_MODE"

#define ACH_GET_5000_OR_MORE_IN_HARD_MODE !IOS() ? "CgkIyb_B9_4ZEAIQDw" : "ACH_GET_5000_OR_MORE_IN_HARD_MODE"

#define ACH_AVERAGE_1000_OR_MORE_IN_50_GAMES_OR_MORE !IOS() ? "CgkIyb_B9_4ZEAIQEA" : "ACH_AVERAGE_1000_OR_MORE_IN_50_GAMES_OR_MORE"

#define ACH_GET_TOTAL_SCORE_100000_OR_MORE !IOS() ? "CgkIyb_B9_4ZEAIQEQ" : "ACH_GET_TOTAL_SCORE_100000_OR_MORE"

#define ACH_PLAY_IN_ACCELEROMETER_MODE !IOS() ? "CgkIyb_B9_4ZEAIQEg" : "ACH_PLAY_IN_ACCELEROMETER_MODE"

#define ACH_PLAY_IN_ACCELEROMETER_MODE_AND_GET_MORE_THAN_3000 !IOS() ? "CgkIyb_B9_4ZEAIQEw" : "ACH_PLAY_IN_ACCELEROMETER_MODE_AND_GET_MORE_THAN_3000"

#define ACH_JUMP_1000_OBSTACLES !IOS() ? "CgkIyb_B9_4ZEAIQFg" : "ACH_JUMP_1000_OBSTACLES"

#define ACH_AVOID_100_OBSTACLES_IN_HARD_MODE !IOS() ? "CgkIyb_B9_4ZEAIQFw" : "ACH_AVOID_100_OBSTACLES_IN_HARD_MODE"

#define ACH_GET_30K_IN_EASY_MODE "CgkIyb_B9_4ZEAIQGw"
#define ACH_GET_15K_IN_NORMAL_MODE "CgkIyb_B9_4ZEAIQHA"
#define ACH_GET_10K_IN_HARD_MODE "CgkIyb_B9_4ZEAIQHQ"


#define LEAD_EASY_MODE !IOS() ? "CgkIyb_B9_4ZEAIQAg" : "LEAD_EASY_MODE"

#define LEAD_NORMAL_MODE !IOS() ? "CgkIyb_B9_4ZEAIQAw" : "LEAD_NORMAL_MODE"

#define LEAD_HARD_MODE !IOS() ? "CgkIyb_B9_4ZEAIQBA" : "LEAD_HARD_MODE"

enum HomeButtons
{
    kTagEasyMode = 0,
    kTagNormalMode = 1,
    kTagHardMode = 2,
    kTagRateApp = 3,
    kTagLeaderboard = 4,
    kTagAchievements = 5,
    kTagSettings = 6,
    kTagHowToPlay = 7
};

#endif /* Constants_h */
