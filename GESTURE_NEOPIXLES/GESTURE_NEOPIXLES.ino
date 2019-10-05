/*
   file simpleGesture.ino

   @ https://github.com/DFRobot/DFRobot_Gesture_Touch

   example for DFRobot Gesture&Touch sensor

   sensor event will print on your serial monitor

   for esp32, rx_pin = D5, tx_pin = D6

   Copyright   [DFRobot](http://www.dfrobot.com), 2016
   Copyright   GNU Lesser General Public License

   version  V1.0
   date  2018-10-19
*/

#include "DFRobot_Gesture_Touch.h"

#define REDUCED_MODES // sketch too big for Arduino Leonardo flash, so invoke reduced modes
#include <WS2812FX.h>


#ifdef __AVR__
#include "SoftwareSerial.h"
SoftwareSerial    mySerial(10, 11);         // example for uno, use software serial
#elif defined ESP_PLATFORM
#include "HardwareSerial.h"
HardwareSerial    mySerial(2);
#endif

DFRobot_Gesture_Touch   DFGT(&mySerial);

#define LED_COUNT 60
#define LED_PIN 7
#define MAX_NUM_CHARS 16 // init sensor object, request write and read function


WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

char cmd[MAX_NUM_CHARS];       // char[] to store incoming serial commands
boolean cmd_complete = false;  // whether the command string is complete


int t = 0;
int m = 0;
void setup()
{
  Serial.begin(115200);
  mySerial.begin(9600);
  delay(500);
  while (mySerial.available()) // flush serial buffer
    mySerial.read();
  Serial.println("gesture&touch sensor test");

  DFGT.setGestureDistance(20);             // suggest default value
  DFGT.enableFunction(DFGT_FUN_ALL);

  ws2812fx.init();
  ws2812fx.setBrightness(30);
  ws2812fx.setSpeed(1000);
  ws2812fx.setColor(0xFF0000);
  ws2812fx.setMode(FX_MODE_STATIC);
  ws2812fx.start();

  printModes();

  // enable all functions
  //DFGT.disableFunction(DFGT_FUN_RIGHT | DFGT_FUN_LEFT);    // disable function test
  // DFGT.enableFunction(DFGT_FUN_RIGHT | DFGT_FUN_LEFT);     // enable function test
  // DFGT.setSleep(4);                        // set auto sleep time out, in sleep mode, something  approach will weak it up
}

void loop()
{
  ws2812fx.service();
  int8_t    rslt = DFGT.getAnEvent();  // get an event that data saved in serial buffer
  if (rslt != DF_ERR) {
    // DFGT.setSleep(DFGT_SLEEP_DISABLE);  // disable auto sleep
    if (rslt == DFGT_EVT_PULLUP)
    {
      Serial.println("get event pull up");
      while ( rslt != DFGT_EVT_PULLREMOVE)
      {
        rslt = DFGT.getAnEvent();
        ws2812fx.increaseBrightness(7);
        Serial.print("Increased brightness by ");
        Serial.print(ws2812fx.getBrightness());
        Serial.print(" ");
        Serial.println(t);
        t += 7;
        if (t > 255)
          t = 255;
        delay(100);
      }
    }
    if (rslt == DFGT_EVT_PULLDOWN)
    {
      Serial.println("get event pull down");
      while ( rslt != DFGT_EVT_PULLREMOVE)
      {
        rslt = DFGT.getAnEvent();
        ws2812fx.decreaseBrightness(7);
        Serial.print("Decreased brightness by ");
        Serial.println(ws2812fx.getBrightness());
        Serial.print(" ");
        Serial.println(t);
        t -= 7;
        if (t < 0)
          t = 0;
        delay(100);
      }
    }
    if (rslt == DFGT_EVT_FORWARD)
    {
      m++;
      Serial.println("get event forward");
      ws2812fx.setMode(m);
      Serial.print(F("Set mode to: "));
      Serial.print(ws2812fx.getMode());
      Serial.print(" - ");
      Serial.println(ws2812fx.getModeName(ws2812fx.getMode()));
    }
    if (rslt == DFGT_EVT_BACK)
    {
      m--;
      Serial.println("get event forward");
      ws2812fx.setMode(m);
      Serial.print(F("Set mode to: "));
      Serial.print(ws2812fx.getMode());
      Serial.print(" - ");
      Serial.println(ws2812fx.getModeName(ws2812fx.getMode()));
    }
    if (rslt == DFGT_EVT_TOUCH1)
    {
      Serial.println("get event Touch 1");
      ws2812fx.setColor(0xFF0000);
      ws2812fx.setMode(FX_MODE_STATIC);
    }
    if (rslt == DFGT_EVT_TOUCH2)
    {
      Serial.println("get event Touch 2");
      ws2812fx.setColor(0x00FF00);
      ws2812fx.setMode(FX_MODE_STATIC);
    }
    if (rslt == DFGT_EVT_TOUCH3)
    {
      Serial.println("get event Touch 3");
      ws2812fx.setColor(0x0000FF);
      ws2812fx.setMode(FX_MODE_STATIC);
    }
    if (rslt == DFGT_EVT_TOUCH4)
    {
      Serial.println("get event Touch 4");
      ws2812fx.setColor(0xFFFF00);
      ws2812fx.setMode(FX_MODE_STATIC);
    }
    if (rslt == DFGT_EVT_TOUCH5)
    {
      Serial.println("get event Touch 5");
      ws2812fx.setColor(0xFF00F);
      ws2812fx.setMode(FX_MODE_STATIC);
    }

    /*
      switch (rslt) {
      case DFGT_EVT_BACK: {
          Serial.println("get event back");
          m--;
          Serial.println("get event forward");
          ws2812fx.setMode(m);
          Serial.print(F("Set mode to: "));
          Serial.print(ws2812fx.getMode());
          Serial.print(" - ");
          Serial.println(ws2812fx.getModeName(ws2812fx.getMode()));
        } break;
      case DFGT_EVT_FORWARD: {
          Serial.println("get event forward");
          m++;
          Serial.println("get event forward");
          ws2812fx.setMode(m);
          Serial.print(F("Set mode to: "));
          Serial.print(ws2812fx.getMode());
          Serial.print(" - ");
          Serial.println(ws2812fx.getModeName(ws2812fx.getMode()));
        } break;
      case DFGT_EVT_RIGHT: {
          Serial.println("get event right");
        } break;
      case DFGT_EVT_LEFT: {
          Serial.println("get event left");
        } break;
      case DFGT_EVT_PULLUP: {
          Serial.println("get event pull up");

        } break;
      case DFGT_EVT_PULLDOWN: {
          Serial.println("get event pull down");

        } break;
      case DFGT_EVT_PULLREMOVE: {
          Serial.println("get event pull and remove");
        } break;
      case DFGT_EVT_TOUCH1: {
          Serial.println("get event touch1");
        } break;
      case DFGT_EVT_TOUCH2: {
          Serial.println("get event touch2");
        } break;
      case DFGT_EVT_TOUCH3: {
          Serial.println("get event touch3");
        } break;
      case DFGT_EVT_TOUCH4: {
          Serial.println("get event touch4");
        } break;
      case DFGT_EVT_TOUCH5: {
          Serial.println("get event touch5");
        } break;
      }
    */
  }
}

  void printModes() {
    Serial.println(F("Supporting the following modes: "));
    Serial.println();
    for (int i = 0; i < ws2812fx.getModeCount(); i++) {
      Serial.print(i);
      Serial.print(F("\t"));
      Serial.println(ws2812fx.getModeName(i));
    }
    Serial.println();
  }
