#include "main.h"

TFT_eSPI tft = TFT_eSPI();

TFT_eSprite setWorkTimeSprite = TFT_eSprite(&tft);
TFT_eSprite setRestTimeSprite = TFT_eSprite(&tft);
TFT_eSprite setTotalRepsSprite = TFT_eSprite(&tft);

TFT_eSprite countInfoSprite = TFT_eSprite(&tft);
TFT_eSprite countDownSprite = TFT_eSprite(&tft);
TFT_eSprite leftButtonSprite = TFT_eSprite(&tft);
TFT_eSprite rightButtonSprite = TFT_eSprite(&tft);

OneButton leftButton(LEFT_BTN, true);
OneButton rightButton(RIGHT_BTN, true);

byte timerState = 0; // 0: setup, 1: running, 2: paused, 3: finished

int menuItem = 0;
bool increaseSelected = false;
bool decreaseSelected = false;

int preCountDown = 3;
int workTime = 12;
int restTime = 5;
int totalReps = 2;
bool isWork = false;

int currentCount = 5;
int currentRep = 0;

long menuPrevMillis = 0;
long menuCurrMillis = 0;
long countDownPrevMillis = 0;
long countDownCurrMillis = 0;
long flashScreenPrevMillis = 0;
long flashScreenCurrMillis = 0;

bool shouldFlashScreen = false;

void handleLeftButtonClick()
{
  Serial.println("Left button clicked");
  if (timerState == 0)
  {
    menuItem--;
    if (menuItem < 0)
      menuItem = 2;
    updateMenuItem();
  }
}

void handleLeftButtonDoubleClick()
{
  Serial.println("Left button double clicked");
}

void handleLeftButtonLongPressStart()
{
  if (timerState == 0)
  {
    if (!increaseSelected)
      decreaseSelected = true;
  }
  else
  {
    timerState = 0;
    currentRep = 0;
    preCountDown = 3;
    removeMenuSprites();
    setupMenuSprites();
    setupButtonSprites();
  }
}
void handleLeftButtonLongPressStop()
{
  if (timerState == 0)
  {
    decreaseSelected = false;
  }
}
void handleRightButtonClick()
{
  Serial.println("Right button clicked");
  if (timerState == 0)
  {
    menuItem++;
    if (menuItem > 2)
      menuItem = 0;
    updateMenuItem();
  }
  else if (timerState == 1)
  { // Run state, goto pause
    timerState = 2;
    updateRunButtons();
  }
  else if (timerState == 2)
  { // Pause state, goto run
    timerState = 1;
    updateRunButtons();
  }
}

void handleRightButtonDoubleClick()
{
  Serial.println("Right button double clicked");
  if (timerState == 0)
  {
    timerState = 1;
    removeMenuSprites();
    setupCountDownSprites();
    setupRunButtons();
  }
}

void handleRightButtonLongPressStart()
{
  if (timerState == 0)
  {
    if (!decreaseSelected)
      increaseSelected = true;
  }
}
void handleRightButtonLongPressStop()
{
  if (timerState == 0)
  {
    increaseSelected = false;
  }
}

void setupButtons()
{
  leftButton.attachClick(handleLeftButtonClick);
  leftButton.attachDoubleClick(handleLeftButtonDoubleClick);
  leftButton.attachLongPressStart(handleLeftButtonLongPressStart);
  leftButton.attachLongPressStop(handleLeftButtonLongPressStop);
  leftButton.setLongPressIntervalMs(800);
  rightButton.attachClick(handleRightButtonClick);
  rightButton.attachDoubleClick(handleRightButtonDoubleClick);
  rightButton.attachLongPressStart(handleRightButtonLongPressStart);
  rightButton.attachLongPressStop(handleRightButtonLongPressStop);
  rightButton.setLongPressIntervalMs(800);
}

void updateCountInfo()
{
  countInfoSprite.fillSprite(TFT_BLACK);
  countInfoSprite.drawString(String(currentRep) + " / " + String(totalReps), 10, 10);
  countInfoSprite.pushSprite(0, 0);
}

void setupCountDownSprites()
{
  countInfoSprite.createSprite(TFT_WIDTH, 50);
  countDownSprite.createSprite(TFT_WIDTH, 100);

  countInfoSprite.setTextSize(3);
  countInfoSprite.drawString("Get Ready", 3, 10);
  countInfoSprite.pushSprite(0, 0);

  countDownSprite.setTextSize(10);
  countDownSprite.setTextDatum(MC_DATUM);
  countDownSprite.drawString(String(preCountDown), countDownSprite.width() / 2, countDownSprite.height() / 2 + 2);
  countDownSprite.pushSprite(0, 50);
}

void updateMenuItem()
{
  Serial.println("Updating menu item: " + String(menuItem));

  if (menuItem == 0)
  {
    setWorkTimeSprite.fillSprite(TFT_WHITE);
    setWorkTimeSprite.setTextColor(TFT_BLACK, TFT_WHITE);
    setRestTimeSprite.fillSprite(TFT_BLACK);
    setRestTimeSprite.setTextColor(TFT_WHITE, TFT_BLACK);
    setTotalRepsSprite.fillSprite(TFT_BLACK);
    setTotalRepsSprite.setTextColor(TFT_WHITE, TFT_BLACK);
  }
  else if (menuItem == 1)
  {
    setWorkTimeSprite.fillSprite(TFT_BLACK);
    setWorkTimeSprite.setTextColor(TFT_WHITE, TFT_BLACK);
    setRestTimeSprite.fillSprite(TFT_WHITE);
    setRestTimeSprite.setTextColor(TFT_BLACK, TFT_WHITE);
    setTotalRepsSprite.fillSprite(TFT_BLACK);
    setTotalRepsSprite.setTextColor(TFT_WHITE, TFT_BLACK);
  }
  else if (menuItem == 2)
  {
    setWorkTimeSprite.fillSprite(TFT_BLACK);
    setWorkTimeSprite.setTextColor(TFT_WHITE, TFT_BLACK);
    setRestTimeSprite.fillSprite(TFT_BLACK);
    setRestTimeSprite.setTextColor(TFT_WHITE, TFT_BLACK);
    setTotalRepsSprite.fillSprite(TFT_WHITE);
    setTotalRepsSprite.setTextColor(TFT_BLACK, TFT_WHITE);
  }
  setWorkTimeSprite.setTextSize(2);
  setWorkTimeSprite.drawString("WORK TIME", setWorkTimeSprite.width() / 2, 20);
  setWorkTimeSprite.setTextSize(4);
  setWorkTimeSprite.drawString(String(workTime), setWorkTimeSprite.width() / 2, 60);
  setWorkTimeSprite.pushSprite(0, 0);

  setRestTimeSprite.setTextSize(2);
  setRestTimeSprite.drawString("REST TIME", setRestTimeSprite.width() / 2, 20);
  setRestTimeSprite.setTextSize(4);
  setRestTimeSprite.drawString(String(restTime), setRestTimeSprite.width() / 2, 60);
  setRestTimeSprite.pushSprite(0, 100);

  setTotalRepsSprite.setTextSize(2);
  setTotalRepsSprite.drawString("TOTAL REPS", setTotalRepsSprite.width() / 2, 20);
  setTotalRepsSprite.setTextSize(4);
  setTotalRepsSprite.drawString(String(totalReps), setTotalRepsSprite.width() / 2, 60);
  setTotalRepsSprite.pushSprite(0, 200);
}

void setupMenuSprites()
{
  setWorkTimeSprite.createSprite(TFT_WIDTH, 100);
  setRestTimeSprite.createSprite(TFT_WIDTH, 100);
  setTotalRepsSprite.createSprite(TFT_WIDTH, 100);

  setWorkTimeSprite.setTextDatum(MC_DATUM);
  setRestTimeSprite.setTextDatum(MC_DATUM);
  setTotalRepsSprite.setTextDatum(MC_DATUM);

  updateMenuItem();
}

void removeMenuSprites()
{
  setWorkTimeSprite.deleteSprite();
  setRestTimeSprite.deleteSprite();
  setTotalRepsSprite.deleteSprite();
  tft.fillScreen(TFT_DARKGREY);
}

void setupButtonSprites()
{
  leftButtonSprite.createSprite(65, 20);
  rightButtonSprite.createSprite(65, 20);
  setupMenuButtons();
}

void setupFinishButtons()
{
  leftButtonSprite.fillSprite(TFT_RED);
  leftButtonSprite.setTextDatum(MC_DATUM);
  leftButtonSprite.setTextColor(TFT_BLACK, TFT_RED);
  leftButtonSprite.setTextSize(2);
  leftButtonSprite.drawString("RESET", leftButtonSprite.width() / 2, leftButtonSprite.height() / 2);
  leftButtonSprite.pushSprite(0, TFT_HEIGHT - 20);

  rightButtonSprite.fillSprite(TFT_DARKGREY);
  rightButtonSprite.pushSprite(TFT_WIDTH - rightButtonSprite.width(), TFT_HEIGHT - 20);
}

void updateRunButtons()
{
  rightButtonSprite.setTextDatum(MC_DATUM);
  rightButtonSprite.setTextSize(2);
  if (timerState == 1)
  {
    rightButtonSprite.fillSprite(TFT_YELLOW);
    rightButtonSprite.setTextColor(TFT_BLACK, TFT_YELLOW);
    rightButtonSprite.drawString("PAUSE", rightButtonSprite.width() / 2, rightButtonSprite.height() / 2);
  }
  else
  {
    rightButtonSprite.fillSprite(TFT_GREEN);
    rightButtonSprite.setTextColor(TFT_BLACK, TFT_GREEN);
    rightButtonSprite.drawString("START", rightButtonSprite.width() / 2, rightButtonSprite.height() / 2);
  }

  rightButtonSprite.pushSprite(TFT_WIDTH - rightButtonSprite.width(), TFT_HEIGHT - 20);
}

void setupMenuButtons()
{
  leftButtonSprite.fillSprite(TFT_DARKGREY);
  leftButtonSprite.setTextDatum(MC_DATUM);
  leftButtonSprite.setTextColor(TFT_BLACK, TFT_DARKGREY);
  leftButtonSprite.setTextSize(2);
  leftButtonSprite.drawString("UP", leftButtonSprite.width() / 2, leftButtonSprite.height() / 2);
  leftButtonSprite.pushSprite(0, TFT_HEIGHT - 20);

  rightButtonSprite.fillSprite(TFT_DARKGREY);
  rightButtonSprite.setTextDatum(MC_DATUM);
  rightButtonSprite.setTextColor(TFT_BLACK, TFT_DARKGREY);
  rightButtonSprite.setTextSize(2);
  rightButtonSprite.drawString("DOWN", rightButtonSprite.width() / 2, rightButtonSprite.height() / 2);
  rightButtonSprite.pushSprite(TFT_WIDTH - rightButtonSprite.width(), TFT_HEIGHT - 20);
}

void setupRunButtons()
{
  leftButtonSprite.fillSprite(TFT_RED);
  leftButtonSprite.setTextDatum(MC_DATUM);
  leftButtonSprite.setTextColor(TFT_BLACK, TFT_RED);
  leftButtonSprite.setTextSize(2);
  leftButtonSprite.drawString("RESET", leftButtonSprite.width() / 2, leftButtonSprite.height() / 2);
  leftButtonSprite.pushSprite(0, TFT_HEIGHT - 20);

  rightButtonSprite.fillSprite(TFT_YELLOW);
  rightButtonSprite.setTextDatum(MC_DATUM);
  rightButtonSprite.setTextColor(TFT_BLACK, TFT_YELLOW);
  rightButtonSprite.setTextSize(2);
  rightButtonSprite.drawString("PAUSE", rightButtonSprite.width() / 2, rightButtonSprite.height() / 2);
  rightButtonSprite.pushSprite(TFT_WIDTH - rightButtonSprite.width(), TFT_HEIGHT - 20);
}

void updateCounterFinish()
{
  countDownSprite.fillSprite(TFT_GREEN);
  countDownSprite.setTextColor(TFT_BLACK, TFT_GREEN);
  countDownSprite.drawString("DONE", countDownSprite.width() / 2 + 3, countDownSprite.height() / 2 + 2);
  countDownSprite.pushSprite(0, 50);
}
void updateCounter()
{
  Serial.println("Updating counter");
  if (isWork)
    countDownSprite.fillSprite(TFT_WHITE);
  else
    countDownSprite.fillSprite(TFT_GREENYELLOW);

  countDownSprite.setTextColor(TFT_BLACK);
  countDownSprite.drawString(String(currentCount), countDownSprite.width() / 2, countDownSprite.height() / 2 + 2);
  countDownSprite.pushSprite(0, 50);
}
void updateCounterInvert()
{
  countDownSprite.fillSprite(TFT_DARKGREY);
  countDownSprite.setTextColor(TFT_WHITE, TFT_DARKGREY);
  countDownSprite.drawString(String(currentCount), countDownSprite.width() / 2, countDownSprite.height() / 2 + 2);
  countDownSprite.pushSprite(0, 50);
}

void setup()
{
  Serial.begin(115200);

  tft.init();
  tft.fillScreen(TFT_DARKGREY);

  setupMenuSprites();
  setupButtonSprites();
  setupButtons();
}

void loop()
{
  leftButton.tick();
  rightButton.tick();

  menuCurrMillis = millis();
  countDownCurrMillis = millis();
  flashScreenCurrMillis = millis();

  /**** Menu 阶段 ****/
  if (timerState == 0 && menuCurrMillis - menuPrevMillis >= 250)
  {
    menuPrevMillis = menuCurrMillis;

    if (increaseSelected)
    {
      if (menuItem == 0)
      {
        workTime++;
        if (workTime > 99)
          workTime = 99;
        updateMenuItem();
      }
      else if (menuItem == 1)
      {
        restTime++;
        if (restTime > 99)
          restTime = 99;
        updateMenuItem();
      }
      else if (menuItem == 2)
      {
        totalReps++;
        if (totalReps > 99)
          totalReps = 99;
        updateMenuItem();
      }
    }
    else if (decreaseSelected)
    {
      if (menuItem == 0)
      {
        workTime--;
        if (workTime < 5)
          workTime = 5;
        updateMenuItem();
      }
      else if (menuItem == 1)
      {
        restTime--;
        if (restTime < 5)
          restTime = 5;
        updateMenuItem();
      }
      else if (menuItem == 2)
      {
        totalReps--;
        if (totalReps < 1)
          totalReps = 1;
        updateMenuItem();
      }
    }
  }
  /**** Menu 阶段 (end) ****/

  /**** Run 阶段 ****/
  if (timerState == 1 && countDownCurrMillis - countDownPrevMillis >= 1000)
  {
    countDownPrevMillis = countDownCurrMillis;

    if (preCountDown > 0)
    {
      currentCount = preCountDown;
      preCountDown--;
    }
    else
    {
      currentCount--;

      if (currentCount < 4 && currentCount > 0)
        shouldFlashScreen = true;
      else if (currentCount < 0)
      {
        if (isWork)
        {
          isWork = false;
          currentCount = restTime;
        }
        else
        {
          isWork = true;
          currentCount = workTime;
          currentRep++;
          if (currentRep == totalReps + 1)
          {
            timerState = 3; // finished
            currentRep--;
          }
          updateCountInfo();
        }
      }
    }

    if (timerState == 1)
      updateCounter();
    else if (timerState == 3)
    {
      updateCounterFinish();
      setupFinishButtons();
    }
  }

  if (timerState == 1)
  {
    if (shouldFlashScreen)
    {
      updateCounterInvert();
      shouldFlashScreen = false;
      flashScreenPrevMillis = flashScreenCurrMillis;
    }
    else if (flashScreenCurrMillis - flashScreenPrevMillis >= 200 && currentCount < 4 && currentCount > 0)
    {
      updateCounter();
    }
  }

  /**** Run 阶段 (end) ****/
}
