#include <Arduino.h>

#include <FastLED.h>          
#include "Wire.h"
#include "rotaryDecoder.h"
#include <OLED_I2C.h>

OLED myOLED(SDA, SCL, 8);

rotaryDecoder firstPCF8574(0x20);
rotaryDecoder secondPCF8574(0x24);

volatile int newZeroEncoderValue = 0;
volatile int oldZeroENcoderValue = 0;
volatile int numberOfActivatedDiods = 1;
int currentNumberOfActivatedDidos = 1;

volatile int newFirstEncoderValue = 0;
volatile int oldFirstEncoderValue = 0;
volatile uint8_t firstEncoderValue = 0;
int currentFirstEncoderValue = 0;

volatile int newSecondEncoderValue = 0;
volatile int oldSecondEncoderValue = 0;
volatile uint8_t gapsBetweenGroupsOfDiods = 3;
int currentGapsBetweenGroupsOfDiods = 3;

volatile int newThirdEncoderValue = 0;
volatile int oldThirdEncoderValue = 0;
volatile uint8_t thirdEncoderValue = 200;
int currentThirdEncoderValue = 200;

volatile int newFourthEncoderValue = 0;
volatile int oldFourthEncoderValue = 0;
volatile unsigned long delayTime = 20;
unsigned long currentDelayTime = 0;

volatile int newFifthEncoderValue = 0;
volatile int oldFifthEncoderValue = 0;
volatile uint8_t fifthEncoderValue = 0;
int currentFifthEncoderValue = 0;
volatile int fifthEncoderMode = 0;
int currentFifthEncoderMode = 1;

volatile int newSixthEncoderValue = 0;
volatile int oldSixthEncoderValue = 0;
volatile uint8_t brightnessValue = 50;
int currentBrightnessValue = 0;

volatile int newSeventhEncoderValue = 0;
volatile int oldSeventhEncoderValue = 0;
volatile int modeNumber = 12;
int currentModeNumber = 12;

volatile bool isInRandomMode = false;
bool isInSetMode = true;
bool isDirectionRight = false;
bool isDirectionLeft = true;

bool hasMillisTimer = false;
unsigned long millisTimer;

int firstColorParam;
int secondColorParam;

volatile bool isModeChanged = false;
volatile bool isSomethingChanged = false;
volatile bool isFifthEncPressed = false;

int modeName;

#define LED_COUNT 80
#define LED_DT 5

struct CRGB leds[LED_COUNT];

int TOP_INDEX = int(LED_COUNT / 2);

extern uint8_t BigNumbers[];

#define THIRDENCMODE_setOrRandom 0
#define THIRDENCMODE_direction 1

#define RIGHT_DIRECTION 0
#define LEFT_DIRECTION 1

#define MODE_sparkle 1
#define MODE_rainbowVertical 2
#define MODE_newRainbowLoop 3
#define MODE_theaterChaseRainbow 4
#define MODE_randomMarch 5
#define MODE_randomBurst 6
#define MODE_rwbMarch 7
#define MODE_rgbPropeller 8
#define MODE_emsLightsOne 9
#define MODE_matrix 10
#define MODE_cylonBounce 11
#define MODE_twoDiodsGroup 12
#define MODE_theaterChase 13
#define MODE_meteorRain 14
#define MODE_runningLights 15
#define MODE_colorWipe 16
#define MODE_sinBrightWave 17
#define MODE_fadeVertical 18
#define MODE_pulseOneColorAllRev 19
#define MODE_fadeInOut 20
#define MODE_customRgbLight 21

// not used
#define ZERO_ENCODER_BUTTON_PIN 31
#define SECOND_ENCODER_BUTTON_PIN 35
#define SEVENTH_ENCODER_BUTTON_PIN 44
// used
#define FIRST_ENCODER_BUTTON_PIN 33
#define THIRD_ENCODER_BUTTON_PIN 37
#define FOURTH_ENCODER_BUTTON_PIN 40
#define FIFTH_ENCODER_BUTTON_PIN 19
#define SIXTH_ENCODER_BUTTON_PIN 42


#define FIRST_PCF8574_PIN 2
#define SECOND_PCF8574_PIN 3


bool isFifthDisplayManageCurrentFifthDisplayMode(int fifthDisplayModeNumber) {
  switch (modeName) {
    case 1:
      switch (fifthDisplayModeNumber) {
        case 0: return true;
        case 1: return false;
      }
    case 2:
      switch (fifthDisplayModeNumber) {
        case 0: return false;
        case 1: return false;
      }
    case 3:
      switch (fifthDisplayModeNumber) {
        case 0: return false;
        case 1: return true;
      }
    case 4:
      switch (fifthDisplayModeNumber) {
        case 0: return false;
        case 1: return true;
      }
    case 5:
      switch (fifthDisplayModeNumber) {
        case 0: return false;
        case 1: return true;
      }
    case 6:
      switch (fifthDisplayModeNumber) {
        case 0: return false;
        case 1: return false;
      }
    case 7:
      switch (fifthDisplayModeNumber) {
        case 0: return false;
        case 1: return true;
      }
    case 8:
      switch (fifthDisplayModeNumber) {
        case 0: return true;
        case 1: return true;
      }
    case 9:
      switch (fifthDisplayModeNumber) {
        case 0: return true;
        case 1: return true;
      }
    case 10:
      switch (fifthDisplayModeNumber) {
        case 0: return true;
        case 1: return true;
      }
    case 11:
      switch (fifthDisplayModeNumber) {
        case 0: return true;
        case 1: return false;
      }
    case 12:
      switch (fifthDisplayModeNumber) {
        case 0: return true;
        case 1: return false;
      }
    case 13:
      switch (fifthDisplayModeNumber) {
        case 0: return true;
        case 1: return true;
      }
    case 14:
      switch (fifthDisplayModeNumber) {
        case 0: return true;
        case 1: return true;
      }
    case 15:
      switch (fifthDisplayModeNumber) {
        case 0: return true;
        case 1: return true;
      }
    case 16:
      switch (fifthDisplayModeNumber) {
        case 0: return true;
        case 1: return true;
      }
    case 17:
      switch (fifthDisplayModeNumber) {
        case 0: return true;
        case 1: return true;
      }
    case 18:
      switch (fifthDisplayModeNumber) {
        case 0: return true;
        case 1: return false;
      }
    case 19:
      switch (fifthDisplayModeNumber) {
        case 0: return true;
        case 1: return false;
      }
    case 20:
      switch (fifthDisplayModeNumber) {
        case 0: return true;
        case 1: return false;
      }
    case 21:
      switch (fifthDisplayModeNumber) {
        case 0: return false;
        case 1: return false;
      }
    default: 
      return false;
  }
}

bool isThisModeHasDirectionAndRandomOrSetMode() {
  switch (modeName) {
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
    case 21: return false;
    default: return false;
  }
}

bool isThisDisplayManageCurrentMode(int displayNumber) {
  switch (modeName) {
    case 1:
      switch (displayNumber) {
        case 0: return false;
        case 2: return false;
        default: return true;
      }
    case 2:
      switch (displayNumber) {
        case 0: return false;
        case 1: return false;
        case 3: return false;
        case 5: return false;
        default: return true;
      }
    case 3:
      switch (displayNumber) {
        case 0: return false;
        case 1: return false;
        case 3: return false;
        default: return true;
      }
    case 4:
      switch (displayNumber) {
        case 3: return false;
        default: return true;
      }
    case 5:
      switch (displayNumber) {
        case 0: return false;
        case 1: return false;
        case 2: return false;
        case 3: return false;
        default: return true;
      }
    case 6:
      switch (displayNumber) {
        case 0: return false;
        case 1: return false;
        case 2: return false;
        case 3: return false;
        case 5: return false;
        default: return true;
      }
    case 7:
      switch (displayNumber) {
        case 0: return false;
        case 1: return false;
        case 2: return false;
        case 3: return false;
        default: return true;
      }
    case 8:
      switch (displayNumber) {
        case 0: return false;
        case 2: return false;
        default: return true;
      }
    case 9:
      switch (displayNumber) {
        case 0: return false;
        case 2: return false;
        case 3: return false;
        default: return true;
      }
    case 10:
      switch (displayNumber) {
        case 0: return false;
        case 2: return false;
        default: return true;
      }
    case 11:
      switch (displayNumber) {
        case 2: return false;
        default: return true;
      }
    case 12:
      switch (displayNumber) {
        case 2: return false;
        default: return true;
      }
    case 13:
      switch (displayNumber) {
        default: return true;
      }
    case 14:
      switch (displayNumber) {
        default: return true;
      }
    case 15:
      switch (displayNumber) {
        case 0: return false;
        case 2: return false;
        default: return true;
      }
    case 16:
      switch (displayNumber) {
        case 0: return false;
        case 2: return false;
        default: return true;
      }
    case 17:
      switch (displayNumber) {
        case 0: return false;
        case 2: return false;
        default: return true;
      }
    case 18:
      switch (displayNumber) {
        case 0:  return false;
        case 2: return false;
        default: return true;
      }
    case 19:
      switch (displayNumber) {
        case 0: return false;
        case 2: return false;
        case 3: return false;
        default: return true;
      }
    case 20:
      switch (displayNumber) {
        case 0: return false;
        case 2: return false;
        default: return true;
      }
    case 21:
      switch (displayNumber) {
        case 0: return false;
        case 2: return false;
        case 4: return false;
        case 5: return false;
        default: return true;
      }
    default:
      return false;
  }
}

void movedFirstPCF8574() {
  if (isThisDisplayManageCurrentMode(0) == true) {
    newZeroEncoderValue = firstPCF8574.getValue(0);
    if (newZeroEncoderValue > oldZeroENcoderValue + 4) {
      if (numberOfActivatedDiods < 255) {
        numberOfActivatedDiods += 1;
      }
      oldZeroENcoderValue = newZeroEncoderValue;
      isSomethingChanged = true;
    } else if (newZeroEncoderValue + 4 < oldZeroENcoderValue) {
      if (numberOfActivatedDiods > 0) {
        numberOfActivatedDiods -= 1;
      }
      oldZeroENcoderValue = newZeroEncoderValue;
      isSomethingChanged = true;
    }
  }

  if (isThisDisplayManageCurrentMode(1) == true) {
    if (isInRandomMode == false) {
      newFirstEncoderValue = firstPCF8574.getValue(1);
      if (newFirstEncoderValue > oldFirstEncoderValue + 4) {
        if (digitalRead(FIRST_ENCODER_BUTTON_PIN) == LOW) {
          if (firstEncoderValue < 255) {
            firstEncoderValue += 1;
          }
        } else if (digitalRead(FIRST_ENCODER_BUTTON_PIN) == HIGH) {
          if (firstEncoderValue < 245) {
            firstEncoderValue += 10;
          } else if (245 <= firstEncoderValue && firstEncoderValue < 255) {
            firstEncoderValue = 255;
          }
        }
        oldFirstEncoderValue = newFirstEncoderValue;
        isSomethingChanged = true;
      } else if (newFirstEncoderValue + 4 < oldFirstEncoderValue) {
        if (digitalRead(FIRST_ENCODER_BUTTON_PIN) == LOW) {
          if (firstEncoderValue > 0) {
            firstEncoderValue -= 1;
          }
        } else if (digitalRead(FIRST_ENCODER_BUTTON_PIN) == HIGH) {
          if (firstEncoderValue > 10) {
            firstEncoderValue -= 10;
          } else if (0 < firstEncoderValue && firstEncoderValue <= 10) {
            firstEncoderValue = 0;
          }
        }
        oldFirstEncoderValue = newFirstEncoderValue;
        isSomethingChanged = true;
      }
    }
  }

  if (isThisDisplayManageCurrentMode(2) == true) {
    newSecondEncoderValue = firstPCF8574.getValue(2);
    if (newSecondEncoderValue > oldSecondEncoderValue + 4) {
      if (gapsBetweenGroupsOfDiods < 255) {
        gapsBetweenGroupsOfDiods += 1;
      }
      oldSecondEncoderValue = newSecondEncoderValue;
      isSomethingChanged = true;
    } else if (newSecondEncoderValue + 4 < oldSecondEncoderValue) {
      if (gapsBetweenGroupsOfDiods > 0) {
        gapsBetweenGroupsOfDiods -= 1;
      }
      oldSecondEncoderValue = newSecondEncoderValue;
      isSomethingChanged = true;
    }
  }

  if (isThisDisplayManageCurrentMode(3) == true) {
    if (isInRandomMode == false) {
      newThirdEncoderValue = firstPCF8574.getValue(3);
      if (newThirdEncoderValue > oldThirdEncoderValue + 4) {
        if (digitalRead(THIRD_ENCODER_BUTTON_PIN) == LOW) {
          if (thirdEncoderValue < 255) {
            thirdEncoderValue += 1;
          }
        } else if (digitalRead(THIRD_ENCODER_BUTTON_PIN) == HIGH) {
          if (thirdEncoderValue < 245) {
            thirdEncoderValue += 10;
          } else if (245 <= thirdEncoderValue && thirdEncoderValue < 255) {
            thirdEncoderValue = 255;
          }
        }
        oldThirdEncoderValue = newThirdEncoderValue;
        isSomethingChanged = true;
      } else if (newThirdEncoderValue + 4 < oldThirdEncoderValue) {
        if (digitalRead(THIRD_ENCODER_BUTTON_PIN) == LOW) {
          if (thirdEncoderValue > 0) {
            thirdEncoderValue -= 1;
          }
        } else if (digitalRead(THIRD_ENCODER_BUTTON_PIN) == HIGH) {
          if (thirdEncoderValue > 10) {
            thirdEncoderValue -= 10;
          } else if (0 < thirdEncoderValue && thirdEncoderValue <= 10) {
            thirdEncoderValue = 0;
          }
        }
        oldThirdEncoderValue = newThirdEncoderValue;
        isSomethingChanged = true;
      }
    }
  }
}

void movedSecondPCF8574() {
  if (isThisDisplayManageCurrentMode(4)) {
    newFourthEncoderValue = secondPCF8574.getValue(0);
    if (newFourthEncoderValue > oldFourthEncoderValue + 4) {
      if (digitalRead(FOURTH_ENCODER_BUTTON_PIN) == LOW) {
        if (delayTime < 255) {
          delayTime += 1;
        }
      } else if (digitalRead(FOURTH_ENCODER_BUTTON_PIN) == HIGH) {
        if (delayTime < 245) {
          delayTime += 10;
        } else if (245 <= delayTime && delayTime < 255) {
          delayTime = 255;
        }
      }
      oldFourthEncoderValue = newFourthEncoderValue;
      isSomethingChanged = true;
    } else if (newFourthEncoderValue + 4 < oldFourthEncoderValue) {
      if (digitalRead(FOURTH_ENCODER_BUTTON_PIN) == LOW) {
        if (delayTime > 0) {
          delayTime -= 1;
        }
      } else if (digitalRead(FOURTH_ENCODER_BUTTON_PIN) == HIGH) {
        if (delayTime > 10) {
          delayTime -= 10;
        } else if (0 < delayTime && delayTime <= 10) {
          delayTime = 0;
        }
      }
      oldFourthEncoderValue = newFourthEncoderValue;
      isSomethingChanged = true;
    }
  }

  if (isThisDisplayManageCurrentMode(5)) {
    newFifthEncoderValue = secondPCF8574.getValue(1);
    if (newFifthEncoderValue > oldFifthEncoderValue + 4) {
      if (fifthEncoderValue < 245) {
        fifthEncoderValue += 1;
      }
      oldFifthEncoderValue = newFifthEncoderValue;
      isSomethingChanged = true;
    } else if (newFifthEncoderValue + 4 < oldFifthEncoderValue) {
      if (fifthEncoderValue > 0) {
        fifthEncoderValue -= 1;
      }
      oldFifthEncoderValue = newFifthEncoderValue;
      isSomethingChanged = true;
    }
  }

  if (isThisDisplayManageCurrentMode(6)) {
    newSixthEncoderValue = secondPCF8574.getValue(2);
    if (newSixthEncoderValue > oldSixthEncoderValue + 4) {
      if (digitalRead(SIXTH_ENCODER_BUTTON_PIN) == LOW) {
        if (brightnessValue < 255) {
          brightnessValue += 1;
        }
      } else if (digitalRead(SIXTH_ENCODER_BUTTON_PIN) == HIGH) {
        if (brightnessValue < 245) {
          brightnessValue += 10;
        } else if (245 <= brightnessValue && brightnessValue < 255) {
          brightnessValue = 255;
        }
      }
      oldSixthEncoderValue = newSixthEncoderValue;
      isSomethingChanged = true;
    } else if (newSixthEncoderValue + 4 < oldSixthEncoderValue) {
      if (digitalRead(SIXTH_ENCODER_BUTTON_PIN) == LOW) {
        if (brightnessValue > 0) {
          brightnessValue -= 1;
        }
      } else if (digitalRead(SIXTH_ENCODER_BUTTON_PIN) == HIGH) {
        if (brightnessValue > 10) {
          brightnessValue -= 10;
        } else if (0 < brightnessValue && brightnessValue <= 10) {
          brightnessValue = 0;
        }
      }
      oldSixthEncoderValue = newSixthEncoderValue;
      isSomethingChanged = true;
    }
  }

  if (isThisDisplayManageCurrentMode(7)) {
    newSeventhEncoderValue = secondPCF8574.getValue(3);
    if (newSeventhEncoderValue > oldSeventhEncoderValue + 4) {
      if (modeNumber < 42) {
        modeNumber += 1;
      }
      oldSeventhEncoderValue = newSeventhEncoderValue;
      isSomethingChanged = true;
      isModeChanged = true;
  } else if (newSeventhEncoderValue + 4 < oldSeventhEncoderValue) {
      if (modeNumber > 0) {
        modeNumber -= 1;
      }
      oldSeventhEncoderValue = newSeventhEncoderValue;
      isSomethingChanged = true;
      isModeChanged = true;
    }
  }
}

int horizontal_index(int i) {
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

void TCA9548A(uint8_t bus) {
  Wire.beginTransmission(0x70);
  Wire.write(1 << bus);
  Wire.endTransmission();
}

void clearDisplays() {
  for (int i = 0; i < 8; i++ ){
    TCA9548A(i);
    myOLED.begin();
    myOLED.clrScr();
    myOLED.update();
    delay(100);
  }
}

void sendValueToDisplay(int displayNumber, int paramValue) {
  TCA9548A(displayNumber);
  myOLED.begin();
  myOLED.setFont(BigNumbers);
  myOLED.clrScr();
  myOLED.printNumI(paramValue, CENTER, 20);
  myOLED.update();
}

void drawChangeDirectionRight() {
  myOLED.drawLine(20, 34, 40, 34);
  myOLED.drawLine(35, 39, 40, 34);
  myOLED.drawLine(35, 29, 40, 34);
}

void drawChangeDirectionLeft() {
  myOLED.drawLine(20, 34, 40, 34);
  myOLED.drawLine(25, 39, 20, 34);
  myOLED.drawLine(25, 29, 20, 34);
}

void drawChangeModeToRandomMode() {
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

void drawChangeModeToSetMode() {
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

void drawFillLeftTriangle() {
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

void drawFillRightTriangle() {
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

void drawEmptyTriangle() {
  myOLED.drawLine(20, 0, 30, 10);
  myOLED.drawLine(30, 10, 40, 0);

  myOLED.drawLine(88, 0, 98, 10);
  myOLED.drawLine(98, 10, 108, 0);
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
    if (fifthEncoderMode == THIRDENCMODE_setOrRandom) {
      drawFillRightTriangle(); 
    } else if (fifthEncoderMode == THIRDENCMODE_direction) {
      drawFillLeftTriangle();
    }
  } else if (isFifthDisplayManageCurrentFifthDisplayMode(THIRDENCMODE_setOrRandom) == true && isFifthDisplayManageCurrentFifthDisplayMode(THIRDENCMODE_direction) == false) {
    if (fifthEncoderMode == THIRDENCMODE_setOrRandom) {
      drawFillRightTriangle();
    } else if (fifthEncoderMode == THIRDENCMODE_direction) {
      fifthEncoderMode = THIRDENCMODE_setOrRandom;
      drawFillRightTriangle();
    }
  } else if (isFifthDisplayManageCurrentFifthDisplayMode(THIRDENCMODE_setOrRandom) == false && isFifthDisplayManageCurrentFifthDisplayMode(THIRDENCMODE_direction) == true) {
    if (fifthEncoderMode == THIRDENCMODE_setOrRandom) {
      drawFillLeftTriangle();
      fifthEncoderMode = THIRDENCMODE_direction;
    } else if (fifthEncoderMode == THIRDENCMODE_direction) {
      drawFillLeftTriangle();
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
    drawChangeDirectionRight();
  } else if (isDirectionLeft == true) {
    drawChangeDirectionLeft();
  }

  if (isInSetMode == true) {
    drawChangeModeToSetMode();
    myOLED.update();
    if (isThisDisplayManageCurrentMode(1) == true) {
      sendValueToDisplay(1, firstEncoderValue);  
    }
    if (isThisDisplayManageCurrentMode(3) == true) {
      sendValueToDisplay(3, thirdEncoderValue);  
    }
  } else if (isInRandomMode == true) {
    drawChangeModeToRandomMode();
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
  if (thirdEncoderValue != currentThirdEncoderValue) {
    currentThirdEncoderValue = thirdEncoderValue;
    sendValueToDisplay(3, thirdEncoderValue);
  }
  if (delayTime != currentDelayTime) {
    currentDelayTime = delayTime;
    sendValueToDisplay(4, delayTime);
  }
  if (isFifthEncPressed == true) {
    fillFifthDisplay();
    isFifthEncPressed = false;
  }
  if (fifthEncoderValue != currentFifthEncoderValue) {
    if (fifthEncoderMode == THIRDENCMODE_setOrRandom) {
      if (fifthEncoderValue > currentFifthEncoderValue) {
        isInRandomMode = true;
        isInSetMode = false;
      } else if (fifthEncoderValue < currentFifthEncoderValue) {
        isInRandomMode = false;
        isInSetMode = true;
      }
    } else if (fifthEncoderMode == THIRDENCMODE_direction) {
      if (fifthEncoderValue > currentFifthEncoderValue) {
        isDirectionRight = true;
        isDirectionLeft = false;
      } else if (fifthEncoderValue < currentFifthEncoderValue) {
        isDirectionRight = false;
        isDirectionLeft = true;
      }
    }
    fillFifthDisplay();
    currentFifthEncoderValue = fifthEncoderValue;
  }
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
      return thirdEncoderValue;
    case 4:
      return delayTime;
    case 5:
      return fifthEncoderValue;
    case 6:
      return brightnessValue;
    case 7:
      return modeNumber;
  }
  return false;
}

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

void moveAlightLeds(int direction) {
  if (direction == RIGHT_DIRECTION) {
    for (int i = LED_COUNT - 1; i > 0 ; i-- ) {
      leds[i].r = leds[i - 1].r;
      leds[i].g = leds[i - 1].g;
      leds[i].b = leds[i - 1].b;
    }
  } else if (direction == LEFT_DIRECTION) {
    for (int i = 0; i < LED_COUNT - 1 ; i++ ) {
      leds[i].r = leds[i + 1].r;
      leds[i].g = leds[i + 1].g;
      leds[i].b = leds[i + 1].b;
    }
  }
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

void checkUpdates() {
  firstPCF8574.update();
  secondPCF8574.update();
  if (isSomethingChanged == true) {
    changeSomethingAndSendItToDisplay();
  }
}

void checkAndSetRandomOrSetMode() {
  if (isInRandomMode == true) {
    firstColorParam = random(0, 255);
    secondColorParam = 255;
  } else if (isInSetMode == true) {
    firstColorParam = firstEncoderValue;
    secondColorParam = thirdEncoderValue;
  }
}

void sparkle() {
  int index;
  while (true) {
    checkUpdates();
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      index = random(LED_COUNT);
      checkAndSetRandomOrSetMode();
      setPixel(index, firstColorParam, secondColorParam);
      FastLED.show();
      setPixel(index, 0, 0, 0);
      
      hasMillisTimer = true;
      millisTimer = millis();
    } else if (hasMillisTimer == true) {
      if (millis() - millisTimer > delayTime) {
        hasMillisTimer = false;
      }
    }
  }
}

void rainbowVertical() {
  int index = 0;
  int ihue = 0;
  while (true) {
    checkUpdates();
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      index++;
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

void newRainbowLoop() { 
  int raindowIhue = 0;
  while (true) {
    checkUpdates();
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      if (isDirectionRight == true) {
        raindowIhue++;
      } else if (isDirectionLeft == true) {
        raindowIhue--;
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
    checkUpdates();
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      int numberOfGapsAndActivatedDiods = gapsBetweenGroupsOfDiods + numberOfActivatedDiods;
      if (isDirectionRight == true) {
        for (int i = 0; i < LED_COUNT - numberOfGapsAndActivatedDiods - index; i = i + numberOfGapsAndActivatedDiods) {  
          c = Wheel((i + indexColour) % 255);
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
      } else if (isDirectionLeft == true) {
        for (int i = LED_COUNT; i > numberOfGapsAndActivatedDiods + index; i = i - numberOfGapsAndActivatedDiods) {
          c = Wheel((i + indexColour) % 255);
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
      }
      index += 1;
      if (index >= numberOfGapsAndActivatedDiods) {
        index = 0;
        indexColour += firstEncoderValue / 10;
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

void randomMarch() {
  while (true) {
    checkUpdates();
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      if (isDirectionRight == true) {
        setPixel(0, random(0, 255), 255);
        moveAlightLeds(RIGHT_DIRECTION);
      } else if (isDirectionLeft == true) {
        setPixel(LED_COUNT - 1, random(0, 255), 255);
        moveAlightLeds(LEFT_DIRECTION);
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

void randomBurst() {
  int firstColorParam = 0;
  int index = 0;
  while (true) {
    checkUpdates();
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      index = random(0, LED_COUNT);
      firstColorParam = random(0, 255);
      setPixel(index, firstColorParam, 255);
      
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

void rwbMarch() {
  int index = 0;
  int indexOfColoredDiod;
  while (true) {
    checkUpdates();
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      index++;
      if (index > 2) {
        index = 0;
      }
      if (isDirectionRight == true) {
        indexOfColoredDiod = 0;
      } else if (isDirectionLeft == true) {
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
      if (isDirectionRight == true) {
        moveAlightLeds(RIGHT_DIRECTION);
      } else if (isDirectionLeft == true) {
        moveAlightLeds(LEFT_DIRECTION);
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

void rgbPropeller() {
  int index = 0;
  while (true) {
    checkUpdates();
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
        checkAndSetRandomOrSetMode();
        index = 0;
      } else if (index < 0) {
        checkAndSetRandomOrSetMode();
        index = LED_COUNT;
      }
      int ghue = (firstColorParam + 80) % 255;
      int bhue = (firstColorParam + 160) % 255;
      int N3  = int(LED_COUNT / 3);
      int N12 = int(LED_COUNT / 12);
      for (int i = 0; i < N3; i++ ) {
        int j0 = (index + i + LED_COUNT - N12) % LED_COUNT;
        int j1 = (j0 + N3) % LED_COUNT;
        int j2 = (j1 + N3) % LED_COUNT;
        setPixel(j0, firstColorParam, secondColorParam);
        setPixel(j1, ghue, secondColorParam);
        setPixel(j2, bhue, secondColorParam);
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

void emsLightsOne() {
  int index = 0;
  while (true) {
    checkUpdates();
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      if (isDirectionRight == true) {
        index++;
        if (index >= LED_COUNT) {
          index = 0;
          checkAndSetRandomOrSetMode();
        }
        int idexR = index;
        int idexB = antipodal_index(idexR);
        int thathue = (firstColorParam + 160) % 255;
        for (int i = 0; i < LED_COUNT; i++ ) {
          if (i == idexR) {
            setPixel(i, firstColorParam, secondColorParam);
          }
          else if (i == idexB) {
            setPixel(i, thathue, secondColorParam);
          }
          else {
            setPixel(i, 0, 0, 0);            
          }
        }
      } else if (isDirectionLeft == true) {
        index--;
        if (index <= 0) {
          index = LED_COUNT - 1;
          checkAndSetRandomOrSetMode();
        }
        int idexR = index;
        int idexB = antipodal_index(idexR);
        int thathue = (firstColorParam + 160) % 255;
        for (int i = LED_COUNT; i > 0; i--) {
          if (i == idexR) {
            setPixel(i, firstColorParam, secondColorParam);
          }
          else if (i == idexB) {
            setPixel(i, thathue, secondColorParam);
          }
          else {
            setPixel(i, 0, 0, 0);            
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

void matrix() {
  while (true) {
    checkUpdates();
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      checkAndSetRandomOrSetMode();
      int rand = random(0, 100);
      if (isDirectionRight == true) {
        if (rand > 90) {
          setPixel(0, firstColorParam, secondColorParam);
        }
        else {
          setPixel(0, 0, 0, 0);
        }
        moveAlightLeds(RIGHT_DIRECTION);
      } else if (isDirectionLeft == true) {
        if (rand > 90) {
          setPixel(LED_COUNT - 1, firstColorParam, secondColorParam);
        }
        else {
          setPixel(LED_COUNT - 1, 0, 0, 0);
        }
        moveAlightLeds(LEFT_DIRECTION);
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

void cylonBounce() {  
  int index = 0;
  bool cylonBounceDirectionForward;
  int firstColorParam;
  int secondColorParam;
  while (true) {
    checkUpdates();
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      if (index == 0) {
        cylonBounceDirectionForward = true;
        if (isInRandomMode == true) {
          firstColorParam = random(0,255);
          secondColorParam = 255;
        } 
      } else if (index == LED_COUNT - numberOfActivatedDiods - 2) {
        cylonBounceDirectionForward = false;
        if (isInRandomMode == true) {
          firstColorParam = random(0,255);
          secondColorParam = 255;
        } 
      }
      if (isInSetMode == true) {
        firstColorParam = firstEncoderValue;
        secondColorParam = secondColorParam;
      }
      if (cylonBounceDirectionForward == true) {
        index++;
      } else if (cylonBounceDirectionForward == false) {
        index--;
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
  while (true) {
    checkUpdates();
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      if (index == 0) {
        twoDiodsGroupDirectionForward = true;
        checkAndSetRandomOrSetMode();
      } else if (index >= (LED_COUNT - numberOfActivatedDiods) / 2) {
        twoDiodsGroupDirectionForward = false;
        checkAndSetRandomOrSetMode();
      }
      if (twoDiodsGroupDirectionForward == true) {
        index++;
      } else if (twoDiodsGroupDirectionForward == false) {
        index--;
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
  int index = 0;
  int counterToChangeColor = 0;
  int firstColorParam;
  int secondColorParam;
  while (true) {
    checkUpdates();
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
          for (int j = 0; j < numberOfActivatedDiods; j++) {
            setPixel(i + j + index, firstColorParam, secondColorParam);
          }
        }
        FastLED.show();
        for (int i = 0; i < LED_COUNT - numberOfGapsAndActivatedDiods - index; i = i + numberOfGapsAndActivatedDiods) {
          for (int j = 0; j < numberOfActivatedDiods; j++) {
            setPixel(i + j + index, 0, 0, 0);
          } 
        }
        index++;
        if (index == numberOfGapsAndActivatedDiods) {
          index = 0;
        }
      } else if (isDirectionLeft == true) {
        int numberOfGapsAndActivatedDiods = gapsBetweenGroupsOfDiods + numberOfActivatedDiods;
        for (int i = LED_COUNT; i > numberOfGapsAndActivatedDiods + index; i = i - numberOfGapsAndActivatedDiods) {
          for (int j = numberOfActivatedDiods; j > 0; j--) {
            setPixel(i - j - index, firstColorParam, secondColorParam);
          }
        }
        FastLED.show();
        for (int i = LED_COUNT; i > numberOfGapsAndActivatedDiods + index; i = i - numberOfGapsAndActivatedDiods) {
          for (int j = numberOfActivatedDiods; j > 0; j--) {
            setPixel(i - j - index, 0, 0, 0);
          }
        }
        index++;
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
  while (true) {
    checkUpdates();
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      if (isDirectionRight == true) {
        index++;
        if (index == LED_COUNT) {
          index = 0;
          checkAndSetRandomOrSetMode();
        }
        for (int j = 0; j < LED_COUNT; j++) {
          if ((random(10)>5) ) {
            leds[j].fadeToBlackBy(100 - gapsBetweenGroupsOfDiods);
          }
        }
        for (int j = 0; j < numberOfActivatedDiods; j++) {
          if ((index - j <LED_COUNT) && (index - j >= 0)) {
            setPixel(index - j, firstColorParam, secondColorParam);
          }
        }
      } else if (isDirectionLeft == true) {
        index--;
        if (index < 0) {
          index = LED_COUNT;
          checkAndSetRandomOrSetMode();
        }
        for (int j = LED_COUNT; j > 0; j--) {
          if((random(10)>5) ) {
            leds[j].fadeToBlackBy(100 - gapsBetweenGroupsOfDiods);
          }
        }
        for (int j = numberOfActivatedDiods; j > 0; j--) {
          if ((index - j < LED_COUNT) && (index - j >= 0)) {
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

void runningLights() {
  int Position = 0;
  int runningLightsIndex = 0;
  while (true) {
    checkUpdates();
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      runningLightsIndex++;
      if (runningLightsIndex == LED_COUNT) {
        runningLightsIndex = 0;
        checkAndSetRandomOrSetMode();
      }
      if (isInSetMode) {
        firstColorParam = firstEncoderValue;
        secondColorParam = thirdEncoderValue;
      }
      if (isDirectionRight == true) {
        Position++;
        for (int i = 0; i < LED_COUNT; i++) {
          setPixel(i, ((sin(i + Position) * 127 + 128) / 255)*firstColorParam, secondColorParam, ((sin(i + Position) * 127 + 128) / 255)*255);
        }
      } else if (isDirectionLeft == true) {
        Position--;
        for (int i = LED_COUNT - 1; i > 0; i--) {
          setPixel(i, ((sin(i + Position) * 127 + 128) / 255)*firstColorParam, secondColorParam, ((sin(i + Position) * 127 + 128) / 255)*255);
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

void colorWipe() {
  int index = 0;
  bool isStartToLightUp = true;
  while (true) {
    checkUpdates();
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      if (isDirectionRight == true) {
        index++;
        if (index == LED_COUNT - 1) {
          checkAndSetRandomOrSetMode();
          index = 0;
          if (isStartToLightUp == true) {
            isStartToLightUp = false;
          } else if (isStartToLightUp == false) {
            isStartToLightUp = true;
          }
        }
        if (isStartToLightUp == true) {
          setPixel(index, firstColorParam, secondColorParam);
        } else if (isStartToLightUp == false) {
          setPixel(index, 0, 0, 0);
        }
      } else if (isDirectionLeft == true) {
        index--;
        if (index <= 0) {
          checkAndSetRandomOrSetMode();
          index = LED_COUNT - 1;
          if (isStartToLightUp == true) {
            isStartToLightUp = false;
          } else if (isStartToLightUp == false) {
            isStartToLightUp = true;
          }
        }
        if (isStartToLightUp == true) {
          setPixel(index, firstColorParam, secondColorParam);
        } else if (isStartToLightUp == false) {
          setPixel(index, 0, 0, 0);
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

void sinBrightWave() {
  int index = 0;
  float tcount = 0.0;
  while (true) {
    checkUpdates();
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      tcount = tcount + .1;
      if (tcount > 3.14) {
        tcount = 0.0;
      }
      int ibright = int(sin(tcount) * 255);
      if (isDirectionRight == true) {
        index++;
        if (index >= LED_COUNT - 1) {
          index = 0;
          checkAndSetRandomOrSetMode();
        }
        setPixel(index, firstColorParam, secondColorParam, ibright);
      } else if (isDirectionLeft == true) {
        index--;
        if (index <= 0) {
          index = LED_COUNT - 1;
          checkAndSetRandomOrSetMode();
        }
        setPixel(index, firstColorParam, secondColorParam, ibright);
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

void fadeVertical() {
  int fadeVerticalCounter = 0;
  int index = 0;
  while (true) {
    checkUpdates();
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      index++;
      if (index > TOP_INDEX) {
        index = 0;
      }
      int indexA = index;
      int indexB = horizontal_index(indexA);
      fadeVerticalCounter = fadeVerticalCounter + 10;
      if (fadeVerticalCounter > 255) {
        checkAndSetRandomOrSetMode();
        fadeVerticalCounter = 0;
      }
      setPixel(indexA, firstColorParam, secondColorParam, fadeVerticalCounter);
      setPixel(indexB, firstColorParam, secondColorParam, fadeVerticalCounter);
      
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

void pulseOneColorAllRev() {
  int pulseOneColorAllRevSecondColorParam = 0;
  bool isBounceDirectonForward = true;
  while (true) {
    checkUpdates();
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      if (isBounceDirectonForward == true) {
        pulseOneColorAllRevSecondColorParam++;
        if (pulseOneColorAllRevSecondColorParam >= 255) {
          isBounceDirectonForward = false;
          }
      }
      if (isBounceDirectonForward == false) {
        pulseOneColorAllRevSecondColorParam = pulseOneColorAllRevSecondColorParam - 1;
        if (pulseOneColorAllRevSecondColorParam <= 1) {
          checkAndSetRandomOrSetMode();
          isBounceDirectonForward = true;
        }
      }
      for (int i = 0; i < LED_COUNT; i++ ) {
        if (isModeChanged == true) {
          return;
        }
        setPixel(i, firstColorParam, pulseOneColorAllRevSecondColorParam);
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

void fadeInOut() {
  int fadeInOutBrightness = 0;
  bool fadeInOutLightsUp;
  while (true) {
    checkUpdates();
    if (isModeChanged == true) {
      return;
    }
    if (hasMillisTimer == false) {
      if (fadeInOutBrightness == 0) {
        checkAndSetRandomOrSetMode();
        fadeInOutLightsUp = true;
      } else if (fadeInOutBrightness == 255) {
        fadeInOutLightsUp = false;
      }
      if (fadeInOutLightsUp == true) {
        fadeInOutBrightness++;
      } else if (fadeInOutLightsUp == false) {
        fadeInOutBrightness--;
      }
      for (int i = 0; i < LED_COUNT; i++ ) {
        setPixel(i, firstColorParam, secondColorParam, fadeInOutBrightness);
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

void customRgbLight() {
  bool isChanging = true;
  while (true) {
    if (isModeChanged == true) {
      return;
    }
    firstPCF8574.update();
    secondPCF8574.update();
    if (isSomethingChanged == true) {
      changeSomethingAndSendItToDisplay();
      isChanging = true;
      firstColorParam = firstEncoderValue;
      secondColorParam = thirdEncoderValue;
    }
    if (isChanging == true) {
      for (int i = 0; i < LED_COUNT - 1; i++ ) {
        setPixel(i, firstColorParam, secondColorParam);
      }

      FastLED.show();  
      isChanging = false;
    }
  }
}

void setModeName(int modeNum) {
  switch (modeNum) {
    case 999: break;     
    case 1: modeName = MODE_sparkle; break;  
    case 2: modeName = MODE_rainbowVertical; break;      
    case 3: modeName = MODE_newRainbowLoop; break;
    case 4: modeName = MODE_theaterChaseRainbow; break; 
    case 5: modeName = MODE_randomMarch; break;  
    case 6: modeName = MODE_randomBurst; break; 
    case 7: modeName = MODE_rwbMarch; break; 
    case 8: modeName = MODE_rgbPropeller; break;
    case 9: modeName = MODE_emsLightsOne; break;
    case 10: modeName = MODE_matrix; break;
    case 11: modeName = MODE_cylonBounce; break;
    case 12: modeName = MODE_twoDiodsGroup; break; 
    case 13: modeName = MODE_theaterChase; break;
    case 14: modeName = MODE_meteorRain; break;
    case 15: modeName = MODE_runningLights; break;  
    case 16: modeName = MODE_colorWipe; break;
    case 17: modeName = MODE_sinBrightWave; break;
    case 18: modeName = MODE_fadeVertical; break;
    case 19: modeName = MODE_pulseOneColorAllRev; break;  
    case 20: modeName = MODE_fadeInOut; break;
    case 21: modeName = MODE_customRgbLight; break; 
  }
}

void changeMode(int newmode) {
  switch (newmode) {
    case 999: break;     
    case 1: sparkle(); break; 
    case 2: rainbowVertical(); break;      
    case 3: newRainbowLoop(); break; 
    case 4: theaterChaseRainbow(); break;
    case 5: randomMarch(); break; 
    case 6: randomBurst(); break; 
    case 7: rwbMarch(); break; 
    case 8: rgbPropeller(); break;  
    case 9: emsLightsOne(); break;  
    case 10: matrix(); break; 
    case 11: cylonBounce(); break; 
    case 12: twoDiodsGroup(); break; 
    case 13: theaterChase(); break; 
    case 14: meteorRain(); break; 
    case 15: runningLights(); break; 
    case 16: colorWipe(); break; 
    case 17: sinBrightWave(); break;
    case 18: fadeVertical(); break; 
    case 19: pulseOneColorAllRev(); break;
    case 20: fadeInOut(); break; 
    case 21: customRgbLight(); break;
  }
}

void changeThirdEncMode() {
  if (isThisModeHasDirectionAndRandomOrSetMode() == true) {
    if (fifthEncoderMode == THIRDENCMODE_setOrRandom) {
      fifthEncoderMode = THIRDENCMODE_direction;
      isSomethingChanged = true;
      isFifthEncPressed = true;
    } else if (fifthEncoderMode == THIRDENCMODE_direction) {
      fifthEncoderMode = THIRDENCMODE_setOrRandom;
      isSomethingChanged = true;
      isFifthEncPressed = true;
    }
  }
} 

void setup() {
  Serial.begin(115200);
  Serial.println("system start");

  pinMode(FIRST_PCF8574_PIN, INPUT_PULLUP);
  attachInterrupt(0, movedFirstPCF8574, FALLING);

  pinMode(SECOND_PCF8574_PIN, INPUT_PULLUP);
  attachInterrupt(1, movedSecondPCF8574, FALLING);

  pinMode(FIFTH_ENCODER_BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(4, changeThirdEncMode, RISING);

  pinMode(FIRST_ENCODER_BUTTON_PIN, INPUT_PULLUP);
  pinMode(THIRD_ENCODER_BUTTON_PIN, INPUT_PULLUP);
  pinMode(FOURTH_ENCODER_BUTTON_PIN, INPUT_PULLUP);
  pinMode(SIXTH_ENCODER_BUTTON_PIN, INPUT_PULLUP);

  Wire.begin();
  Wire.setClock(100000);
  firstPCF8574.begin(4);
  firstPCF8574.readInitialState();
  secondPCF8574.begin(4);
  secondPCF8574.readInitialState();

  setModeName(modeNumber);

  clearDisplays();
  enableCurrentModeDisplayControls();
  sendValueToDisplay(7, modeNumber);

  FastLED.setBrightness(brightnessValue);
  FastLED.addLeds<WS2812, LED_DT, RGB>(leds, LED_COUNT);
  
  setAll(0, 0, 0);
  
  changeMode(modeNumber);
}

void loop() {
  checkUpdates();
  if (isModeChanged == true) {
    if (currentModeNumber != modeNumber) {
      setAll(0, 0, 0);
      currentModeNumber = modeNumber;
      sendValueToDisplay(7, modeNumber);
      setModeName(modeNumber);
      enableCurrentModeDisplayControls();
      hasMillisTimer = false;
      isModeChanged = false;
      isSomethingChanged = false;
      changeMode(modeNumber);
    }
  }
}



