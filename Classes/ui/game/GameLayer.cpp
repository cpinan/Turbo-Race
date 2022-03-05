//
//  GameLayer.cpp
//  TurboRace
//
//  Created by Carlos Eduardo Pinan Indacochea on 21/02/22.
//

#include "GameLayer.hpp"
#include "../../common/Constants.h"
#include "../../common/LocalStorageManager.hpp"
#include "audio/include/AudioEngine.h"

#include "../../models/AirDoubleObstacle.hpp"
#include "../../models/DoubleObstacle.hpp"
#include "../../models/SingleObstacle.hpp"
#include "../../models/VehicleFrog.hpp"

#include "../home/HomeScene.hpp"

#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>

#define SP_PISTA "pista.png"
#define SP_CIELO "cielo.png"
#define SP_NUBE "nube.png"
#define SP_BG_BACK "background_2.png"
#define SP_BG_MID "background_1.png"
#define SP_BG_FRONT "humo.png"

#define DT_SPEED_PISTA 1.0f
#define DT_SPEED_OBSTACULOS (DT_SPEED_PISTA * 1.0f)
#define DT_SPEED_NUBE (DT_SPEED_PISTA * 0.2f)
#define DT_SPEED_BG_BACK (DT_SPEED_PISTA * 0.5f)
#define DT_SPEED_BG_MID (DT_SPEED_PISTA * 1.0f)
#define DT_SPEED_BG_FRONT (DT_SPEED_PISTA * 1.3f)
#define MIN_COLOR 100
#define MAX_COLOR 255

USING_NS_CC;
using namespace std;

#pragma mark - Level Definition, Constructor and Destructor

// Level definition
/*
 0 = Obstaculo Simple Abajo
 1 = Obstaculo Simple Arriba
 2 = Obstaculo Doble en Tierra
 3 = Obstaculo Doble en Aire
 4 = x2 Obstaculo Simple Abajo
 5 = x2 Obstaculo Simple Arriba
 6 = x3 Obstaculo Doble en Tierra
 7 = x3 Obstaculo Doble en Aire
 8 = x2 Obstaculo Doble en Tierra
 9 = x2 Obstaculo Doble en Aire
 */

int easyMap[] = {
    0,0,1,1,0,0,1,1,0,0,1,1,2,0,1,0,1,0,1,
    2,3,3,2,3,3,2,3,2,0,0,1,0,0,0,1,0,1,0,
    1,0,0,0,1,1,1,0,1,1,3,2,3,3,3,2,3,1,8,
    9,0,1,0,1,2,0,0,1,1,2,0,1,2,0,0,8,0,0,
    1,1,9,1,0,1,2,9,1,2,9,0,1,2,1,0,2,1,2,
    0,0,1,0,0,1,1,0,1,1,0,0,1,4,5,0,1,3,8,
    0,0,1,1,0,1,1,0,1,0,1,0,1,0,1,0,1,3,2
};

int normalMap[] = {
    3,2,1,1,0,0,1,0,1,0,1,0,1,2,3,2,2,3,3,
    0,1,0,1,5,4,5,4,5,4,2,5,4,2,5,4,2,5,4,
    3,2,0,1,0,8,9,2,0,1,4,5,4,5,8,0,1,0,2,
    3,3,2,2,3,8,0,1,0,1,4,5,4,5,2,4,5,3,2,
    0,1,1,0,0,1,1,0,0,1,2,3,2,2,3,3,8,9,6,
    3,7,0,0,1,1,4,5,5,4,2,3,2,1,0,1,0,1,0,
    3,2,3,2,3,3,8,3,8,3,2,9,2,0,2,1,2,0,2
};

int hardMap[] = {
    6,7,8,7,6,9,5,2,4,2,5,3,4,3,8,9,0,1,0,
    1,2,9,8,3,4,5,4,5,4,5,4,5,4,5,0,1,0,1,
    8,3,6,3,8,3,2,3,8,9,6,7,0,2,1,2,0,3,9,
    6,9,6,9,8,9,3,8,9,2,9,9,2,9,9,2,4,5,8,
    9,3,8,9,8,3,3,4,5,4,5,0,1,0,1,2,3,3,8,
    6,9,6,9,6,0,1,6,9,3,6,0,9,8,9,6,8,0,9,
    6,7,6,7,6,3,3,6,3,8,9,8,9,8,1,8,0,8,1
};

static int _currentMusic = 0;

vector<int> _vectorMap;
vector<MusicPlaying> _vectorMusics;
MusicPlaying _music;

// End Level definition
GameLayer::~GameLayer() {
    Device::setAccelerometerEnabled(false);
}

GameLayer::GameLayer(HudLayer* hudLayer, GameMode gameMode, GameLevel gameLevel) : _hudLayer(hudLayer), _gameMode(gameMode)
{
    srand( static_cast<unsigned int>(time(nullptr)));
    
    // Preload obstacles
    
    BaseObstacle* _tmp;
    
    _tmp = new SingleObstacle();
    _tmp->release();
    
    _tmp = new DoubleObstacle();
    _tmp->release();
    
    _tmp = new AirDoubleObstacle();
    _tmp->release();
    
    // End obstacles
    
    _color = 255;
    _colorSign = -1;
    
    _selectRandomMusic();
    
    _player = NULL;
    _obstaclesJumped = 0;
    _obstaclesAvoided = 0;
    _isJoypad = true;
    
    _eventDispatcher->addCustomEventListener(NOTIFICATION_PAUSE_GAME, [this](EventCustom* event) {
        pauseGame(nullptr);
      });
    
    _eventDispatcher->addCustomEventListener(NOTIFICATION_RESUME_GAME, [this](EventCustom* event) {
        resumeGame();
      });
    
    _eventDispatcher->addCustomEventListener(NOTIFICATION_PLAY_AGAIN, [this](EventCustom* event) {
        _playAgain();
      });
    
    _eventDispatcher->addCustomEventListener(NOTIFICATION_GO_HOME, [this](EventCustom* event) {
        _goHome();
      });
    
    _createMap();
    _initLayers();
    _gameLevel = gameLevel;
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}


void GameLayer::onEnterTransitionDidFinish() {
    if(_gameMode == kGameModePlay || _gameMode == kGameModePlayAgain || _gameMode == kGameModeReplayView) {
        configureGame(_gameLevel);
        runGame();
    }
}

void GameLayer::_selectRandomMusic() {
    MusicPlaying mp;
    
    mp.bg_music = BG_MUSIC_01;
    mp.description = "BT Turbo Tunnel - VGMusic.com";
    _vectorMusics.push_back(mp);
    
    mp.bg_music = BG_MUSIC_02;
    mp.description = "Music by Diego Rodriguez";
    _vectorMusics.push_back(mp);
    
    mp.bg_music = BG_MUSIC_03;
    mp.description = "Turtle Blues - PlayOnLoop.com";
    _vectorMusics.push_back(mp);
    
    _music = _vectorMusics[_currentMusic];
    
    _currentMusic++;
    if(_currentMusic > _vectorMusics.size() - 1)
        _currentMusic = 0;
    
}

void GameLayer::_showAudioPlaying() {
    Point origin = Director::getInstance()->getVisibleOrigin();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Size winSize = Director::getInstance()->getWinSize();
    
    string playingMusicText = StringUtils::format("Playing %s", _music.description);
    Label* lblMusic = Label::createWithTTF(playingMusicText, FONT_GAME, 35.0f);
    lblMusic->setAnchorPoint(Vec2(0, 0.5f));
    auto musicSize = lblMusic->getContentSize();
    lblMusic->setPositionX(origin.x + visibleSize.width - musicSize.width * 1.1f);
    lblMusic->setPositionY(origin.y - visibleSize.height * 0.25f);
    
    MoveTo* ac1 = MoveTo::create(1.0f, Vec2(lblMusic->getPositionX(), origin.y + visibleSize.height * 0.05f));
    
    DelayTime* ac2 = DelayTime::create(2.1f);
    FadeOut* ac3 = FadeOut::create(0.9f);
    CallFuncN* ac4 = CallFuncN::create(CC_CALLBACK_1(GameLayer::_removeNode, this));
    
    addChild(lblMusic, kDeepGameFinish);
    
    lblMusic->runAction(Sequence::create(ac1, ac2, ac3, ac4, NULL));
    
    int musicId = AudioEngine::play2d(_music.bg_music);
    AudioEngine::setLoop(musicId, true);
    
}

void GameLayer::refreshLayer() {
    AudioEngine::stopAll();
    
    _eventDispatcher->removeCustomEventListeners(NOTIFICATION_PAUSE_GAME);
    _eventDispatcher->removeCustomEventListeners(NOTIFICATION_RESUME_GAME);
    _eventDispatcher->removeCustomEventListeners(NOTIFICATION_PLAY_AGAIN);
    _eventDispatcher->removeCustomEventListeners(NOTIFICATION_GO_HOME);
    
    _parallaxSky.clear();
    _parallaxBGMid.clear();
    _parallaxBGFront.clear();
    _parallaxFloor.clear();
    _arrayObstacles.clear();
}

#pragma mark - Init layers, Create Players and Game Elements

void GameLayer::_createMap() {
    Point visibleOrigin = Director::getInstance()->getVisibleOrigin();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    int i = 0;
    float x = 0;
    float yPos = visibleOrigin.y * 0.5f;
    Sprite* _tmpSprite;
    
    
    // Creamos el parallax para la pista
    _parallaxFloor = Vector<Sprite*>(MAX_PARALLAX);
    
    Sprite* spFloor = Sprite::create(SP_PISTA);
    _playerStartY = spFloor->getContentSize().height * 0.55f + yPos;
    _wallHeight = spFloor->getContentSize().height * 0.25f;
    
    // Creamos el cielo
    _parallaxSky = Vector<Sprite*>(2);
    
    for(i = 0; i < 2; i++)
    {
        _tmpSprite = Sprite::create(SP_CIELO);
        _tmpSprite->setAnchorPoint(Vec2(0,0));
        _tmpSprite->setPosition(Vec2(x, WIN_SIZE.height - _tmpSprite->getContentSize().height));
        addChild(_tmpSprite, kDeepSky);
        x += _tmpSprite->getContentSize().width;
        _parallaxSky.pushBack(_tmpSprite);
    }
    
    // Creamos la nube
    _spCloud = Sprite::create(SP_NUBE);
    _spCloud->setPosition(Vec2(WIN_SIZE.width + _spCloud->getContentSize().width * 0.7f, visibleSize.height - _spCloud->getContentSize().height * 0.9f + visibleOrigin.y));
    addChild(_spCloud, kDeepCloud);
    
    // Creamos el BG que esta mas atras
    x = 0;
    _parallaxBGBack = Vector<Sprite*>(MAX_PARALLAX);
    for(i = 0; i < MAX_PARALLAX; i++)
    {
        _tmpSprite = Sprite::create(SP_BG_BACK);
        _tmpSprite->setAnchorPoint(Vec2(0,0));
        _tmpSprite->setPosition(Vec2(x, _playerStartY + _wallHeight * 1.9f));
        addChild(_tmpSprite, kDeepBGBack);
        _parallaxBGBack.pushBack(_tmpSprite);
        x += _tmpSprite->getContentSize().width;
    }
    
    // Creamos el BG que esta al medio
    x = 0;
    _parallaxBGMid = Vector<Sprite*>(MAX_PARALLAX);
    for(i = 0; i < MAX_PARALLAX; i++)
    {
        _tmpSprite = Sprite::create(SP_BG_MID);
        _tmpSprite->setAnchorPoint(Vec2(0,0));
        _tmpSprite->setPosition(Vec2(x, _playerStartY + _wallHeight * 1.0f));
        addChild(_tmpSprite, kDeepBGMid);
        _parallaxBGMid.pushBack(_tmpSprite);
        x += _tmpSprite->getContentSize().width;
    }
    
    // Creamos el BG que esta al frente
    x = 0;
    _parallaxBGFront = Vector<Sprite*>(MAX_PARALLAX);
    for(i = 0; i < MAX_PARALLAX; i++)
    {
        _tmpSprite = Sprite::create(SP_BG_FRONT);
        _tmpSprite->setAnchorPoint(Vec2(0,0));
        _tmpSprite->setPosition(Vec2(x, _playerStartY + _wallHeight * 1.0f));
        addChild(_tmpSprite, kDeepBGFront);
        _parallaxBGFront.pushBack(_tmpSprite);
        x += _tmpSprite->getContentSize().width;
    }
    
    // Pista
    x = 0;
    
    for(i = 0; i < MAX_PARALLAX; i++)
    {
        spFloor = Sprite::create(SP_PISTA);
        spFloor->setAnchorPoint(Vec2(0, 0));
        spFloor->setPosition(Vec2(x, yPos));
        _parallaxFloor.pushBack(spFloor);
        addChild(spFloor, kDeepTracks);
        x += spFloor->getContentSize().width;
    }
    
    OBSTACLE_SIMPLE_BOT_Y = _playerStartY + _wallHeight * 0.85f;
    OBSTACLE_DOBLE_GROUND_Y = _playerStartY + _wallHeight * 0.7f;
    OBSTACLE_SIMPLE_TOP_Y = _playerStartY + _wallHeight * 1.55f;
    OBSTACLE_DOBLE_AIR_Y = _playerStartY + _wallHeight * 1.8f;
    
}

void GameLayer::configureGame(GameLevel gameLevel) {
    _gameLevel = gameLevel;
    
    _accelVelocity = Vec2(0,0);
    
    Point visOrigin = Director::getInstance()->getVisibleOrigin();
    Size visSize = Director::getInstance()->getVisibleSize();
    
    _menuPause = MenuItemImage::create("pause_off.png", "pause.png", CC_CALLBACK_1(GameLayer::pauseGame, this));
    _menuPause->setVisible(false);
    _menuPause->setPositionX(visOrigin.x + _menuPause->getContentSize().width * 0.9f);
    _menuPause->setPositionY(visOrigin.y + visSize.height - _menuPause->getContentSize().width * 0.6f);
    
    Menu* menu = Menu::create();
    menu->setAnchorPoint(Vec2(0, 0));
    menu->setPosition(Vec2(0,0));
    menu->addChild(_menuPause);
    addChild(menu, kDeepPauseLayer);
    
    _touchEnabled = true;
    
    _worldSpeed = START_WORLD_SPEED;
    _minDistanceObstaclesX = MIN_DISTANCE_OBSTACLES;
    
    if(_gameLevel == kGameLevelEasy) {
        _minDistanceObstaclesX *= 2.0f; // 1.0
        _worldSpeed *= 1.0f; // 0.9
        _vectorMap.insert(_vectorMap.begin(), easyMap, easyMap + sizeof(easyMap) / sizeof(int));
    }
    else if(_gameLevel == kGameLevelNormal)
    {
        _minDistanceObstaclesX *= 1.3f; // 0.9
        _worldSpeed *= 1.7f; // 1.8
        _vectorMap.insert(_vectorMap.begin(), normalMap, normalMap + sizeof(normalMap) / sizeof(int));
    }
    else if(_gameLevel == kGameLevelHard)
    {
        _minDistanceObstaclesX *= 1.0f; // 0.7
        _worldSpeed *= 2.2f; // 2.4
        _vectorMap.insert(_vectorMap.begin(), hardMap, hardMap + sizeof(hardMap) / sizeof(int));
    }
    
    _itemMap = 0;
    
    _gameState = kGameStarting;
    
    _pause = false;
    _gameOver = false;
    
    _createPlayer();
    _initElements();
    
}

void GameLayer::_initLayers() {
    Point origin = Director::getInstance()->getVisibleOrigin();
    Size size = Director::getInstance()->getVisibleSize();
    
    _lblScore = Label::createWithBMFont(FONT_GAME_BMP, "0");
    _lblScore->setAnchorPoint(Vec2(0, -0.5f));
    _lblScore->setVisible(false);
    _lblScore->setColor(Color3B(255, 255, 255));
    _lblScore->setPosition(Vec2(origin.x + size.width * 0.9f, origin.y + size.height * 0.82f));
    addChild(_lblScore, kDeepScore);
    
    _pauseLayer = new PauseLayer();
    _pauseLayer->setVisible(false);
    _pauseLayer->setPosition(Vec2(0, -WIN_SIZE.height));
    _pauseLayer->setPositionY(0);
    _pauseLayer->autorelease();
    addChild(_pauseLayer, kDeepPauseLayer);
    
    _popUpLoseLayer = new PopUpLoseLayer();
    _popUpLoseLayer->setPosition(Vec2(0, -WIN_SIZE.height));
    _popUpLoseLayer->autorelease();
    addChild(_popUpLoseLayer, kDeepPopUpLoseLayer);
    
}

void GameLayer::_createPlayer() {
    _player = new VehicleFrog();
    _player->setLimits(_playerStartY - _wallHeight * 0.1f, _wallHeight * 0.9f);
    _player->setPositionY(_playerStartY + _wallHeight * 0.5f);
    _player->setPositionX(-_player->getContentSize().width * 2.5f);
    _player->autorelease();
    addChild(_player);
}

void GameLayer::_initElements() {
    int i = 0;
    float x;
    
    // Obstacles
    x = START_X_OBSTACLES;
    
    for(i = 0; i < MAX_OBSTACLES; i++) {
        _createObstacle(x);
        BaseObstacle* lastObstacle = _arrayObstacles.back();
        x = lastObstacle->getPositionX() + _minDistanceObstaclesX;
    }
}

#pragma mark - Create Obstacle

void GameLayer::_createObstacle(float x) {
    BaseObstacle* obstacle = NULL;
    float y;
    int z = 0;
    
    int type = _vectorMap[_itemMap];
    
    if(type == 0) {
        obstacle = new SingleObstacle();
        y = OBSTACLE_SIMPLE_BOT_Y;
        z = OBSTACLE_SIMPLE_BOT_Y;
    } else if(type == 1) {
        obstacle = new SingleObstacle();
        y = OBSTACLE_SIMPLE_TOP_Y;
        z = OBSTACLE_SIMPLE_TOP_Y;
    }else if(type == 2){
        obstacle = new DoubleObstacle();
        y = OBSTACLE_DOBLE_GROUND_Y;
        z = WIN_SIZE.height * 0.5f;
    }else if(type == 3){
        obstacle = new AirDoubleObstacle();
        y = OBSTACLE_DOBLE_AIR_Y;
    }else{
        _createMultipleObstacles(x, type);
    }
    
    if(obstacle != NULL && type <= 3){
        obstacle->setPosition(Vec2(x, y));
        obstacle->autorelease();
        _arrayObstacles.pushBack(obstacle);
        addChild(obstacle, (WIN_SIZE.height - z) + kDeepGameElements);
    }
    
    _itemMap++;
    if(_itemMap > _vectorMap.size() - 1)
        _itemMap = 0;
}

void GameLayer::_createMultipleObstacles(float x, int type) {
    float y;
    int z = 0, i = 0;
    BaseObstacle* obstacle;
    
    float distance = 0;
    float dt_distance = 0.8f;
    
    if(type == 4)
    {
        // Crear 2 obstaculos simples abajo
        y = OBSTACLE_SIMPLE_BOT_Y;
        z = OBSTACLE_SIMPLE_BOT_Y;
        
        for(i = 0; i < 2; i++)
        {
            obstacle = new SingleObstacle();
            distance = obstacle->getContentSize().width * dt_distance * 1.5f;
            obstacle->setNumObjects(0);
            if(i == 0)
            {
                obstacle->setNumObjects(2);
                obstacle->setDistanceObjects(distance);
            }
            
            obstacle->setTag((i - 1) * -1);
            obstacle->setPosition(Vec2(x, y));
            obstacle->autorelease();
            _arrayObstacles.pushBack(obstacle);
            addChild(obstacle, (WIN_SIZE.height - z) + kDeepGameElements);
            x += distance;
        }
        
    }
    else if(type == 5)
    {
        // Crear 2 obstaculos simples arriba
        y = OBSTACLE_SIMPLE_TOP_Y;
        z = OBSTACLE_SIMPLE_TOP_Y;
        
        for(i = 0; i < 2; i++)
        {
            obstacle = new SingleObstacle();
            distance = obstacle->getContentSize().width * dt_distance * 1.5f;
            obstacle->setNumObjects(0);
            if(i == 0)
            {
                obstacle->setNumObjects(2);
                obstacle->setDistanceObjects(distance);
            }
            
            obstacle->setTag((i - 1) * -1);
            obstacle->setPosition(Vec2(x, y));
            obstacle->autorelease();
            _arrayObstacles.pushBack(obstacle);
            addChild(obstacle, (WIN_SIZE.height - z) + kDeepGameElements);
            x += distance;
        }
        
    }
    else if(type == 6)
    {
        // Crear 3 obstaculos dobles en tierra
        y = OBSTACLE_DOBLE_GROUND_Y;
        z = WIN_SIZE.height * 0.5f;
        
        for(i = 0; i < 3; i++)
        {
            obstacle = new DoubleObstacle();
            distance = obstacle->getContentSize().width * dt_distance;
            obstacle->setNumObjects(0);
            if(i == 0)
            {
                obstacle->setNumObjects(3);
                obstacle->setDistanceObjects(distance);
            }
            
            obstacle->setTag((i - 1) * -1);
            obstacle->setPosition(Vec2(x, y));
            obstacle->autorelease();
            _arrayObstacles.pushBack(obstacle);
            addChild(obstacle, (WIN_SIZE.height - z) + kDeepGameElements);
            x += distance;
        }
        
    }
    else if(type == 7)
    {
        // Crear 3 obstaculos dobles en el aire
        y = OBSTACLE_DOBLE_AIR_Y;
        
        for(i = 0; i < 3; i++)
        {
            obstacle = new AirDoubleObstacle();
            distance = obstacle->getContentSize().width * dt_distance;
            obstacle->setNumObjects(0);
            if(i == 0)
            {
                obstacle->setNumObjects(3);
                obstacle->setDistanceObjects(distance);
            }
            
            obstacle->setTag((i - 1) * -1);
            obstacle->setPosition(Vec2(x, y));
            obstacle->autorelease();
            _arrayObstacles.pushBack(obstacle);
            addChild(obstacle, (WIN_SIZE.height - z) + kDeepGameElements);
            x += distance;
        }
    }
    else if(type == 8)
    {
        // Crear 2 obstaculos dobles en tierra
        y = OBSTACLE_DOBLE_GROUND_Y;
        z = WIN_SIZE.height * 0.5f;
        
        for(i = 0; i < 2; i++)
        {
            obstacle = new DoubleObstacle();
            distance = obstacle->getContentSize().width * dt_distance;
            obstacle->setNumObjects(0);
            if(i == 0)
            {
                obstacle->setNumObjects(2);
                obstacle->setDistanceObjects(distance);
            }
            
            obstacle->setTag((i - 1) * -1);
            obstacle->setPosition(Vec2(x, y));
            obstacle->autorelease();
            _arrayObstacles.pushBack(obstacle);
            addChild(obstacle, (WIN_SIZE.height - z) + kDeepGameElements);
            x += distance;
        }
        
    }
    else if(type == 9)
    {
        // Crear 2 obstaculos dobles en el aire
        y = OBSTACLE_DOBLE_AIR_Y;
        
        for(i = 0; i < 2; i++)
        {
            obstacle = new AirDoubleObstacle();
            distance = obstacle->getContentSize().width * dt_distance;
            obstacle->setNumObjects(0);
            if(i == 0)
            {
                obstacle->setNumObjects(2);
                obstacle->setDistanceObjects(distance);
            }
            
            obstacle->setTag((i - 1) * -1);
            obstacle->setPosition(Vec2(x, y));
            obstacle->autorelease();
            _arrayObstacles.pushBack(obstacle);
            addChild(obstacle, (WIN_SIZE.height - z) + kDeepGameElements);
            x += distance;
        }
    }
    
}

#pragma mark - Accelerometer manager

void GameLayer::didAccelerate(Acceleration *pAccelerationValue, Event* event) {
    if(!_pause && !_gameOver)
    {
        if(_gameState == kGameReady)
        {
            float x = pAccelerationValue->x * _player->getSpeed() * 0.5f;
            float y = pAccelerationValue->y * _player->getSpeed() * 0.5f;
            _accelVelocity = Vec2(x,y) * _player->getSpeed() * 0.5f;
        }
    }
}

#pragma mark - Game States Manager

void GameLayer::playGame() {
    _gameMode = kGameModePlay;
    runGame();
}

void GameLayer::runGame() {
    _isJoypad = LocalStorageManager::isUsingJoypad();
    
    if(!_isJoypad) {
        _accelerometerEnabled = true;
        Device::setAccelerometerEnabled(true);
        auto listener = EventListenerAcceleration::create(CC_CALLBACK_2(GameLayer::didAccelerate, this));
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    }
    _hudLayer->updateVisibility();
    
    unscheduleUpdate();
    scheduleUpdate();
}

void GameLayer::pauseGame(Ref* pSender) {
    if(_gameState == kGameFinish)
        return;
    
    if(_gameMode == kGameModePlay || _gameMode == kGameModePlayAgain || _gameMode == kGameModeReplayView) {
        if(!_pause) {
            AudioEngine::play2d(SFX_BUTTON);
            
            _pause = true;
            _previousGameState = _gameState;
            _gameState = kGamePause;
            
            unscheduleAllCallbacks();
            _pauseAllActions();
            
            _menuPause->setVisible(false);
            _hudLayer->setVisible(false);
            _pauseLayer->setVisible(true);
            
        }
    }
}

void GameLayer::resumeGame()
{
    if(_gameState == kGameFinish)
        return;
    
    if(_gameMode == kGameModePlay || _gameMode == kGameModePlayAgain || _gameMode == kGameModeReplayView)
    {
        if(_gameState == kGamePause)
        {
            _gameState = _previousGameState;
            _hudLayer->setVisible(true);
            _pauseLayer->setVisible(false);
            _menuPause->setVisible(true);
            _resumeEvents();
        }
    }
}

void GameLayer::_resumeEvents() {
    scheduleUpdate();
    _resumeAllActions();
    _pause = false;
}

#pragma mark - Remove Nodes

void GameLayer::_removeNode(Node *node) {
    node->removeFromParentAndCleanup(false);
}

#pragma mark - Game Logic

void GameLayer::_gameLogic(float dt) {
    if(_isJoypad) {
        // Update Control
        _hudLayer->updateControl(*_player, dt);
    } else {
        // Accelerometer mode
        _player->doMove(_accelVelocity);
    }
    
    // Increment map speed
    _worldSpeed += dt * 2;
    
    string scoreText = StringUtils::format("%d", _obstaclesAvoided);
    _lblScore->setString(scoreText);
    
    int z = (WIN_SIZE.height - (_player->getPlayerY() + _player->getContentSize().height * 0.75f)) + kDeepGameElements;
    
    this->reorderChild(_player, z);
    
    Sprite* sprite;
    BaseObstacle* obstacle;
    float spriteWidth;
    
    // Move cloud
    if(_spCloud->getPositionX() <= -_spCloud->getContentSize().width * 0.5f)
        _spCloud->setPositionX(WIN_SIZE.width + _spCloud->getContentSize().width * 0.7f);
    _spCloud->setPositionX(_spCloud->getPositionX() - _worldSpeed * dt * DT_SPEED_NUBE);
    
    Color3B color = Color3B((int)_color, (int)_color, (int)_color);
    _parallaxSky.at(0)->setColor(color);
    _parallaxSky.at(1)->setColor(color);
    _spCloud->setColor(color);
    
    // Bucle for _parallaxBGBack
    for(int i = 0; i < _parallaxBGBack.size(); i++) {
        sprite = _parallaxBGBack.at(i);
        spriteWidth = sprite->getContentSize().width;
        if(sprite->getPositionX() <= -spriteWidth) {
            float diff = spriteWidth + sprite->getPositionX();
            sprite->setPositionX((_parallaxBGBack.size() - 1) * spriteWidth + diff);
        }
        sprite->setPositionX(sprite->getPositionX() - _worldSpeed * dt * DT_SPEED_BG_BACK);
        sprite->setColor(color);
    }
 
    // Bucle for _parallaxBGMid
    for(int i = 0; i < _parallaxBGMid.size(); i++) {
        sprite = _parallaxBGMid.at(i);
        spriteWidth = sprite->getContentSize().width;
        if(sprite->getPositionX() <= -spriteWidth) {
            float diff = spriteWidth + sprite->getPositionX();
            sprite->setPositionX((_parallaxBGMid.size() - 1) * spriteWidth + diff);
        }
        sprite->setPositionX(sprite->getPositionX() - _worldSpeed * dt * DT_SPEED_BG_MID);
        sprite->setColor(color);
    }
        
    // Bucle for _parallaxBGFront
    for(int i = 0; i < _parallaxBGFront.size(); i++) {
        sprite = _parallaxBGFront.at(i);
        spriteWidth = sprite->getContentSize().width;
        
        if(sprite->getPositionX() <= -spriteWidth) {
            float diff = spriteWidth + sprite->getPositionX();
            sprite->setPositionX((_parallaxBGFront.size() - 1) * spriteWidth + diff);
        }
        sprite->setPositionX(sprite->getPositionX() - _worldSpeed * dt * DT_SPEED_BG_FRONT);
        sprite->setColor(color);
    }
    
    _color += dt * _colorSign * 3;
    
    if(_color < MIN_COLOR && _colorSign == -1)
        _colorSign = 1;
    else if(_color > MAX_COLOR && _colorSign == 1)
        _colorSign = -1;
    
    if(_color > MAX_COLOR)
        _color = MAX_COLOR;
    
    if(_color < MIN_COLOR)
        _color = MIN_COLOR;
    
    // Bucle for _parallaxFloor
    for(int i = 0; i < _parallaxFloor.size(); i++) {
        sprite = _parallaxFloor.at(i);
        spriteWidth = sprite->getContentSize().width;
        
        if(sprite->getPositionX() <= -spriteWidth)
        {
            float diff = spriteWidth + sprite->getPositionX();
            sprite->setPositionX((_parallaxFloor.size() - 1) * spriteWidth + diff);
        }
        sprite->setPositionX(sprite->getPositionX() - _worldSpeed * dt * DT_SPEED_PISTA);
    }
    
    // Move obstacles, detect collisions, play sfx and remove
    std::vector<BaseObstacle*> _removeObstacles;
    for(int i = 0; i < _arrayObstacles.size(); i++) {
        obstacle = _arrayObstacles.at(i);
        float positionX = obstacle->getPositionX();
        
        // Show object
        obstacle->doUpdate(positionX, _worldSpeed * dt * DT_SPEED_OBSTACULOS);
        
        if(obstacle->getPositionX() < -obstacle->getContentSize().width * 0.5f)
        {
            _removeObstacles.push_back(obstacle);
        }
        else
        {
            if(obstacle->getPositionX() < WIN_SIZE.width && obstacle->getPositionX() > 0 && obstacle->collision(*_player))
            {
                if(_hudLayer->isVisible())
                {
                    _hudLayer->setVisible(false);
                    _hudLayer->joypad->setEnabled(false);
                }
                
                _lblScore->setVisible(false);
                _menuPause->setVisible(false);
                _player->dead();
                this->reorderChild(_player, kDeepGameFinish);
                _gameOver = true;
                _gameState = kGameFinish;
                break;
            } else {
                if(!obstacle->getPassPlayerSFX() && obstacle->getPositionX() + obstacle->getContentSize().width * 1.0f < _player->getPositionX())
                {
                    obstacle->setPassPlayerSFX(true);
                    if(obstacle->getObstacType() == kJumpObstacle)
                    {
                        _obstaclesJumped++;
                    }
                    _obstaclesAvoided++;
                    AudioEngine::play2d(SFX_SWOOSH);
                }
                
            }
        }
        
    }
    
    // Remove and Add objects
    for(int i = 0; i < _removeObstacles.size(); i++) {
        BaseObstacle* currentObstacle = _removeObstacles[i];
        BaseObstacle* lastObstacle = _arrayObstacles.back();
        
        int currentTag = currentObstacle->getTag();
        _arrayObstacles.eraseObject(currentObstacle);
        currentObstacle->removeFromParentAndCleanup(false);
        
        if(currentTag == 1) {
            float x = lastObstacle->getPositionX() + _minDistanceObstaclesX;
            _createObstacle(x);
        }
    }
}

bool GameLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
    if(!_pause && !_gameOver) {
        if(_gameState == kGameReady) {
            Point location = touch->getLocationInView();
            
            if(location.x >= WIN_SIZE.width * 0.5f)
                _player->doJump();
            return true;
        }
    }
    return false;
}

#pragma mark - Update method

void GameLayer::update(float dt)
{
    if(_pause)
        return;
    
    if(_gameState == kGameStarting) {
        _gameState = kGamePreparing;
        _player->runAction(MoveTo::create(1.5f, Vec2(_player->getContentSize().width * 2.5f, _player->getPositionY())));
    }
    
    else if(_gameState == kGamePreparing)
    {
        
        if(_player->getNumberOfRunningActions() <= 1)
        {
            
            _lblScore->setVisible(true);
            if(_isJoypad)
                _hudLayer->setVisible(true);
            
            if(LocalStorageManager::showTutorial())
                _showTutorial();
            else
                _gameIsReady();
        }
        
    }
    else if(_gameState == kGameReady)
    {
        if(!_gameOver)
        {
            _gameLogic(dt);
        }
        
    }
    else if(_gameOver && _gameState == kGameFinish)
    {
        if(_player->getNumberOfRunningActions() == 0)
        {
            _gameState = kGameEnd;
            AudioEngine::stopAll();
            
            _checkAchievements();
            
            _touchEnabled = false;
            if(!_isJoypad) {
                _accelerometerEnabled = false;
            }
            _hudLayer->setVisible(false);
            _menuPause->setVisible(false);
            _popUpLoseLayer->updateScore(_gameLevel, _obstaclesAvoided * kScoreFactor, _obstaclesAvoided);
            _popUpLoseLayer->runAction(MoveBy::create(0.25f, Vec2(0, WIN_SIZE.height)));
            unscheduleUpdate();
        }
    }
    
}

void GameLayer::_gameIsReady()
{
    _gameState = kGameReady;
    _touchEnabled = true;
    _menuPause->setVisible(true);
    
    if(_hudLayer->isVisible())
        _hudLayer->joypad->setEnabled(true);
    
    if(!LocalStorageManager::isMute())
        _showAudioPlaying();
}

void GameLayer::_showTutorial()
{
    
    unscheduleUpdate();
    _pauseAllActions();
    
    const char* tap_to_jump = "Tap to Jump";
    const char* joypad_move = "Joypad to move";
    const char* tilt_move = "Tilt to move";
    const char* tap_continue = "Tap here to continue";
    const char* avoid_obstacles = "Avoid the obstacles!";

    _gameState = kGameTutorial;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point visibleOrigin = Director::getInstance()->getVisibleOrigin();
    
    Point center = Vec2(0,0);
    center.x = visibleOrigin.x + visibleSize.width * 0.5f;
    center.y = visibleOrigin.y + visibleSize.height * 0.5f;
    
    LayerColor* layer = LayerColor::create(Color4B(0, 0, 0, 128));
    layer->setTag(kTagTutorialLayer);
    
    Sprite* spriteJump = Sprite::create("tap.png");
    spriteJump->setPositionX(visibleOrigin.x + visibleSize.width - spriteJump->getContentSize().width * 0.5f);
    spriteJump->setPositionY(visibleOrigin.y + spriteJump->getContentSize().height * 0.4f);
    layer->addChild(spriteJump);
    spriteJump->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.5f, Vec2(0, -9)), MoveBy::create(0.5f, Vec2(0, 9)), NULL)));
    
    // Jump Tutorial
    Label* lblJump = Label::createWithTTF(tap_to_jump, FONT_GAME, SIZE_TUT_INST);
    lblJump->setPosition(center);
    lblJump->setPositionY(spriteJump->getPositionY() + spriteJump->getContentSize().height * 0.6f);
    lblJump->setPositionX(lblJump->getPositionX() + visibleSize.width * 0.3f);
    layer->addChild(lblJump);
    
    if(_isJoypad)
    {
        // Press Joypad Tutorial
        Label* lblJoypad = Label::createWithTTF(joypad_move, FONT_GAME, SIZE_TUT_INST);
        lblJoypad->setPosition(center);
        lblJoypad->setPositionY(lblJump->getPositionY() - spriteJump->getContentSize().height * 0.1f);
        lblJoypad->setPositionX(lblJoypad->getPositionX() - visibleSize.width * 0.32f);
        layer->addChild(lblJoypad);
        
        _hudLayer->runTutorialJoypad();
    }
    else
    {
        
        Sprite* tilt = Sprite::create("tilt_icon.png");
        tilt->setScale(1.9f);
        tilt->setPositionX(center.x - visibleSize.width * 0.35f);
        tilt->setPositionY(lblJump->getPositionY() - tilt->getContentSize().height * 0.95f);
        layer->addChild(tilt);
        
        tilt->runAction(RepeatForever::create(Sequence::create(RotateTo::create(0.5f, -8), RotateTo::create(0.5f, 8), NULL)));
        
        // Accelerometer
        Label* lblAccelerometer = Label::createWithTTF(tilt_move, FONT_GAME, SIZE_TUT_INST);
        lblAccelerometer->setPosition(center);
        lblAccelerometer->setPositionY(lblJump->getPositionY() + tilt->getContentSize().height * 0.55f);
        lblAccelerometer->setPositionX(lblAccelerometer->getPositionX() - visibleSize.width * 0.35f);
        layer->addChild(lblAccelerometer);
    }
    
    Label* closeTutorialLabel = Label::createWithTTF(tap_continue, FONT_GAME, SIZE_TUT_TITLE);
    
    MenuItemLabel* menuCloseTutorial = MenuItemLabel::create(closeTutorialLabel, CC_CALLBACK_1(GameLayer::_finishTutorial, this));
    menuCloseTutorial->setPosition(center);
    menuCloseTutorial->setPositionY(visibleOrigin.y + visibleSize.height - menuCloseTutorial->getContentSize().height * 1.9f);
    
    // Avoid the obstacles
    Label* avoidLabel = Label::createWithTTF(avoid_obstacles, FONT_GAME, SIZE_TUT_INST);
    avoidLabel->setPosition(center);
    avoidLabel->setPositionY(visibleOrigin.y +  menuCloseTutorial->getContentSize().height * 1.2f);
    layer->addChild(avoidLabel);
    
    Menu* menu = Menu::create(menuCloseTutorial, NULL);
    menu->setPosition(Vec2(0,0));
    layer->addChild(menu);
    
    addChild(layer, kDeepTutorial);
    
    menuCloseTutorial->runAction(RepeatForever::create(Sequence::create(RotateTo::create(0.5f, -2), RotateTo::create(0.5f, 2), NULL)));
}

void GameLayer::_finishTutorial(cocos2d::Ref* pSender) {
    LocalStorageManager::isTutorialOn(false);
    
    MenuItem* item = (MenuItem *) pSender;
    item->setEnabled(false);
    
    if(_isJoypad)
        _hudLayer->stopTutorialJoypad();
    
    LayerColor* layer = (LayerColor *) getChildByTag(kTagTutorialLayer);
    layer->removeFromParent();
    
    scheduleUpdate();
    _resumeAllActions();
    _gameIsReady();
    pauseGame(nullptr);
}

void GameLayer::_checkAchievements()
{
    
    long longScore = (long) (_obstaclesAvoided * kScoreFactor);
    
    // Obstacles avoidment
    
    if(_gameLevel == kGameLevelEasy && _obstaclesAvoided >= 100) {
        if(!LocalStorageManager::isAchievementUnlocked(ACH_AVOID_100_OBSTACLES_IN_EASY_MODE)) {
            LocalStorageManager::unlockAchievement(ACH_AVOID_100_OBSTACLES_IN_EASY_MODE);
        }
        
    } else if(_gameLevel == kGameLevelNormal && _obstaclesAvoided >= 50) {
        
        if(!LocalStorageManager::isAchievementUnlocked(ACH_AVOID_50_OBSTACLES_IN_NORMAL_MODE)) {
            LocalStorageManager::unlockAchievement(ACH_AVOID_50_OBSTACLES_IN_NORMAL_MODE);
        }
        
    }
    else if(_gameLevel == kGameLevelHard && _obstaclesAvoided >= 25)
    {
        if(!LocalStorageManager::isAchievementUnlocked(ACH_AVOID_25_OBSTACLES_IN_HARD_MODE)) {
            LocalStorageManager::unlockAchievement(ACH_AVOID_25_OBSTACLES_IN_HARD_MODE);
        }
    }
    
    if(_gameLevel == kGameLevelHard && _obstaclesAvoided >= 100) {
        if(!LocalStorageManager::isAchievementUnlocked(ACH_AVOID_100_OBSTACLES_IN_HARD_MODE)) {
            LocalStorageManager::unlockAchievement(ACH_AVOID_100_OBSTACLES_IN_HARD_MODE);
        }
    }

    if(!LocalStorageManager::isAchievementUnlocked(ACH_MORE_THAN_3000) && longScore > 3000) {
        LocalStorageManager::unlockAchievement(ACH_MORE_THAN_3000);
    }
    
    if(!LocalStorageManager::isAchievementUnlocked(ACH_GET_10000_OR_MORE_IN_EASY_MODE) && _gameLevel == kGameLevelEasy && longScore >= 10000) {
        LocalStorageManager::unlockAchievement(ACH_GET_10000_OR_MORE_IN_EASY_MODE);
    }
    
    if(!LocalStorageManager::isAchievementUnlocked(ACH_GET_8000_OR_MORE_IN_NORMAL_MODE) && _gameLevel == kGameLevelNormal && longScore >= 8000){
        LocalStorageManager::unlockAchievement(ACH_GET_8000_OR_MORE_IN_NORMAL_MODE);
    }
    
    if(!LocalStorageManager::isAchievementUnlocked(ACH_GET_5000_OR_MORE_IN_HARD_MODE) && _gameLevel == kGameLevelHard && longScore >= 5000){
        LocalStorageManager::unlockAchievement(ACH_GET_5000_OR_MORE_IN_HARD_MODE);
    }
    
    if(!LocalStorageManager::isAchievementUnlocked(ACH_PLAY_IN_ACCELEROMETER_MODE_AND_GET_MORE_THAN_3000) && !_isJoypad && longScore >= 3000){
        LocalStorageManager::unlockAchievement(ACH_PLAY_IN_ACCELEROMETER_MODE_AND_GET_MORE_THAN_3000);
    }
    
    if(!LocalStorageManager::isAchievementUnlocked(ACH_GET_30K_IN_EASY_MODE) && _gameLevel == kGameLevelEasy && longScore >= 30000){
        LocalStorageManager::unlockAchievement(ACH_GET_30K_IN_EASY_MODE);
    }
    
    if(!LocalStorageManager::isAchievementUnlocked(ACH_GET_15K_IN_NORMAL_MODE) && _gameLevel == kGameLevelNormal && longScore >= 15000){
        LocalStorageManager::unlockAchievement(ACH_GET_15K_IN_NORMAL_MODE);
    }
    
    if(!LocalStorageManager::isAchievementUnlocked(ACH_GET_10K_IN_HARD_MODE) && _gameLevel == kGameLevelHard && longScore >= 10000){
        LocalStorageManager::unlockAchievement(ACH_GET_10K_IN_HARD_MODE);
    }
    
    if(_obstaclesJumped > 0) {
        int totalObstaclesJumped = LocalStorageManager::getObstaclesJumped() + _obstaclesJumped;
        LocalStorageManager::updateObstaclesJumped(totalObstaclesJumped);
    }
    _obstaclesJumped = 0;
    
}

#pragma mark - Draw Method
void GameLayer::draw(Renderer* renderer, const Mat4& transform, uint32_t flags) {
    
}
/*
 void GameLayer::draw()
 {
 CCLayer::draw();
 
 if(DRAW_COLLISIONS && _gameState == kGameReady)
 {
 
 CCObject* object;
 
 CCARRAY_FOREACH(_arrayObstacles, object)
 {
 BaseObstacle* obstacle = (BaseObstacle*) object;
 if(obstacle != NULL)
 {
 int i;
 CCRect area;
 float left, top, right, bottom;
 
 std::vector<CCRect> areas = obstacle->getVCollision();
 
 if(areas.size() > 0)
 {
 for(i = 0; i < areas.size(); i++)
 {
 area = obstacle->currentCollisionArea(areas[i]);
 
 left = area.getMinX();
 top = area.getMinY();
 right = area.getMaxX();
 bottom = area.getMaxY();
 
 CCPoint origin = ccp(left, top);
 CCPoint destination = ccp(right, bottom);
 ccDrawSolidRect(origin, destination, ccc4f(0.0f, 1.0f, 0.0f, 0.25f));
 
 
 }
 }
 
 if(obstacle->getObstacType() == kSimpleObstacle)
 {
 area = obstacle->boundingBox();
 top = area.getMinY() + obstacle->getContentSize().height * 0.0f;
 bottom = top + obstacle->getContentSize().height * 0.37f;
 left = area.getMinX();
 right = area.getMaxX();
 
 CCPoint origin = ccp(left, top);
 CCPoint destination = ccp(right, bottom);
 ccDrawSolidRect(origin, destination, ccc4f(0.0f, 0.0f, 1.0f, 0.25f));
 
 }
 
 }
 }
 
 if(_player == NULL)
 return;
 
 // Verde
 CCPoint originGreenPlayer = ccp(_player->getGroundCollision().getMinX(), _player->getGroundCollision().getMinY());
 CCPoint destionationGreenPlayer = ccp(_player->getGroundCollision().getMaxX(), _player->getGroundCollision().getMaxY());
 ccDrawSolidRect(originGreenPlayer, destionationGreenPlayer, ccc4f(0.0f, 1.0f, 0.0f, 0.25f));
 
 // Rojo
 CCPoint originRedPlayer = ccp(_player->getAirCollision().getMinX(), _player->getAirCollision().getMinY());
 CCPoint destionationRedPlayer = ccp(_player->getAirCollision().getMaxX(), _player->getAirCollision().getMaxY());
 ccDrawSolidRect(originRedPlayer, destionationRedPlayer, ccc4f(1.0f, 0.0f, 0.0f, 0.25f));
 
 
 }
 
 if(TEST_OBSTACLE)
 {
 
 CCArray* _testObstacles = CCArray::create();
 _testObstacles->addObject((CCSprite *) getChildByTag(1000));
 _testObstacles->addObject((CCSprite *) getChildByTag(1001));
 _testObstacles->addObject((CCSprite *) getChildByTag(1002));
 _testObstacles->addObject((CCSprite *) getChildByTag(1003));
 
 CCObject* object;
 
 CCARRAY_FOREACH(_testObstacles, object)
 {
 BaseObstacle* obstacle = (BaseObstacle*) object;
 if(obstacle != NULL)
 {
 int i;
 CCRect area;
 float left, top, right, bottom;
 
 std::vector<CCRect> areas = obstacle->getVCollision();
 
 if(areas.size() > 0)
 {
 for(i = 0; i < areas.size(); i++)
 {
 area = obstacle->currentCollisionArea(areas[i]);
 
 left = area.getMinX();
 top = area.getMinY();
 right = area.getMaxX();
 bottom = area.getMaxY();
 
 CCPoint origin = ccp(left, top);
 CCPoint destination = ccp(right, bottom);
 ccDrawSolidRect(origin, destination, ccc4f(0.0f, 1.0f, 0.0f, 0.25f));
 
 
 }
 }
 
 if(obstacle->getObstacType() == kSimpleObstacle)
 {
 area = obstacle->boundingBox();
 top = area.getMinY() + obstacle->getContentSize().height * 0.1f;
 bottom = top + obstacle->getContentSize().height * 0.37f;
 left = area.getMinX();
 right = area.getMaxX();
 
 CCPoint origin = ccp(left, top);
 CCPoint destination = ccp(right, bottom);
 ccDrawSolidRect(origin, destination, ccc4f(0.0f, 0.0f, 1.0f, 0.25f));
 
 }
 
 }
 }
 }
 
 }
 */
#pragma mark - Notification

void GameLayer::_playAgain() {
    refreshLayer();
    Scene* scene = HomeScene::scene(kGameModePlayAgain, _gameLevel);
    Director::getInstance()->replaceScene(TransitionFadeDown::create(0.5f, scene));
}

void GameLayer::_goHome() {
    refreshLayer();
    Scene* scene = HomeScene::scene(kGameModeHome, kGameLevelNone);
    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
}

#pragma mark - Pause All Actions and Resume

void GameLayer::_pauseAllActions() {
    Node* node;
    auto children = getChildren();
    for(int i = 0; i < children.size(); i++) {
        Sprite* sprite = (Sprite*) children.at(i);
        if(sprite != NULL) {
            sprite->pause();
        }
    }
}

void GameLayer::_resumeAllActions()
{
    Node* node;
    auto children = getChildren();
    for(int i = 0; i < children.size(); i++) {
        Sprite* sprite = (Sprite*) children.at(i);
        if(sprite != NULL) {
            sprite->resume();
        }
    }
}


void GameLayer::keyBackClicked() {
    Director::getInstance()->stopAnimation();
    AudioEngine::stopAll();
    Director::getInstance()->end();
}
