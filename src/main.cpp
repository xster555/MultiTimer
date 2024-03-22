#include "main.h"

TFT_eSPI tft = TFT_eSPI();

TFT_eSprite setWorkTimeSprite = TFT_eSprite(&tft);
TFT_eSprite setRestTimeSprite = TFT_eSprite(&tft);
TFT_eSprite setTotalRepsSprite = TFT_eSprite(&tft);

TFT_eSprite countInfoSprite = TFT_eSprite(&tft);
TFT_eSprite countDownSprite = TFT_eSprite(&tft);
TFT_eSprite leftButtonSprite = TFT_eSprite(&tft);
TFT_eSprite rightButtonSprite = TFT_eSprite(&tft);

TFT_eSprite logoSprite = TFT_eSprite(&tft);

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

/** 按键事件处理相关 **/
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
    isWork = false;
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
    saveData();
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
    saveData();
  }
}
/** 按键事件处理相关 end **/

// 初始化菜单Sprite
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

// 移除菜单Sprite
void removeMenuSprites()
{
  setWorkTimeSprite.deleteSprite();
  setRestTimeSprite.deleteSprite();
  setTotalRepsSprite.deleteSprite();
  tft.fillScreen(TFT_DARKGREY);
}

// 初始化菜单界面的按键
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
  rightButtonSprite.drawString("DN/GO", rightButtonSprite.width() / 2, rightButtonSprite.height() / 2);
  rightButtonSprite.pushSprite(TFT_WIDTH - rightButtonSprite.width(), TFT_HEIGHT - 20);
}

// 配置按键事件
void setupButtonEvents()
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

// 更新菜单项
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

// 更新计数器信息栏
void updateCountInfo()
{
  countInfoSprite.fillSprite(TFT_BLACK);
  countInfoSprite.drawString(String(currentRep) + " / " + String(totalReps), 10, 10);
  countInfoSprite.pushSprite(0, 0);
}

// 初始化计数器Sprite
void setupCountDownSprites()
{
  logoSprite.createSprite(TFT_WIDTH, 55);
  logoSprite.fillSprite(TFT_DARKGREY);
  logoSprite.setTextColor(TFT_WHITE, TFT_DARKGREY);
  logoSprite.setTextSize(2);
  logoSprite.drawString("Designed by", 5, 5);
  logoSprite.setTextSize(3);
  logoSprite.drawString("XSTER", 5, 30);
  logoSprite.pushSprite(0, 220);

  countInfoSprite.createSprite(TFT_WIDTH, 50);
  countDownSprite.createSprite(TFT_WIDTH, 170);

  countInfoSprite.setTextSize(3);
  countInfoSprite.drawString("Get Ready", 3, 10);
  countInfoSprite.pushSprite(0, 0);

  countDownSprite.setTextSize(10);
  countDownSprite.setTextDatum(MC_DATUM);
  countDownSprite.drawString(String(preCountDown), countDownSprite.width() / 2, countDownSprite.height() / 2 + 2);
  countDownSprite.pushSprite(0, 50);
}

// 初始化按键Sprite
void setupButtonSprites()
{
  leftButtonSprite.createSprite(65, 20);
  rightButtonSprite.createSprite(65, 20);
  setupMenuButtons();
}

// 初始化结束界面的按键
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

// 更新运行界面的按键状态
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

// 初始化运行界面的按键
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

// 更新计数器，结束界面
void updateCounterFinish()
{
  countDownSprite.fillSprite(TFT_GREEN);
  countDownSprite.setTextColor(TFT_BLACK, TFT_GREEN);
  countDownSprite.drawString("DONE", countDownSprite.width() / 2 + 3, countDownSprite.height() / 2 + 2);
  countDownSprite.pushSprite(0, 50);
}

// 更新计数器，运行界面
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

// 更新计数器，运行界面反色
void updateCounterInvert()
{
  countDownSprite.fillSprite(TFT_DARKGREY);
  countDownSprite.setTextColor(TFT_WHITE, TFT_DARKGREY);
  countDownSprite.drawString(String(currentCount), countDownSprite.width() / 2, countDownSprite.height() / 2 + 2);
  countDownSprite.pushSprite(0, 50);
}

// 加载EEPROM数据
void loadData()
{
  EEPROM.begin(4096);
  workTime = EEPROM.read(0);
  if (workTime < MIN_WORK_TIME)
    workTime = MIN_WORK_TIME;
  else if (workTime > MAX_WORK_TIME)
    workTime = MAX_WORK_TIME;

  restTime = EEPROM.read(1);
  if (restTime < MIN_REST_TIME)
    restTime = MIN_REST_TIME;
  else if (restTime > MAX_REST_TIME)
    restTime = MAX_REST_TIME;

  totalReps = EEPROM.read(2);
  if (totalReps < MIN_REPS)
    totalReps = MIN_REPS;
  else if (totalReps > MAX_REPS)
    totalReps = MAX_REPS;

  EEPROM.end();
}

// 保存数据到EEPROM
void saveData()
{
  EEPROM.begin(4096);
  EEPROM.write(0, workTime);
  EEPROM.write(1, restTime);
  EEPROM.write(2, totalReps);
  EEPROM.commit();
  EEPROM.end();
}

TFT_eSprite H = TFT_eSprite(&tft);
void playAnimation()
{

  tft.setTextDatum(BL_DATUM);
  tft.drawString("Designed by", 10, TFT_HEIGHT - 10);
  tft.setTextSize(2);
  tft.drawString("XSTER", 83, TFT_HEIGHT - 10);
  H.createSprite(TFT_WIDTH, 280);

  H.fillSprite(TFT_BLACK);
  for (int i = 0; i <= 280; i += 20)
  {
    H.fillRect(10, 10, 25, i, TFT_RED);
    H.fillRect(TFT_WIDTH - 35, 10, 25, i, TFT_RED);
    if (i <= 120)
      H.fillTriangle(30, 120, 30, 145, 30 + i, 120, TFT_RED);

    H.pushSprite(0, 0);
    delay(10);
  }

  for (int i = 0; i <= 20; i += 10)
  {
    H.fillRect(50, 10, i, i, TFT_LIGHTGREY);
    H.fillRect(TFT_WIDTH / 2 + 15, 10, i, i, TFT_LIGHTGREY);
    H.pushSprite(0, 0);
    delay(10);
  }

  for (int i = 0; i <= 60; i += 10)
  {
    H.fillRect(50, 40, 20, i, TFT_LIGHTGREY);
    H.fillRect(TFT_WIDTH / 2 + 15, 40, 20, i, TFT_LIGHTGREY);
    H.pushSprite(0, 0);
    delay(10);
  }

  for (int i = 0; i <= 150; i += 15)
  {
    if (i <= 110)
    {
      H.fillTriangle(TFT_WIDTH - 35, 120, TFT_WIDTH - 35, 145, TFT_WIDTH - 35 - i, 145, TFT_LIGHTGREY);
    }
    H.fillRect(TFT_WIDTH / 2 - 14, 140, 32, i, TFT_LIGHTGREY);
    H.pushSprite(0, 0);
    delay(10);
  }
}

void setup()
{
  Serial.begin(115200);

  loadData();

  tft.init();
  tft.fillScreen(TFT_BLACK);
  delay(200);
  playAnimation();
  delay(800);

  tft.fillScreen(TFT_DARKGREY);

  setupMenuSprites();
  setupButtonSprites();
  setupButtonEvents();
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
        if (workTime > MAX_WORK_TIME)
          workTime = MAX_WORK_TIME;
        updateMenuItem();
      }
      else if (menuItem == 1)
      {
        restTime++;
        if (restTime > MAX_REST_TIME)
          restTime = MAX_REST_TIME;
        updateMenuItem();
      }
      else if (menuItem == 2)
      {
        totalReps++;
        if (totalReps > MAX_REPS)
          totalReps = MAX_REPS;
        updateMenuItem();
      }
    }
    else if (decreaseSelected)
    {
      if (menuItem == 0)
      {
        workTime--;
        if (workTime < MIN_WORK_TIME)
          workTime = MIN_WORK_TIME;
        updateMenuItem();
      }
      else if (menuItem == 1)
      {
        restTime--;
        if (restTime < MIN_REST_TIME)
          restTime = MIN_REST_TIME;
        updateMenuItem();
      }
      else if (menuItem == 2)
      {
        totalReps--;
        if (totalReps < MIN_REPS)
          totalReps = MIN_REPS;
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
