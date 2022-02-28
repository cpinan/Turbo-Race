LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := TurboRace_shared

LOCAL_MODULE_FILENAME := libTurboRace

LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := main.cpp \
../../../Classes/common/SneakyInput/SneakyButton.cpp \
../../../Classes/common/SneakyInput/SneakyButtonSkinnedBase.cpp \
../../../Classes/common/SneakyInput/SneakyJoystick.cpp \
../../../Classes/common/SneakyInput/SneakyJoystickSkinnedBase.cpp \
../../../Classes/common/Globals.cpp \
../../../Classes/common/LocalStorageManager.cpp \
../../../Classes/models/AirDoubleObstacle.cpp \
../../../Classes/models/BaseObstacle.cpp \
../../../Classes/models/BaseVehicle.cpp \
../../../Classes/models/DoubleObstacle.cpp \
../../../Classes/models/SingleObstacle.cpp \
../../../Classes/models/VehicleFrog.cpp \
../../../Classes/ui/common/HudLayer.cpp \
../../../Classes/ui/game/GameLayer.cpp \
../../../Classes/ui/gameover/PopUpLoseLayer.cpp \
../../../Classes/ui/home/HomeLayer.cpp \
../../../Classes/ui/home/HomeScene.cpp \
../../../Classes/ui/pause/PauseLayer.cpp \
../../../Classes/ui/settings/SettingsLayer.cpp \
../../../Classes/AppDelegate.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes

LOCAL_STATIC_LIBRARIES := cc_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module, cocos)
