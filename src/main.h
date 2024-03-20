#include <Arduino.h>
#include "TFT_eSPI.h"
#include <OneButton.h>
#include <SPI.h>
#include <EEPROM.h>

#define LEFT_BTN 0
#define RIGHT_BTN 14

#define MIN_WORK_TIME 5
#define MAX_WORK_TIME 99
#define MIN_REST_TIME 0
#define MAX_REST_TIME 99
#define MIN_REPS 1
#define MAX_REPS 99

/** 按键事件处理相关 **/
/*
 * state == 0 选择上一菜单项
 */
void handleLeftButtonClick();
void handleLeftButtonDoubleClick();
/*
 * state == 0 菜单项对应值 -1 开始
 * state == 1|2|3 返回菜单页面
 */
void handleLeftButtonLongPressStart();
/*
 * state == 0 菜单项对应值 -1 停止
 */
void handleLeftButtonLongPressStop();
/*
 * state == 0 选择下一菜单项
 * state == 1 暂停倒计时
 * state == 2 开始倒计时
 */
void handleRightButtonClick();
/*
 * state == 0 开始倒计时
 */
void handleRightButtonDoubleClick();
/*
 * state == 0 菜单项对应值 +1 开始
 */
void handleRightButtonLongPressStart();
/*
 * state == 0 菜单项对应值 +1 停止
 */
void handleRightButtonLongPressStop();
/** 按键事件处理相关 end **/

// 初始化菜单Sprite
void setupMenuSprites();
// 移除菜单Sprite
void removeMenuSprites();
// 更新菜单项
void updateMenuItem();
// 初始化菜单界面的按键
void setupMenuButtons();
// 配置按键事件
void setupButtonEvents();
// 更新计数器，结束界面
void updateCounterFinish();
// 更新计数器，运行界面
void updateCounter();
// 更新计数器，运行界面反色
void updateCounterInvert();
// 更新计数器信息栏
void updateCountInfo();
// 更新运行界面的按键状态
void updateRunButtons();
// 初始化运行界面的按键
void setupRunButtons();
// 初始化结束界面的按键
void setupFinishButtons();
// 初始化按键Sprite
void setupButtonSprites();
// 初始化计数器Sprite
void setupCountDownSprites();
// 移除计数器Sprite
void removeCountDownSprites();
// 加载EEPROM数据
void loadData();
// 保存数据到EEPROM
void saveData();