//
//  GameLayer.hpp
//  TurboRace
//
//  Created by Carlos Eduardo Pinan Indacochea on 21/02/22.
//

#ifndef GameLayer_hpp
#define GameLayer_hpp

#include "cocos2d.h"
#include "../../models/BaseVehicle.hpp"
#include "../../models/BaseObstacle.hpp"
#include "../common/HudLayer.hpp"
#include "../pause/PauseLayer.hpp"
#include "../gameover/PopUpLoseLayer.hpp"

struct MusicPlaying
{
    const char* bg_music;
    const char* description;
};

enum GameTag
{
    kTagTutorialLayer = 100
};

enum GameMode
{
    kGameModeHome = 0,
    kGameModePlay = 1,
    kGameModeEnd = 2,
    kGameModeReplayView = 3,
    kGameModePlayAgain = 4
};

enum GameState
{
    kGameStarting = 0,
    kGamePreparing = 1,
    kGameReady = 2,
    kGamePause = 3,
    kGameFinish = 4,
    kGameTutorial = 5,
    kGameEnd = 6
};

enum GameLevel
{
    kGameLevelEasy = 0,
    kGameLevelNormal = 1,
    kGameLevelHard = 2,
    kGameLevelStory = 3,
    kGameLevelNone = 4
};

enum GameDeep
{
    kDeepSky = -9999,
    kDeepCloud = -9900,
    kDeepBGBack = -9500,
    kDeepBGMid = -9250,
    kDeepBGFront = -9000,
    kDeepTracks = -8500,
    kDeepGameElements = -2500,
    kDeepGameFinish = 500,
    kDeepScore = 1000,
    kDeepPauseLayer = 2500,
    kDeepPopUpLoseLayer = 3500,
    kDeepTutorial = 2200
};

class GameLayer : public cocos2d::Layer {
public:
    
    GameLayer(HudLayer* hudLayer, GameMode gameMode = kGameModeHome, GameLevel = kGameLevelNone);
    
    void configureGame(GameLevel gameLevel = kGameLevelEasy);
    void pauseGame(cocos2d::Ref* pSender);
    void resumeGame();
    void playGame();
    void runGame();
    void refreshLayer();
    
    void update(float dt);
    
    void didAccelerate(cocos2d::Acceleration *pAccelerationValue);
        
    void onEnterTransitionDidFinish();
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    
    void keyBackClicked();
    
    virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags);
    
private:
    void _createMap();
    void _initLayers();
    void _createPlayer();
    void _initElements();
    
    void _createObstacle(float x);
    void _createMultipleObstacles(float x, int type);
    
    void _resumeEvents();
    void _removeNode(cocos2d::Node* node);
    void _gameLogic(float dt);
    
    void _playAgain();
    
    void _goHome();
    
    void _pauseAllActions();
    void _resumeAllActions();
    
    void _checkAchievements();
    
    void _showAudioPlaying();
    
    void _selectRandomMusic();
    
    void _gameIsReady();
    void _showTutorial();
    void _finishTutorial(cocos2d::Ref* object);
    void _showAds();
    
    
private:
    BaseVehicle* _player;
    
    HudLayer* _hudLayer;
    PauseLayer* _pauseLayer;
    PopUpLoseLayer* _popUpLoseLayer;
    
    GameState _gameState;
    GameState _previousGameState;
    
    GameMode _gameMode;
    GameLevel _gameLevel;
    
    cocos2d::Sprite* _spCloud;
    cocos2d::Vector<cocos2d::Sprite*> _parallaxSky;
    cocos2d::Vector<cocos2d::Sprite*> _parallaxBGBack;
    cocos2d::Vector<cocos2d::Sprite*> _parallaxBGMid;
    cocos2d::Vector<cocos2d::Sprite*> _parallaxBGFront;
    cocos2d::Vector<cocos2d::Sprite*> _parallaxFloor;
    cocos2d::Vector<BaseObstacle*> _arrayObstacles;
    cocos2d::MenuItemImage* _menuPause;
    
    bool _pause;
    bool _gameOver;
    bool _isJoypad;
    
    float _worldSpeed;
    float _minDistanceObstaclesX;
    
    int _itemMap;
    int _obstaclesAvoided;
    int _obstaclesJumped;
    
    float _color;
    int _colorSign;
    
    float _playerStartY;
    float _wallHeight;
    
    float OBSTACLE_SIMPLE_BOT_Y;
    float OBSTACLE_SIMPLE_TOP_Y;
    float OBSTACLE_DOBLE_AIR_Y;
    float OBSTACLE_DOBLE_GROUND_Y;
    
    cocos2d::Label* _lblScore;
    cocos2d::Point _accelVelocity;
};

#endif /* GameLayer_hpp */
