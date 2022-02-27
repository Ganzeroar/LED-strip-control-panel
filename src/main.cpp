#include <Arduino.h>

#include <FastLED.h>          
#include "Wire.h"
#include "rotaryDecoder.h"
#include <OLED_I2C.h>

OLED  myOLED(SDA, SCL, 8);

rotaryDecoder decoder(0x20);
rotaryDecoder decoder2(0x24);

volatile int newModeNumber = 0;
volatile int oldModeNumber = 0;
volatile int modeNumber = 10;
int currentModeNumber = 10;

volatile int newSecondEncoderValue = 0;
volatile int oldSecondEncoderValue = 0;
volatile uint8_t secondEncoderValue = 200;
int currentSecondEncoderValue = 200;

volatile int newNumberOfActivatedDidos = 0;
volatile int oldNumberOfActivatedDidos = 0;
volatile int numberOfActivatedDiods = 1;
int currentNumberOfActivatedDidos = 1;

volatile int newGapsBetweenGroupsOfDiods = 0;
volatile int oldGapsBetweenGroupsOfDiods = 0;
volatile uint8_t gapsBetweenGroupsOfDiods = 3;
int currentGapsBetweenGroupsOfDiods = 3;

volatile int newDelayTime = 0;
volatile int oldDelayTime = 0;
volatile unsigned long delayTime = 20;
unsigned long currentDelayTime = 0;

volatile int newBrightnessValue = 0;
volatile int oldBrightnessValue = 0;
volatile uint8_t brightnessValue = 50;
int currentBrightnessValue = 0;

volatile int newFirstEncoderValue = 0;
volatile int oldFirstEncoderValue = 0;
volatile uint8_t firstEncoderValue = 0;
int currentFirstEncoderValue = 0;

volatile int newThirdEncoderValue = 0;
volatile int oldThirdEncoderValue = 0;
volatile uint8_t thirdEncoderValue = 0;
int currentThirdEncoderValue = 0;
volatile int thirdEncoderMode = 0;
int currentThirdEncoderMode = 1;

volatile bool isInRandomMode = false;
bool isInSetMode = true;
bool isDirectionRight = false;
bool isDirectionLeft = true;

#define THIRDENCMODE_setOrRandom 0
#define THIRDENCMODE_direction 1


volatile bool isModeChanged = false;

#define LED_COUNT 80          // число светодиодов в кольце/ленте
#define LED_DT 5            // пин, куда подключен DIN ленты

struct CRGB leds[LED_COUNT];

int TOP_INDEX = int(LED_COUNT / 2);

void TCA9548A(uint8_t bus)
{
  Wire.beginTransmission(0x70);
  Wire.write(1 << bus);
  Wire.endTransmission();
}


#define MODE_Sparkle 1
#define MODE_activate_rainbow_vertical 2
#define MODE_activate_new_rainbow_loop 3
#define MODE_theaterChaseRainbow 4
#define MODE_activate_random_march 5
#define MODE_activate_random_burst 6
#define MODE_activate_rwb_march 7
#define MODE_activate_rgb_propeller 8
#define MODE_activate_ems_lightsONE 9
#define MODE_activate_matrix 10
#define MODE_CylonBounce 11
#define MODE_twoDiodsGroup 12
#define MODE_theaterChase 13
#define MODE_meteorRain 14
#define MODE_RunningLights 15
#define MODE_colorWipe 16
#define MODE_activate_sin_bright_wave 17
#define MODE_activate_fade_vertical 18
#define MODE_activate_pulse_one_color_all_rev 19
#define MODE_FadeInOut 20

int modeName;

bool isFifthDisplayManageCurrentFifthDisplayMode(int fifthDisplayModeNumber) {
  switch (modeName)
  {
  case 1:
    switch (fifthDisplayModeNumber)
    {
    case 0: return true;
    case 1: return false;
    }
  case 2:
    switch (fifthDisplayModeNumber)
    {
    case 0: return false;
    case 1: return false;
    }
  case 3:
    switch (fifthDisplayModeNumber)
    {
    case 0: return false;
    case 1: return true;
    }
  case 4:
    switch (fifthDisplayModeNumber)
    {
    case 0: return false;
    case 1: return true;
    }
  case 5:
    switch (fifthDisplayModeNumber)
    {
    case 0: return false;
    case 1: return true;
    }
  case 6:
    switch (fifthDisplayModeNumber)
    {
    case 0: return false;
    case 1: return false;
    }
  case 7:
    switch (fifthDisplayModeNumber)
    {
    case 0: return false;
    case 1: return true;
    }
  case 8:
    switch (fifthDisplayModeNumber)
    {
    case 0: return true;
    case 1: return true;
    }
  case 9:
    switch (fifthDisplayModeNumber)
    {
    case 0: return true;
    case 1: return true;
    }
  case 10:
    switch (fifthDisplayModeNumber)
    {
    case 0: return true;
    case 1: return true;
    }
  case 11:
    switch (fifthDisplayModeNumber)
    {
    case 0: return true;
    case 1: return false;
    }
  case 12:
    switch (fifthDisplayModeNumber)
    {
    case 0: return true;
    case 1: return false;
    }
  case 13:
    switch (fifthDisplayModeNumber)
    {
    case 0: return true;
    case 1: return true;
    }
  case 14:
    switch (fifthDisplayModeNumber)
    {
    case 0: return true;
    case 1: return true;
    }
  case 15:
    switch (fifthDisplayModeNumber)
    {
    case 0: return true;
    case 1: return true;
    }
  case 16:
    switch (fifthDisplayModeNumber)
    {
    case 0: return true;
    case 1: return true;
    }
  case 17:
    switch (fifthDisplayModeNumber)
    {
    case 0: return true;
    case 1: return true;
    }
  case 18:
    switch (fifthDisplayModeNumber)
    {
    case 0: return true;
    case 1: return true;
    }
  case 19:
    switch (fifthDisplayModeNumber)
    {
    case 0: return true;
    case 1: return false;
    }
  case 20:
    switch (fifthDisplayModeNumber)
    {
    case 0: return true;
    case 1: return false;
    }
  }
}


bool isThisModeHasDirectionAndRandomOrSetMode() {
  switch (modeName)
  {
  case 1: return false;
  case 2: return false;
  case 3: return false;
  case 4: return false;
  case 5: return false;
  case 6: return false;
  case 7: return false;
  case 8: return true;
  case 9: return true;
  case 10: return true;
  case 11: return false;
  case 12: return false;
  case 13: return true;
  case 14: return true;
  case 15: return true;
  case 16: return true;
  case 17: return true;
  case 18: return true;
  case 19: return false;
  case 20: return false;
  }
}


bool isThisDisplayManageCurrentMode(int displayNumber) {
  switch (modeName)
  {
  case 1:
    switch (displayNumber)
    {
    case 0: return false;
    case 2: return false;
    default: return true;
    }
  case 2:
    switch (displayNumber)
    {
    case 0: return false;
    case 1: return false;
    case 3: return false;
    case 5: return false;
    default: return true;
    }
  case 3:
    switch (displayNumber)
    {
    case 0: return false;
    case 1: return false;
    case 3: return false;
    default: return true;
    }
  case 4:
    switch (displayNumber)
    {
    case 3: return false;
    default: return true;
    }
  case 5:
    switch (displayNumber)
    {
    case 0: return false;
    case 1: return false;
    case 2: return false;
    case 3: return false;
    default: return true;
    }
  case 6:
    switch (displayNumber)
    {
    case 0: return false;
    case 1: return false;
    case 2: return false;
    case 3: return false;
    case 5: return false;
    default: return true;
    }
  case 7:
    switch (displayNumber)
    {
    case 0: return false;
    case 1: return false;
    case 2: return false;
    case 3: return false;
    case 5: return false;
    default: return true;
    }
  case 8:
    switch (displayNumber)
    {
    case 0: return false;
    case 2: return false;
    default: return true;
    }
  case 9:
    switch (displayNumber)
    {
    case 0: return false;
    case 2: return false;
    case 3: return false;
    default: return true;
    }
  case 10:
    switch (displayNumber)
    {
    case 0: return false;
    case 2: return false;
    default: return true;
    }
  case 11:
    switch (displayNumber)
    {
    case 2: return false;
    default: return true;
    }
  case 12:
    switch (displayNumber)
    {
    case 2: return false;
    default: return true;
    }
  case 13:
    switch (displayNumber)
    {
    default: return true;
    }
  case 14:
    switch (displayNumber)
    {
    default: return true;
    }
  case 15:
    switch (displayNumber)
    {
    case 0: return false;
    case 2: return false;
    default: return true;
    }
  case 16:
    switch (displayNumber)
    {
    case 0: return false;
    case 2: return false;
    default: return true;
    }
  case 17:
    switch (displayNumber)
    {
    case 0: return false;
    case 2: return false;
    default: return true;
    }
  case 18:
    switch (displayNumber)
    {
    case 0:  return false;
    case 2: return false;
    default: return true;
    }
  case 19:
    switch (displayNumber)
    {
    case 0: return false;
    case 2: return false;
    case 3: return false;
    default: return true;
    }
  case 20:
    switch (displayNumber)
    {
    case 0: return false;
    case 2: return false;
    default: return true;
    }
  default:
    return false;
  }
}

volatile bool isSomethingChanged = false;
volatile bool isFifthEncPressed = false;


void moved()
{
  if (isThisDisplayManageCurrentMode(3) == true) {
    if (isInRandomMode == false) {
      newSecondEncoderValue = decoder.getValue(0);
      if (newSecondEncoderValue > oldSecondEncoderValue + 4) {
        if (secondEncoderValue < 245) {
          secondEncoderValue += 10;
        }
        oldSecondEncoderValue = newSecondEncoderValue;
        isSomethingChanged = true;
      } else if (newSecondEncoderValue + 4 < oldSecondEncoderValue) {
        if (secondEncoderValue > 0) {
          secondEncoderValue -= 10;
        }
        oldSecondEncoderValue = newSecondEncoderValue;
        isSomethingChanged = true;
      }
    }
  }

  if (isThisDisplayManageCurrentMode(2) == true) {
    newGapsBetweenGroupsOfDiods = decoder.getValue(1);
    if (newGapsBetweenGroupsOfDiods > oldGapsBetweenGroupsOfDiods + 4) {
      if (gapsBetweenGroupsOfDiods < 255) {
        gapsBetweenGroupsOfDiods += 1;
      }
      oldGapsBetweenGroupsOfDiods = newGapsBetweenGroupsOfDiods;
      isSomethingChanged = true;
    } else if (newGapsBetweenGroupsOfDiods + 4 < oldGapsBetweenGroupsOfDiods) {
      if (gapsBetweenGroupsOfDiods > 0) {
        gapsBetweenGroupsOfDiods -= 1;
      }
      oldGapsBetweenGroupsOfDiods = newGapsBetweenGroupsOfDiods;
      isSomethingChanged = true;
    }
  }

  if (isThisDisplayManageCurrentMode(1) == true) {
    if (isInRandomMode == false) {
      newFirstEncoderValue = decoder.getValue(2);
      if (newFirstEncoderValue > oldFirstEncoderValue + 4) {
        if (firstEncoderValue < 245) {
          firstEncoderValue += 10;
        }
        oldFirstEncoderValue = newFirstEncoderValue;
        isSomethingChanged = true;
      } else if (newFirstEncoderValue + 4 < oldFirstEncoderValue) {
        if (firstEncoderValue > 0) {
          firstEncoderValue -= 10;
        }
        oldFirstEncoderValue = newFirstEncoderValue;
        isSomethingChanged = true;
      }
    }
  }
 
  if (isThisDisplayManageCurrentMode(0) == true) {
    newNumberOfActivatedDidos = decoder.getValue(3);
    if (newNumberOfActivatedDidos > oldNumberOfActivatedDidos + 4) {
      if (numberOfActivatedDiods < 255) {
        numberOfActivatedDiods += 1;
      }
      oldNumberOfActivatedDidos = newNumberOfActivatedDidos;
      isSomethingChanged = true;
    } else if (newNumberOfActivatedDidos + 4 < oldNumberOfActivatedDidos) {
      if (numberOfActivatedDiods > 0) {
        numberOfActivatedDiods -= 1;
      }
      oldNumberOfActivatedDidos = newNumberOfActivatedDidos;
      isSomethingChanged = true;
    }
  }
}

void moved2() {
  if (isThisDisplayManageCurrentMode(7)) {
    newModeNumber = decoder2.getValue(0);
    if (newModeNumber > oldModeNumber + 4) {
      if (modeNumber < 42) {
        modeNumber += 1;
      }
      oldModeNumber = newModeNumber;
      isModeChanged = true;
  } else if (newModeNumber + 4 < oldModeNumber) {
      if (modeNumber > 0) {
        modeNumber -= 1;
      }
      oldModeNumber = newModeNumber;
      isModeChanged = true;
    }
  }

  if (isThisDisplayManageCurrentMode(6)) {
    newBrightnessValue = decoder2.getValue(1);
    if (newBrightnessValue > oldBrightnessValue + 4) {
      if (brightnessValue < 245) {
        brightnessValue += 10;
      }
      oldBrightnessValue = newBrightnessValue;
      isSomethingChanged = true;
    } else if (newBrightnessValue + 4 < oldBrightnessValue) {
      if (brightnessValue > 0) {
        brightnessValue -= 10;
      }
      oldBrightnessValue = newBrightnessValue;
      isSomethingChanged = true;
    }
  }

  if (isThisDisplayManageCurrentMode(5)) {
 
    newThirdEncoderValue = decoder2.getValue(2);
    if (newThirdEncoderValue > oldThirdEncoderValue + 4) {
      if (thirdEncoderValue < 245) {
        thirdEncoderValue += 1;
      }
      oldThirdEncoderValue = newThirdEncoderValue;
      isSomethingChanged = true;
    } else if (newThirdEncoderValue + 4 < oldThirdEncoderValue) {
      if (thirdEncoderValue > 0) {
        thirdEncoderValue -= 1;
      }
      oldThirdEncoderValue = newThirdEncoderValue;
      isSomethingChanged = true;
    }
  }
  
  if (isThisDisplayManageCurrentMode(4)) {
    newDelayTime = decoder2.getValue(3);
    if (newDelayTime > oldDelayTime + 4) {
      if (delayTime < 255) {
        delayTime += 10;
      }
      oldDelayTime = newDelayTime;
      isSomethingChanged = true;
    } else if (newDelayTime + 4 < oldDelayTime) {
      if (delayTime > 0) {
        delayTime -= 10;
      }
      oldDelayTime = newDelayTime;
      isSomethingChanged = true;
    }
  }
}

volatile bool isEffectDirectionForward = false;
void changeDirectionOfEffect() {
  // поменять на 2 кнопки так как тумблер?
  if (isEffectDirectionForward == true) {
    isEffectDirectionForward = false;
  } else if (isEffectDirectionForward == false) {
    isEffectDirectionForward = true;
  }
}

int horizontal_index(int i) {
  //-ONLY WORKS WITH INDEX < TOPINDEX
  if (i == 0) {
    return 0;
  }
  if (i == TOP_INDEX && LED_COUNT % 2 == 1) {
    return TOP_INDEX + 1;
  }
  if (i == TOP_INDEX && LED_COUNT % 2 == 0) {
    return TOP_INDEX;
  }
  return LED_COUNT - i;
}


void clearDisplays() {
  for (int i = 0; i < 8; i++ ){
    TCA9548A(i);
    myOLED.begin();
    myOLED.clrScr();
    myOLED.update();
    Serial.print("clear display: ");
    Serial.println(i);
    delay(100);
  }
}

extern uint8_t BigNumbers[];


void sendValueToDisplay(int displayNumber, int paramValue) {
  TCA9548A(displayNumber);
  myOLED.begin();
  myOLED.setFont(BigNumbers);
  myOLED.clrScr();
  myOLED.printNumI(paramValue, CENTER, 20);
  myOLED.update();
}



void changeDirectionRight() {
  myOLED.drawLine(20, 34, 40, 34);
  myOLED.drawLine(35, 39, 40, 34);
  myOLED.drawLine(35, 29, 40, 34);
}

void changeDirectionLeft() {
  myOLED.drawLine(20, 34, 40, 34);
  myOLED.drawLine(25, 39, 20, 34);
  myOLED.drawLine(25, 29, 20, 34);

}

void changeModeToRandomMode() {
  myOLED.drawLine(86, 24, 86, 44);
  myOLED.drawLine(86, 24, 96, 24);
  myOLED.drawLine(96, 24, 96, 34);
  myOLED.drawLine(96, 34, 86, 34);
  myOLED.drawLine(86, 34, 96, 44);

  myOLED.drawLine(100, 24, 100, 44);
  myOLED.drawLine(100, 24, 105, 34);
  myOLED.drawLine(105, 34, 110, 24);
  myOLED.drawLine(110, 24, 110, 44);

}

void changeModeToSetMode() {
  myOLED.drawLine(96, 24, 96, 29);
  myOLED.drawLine(96, 24, 86, 24);
  myOLED.drawLine(86, 24, 86, 34);
  myOLED.drawLine(86, 34, 96, 34);
  myOLED.drawLine(96, 34, 96, 44);
  myOLED.drawLine(96, 44, 86, 44);
  myOLED.drawLine(86, 44, 86, 39);

  myOLED.drawLine(100, 24, 100, 44);
  myOLED.drawLine(100, 24, 105, 34);
  myOLED.drawLine(105, 34, 110, 24);
  myOLED.drawLine(110, 24, 110, 44);

}

void fillLeftTriangle() {
  myOLED.drawLine(20, 0, 40, 0);
  myOLED.drawLine(21, 1, 39, 1);
  myOLED.drawLine(22, 2, 38, 2);
  myOLED.drawLine(23, 3, 37, 3);
  myOLED.drawLine(24, 4, 36, 4);
  myOLED.drawLine(25, 5, 35, 5);
  myOLED.drawLine(26, 6, 34, 6);
  myOLED.drawLine(27, 7, 33, 7);
  myOLED.drawLine(28, 8, 32, 8);
  myOLED.drawLine(29, 9, 31, 9);

  myOLED.drawLine(88, 0, 98, 10);
  myOLED.drawLine(98, 10, 108, 0);
}

void fillRightTriangle() {
  myOLED.drawLine(20, 0, 30, 10);
  myOLED.drawLine(30, 10, 40, 0);
  
  myOLED.drawLine(88, 0, 108, 0);
  myOLED.drawLine(89, 1, 107, 1);
  myOLED.drawLine(90, 2, 106, 2);
  myOLED.drawLine(91, 3, 105, 3);
  myOLED.drawLine(92, 4, 104, 4);
  myOLED.drawLine(93, 5, 103, 5);
  myOLED.drawLine(94, 6, 102, 6);
  myOLED.drawLine(95, 7, 101, 7);
  myOLED.drawLine(96, 8, 101, 8);
  myOLED.drawLine(97, 9, 99, 9);
}

void deactivateFifthDisplayMode(int fifthDisplayModeNumber) {
  if (fifthDisplayModeNumber == THIRDENCMODE_direction) {
    for (int i = 0; i < 6; i++) {
      myOLED.drawLine(0 + i, 0, 55 + i, 64);
    }
    for (int i = 0; i < 6; i++) {
      myOLED.drawLine(0 + i, 64, 55 + i, 0);
    }
  } else if (fifthDisplayModeNumber == THIRDENCMODE_setOrRandom) {
    for (int i = 0; i < 6; i++) {
        myOLED.drawLine(68 + i, 0, 122 + i, 64);
    }
    for (int i = 0; i < 6; i++) {
        myOLED.drawLine(68 + i, 64, 122 + i, 0);
    }
  }
}

void drawEmptyTriangle() {
  myOLED.drawLine(20, 0, 30, 10);
  myOLED.drawLine(30, 10, 40, 0);

  myOLED.drawLine(88, 0, 98, 10);
  myOLED.drawLine(98, 10, 108, 0);
}

void deactivateDisplay(int displayNumber) {
  TCA9548A(displayNumber);
  myOLED.begin();
  myOLED.clrScr();
  for (int i = 0; i < 6; i++) {
    myOLED.drawLine(30 + i, 0, 94 + i, 64);
  }
  for (int i = 0; i < 6; i++) {
    myOLED.drawLine(30 + i, 64, 94 + i, 0);
  }
  myOLED.update();   
}

void fillFifthDisplay() {
  TCA9548A(5);
  myOLED.begin();
  myOLED.clrScr();

  if (isFifthDisplayManageCurrentFifthDisplayMode(THIRDENCMODE_setOrRandom) == true && isFifthDisplayManageCurrentFifthDisplayMode(THIRDENCMODE_direction) == true){
    if (thirdEncoderMode == THIRDENCMODE_setOrRandom) {
      fillRightTriangle(); 
    } else if (thirdEncoderMode == THIRDENCMODE_direction) {
      fillLeftTriangle();
    }
  } else if (isFifthDisplayManageCurrentFifthDisplayMode(THIRDENCMODE_setOrRandom) == true && isFifthDisplayManageCurrentFifthDisplayMode(THIRDENCMODE_direction) == false) {
    if (thirdEncoderMode == THIRDENCMODE_setOrRandom) {
      fillRightTriangle();
    } else if (thirdEncoderMode == THIRDENCMODE_direction) {
      thirdEncoderMode = THIRDENCMODE_setOrRandom;
      fillRightTriangle();
    }
  } else if (isFifthDisplayManageCurrentFifthDisplayMode(THIRDENCMODE_setOrRandom) == false && isFifthDisplayManageCurrentFifthDisplayMode(THIRDENCMODE_direction) == true) {
    if (thirdEncoderMode == THIRDENCMODE_setOrRandom) {
      fillLeftTriangle();
      thirdEncoderMode = THIRDENCMODE_direction;
    } else if (thirdEncoderMode == THIRDENCMODE_direction) {
      fillLeftTriangle();
    }
  } else if (isFifthDisplayManageCurrentFifthDisplayMode(THIRDENCMODE_setOrRandom) == false && isFifthDisplayManageCurrentFifthDisplayMode(THIRDENCMODE_direction) == false) {
    drawEmptyTriangle();
  }

  if (isFifthDisplayManageCurrentFifthDisplayMode(THIRDENCMODE_direction) == false) {
    deactivateFifthDisplayMode(THIRDENCMODE_direction);
  }
  if (isFifthDisplayManageCurrentFifthDisplayMode(THIRDENCMODE_setOrRandom) == false) {
    deactivateFifthDisplayMode(THIRDENCMODE_setOrRandom);
  }

  if (isDirectionRight == true) {
    changeDirectionRight();
  } else if (isDirectionLeft == true) {
    changeDirectionLeft();
  }

  if (isInSetMode == true) {
    changeModeToSetMode();
    myOLED.update();
    if (isThisDisplayManageCurrentMode(1) == true) {
      sendValueToDisplay(1, firstEncoderValue);  
    }
    if (isThisDisplayManageCurrentMode(3) == true) {
      sendValueToDisplay(3, secondEncoderValue);  
    }
  } else if (isInRandomMode == true) {
    changeModeToRandomMode();
    myOLED.update();
    deactivateDisplay(1);
    deactivateDisplay(3);
  }


}

void changeSomethingAndSendItToDisplay() {
  isSomethingChanged = false;
  if (numberOfActivatedDiods != currentNumberOfActivatedDidos) {
      currentNumberOfActivatedDidos = numberOfActivatedDiods;
      sendValueToDisplay(0, numberOfActivatedDiods);
    }
    if (firstEncoderValue != currentFirstEncoderValue) {
      currentFirstEncoderValue = firstEncoderValue;
      sendValueToDisplay(1, firstEncoderValue);
    }
    if (gapsBetweenGroupsOfDiods != currentGapsBetweenGroupsOfDiods) {
      currentGapsBetweenGroupsOfDiods = gapsBetweenGroupsOfDiods;
      sendValueToDisplay(2, gapsBetweenGroupsOfDiods);
    }
    if (secondEncoderValue != currentSecondEncoderValue) {
      currentSecondEncoderValue = secondEncoderValue;
      sendValueToDisplay(3, secondEncoderValue);
    }
    if (delayTime != currentDelayTime) {
      currentDelayTime = delayTime;
      sendValueToDisplay(4, delayTime);
    }
    if (isFifthEncPressed == true) {
      fillFifthDisplay();
      isFifthEncPressed = false;
    }
    if (thirdEncoderValue != currentThirdEncoderValue) {
      
      if (thirdEncoderMode == THIRDENCMODE_setOrRandom) {
        if (thirdEncoderValue > currentThirdEncoderValue) {
          isInRandomMode = true;
          isInSetMode = false;
        } else if (thirdEncoderValue < currentThirdEncoderValue) {
          isInRandomMode = false;
          isInSetMode = true;
        }
      } else if (thirdEncoderMode == THIRDENCMODE_direction) {
        if (thirdEncoderValue > currentThirdEncoderValue) {
          isDirectionRight = true;
          isDirectionLeft = false;
        } else if (thirdEncoderValue < currentThirdEncoderValue) {
          isDirectionRight = false;
          isDirectionLeft = true;
        }
      }
      fillFifthDisplay();
      currentThirdEncoderValue = thirdEncoderValue;

      //currentThirdEncoderMode = thirdEncoderMode;
    }
    //if (thirdEncoderValue != currentThirdEncoderValue) {
    //  currentThirdEncoderValue = thirdEncoderValue;
    //  sendValueToDisplay(5, thirdEncoderValue);
    //}
    if (brightnessValue != currentBrightnessValue) {
      currentBrightnessValue = brightnessValue;
      FastLED.setBrightness(brightnessValue);
      sendValueToDisplay(6, brightnessValue);
    }
    
}


int getDisplayValue(int displayNumber) {
  switch (displayNumber) {
    case 0:
      return numberOfActivatedDiods;
    case 1:
      return firstEncoderValue;
    case 2:
      return gapsBetweenGroupsOfDiods;
    case 3:
      return secondEncoderValue;
    case 4:
      return delayTime;
    case 5:
      return thirdEncoderValue;
    case 6:
      return brightnessValue;
    case 7:
      return modeNumber;
  }
  return false;
}





//void activateFifthDisplay() {
//  fillLeftTriangle();
  //delay(1000);
  //fillRightTriangle();
  //TCA9548A(5);
  //myOLED.begin();
  //myOLED.clrScr();
  //myOLED.drawLine(20, 0, 30, 10);
  //myOLED.drawLine(30, 10, 40, 0);
  //myOLED.drawLine(21, 0, 40, 0);
  //myOLED.drawLine(22, 1, 39, 1);
  //myOLED.drawLine(23, 2, 38, 2);
  //myOLED.drawLine(24, 3, 37, 3);
  //myOLED.drawLine(25, 4, 36, 4);
  //myOLED.drawLine(26, 5, 35, 5);
  //myOLED.drawLine(27, 6, 34, 6);
  //myOLED.drawLine(28, 7, 33, 7);
  //myOLED.drawLine(29, 8, 32, 8);
  //myOLED.drawLine(30, 9, 31, 9);
//
//
  //myOLED.drawLine(88, 0, 98, 10);
  //myOLED.drawLine(98, 10, 108, 0);
  //for (int i = 0; i < 6; i++) {
  //  myOLED.drawLine(30 + i, 0, 94 + i, 64);
  //}
  //for (int i = 0; i < 6; i++) {
  //  myOLED.drawLine(30 + i, 64, 94 + i, 0);
  //}
  //myOLED.update();
//}


void enableCurrentModeDisplayControls() {
  for (int i = 0; i < 7; i++ ){
    if (isThisDisplayManageCurrentMode(i) == false) {
      deactivateDisplay(i);
    } else if (isThisDisplayManageCurrentMode(i) == true) {
      if (i == 5) {
        fillFifthDisplay();
        continue;
      }
      sendValueToDisplay(i, getDisplayValue(i));
    }
  }
}

void setPixelRGB(int Pixel, byte red, byte green, byte blue) {
  leds[Pixel].r = red;
  leds[Pixel].g = green;
  leds[Pixel].b = blue;
}

void setPixel(int ledIndex, byte firstColorParam, byte secondColorParam, byte thirdColorParam = 255) {
  leds[ledIndex] = CHSV(firstColorParam, secondColorParam, thirdColorParam);
}

void setAll(byte red, byte green, byte blue) {
  for (int i = 0; i < LED_COUNT; i++ ) {
    setPixelRGB(i, red, green, blue);
  }
  FastLED.show();
}

byte * Wheel(byte WheelPos) {
  static byte c[3];

  if (WheelPos < 85) {
    c[0] = WheelPos * 3;
    c[1] = 255 - WheelPos * 3;
    c[2] = 0;
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    c[0] = 255 - WheelPos * 3;
    c[1] = 0;
    c[2] = WheelPos * 3;
  } else {
    WheelPos -= 170;
    c[0] = 0;
    c[1] = WheelPos * 3;
    c[2] = 255 - WheelPos * 3;
  }

  return c;
}

bool hasMillisTimer = false;
unsigned long millisTimer;

//void Sparkle() {
//  int index;
//  while (true) {
//    decoder.update();
//    decoder2.update();
//    if (isSomethingChanged == true) {
//      changeSomethingAndSendItToDisplay();
//    }
//    if (isModeChanged == true) {
//      return;
//    }
//    if (hasMillisTimer == false) {
//      index = random(LED_COUNT);
//      if (isInRandomMode == true) {
//        leds[index] = CHSV(random(0,255), 255, 255);
//      } else if (isInSetMode == true) {
//        leds[index] = CHSV(firstEncoderValue, secondEncoderValue, 255);
//      }
//      FastLED.show();
//      leds[index] = CHSV(0, 0, 0);
//      hasMillisTimer = true;
//      millisTimer = millis();
//
//    } else if (hasMillisTimer == true) {
//      if (millis() - millisTimer > delayTime) {
//        hasMillisTimer = false;
//      }
//    }
//  }
//}

// соединить isSomethingChanged и isModeChanged
void Sparkle() {
  int index;
  while (true) {
    if (isSomethingChanged == true) {
      decoder.update();
      decoder2.update();
      changeSomethingAndSendItToDisplay();
    }
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      index = random(LED_COUNT);
      if (isInRandomMode == true) {
        setPixel(index, random(0,255), 255, 255);
      } else if (isInSetMode == true) {
        setPixel(index, firstEncoderValue, secondEncoderValue);
      }
      setPixel(index, 0, 0, 0);

      FastLED.show();
      hasMillisTimer = true;
      millisTimer = millis();
    } else if (hasMillisTimer == true) {
      if (millis() - millisTimer > delayTime) {
        hasMillisTimer = false;
      }
    }
  }
}

void activate_rainbow_vertical() {
  int index = 0;
  int ihue = 0;
  while (true) {
    decoder.update();
    decoder2.update();
    if (isSomethingChanged == true) {
      changeSomethingAndSendItToDisplay();
    }
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      index += 1;
      if (index > TOP_INDEX) {
        index = 0;
      }
      ihue = ihue + gapsBetweenGroupsOfDiods;
      if (ihue > 255) {
        ihue = 0;
      }
      int secondIndex = horizontal_index(index);
      setPixel(index, ihue, 255);
      setPixel(secondIndex, ihue, 255);

      FastLED.show();
      hasMillisTimer = true;
      millisTimer = millis();
    } else if (hasMillisTimer == true) {
      if (millis() - millisTimer > delayTime) {
        hasMillisTimer = false;
      }
    }
  }
}

void activate_new_rainbow_loop() { 
  int raindowIhue = 0;
  while (true) {
    decoder.update();
    decoder2.update();
    if (isSomethingChanged == true) {
      changeSomethingAndSendItToDisplay();
    }
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      if (isDirectionRight == true) {
        raindowIhue += 1;
      } else if (isDirectionLeft == true) {
        raindowIhue -= 1;
      }
      if (raindowIhue >= 255) {
        raindowIhue = 1;
      } else if (raindowIhue <= 0) {
        raindowIhue = 254;
      }
      fill_rainbow(leds, LED_COUNT, raindowIhue, gapsBetweenGroupsOfDiods);

      FastLED.show();
      hasMillisTimer = true;
      millisTimer = millis();
    } else if (hasMillisTimer == true) {
      if (millis() - millisTimer > delayTime) {
        hasMillisTimer = false;
      }
    }
  }
}

void theaterChaseRainbow() {
  byte *c;
  int index = 0;
  int indexColour = 0;
  while (true) {
    decoder.update();
    decoder2.update();
    if (isSomethingChanged == true) {
      changeSomethingAndSendItToDisplay();
    }
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      if (isDirectionRight == true) {
        int numberOfGapsAndActivatedDiods = gapsBetweenGroupsOfDiods + numberOfActivatedDiods;

        for (int i = 0; i < LED_COUNT - numberOfGapsAndActivatedDiods - index; i = i + numberOfGapsAndActivatedDiods) {  
          c = Wheel( (i + indexColour) % 255);
          for (int j = 0; j < numberOfActivatedDiods; j = j + 1) {
            setPixelRGB(i + j + index, *c, *(c + 1), *(c + 2));
          }
        }
        FastLED.show();
        for (int i = 0; i < LED_COUNT - numberOfGapsAndActivatedDiods - index; i = i + numberOfGapsAndActivatedDiods) {
          for (int j = 0; j < numberOfActivatedDiods; j = j + 1) {
          setPixelRGB(i + j + index, 0, 0, 0);
          } 
        }
        index += 1;
        if (index == numberOfGapsAndActivatedDiods) {
          index = 0;
          indexColour += firstEncoderValue / 10;
        }
      } else if (isDirectionLeft == true) {
        int numberOfGapsAndActivatedDiods = gapsBetweenGroupsOfDiods + numberOfActivatedDiods;

        for (int i = LED_COUNT; i > numberOfGapsAndActivatedDiods + index; i = i - numberOfGapsAndActivatedDiods) {
          c = Wheel( (i + indexColour) % 255);
          for (int j = numberOfActivatedDiods; j > 0; j = j - 1) {
            setPixelRGB(i - j - index,  *c, *(c + 1), *(c + 2));
          }
        }
        FastLED.show();
        for (int i = LED_COUNT; i > numberOfGapsAndActivatedDiods + index; i = i - numberOfGapsAndActivatedDiods) {
          for (int j = numberOfActivatedDiods; j > 0; j = j - 1) {
            setPixelRGB(i - j - index,  0, 0, 0);
          }
        }
        index += 1;
        if (index == numberOfGapsAndActivatedDiods) {
          index = 0;
          indexColour += firstEncoderValue / 10;
        }
      }

      hasMillisTimer = true;
      millisTimer = millis();
    } else if (hasMillisTimer == true) {
      if (millis() - millisTimer > delayTime) {
        hasMillisTimer = false;
      }
    }
  }
}

void activate_random_march() {
  while (true) {
    decoder.update();
    decoder2.update();
    if (isSomethingChanged == true) {
      changeSomethingAndSendItToDisplay();
    }
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      if (isDirectionRight == true) {
        setPixel(0, random(0, 255), 255);
        for (int i = LED_COUNT - 1; i > 0 ; i-- ) {
          leds[i].r = leds[i - 1].r;
          leds[i].g = leds[i - 1].g;
          leds[i].b = leds[i - 1].b;
        }
      } else if (isDirectionLeft == true) {
        setPixel(LED_COUNT - 1, random(0, 255), 255);
        for (int i = 0; i < LED_COUNT - 1 ; i++ ) {
          leds[i].r = leds[i + 1].r;
          leds[i].g = leds[i + 1].g;
          leds[i].b = leds[i + 1].b;
        }
      }

      FastLED.show();
      hasMillisTimer = true;
      millisTimer = millis();
    } else if (hasMillisTimer == true) {
      if (millis() - millisTimer > delayTime) {
        hasMillisTimer = false;
      }
    }
  }
}

void activate_random_burst() {
  int ihue = 0;
  int index = 0;
  while (true) {
    decoder.update();
    decoder2.update();
    if (isSomethingChanged == true) {
      changeSomethingAndSendItToDisplay();
    }
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      index = random(0, LED_COUNT);
      ihue = random(0, 255);
      setPixel(index, ihue, 255);
      
      FastLED.show();
      hasMillisTimer = true;
      millisTimer = millis();
    } else if (hasMillisTimer == true) {
      if (millis() - millisTimer > delayTime) {
        hasMillisTimer = false;
      }
    }
  }
}

void activate_rwb_march() {
  int index = 0;
  int indexOfColoredDiod;
  while (true) {
    decoder.update();
    decoder2.update();
    if (isSomethingChanged == true) {
      changeSomethingAndSendItToDisplay();
    }
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      index++;
      if (index > 2) {
        index = 0;
      }
      if (isEffectDirectionForward == true) {
        indexOfColoredDiod = 0;
      } else if (isEffectDirectionForward == false) {
        indexOfColoredDiod = LED_COUNT - 1;
      }
      switch (index) {
        case 0:
          leds[indexOfColoredDiod].r = 255;
          leds[indexOfColoredDiod].g = 0;
          leds[indexOfColoredDiod].b = 0;
          break;
        case 1:
          leds[indexOfColoredDiod].r = 255;
          leds[indexOfColoredDiod].g = 255;
          leds[indexOfColoredDiod].b = 255;
          break;
        case 2:
          leds[indexOfColoredDiod].r = 0;
          leds[indexOfColoredDiod].g = 0;
          leds[indexOfColoredDiod].b = 255;
          break;
      }
      if (isEffectDirectionForward == true) {
        for (int i = LED_COUNT - 1; i > 0 ; i-- ) {
          leds[i].r = leds[i - 1].r;
          leds[i].g = leds[i - 1].g;
          leds[i].b = leds[i - 1].b;
        }
      } else if (isEffectDirectionForward == false) {
        for (int i = 0; i < LED_COUNT - 1 ; i++ ) {
          leds[i].r = leds[i + 1].r;
          leds[i].g = leds[i + 1].g;
          leds[i].b = leds[i + 1].b;
        }
      }

      FastLED.show();
      hasMillisTimer = true;
      millisTimer = millis();
    } else if (hasMillisTimer == true) {
      if (millis() - millisTimer > delayTime) {
        hasMillisTimer = false;
      }
    }
  }
}

void activate_rgb_propeller() {
  int index = 0;
  int colorParam;
  if (isInRandomMode) {
    colorParam = random(0, 255);
  } else if (isInSetMode) {
    colorParam = firstEncoderValue;
  }
  while (true) {
    decoder.update();
    decoder2.update();
    if (isSomethingChanged == true) {
      changeSomethingAndSendItToDisplay();
    }
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      if (isDirectionRight == true) {
        index++;  
      } else if (isDirectionLeft == true) {
        index--;
      }
      if (index > LED_COUNT) {
        if (isInRandomMode) {
          colorParam = random(0, 255);
        } else if (isInSetMode) {
          colorParam = firstEncoderValue;
        }
        index = 0;
      } else if (index < 0) {
        if (isInRandomMode) {
          colorParam = random(0, 255);
        } else if (isInSetMode) {
          colorParam = firstEncoderValue;
        }
        index = LED_COUNT;
      }
      int ghue = (colorParam + 80) % 255;
      int bhue = (colorParam + 160) % 255;
      int N3  = int(LED_COUNT / 3);
      int N12 = int(LED_COUNT / 12);
      for (int i = 0; i < N3; i++ ) {
        int j0 = (index + i + LED_COUNT - N12) % LED_COUNT;
        int j1 = (j0 + N3) % LED_COUNT;
        int j2 = (j1 + N3) % LED_COUNT;
        leds[j0] = CHSV(colorParam, secondEncoderValue, 255);
        leds[j1] = CHSV(ghue, secondEncoderValue, 255);
        leds[j2] = CHSV(bhue, secondEncoderValue, 255);
      }

      FastLED.show();
      hasMillisTimer = true;
      millisTimer = millis();
    } else if (hasMillisTimer == true) {
      if (millis() - millisTimer > delayTime) {
        hasMillisTimer = false;
      }
    }
  }
}

int antipodal_index(int i) {
  int iN = i + TOP_INDEX;
  if (i >= TOP_INDEX) {
    iN = ( i + TOP_INDEX ) % LED_COUNT;
  }
  return iN;
}

void activate_ems_lightsONE() {
  int index = 0;
  int firstColorParam;
  if (isInRandomMode) {
    firstColorParam = random(0, 255);
  } else if (isInSetMode) {
    firstColorParam = firstEncoderValue;
  }
  while (true) {
    decoder.update();
    decoder2.update();
    if (isSomethingChanged == true) {
      changeSomethingAndSendItToDisplay();
    }
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      if (isDirectionRight == true) {
        index++;
        if (index >= LED_COUNT) {
          index = 0;
          if (isInRandomMode) {
            firstColorParam = random(0, 255);
          } else if (isInSetMode) {
            firstColorParam = firstEncoderValue;
          }
        }
        int idexR = index;
        int idexB = antipodal_index(idexR);
        int thathue = (firstColorParam + 160) % 255;
        for (int i = 0; i < LED_COUNT; i++ ) {
          if (i == idexR) {
            setPixel(i, firstColorParam, 255);
          }
          else if (i == idexB) {
            setPixel(i, thathue, 255);
          }
          else {
            setPixel(i, 0, 0);            
          }
        }
      } else if (isDirectionLeft == true) {
        index--;
        if (index <= 0) {
          index = LED_COUNT -1;
          if (isInRandomMode) {
            firstColorParam = random(0, 255);
          } else if (isInSetMode) {
            firstColorParam = firstEncoderValue;
          }

        }
        int idexR = index;
        int idexB = antipodal_index(idexR);
        int thathue = (firstColorParam + 160) % 255;
        for (int i = LED_COUNT; i > 0; i = i - 1) {
          if (i == idexR) {
            setPixel(i, firstColorParam, 255);
          }
          else if (i == idexB) {
            setPixel(i, thathue, 255);
          }
          else {
            setPixel(i, 0, 0);            
          }
        }
      }

      FastLED.show();
      hasMillisTimer = true;
      millisTimer = millis();
    } else if (hasMillisTimer == true) {
      if (millis() - millisTimer > delayTime) {
        hasMillisTimer = false;
      }
    }
  }
}

void activate_matrix() {
  int firstColorParam;
  int secondColorParam;
  while (true) {
    decoder.update();
    decoder2.update();
    if (isSomethingChanged == true) {
      changeSomethingAndSendItToDisplay();
    }
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      if (isInRandomMode == true) {
        firstColorParam = random(0,255);
        secondColorParam = 255;
      } else if (isInSetMode == true) {
        firstColorParam = firstEncoderValue;
        secondColorParam = secondColorParam;
      }
      if (isDirectionRight == true) {
        int rand = random(0, 100);
        if (rand > 90) {
          setPixel(0, firstColorParam, secondColorParam);
        }
        else {
          // зачем передавать диоду первые 2 параметра, если он
          // не будет гореть? 
          //TODO протестировать без первых двух параметров 
          setPixel(0, firstColorParam, secondColorParam, 0);
          //leds[0] = CHSV(firstColorParam, secondColorParam, 0);
        }
        for (int i = LED_COUNT-1; i > 0 ; i-- ) {
          leds[i].r = leds[i - 1].r;
          leds[i].g = leds[i - 1].g;
          leds[i].b = leds[i - 1].b;
        }
      } else if (isDirectionLeft == true) {
        int rand = random(0, 100);
        if (rand > 90) {
          setPixel(LED_COUNT - 1, firstColorParam, secondColorParam);
        }
        else {
          setPixel(LED_COUNT - 1, firstColorParam, secondColorParam, 0);
        }
        for (int i = 0; i < LED_COUNT ; i++ ) {
          leds[i].r = leds[i + 1].r;
          leds[i].g = leds[i + 1].g;
          leds[i].b = leds[i + 1].b;
        }
      }

      FastLED.show();
      hasMillisTimer = true;
      millisTimer = millis();
    } else if (hasMillisTimer == true) {
      if (millis() - millisTimer > delayTime) {
        hasMillisTimer = false;
      }
    }
  }
}

void CylonBounce() {  
  int index = 0;
  bool cylonBounceDirectionForward;
  int firstColorParam;
  int secondColorParam;
  while (true) {
    decoder.update();
    decoder2.update();
    if (isSomethingChanged == true) {
      changeSomethingAndSendItToDisplay();
    }
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      
      if (index == 0) {
        cylonBounceDirectionForward = true;
        if (isInRandomMode == true) {
          firstColorParam = random(0,255);
          secondColorParam = 255;
        } else if (isInSetMode == true) {
          firstColorParam = firstEncoderValue;
          secondColorParam = secondColorParam;
        }
      } else if (index == LED_COUNT - numberOfActivatedDiods - 2) {
        cylonBounceDirectionForward = false;
        if (isInRandomMode == true) {
          firstColorParam = random(0,255);
          secondColorParam = 255;
        } else if (isInSetMode == true) {
          firstColorParam = firstEncoderValue;
          secondColorParam = secondColorParam;
        }
      }

      if (cylonBounceDirectionForward == true) {
        index += 1;
      } else if (cylonBounceDirectionForward == false) {
        index -= 1;
      }

      setAll(0, 0, 0);
      setPixel(index, firstColorParam, secondColorParam, 100);
      for (int j = 1; j <= numberOfActivatedDiods; j++) {
        setPixel(index + j, firstColorParam, secondColorParam);      
      }
      setPixel(index + numberOfActivatedDiods + 1, firstColorParam, secondColorParam, 100);
      
      FastLED.show();
      hasMillisTimer = true;
      millisTimer = millis();
    } else if (hasMillisTimer == true) {
      if (millis() - millisTimer > delayTime) {
        hasMillisTimer = false;
      }
    }
  }
}

void twoDiodsGroup() {
  int index = 0;
  bool twoDiodsGroupDirectionForward;
  int firstColorParam;
  int secondColorParam;
  while (true) {
    decoder.update();
    decoder2.update();
    if (isSomethingChanged == true) {
      changeSomethingAndSendItToDisplay();
    }
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      
      if (index == 0) {
        twoDiodsGroupDirectionForward = true;
        if (isInRandomMode == true) {
          firstColorParam = random(0,255);
          secondColorParam = 255;
        } else if (isInSetMode == true) {
          firstColorParam = firstEncoderValue;
          secondColorParam = secondColorParam;
        }
      } else if (index == (LED_COUNT - numberOfActivatedDiods) / 2) {
        twoDiodsGroupDirectionForward = false;
        if (isInRandomMode == true) {
          firstColorParam = random(0,255);
          secondColorParam = 255;
        } else if (isInSetMode == true) {
          firstColorParam = firstEncoderValue;
          secondColorParam = secondColorParam;
        }
      }

      if (twoDiodsGroupDirectionForward == true) {
        index += 1;
      } else if (twoDiodsGroupDirectionForward == false) {
        index -= 1;
      }

      setAll(0, 0, 0);

      setPixel(index, firstColorParam, secondColorParam, 100);
      for (int j = 1; j <= numberOfActivatedDiods; j++) {
        setPixel(index + j, firstColorParam, secondColorParam);      
      }
      setPixel(index + numberOfActivatedDiods + 1, firstColorParam, secondColorParam, 100);

      setPixel(LED_COUNT - index, firstColorParam, secondColorParam, 100);
      for (int j = 1; j <= numberOfActivatedDiods; j++) {
        setPixel(LED_COUNT - index - j, firstColorParam, secondColorParam);
        //TODO почему тут 100?      
        //leds[LED_COUNT - index - j] = CHSV(firstColorParam, secondColorParam, 100);
      }
      setPixel(LED_COUNT - index - numberOfActivatedDiods - 1, firstColorParam, secondColorParam, 100);

      FastLED.show();
      hasMillisTimer = true;
      millisTimer = millis();
    } else if (hasMillisTimer == true) {
      if (millis() - millisTimer > delayTime) {
        hasMillisTimer = false;
      }
    }
  }
}

void theaterChase() {
  // переписать на подобие решения того способа что предложил Лео?
  int index = 0;
  int counterToChangeColor = 0;
  int firstColorParam;
  int secondColorParam;
  while (true) {
    decoder.update();
    decoder2.update();
    if (isSomethingChanged == true) {
      changeSomethingAndSendItToDisplay();
    }
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      counterToChangeColor++;
      if (counterToChangeColor == LED_COUNT) {
        counterToChangeColor = 0;
        if (isInRandomMode == true) {
          firstColorParam = random(0,255);
          secondColorParam = 255;
        }
      }
      if (isInSetMode == true) {
        firstColorParam = firstEncoderValue;
        secondColorParam = secondColorParam;
      }
      if (isDirectionRight == true) {
        int numberOfGapsAndActivatedDiods = gapsBetweenGroupsOfDiods + numberOfActivatedDiods;

        for (int i = 0; i < LED_COUNT - numberOfGapsAndActivatedDiods - index; i = i + numberOfGapsAndActivatedDiods) {  
          for (int j = 0; j < numberOfActivatedDiods; j = j + 1) {
            setPixel(i + j + index, firstColorParam, secondColorParam);
          }
        }
        FastLED.show();
        for (int i = 0; i < LED_COUNT - numberOfGapsAndActivatedDiods - index; i = i + numberOfGapsAndActivatedDiods) {
          for (int j = 0; j < numberOfActivatedDiods; j = j + 1) {
            setPixel(i + j + index, 0, 0);
          } 
        }
        index += 1;
        if (index == numberOfGapsAndActivatedDiods) {
          index = 0;
        }
      } else if (isDirectionLeft == true) {
        int numberOfGapsAndActivatedDiods = gapsBetweenGroupsOfDiods + numberOfActivatedDiods;

        for (int i = LED_COUNT; i > numberOfGapsAndActivatedDiods + index; i = i - numberOfGapsAndActivatedDiods) {
          for (int j = numberOfActivatedDiods; j > 0; j = j - 1) {
            setPixel(i - j - index, firstColorParam, secondColorParam);
          }
        }
        FastLED.show();
        for (int i = LED_COUNT; i > numberOfGapsAndActivatedDiods + index; i = i - numberOfGapsAndActivatedDiods) {
          for (int j = numberOfActivatedDiods; j > 0; j = j - 1) {
            setPixel(i - j - index, 0, 0);
          }
        }
        index += 1;
        if (index == numberOfGapsAndActivatedDiods) {
          index = 0;
        }
      }

      hasMillisTimer = true;
      millisTimer = millis();
    } else if (hasMillisTimer == true) {
      if (millis() - millisTimer > delayTime) {
        hasMillisTimer = false;
      }
    }
  }
}

void meteorRain() {  
  int index = 0;
  int firstColorParam;
  int secondColorParam;
  while (true) {
    decoder.update();
    decoder2.update();
    if (isSomethingChanged == true) {
      changeSomethingAndSendItToDisplay();
    }
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      if (isDirectionRight == true) {
        index += 1;
        if (index == LED_COUNT) {
          index = 0;
          if (isInRandomMode) {
            firstColorParam = random(0, 255);
            secondColorParam = 255;
          } else if (isInSetMode) {
            firstColorParam = firstEncoderValue;
            secondColorParam = secondEncoderValue;
          }
        }
        for(int j=0; j<LED_COUNT; j++) {
          if((random(10)>5) ) {
            leds[j].fadeToBlackBy(100 - gapsBetweenGroupsOfDiods);
          }
        }
        for(int j = 0; j < numberOfActivatedDiods; j++) {
          if( (index-j <LED_COUNT) && (index-j>=0) ) {
            setPixel(index - j, firstColorParam, secondColorParam);
          }
        }
        
      } else if (isDirectionLeft == true) {
        index -= 1;
        if (index < 0) {
          index = LED_COUNT;
          if (isInRandomMode) {
            firstColorParam = random(0, 255);
            secondColorParam = 255;
          } else if (isInSetMode) {
            firstColorParam = firstEncoderValue;
            secondColorParam = secondEncoderValue;
          }

        }
        for(int j=LED_COUNT; j > 0; j--) {
          if((random(10)>5) ) {
            leds[j].fadeToBlackBy(100 - gapsBetweenGroupsOfDiods);
          }
        }
        for(int j = numberOfActivatedDiods; j > 0; j--) {
          if( (index-j < LED_COUNT) && (index-j>=0) ) {
            setPixel(index - j, firstColorParam, secondColorParam);
          }
        }
        
      }

      FastLED.show();
      hasMillisTimer = true;
      millisTimer = millis();  
    } else if (hasMillisTimer == true) {
      if (millis() - millisTimer > delayTime) {
        hasMillisTimer = false;
      }
    }
  }
}

void RunningLights() {
  int Position = 0;
  while (true) {
    decoder.update();
    decoder2.update();
    if (isSomethingChanged == true) {
      changeSomethingAndSendItToDisplay();
    }
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      Position++;
      for (int i = 0; i < LED_COUNT; i++) {
        leds[i] = CHSV(((sin(i + Position) * 127 + 128) / 255)*firstEncoderValue, secondEncoderValue, 255);
        //setPixelRGB(i, ((sin(i + Position) * 127 + 128) / 255)*secondEncoderValue,
        //         ((sin(i + Position) * 127 + 128) / 255)*firstEncoderValue,
        //         ((sin(i + Position) * 127 + 128) / 255)*thirdEncoderValue);
      }

    FastLED.show();
    hasMillisTimer = true;
    millisTimer = millis();
    } else if (hasMillisTimer == true) {
      if (millis() - millisTimer > delayTime) {
        hasMillisTimer = false;
      }
    }
  }
}

void colorWipe() {
  int index = 0;
  bool isStartToLightUp = true;
  while (true) {
    decoder.update();
    decoder2.update();
    if (isSomethingChanged == true) {
      changeSomethingAndSendItToDisplay();
    }
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      index += 1;
      if (isStartToLightUp == true) {
        setPixel(index, firstEncoderValue, secondEncoderValue);
      } else if (isStartToLightUp == false) {
        setPixel(index, 0, 0, 0);
      }
      if (index == LED_COUNT - 1) {
        index = 0;
        if (isStartToLightUp == true) {
          isStartToLightUp = false;
        } else if (isStartToLightUp == false) {
          isStartToLightUp = true;
        }
      }

      FastLED.show();
      hasMillisTimer = true;
      millisTimer = millis();
    } else if (hasMillisTimer == true) {
      if (millis() - millisTimer > delayTime) {
        hasMillisTimer = false;
      }
    }
  }
}

void activate_sin_bright_wave() {
  int index = 0;
  float tcount = 0.0;
  while (true) {
    decoder.update();
    decoder2.update();
    if (isSomethingChanged == true) {
      changeSomethingAndSendItToDisplay();
    }
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      index += 1;
      tcount = tcount + .1;
      if (tcount > 3.14) {
        tcount = 0.0;
      }
      int ibright = int(sin(tcount) * 255);
      setPixel(index, firstEncoderValue, secondEncoderValue, ibright);
      if (index == LED_COUNT - 1) {
        index = 0;
      }

      FastLED.show();
      hasMillisTimer = true;
      millisTimer = millis();
    } else if (hasMillisTimer == true) {
      if (millis() - millisTimer > delayTime) {
        hasMillisTimer = false;
      }
    }
  }
}

void activate_fade_vertical() {
  int fadeVerticalCounter = 0;
  int index = 0;
  while (true) {
    decoder.update();
    decoder2.update();
    if (isSomethingChanged == true) {
      changeSomethingAndSendItToDisplay();
    }
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      index++;
      if (index > TOP_INDEX)
      {
        index = 0;
      }
      int indexA = index;
      int indexB = horizontal_index(indexA);
      fadeVerticalCounter = fadeVerticalCounter + 10;
      if (fadeVerticalCounter > 255)
      {
        fadeVerticalCounter = 0;
      }
      setPixel(indexA, firstEncoderValue, secondEncoderValue, fadeVerticalCounter);
      setPixel(indexB, firstEncoderValue, secondEncoderValue, fadeVerticalCounter);
      
      FastLED.show();
      hasMillisTimer = true;
      millisTimer = millis();
    } else if (hasMillisTimer == true) {
      if (millis() - millisTimer > delayTime) {
        hasMillisTimer = false;
      }
    }
  }
}

void activate_pulse_one_color_all_rev() {
  int pulseOneColorAllRevCounter = 0;
  bool isBounceDirectonForward = true;
  while (true) {
    decoder.update();
    decoder2.update();
    if (isSomethingChanged == true) {
      changeSomethingAndSendItToDisplay();
    }
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      if (isSomethingChanged == true) {
        return;
      }
      if (isBounceDirectonForward == true) {
        pulseOneColorAllRevCounter++;
        if (pulseOneColorAllRevCounter >= 255) {
          isBounceDirectonForward = false;
          }
      }
      if (isBounceDirectonForward == false) {
        pulseOneColorAllRevCounter = pulseOneColorAllRevCounter - 1;
        if (pulseOneColorAllRevCounter <= 1) {
          isBounceDirectonForward = true;
        }
      }
      for (int i = 0; i < LED_COUNT; i++ ) {
        if (isSomethingChanged == true) {
          changeSomethingAndSendItToDisplay();
        }
        if (isModeChanged == true) {
          return;
        }
        setPixel(i, firstEncoderValue, pulseOneColorAllRevCounter);
      }

      FastLED.show();  
      hasMillisTimer = true;
      millisTimer = millis();
    } else if (hasMillisTimer == true) {
      if (millis() - millisTimer > delayTime) {
        hasMillisTimer = false;
      }
    }
  }
}

void FadeInOut() {
  int fadeInOutCounter = 0;
  bool fadeInOutLightsUp;
  while (true) {
    decoder.update();
    decoder2.update();
    if (isSomethingChanged == true) {
      changeSomethingAndSendItToDisplay();
    }
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {

      if (fadeInOutCounter == 0) {
        fadeInOutLightsUp = true;
      } else if (fadeInOutCounter == 255) {
        fadeInOutLightsUp = false;
      }

      if (fadeInOutLightsUp == true) {
        fadeInOutCounter += 1;
      } else if (fadeInOutLightsUp == false) {
        fadeInOutCounter -= 1;
      }
      for (int i = 0; i < LED_COUNT; i++ ) {
        setPixel(i, firstEncoderValue, secondEncoderValue, fadeInOutCounter);
      }
      
      FastLED.show();
      hasMillisTimer = true;
      millisTimer = millis();
    } else if (hasMillisTimer == true) {
      if (millis() - millisTimer > delayTime) {
        hasMillisTimer = false;
      }
    }
  }
}

void activate_custom_rgb_light() {
  // полностью переписать функцию
  int green_led = 0;
  int red_led = 0;
  int blue_led = 0;

  while (true)
  {
    
    if (isSomethingChanged == true) {
      changeSomethingAndSendItToDisplay();
    }
    if (isModeChanged == true) {
      return;
    }
  
  

    //btn[1].tick();                      
    //btn[2].tick();                      
    //btn[0].tick();                      

    // =============== ЭНКОДЕР ===============

    // красный цвет, энкодер 1
    //if (btn[1].left()) {
    //  if (green_led > 0) {
    //    green_led-=10;     // поворот налево
    //  }
    //}
    //if (btn[1].right()){
    //  if (green_led < 255) {
    //    green_led+=10;   // поворот направо
    //  }
    //}
    //if (btn[1].leftH()) {
    //  if (green_led >= 10) {
    //    green_led-=10;     // поворот налево
    //  }
    //}
    //if (btn[1].rightH()){
    //  if (green_led <= 245) {
    //    green_led+=10;   // поворот направо
    //  }
    //}
  //
//
//
    //// зелёный цвет, энкодер 2
    //if (btn[2].left()) {
    //  if (red_led > 0) {
    //    red_led-=10;     // поворот налево
    //  }
    //}
    //if (btn[2].right()){
    //  if (red_led < 255) {
    //    red_led+=10;   // поворот направо
    //  }
    //}
    //if (btn[2].leftH()) {
    //  if (red_led >= 10) {
    //    red_led-=10;     // поворот налево
    //  }
    //}
    //if (btn[2].rightH()){
    //  if (red_led <= 245) {
    //    red_led+=10;   // поворот направо
    //  }
    //}
//
    //if (btn[1].turn() || btn[1].turnH() || btn[2].turn() || btn[2].turnH()) {
      Serial.println("red:");
      Serial.println(green_led);
      Serial.println("green:");
      Serial.println(red_led);
      Serial.println("blue:");
      Serial.println(blue_led);
      for(int one_led = 0; one_led < LED_COUNT; one_led = one_led + 1) {
        leds[one_led] = CRGB(green_led, red_led, blue_led);
        FastLED.show();  
      }
    
  
    }
  }

void setModeName(int modeNum) {
  switch (modeNum) {
    case 999: break;     
    case 1: modeName = MODE_Sparkle; break;  
    case 2: modeName = MODE_activate_rainbow_vertical; break;      
    case 3: modeName = MODE_activate_new_rainbow_loop; break;
    case 4: modeName = MODE_theaterChaseRainbow; break; 
    case 5: modeName = MODE_activate_random_march; break;  
    case 6: modeName = MODE_activate_random_burst; break; 
    case 7: modeName = MODE_activate_rwb_march; break; 
    case 8: modeName = MODE_activate_rgb_propeller; break;
    case 9: modeName = MODE_activate_ems_lightsONE; break;
    case 10: modeName = MODE_activate_matrix; break;
    case 11: modeName = MODE_CylonBounce; break;
    case 12: modeName = MODE_twoDiodsGroup; break; 
    case 13: modeName = MODE_theaterChase; break;
    case 14: modeName = MODE_meteorRain; break;
    case 15: modeName = MODE_RunningLights; break;  
    case 16: modeName = MODE_colorWipe; break;
    case 17: modeName = MODE_activate_sin_bright_wave; break;
    case 18: modeName = MODE_activate_fade_vertical; break;
    case 19: modeName = MODE_activate_pulse_one_color_all_rev; break;  
    case 20: modeName = MODE_FadeInOut; break;

    case 38: activate_custom_rgb_light(); 
    
  }
}

void changeMode(int newmode) {
  switch (newmode) {
    case 999: break;     
    case 1: Sparkle(); break; // готово
    case 2: activate_rainbow_vertical(); break; // готово     
    case 3: activate_new_rainbow_loop(); break; // готово
    case 4: theaterChaseRainbow(); break; // много багов, найти и исправить
    case 5: activate_random_march(); break; // готово
    case 6: activate_random_burst(); break; // готово
    case 7: activate_rwb_march(); break; // готово
    case 8: activate_rgb_propeller(); break;  // добавить смену направления
    case 9: activate_ems_lightsONE(); break;  // добавить смену направления
    case 10: activate_matrix(); break; // добавить смену направления
    case 11: CylonBounce(); break; // готово
    case 12: twoDiodsGroup(); break; // готовто
    case 13: theaterChase(); break; // готово
    case 14: meteorRain(); break; // добавить смену направления
    case 15: RunningLights(); break; // добавить смену направления
    case 16: colorWipe(); break; // добавить смену направления
    case 17: activate_sin_bright_wave(); break; // добавить смену направления, что-то не так
    case 18: activate_fade_vertical(); break; // готово
    case 19: activate_pulse_one_color_all_rev(); break;
    case 20: FadeInOut(); break; // готово
    case 27: 

    // в некоторые режимы добавить возможность регулировать цвет первым параметром ХСВ на РГБшных энкодерах
    case 38: activate_custom_rgb_light(); 
    
  }
  
}

void changeThirdEncMode() {
  if (isThisModeHasDirectionAndRandomOrSetMode() == true) {
    if (thirdEncoderMode == THIRDENCMODE_setOrRandom) {
      thirdEncoderMode = THIRDENCMODE_direction;
      isSomethingChanged = true;
      isFifthEncPressed = true;
    } else if (thirdEncoderMode == THIRDENCMODE_direction) {
      thirdEncoderMode = THIRDENCMODE_setOrRandom;
      isSomethingChanged = true;
      isFifthEncPressed = true;
    }
  }
} 
 
void setup()
{
  Serial.begin(115200);
  Serial.println("system start");

  pinMode(2, INPUT_PULLUP);
  attachInterrupt(0, moved, FALLING);

  pinMode(3, INPUT_PULLUP);
  attachInterrupt(1, moved2, FALLING);

  pinMode(19, INPUT_PULLUP);
  //attachInterrupt(4, changeDirectionOfEffect, CHANGE);
  attachInterrupt(4, changeThirdEncMode, RISING);

  Serial.println("1");
  Wire.begin();
  Wire.setClock(100000);
  Serial.println("2");
  decoder.begin(4);
  decoder.readInitialState();
  Serial.println("3");
  decoder2.begin(4);
  decoder2.readInitialState();

  Serial.println("4");
  clearDisplays();

  //Serial.println("5");
  //sendValueToDisplay(0, numberOfActivatedDiods);
  //Serial.println("5.1");
  //delay(100);
  //sendValueToDisplay(1, firstEncoderValue);
  //Serial.println("5.2");
  //delay(100);
  //sendValueToDisplay(2, gapsBetweenGroupsOfDiods);
  //Serial.println("5.3");
  //delay(100);
  //sendValueToDisplay(3, secondEncoderValue);
  //Serial.println("5.4");
  //delay(100);
  //sendValueToDisplay(4, delayTime);
  //Serial.println("5.5");
  //delay(100);
  //fillFifthDisplay();
  //delay(100);
  //sendValueToDisplay(6, brightnessValue);
  //Serial.println("5.6");
  //delay(100);
  sendValueToDisplay(7, modeNumber);
  //Serial.println("6");

  FastLED.setBrightness(brightnessValue);
  FastLED.addLeds<WS2812, LED_DT, RGB>(leds, LED_COUNT);
  Serial.println("7");
  setAll(0, 0, 0);
  setModeName(modeNumber);
  enableCurrentModeDisplayControls();
  changeMode(modeNumber);

}

void loop()
{
  decoder.update(); // потенциально не нужно потому что уже вызывается
  decoder2.update(); // потенциально не нужно потому что уже вызывается

  if (isSomethingChanged == true) { // потенциально не нужно потому что уже вызывается
    changeSomethingAndSendItToDisplay(); // потенциально не нужно потому что уже вызывается
  }
  if (isModeChanged == true) { // потенциально не нужно потому что уже вызывается
    if (currentModeNumber != modeNumber) {
      setAll(0, 0, 0);
      currentModeNumber = modeNumber;
      // функция меняющая рабочие и нерабочие дисплеи
      sendValueToDisplay(7, modeNumber);
      setModeName(modeNumber);
      Serial.println("loop");
      Serial.println(modeNumber);
      Serial.println(modeName);
      enableCurrentModeDisplayControls();
      hasMillisTimer = false;
      isModeChanged = false;
      isSomethingChanged = false;
      changeMode(modeNumber);
    }
  }
}



