#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using I2C
#define OLED_RESET     -1 // Reset pin
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int16_t life = 0;

int8_t textSize = 7;
int16_t cursorX = 24;
int16_t cursorY = 8;

int buttonInc = 16;
int buttonStatus = 0;

void setup()
{  
  pinMode(LED_BUILTIN, OUTPUT);
  
  pinMode (buttonInc, INPUT_PULLUP);

  Serial.begin(9600);

  TryBeginDisplay();

  UpdateDisplay();
}

void TryBeginDisplay() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    // Don't proceed, blink forever
    for(int i = 0; i > -1; ++i) {
      LED(i%2==0);
      delay(500*((i%2)+1));
    }
  }
}

void LED(bool lightUp) {
  digitalWrite(LED_BUILTIN, lightUp == true ? LOW : HIGH); //opposite for this board
}

void DecrementLife() {
  --life;
  if(life < 0) {
    life = 0;
  } 
}

void IncrementLife() {
  ++life;
  if(life > 999) {
    life = 999;
  } 
}

void DrawBorder() {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.drawRoundRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 8, WHITE);
}

void DrawLife() {

  //Single Digits
  if (life < 10) {
    textSize = 7;
    cursorX = 48;
    cursorY = 8;
  }
  //Triple digits
  else if (life > 99) {
    textSize = 5;
    cursorX = 20;
    cursorY = 14;
  }
  //Double digits
  else {
    textSize = 7;
    cursorX = 24;
    cursorY = 8;
  }

  display.setTextSize(textSize);
  display.setTextColor(WHITE);
  display.setCursor(cursorX, cursorY);
  display.println(life);
}

void UpdateDisplay() {
  display.clearDisplay();
  DrawBorder();
  DrawLife();
  display.display();
}

void HandleUserInput(int input) {
  if (input == HIGH) {
    Serial.println("Button pressed!");
    //DecrementLife(); 
    IncrementLife(); 
  }
}

void loop() {
  buttonStatus = digitalRead(buttonInc);

  LED(buttonStatus == HIGH);
  
  HandleUserInput(buttonStatus);

  UpdateDisplay();
}