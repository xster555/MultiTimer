#include <Arduino.h>
#include "TFT_eSPI.h"
#include <OneButton.h>
#include <SPI.h>

#define LEFT_BTN 0
#define RIGHT_BTN 14

/** 按键事件处理相关 **/
void handleLeftButtonClick();
void handleLeftButtonDoubleClick();
void handleLeftButtonLongPressStart();
void handleLeftButtonLongPressStop();
void handleRightButtonClick();
void handleRightButtonDoubleClick();
void handleRightButtonLongPressStart();
void handleRightButtonLongPressStop();
/** 按键事件处理相关 end **/

// 更新计数器，结束界面
void updateCounterFinish();
// 更新计数器，运行界面
void updateCounter();
// 更新计数器，运行界面反色
void updateCounterInvert();
// 更新计数器信息栏
void updateCountInfo();
// 更新菜单项
void updateMenuItem();
// 更新运行界面的按键状态
void updateRunButtons();
// 初始化菜单界面的按键
void setupMenuButtons();
// 初始化运行界面的按键
void setupRunButtons();
// 初始化结束界面的按键
void setupFinishButtons();

void setupButtonSprites();
void setupCountDownSprites();
void removeCountDownSprites();
void setupMenuSprites();
void removeMenuSprites();