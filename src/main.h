#include <Arduino.h>
#include "TFT_eSPI.h"
#include <OneButton.h>
#include <SPI.h>

#define LEFT_BTN 0
#define RIGHT_BTN 14

void handleLeftButtonClick();
void handleLeftButtonDoubleClick();
void handleLeftButtonLongPressStart();
void handleLeftButtonLongPressStop();
void handleRightButtonClick();
void handleRightButtonDoubleClick();
void handleRightButtonLongPressStart();
void handleRightButtonLongPressStop();

void updateCounterFinish();
void updateCounter();
void updateCounterInvert();
void updateCountInfo();

void updateMenuItem();

void updateMenuButtons();

void setupMenuButtons();
void setupRunButtons();
void setupFinishButtons();

void setupButtonSprites();
void setupCountDownSprites();
void removeCountDownSprites();
void setupMenuSprites();
void removeMenuSprites();