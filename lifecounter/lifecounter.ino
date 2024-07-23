#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "melodies.hpp"
#include "images.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned long BAUD = 9600;

int8_t textSize = 7;
int16_t cursorX = 24;
int16_t cursorY = 8;

 // Rotary Encoder Inputs
 #define pinA 16  //D0
 #define pinB 2    //D4
 #define pinPush 0    // D3

 // Buzzer 
#define pinBuzzer 14 // D5

int buzzerTone = 100;
unsigned long buzzerDuration = 5;

int currentA;
int currentB;
int previousA;

int pushState = 0;
bool restarting = false;

int startingLife = 40;
int maxLife = 999;
int minLife = 0;
int deathTrigger = 0;
int life = startingLife;

int deathImageIndex;

bool hasDied = false;

void setup()
{
  randomSeed(GetRandomSeed());
  deathImageIndex = random(deathImagesArrayLength);

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode (pinBuzzer, OUTPUT);

  pinMode (pinA, INPUT);
  pinMode (pinB, INPUT);
  pinMode (pinPush, INPUT_PULLUP);

  //Serial.begin(BAUD);

  TryBeginDisplay();

  UpdateDisplay();

  previousA = digitalRead(pinA);

  PlayStartupMelody(pinBuzzer);
}

uint32_t GetRandomSeed()
{
  uint8_t  seedBitValue  = 0;
  uint8_t  seedByteValue = 0;
  uint32_t seedWordValue = 0;
 
  for (uint8_t wordShift = 0; wordShift < 4; wordShift++)
  {
    for (uint8_t byteShift = 0; byteShift < 8; byteShift++)
    {
      for (uint8_t bitSum = 0; bitSum <= 8; bitSum++)
      {
        seedBitValue = seedBitValue + (analogRead(A0) & 0x01);
      }
      delay(1);
      seedByteValue = seedByteValue | ((seedBitValue & 0x01) << byteShift);
      seedBitValue = 0;
    }
    seedWordValue = seedWordValue | (uint32_t)seedByteValue << (8 * wordShift);
    seedByteValue = 0;
  }

  return seedWordValue;
}

void TryBeginDisplay() 
{
  if(display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS) == false) 
  {
    //Serial.println(F("SSD1306 allocation failed"));    
    // Don't proceed; blink and buzz forever
    for(int i = 0; i > -1; ++i) 
    {
      tone(pinBuzzer, i%2==0 == true ? 0 : 100);
      digitalWrite(LED_BUILTIN, i%2==0 == true ? LOW : HIGH);
      delay(500*((i%2)+1));
    }
  }
}

void DecrementLife() 
{
  life = max(--life, minLife);
}

void IncrementLife() 
{
  life = min(++life, maxLife);
}

void DrawBorder() 
{
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.drawRoundRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 8, WHITE);
}

void DrawLife() 
{
  if (life == deathTrigger) 
  {
    ShowDeath();
    return;
  }
  //Single Digits
  else if (life < 10) 
  {
    textSize = 7;
    cursorX = 48;
    cursorY = 8;
  }
  //Triple digits
  else if (life > 99) 
  {
    textSize = 5;
    cursorX = 20;
    cursorY = 14;
  }
  //Double digits
  else 
  {
    textSize = 7;
    cursorX = 24;
    cursorY = 8;
  }

  display.setTextSize(textSize);
  display.setTextColor(WHITE);
  display.setCursor(cursorX, cursorY);
  display.println(life);
}

void ShowDeath() 
{
  display.clearDisplay(); //remove border that was prolly already drawn
  display.drawBitmap(0, 0, allImagesArray[(++deathImageIndex)%deathImagesArrayLength], SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
  //display.invertDisplay(1);
}

void UpdateDisplay() 
{
  display.clearDisplay();
  DrawBorder();
  DrawLife();
  display.display();
}

bool IsRestarting() 
{
  pushState = digitalRead(pinPush);

  if (pushState == LOW && restarting == false)
  {
    restarting = true;    
  }  
  else if (pushState == HIGH && restarting == true) 
  {
    ESP.restart();
  }

  return restarting;
}

void PlayTick()
{
  tone(pinBuzzer, buzzerTone);
  delay(buzzerDuration);
  noTone(pinBuzzer);
}

void PlaySound() 
{
  if (life != deathTrigger) 
  {  
    PlayTick();
  }
  else if (hasDied) 
  {
    PlayInvalidMelody(pinBuzzer);
  }  
  else 
  {
    hasDied = true;
    PlayDeathMelody(pinBuzzer);    
  }
}

void CheckRotary() 
{
  currentA = digitalRead(pinA);
  currentB = digitalRead(pinB);

  if (currentA != previousA && currentA == LOW) 
  { 
    currentB == LOW ? DecrementLife() : IncrementLife();

    UpdateDisplay(); 

    PlaySound();
  }

  previousA = currentA;
}

void loop() 
{
  if (IsRestarting()) 
  {
    return;
  }

  CheckRotary();
}