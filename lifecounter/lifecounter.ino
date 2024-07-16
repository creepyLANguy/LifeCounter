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

// 'Project1', 128x64px
const unsigned char zeroImage [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x07, 0xf0, 0x00, 0x1c, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x1c, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0xff, 0xf8, 0x00, 0x38, 0x01, 0xc0, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x03, 0xff, 0xfc, 0x00, 0x38, 0x01, 0xc0, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x0f, 0xff, 0xfc, 0x00, 0x38, 0x03, 0xc0, 0x78, 0x01, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x38, 0x03, 0x80, 0x70, 0x03, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x78, 0x03, 0x80, 0x70, 0x03, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x01, 0xff, 0xff, 0xfe, 0x00, 0x70, 0x03, 0x80, 0x70, 0x07, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x03, 0xff, 0xff, 0xfe, 0x00, 0x70, 0x07, 0x80, 0x70, 0x07, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x07, 0xff, 0xff, 0xfe, 0x00, 0x70, 0x07, 0x80, 0x70, 0x07, 0x00, 0x00, 0x03, 0xc0, 
	0x00, 0x00, 0x1f, 0x80, 0xff, 0xff, 0x00, 0x70, 0x07, 0x00, 0x70, 0x0f, 0x00, 0x00, 0x0f, 0xc0, 
	0x00, 0x00, 0x3f, 0x00, 0x7f, 0xff, 0x00, 0x70, 0x07, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x3f, 0xc0, 
	0x00, 0x00, 0x7e, 0x00, 0x3f, 0xff, 0x00, 0x70, 0x07, 0x00, 0x70, 0x0f, 0x00, 0x00, 0xff, 0xc0, 
	0x00, 0x00, 0xfe, 0x00, 0x1f, 0xff, 0x00, 0x70, 0x07, 0x00, 0xf0, 0x0e, 0x00, 0x03, 0xff, 0x80, 
	0x00, 0x01, 0xfc, 0x00, 0x1f, 0xff, 0x00, 0x70, 0x07, 0x00, 0x70, 0x0e, 0x00, 0x07, 0xff, 0x00, 
	0x00, 0x03, 0xfc, 0x00, 0x1f, 0xff, 0x00, 0x70, 0x07, 0x00, 0x70, 0x0e, 0x00, 0x0f, 0xfe, 0x00, 
	0x00, 0x07, 0xfc, 0x00, 0x1f, 0xff, 0x00, 0x70, 0x07, 0x00, 0x70, 0x1e, 0x00, 0x1f, 0xfe, 0x00, 
	0x00, 0x0f, 0xfc, 0x00, 0x1f, 0xff, 0x80, 0x70, 0x07, 0x00, 0x70, 0x0e, 0x00, 0x3f, 0xfc, 0x00, 
	0x00, 0x1f, 0xfc, 0x00, 0x1f, 0xff, 0x80, 0x70, 0x07, 0x00, 0xf0, 0x1e, 0x00, 0x7f, 0xf8, 0x00, 
	0x00, 0x3f, 0xfe, 0x00, 0x1f, 0xff, 0x80, 0x70, 0x07, 0xbf, 0xff, 0x1e, 0x00, 0x7f, 0xf8, 0x00, 
	0x00, 0x3f, 0xff, 0x00, 0x3f, 0xff, 0x87, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xf0, 0x00, 
	0x00, 0x7f, 0xff, 0x80, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x80, 0x70, 0x1f, 0xff, 0xff, 0xf0, 0x00, 
	0x00, 0x3f, 0xff, 0xc1, 0xff, 0xff, 0xff, 0xf0, 0x07, 0x80, 0x70, 0x0e, 0x1f, 0xff, 0xe0, 0x00, 
	0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xf8, 0x70, 0x03, 0x80, 0x70, 0x0e, 0x01, 0xff, 0xe0, 0x00, 
	0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0x80, 0x70, 0x03, 0x80, 0x70, 0x0e, 0x00, 0xff, 0xf0, 0x00, 
	0x00, 0x38, 0x03, 0xff, 0xff, 0xff, 0x80, 0x70, 0x03, 0x80, 0x70, 0x0e, 0x00, 0x7f, 0xf0, 0x00, 
	0x00, 0x7f, 0x83, 0xff, 0xff, 0xff, 0x80, 0x70, 0x03, 0xc0, 0x70, 0x0e, 0x00, 0x3f, 0xf8, 0x00, 
	0x00, 0x3f, 0xe1, 0xff, 0xff, 0xff, 0x80, 0x70, 0x03, 0xc0, 0x30, 0x0e, 0x00, 0x1f, 0xf8, 0x00, 
	0x00, 0x1f, 0xf3, 0xff, 0xff, 0xff, 0x80, 0x70, 0x03, 0xc0, 0x38, 0x0e, 0x00, 0x0f, 0xfc, 0x00, 
	0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0x80, 0x78, 0x01, 0xc0, 0x38, 0x0e, 0x00, 0x07, 0xfc, 0x00, 
	0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0x80, 0x38, 0x01, 0xc0, 0x38, 0x06, 0x00, 0x01, 0xfe, 0x00, 
	0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0x80, 0x38, 0x01, 0xc0, 0x38, 0x06, 0x00, 0x00, 0xfe, 0x00, 
	0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0x80, 0x38, 0x01, 0xe0, 0x18, 0x07, 0x00, 0x00, 0x7e, 0x00, 
	0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0x80, 0x3c, 0x00, 0xe0, 0x1c, 0x07, 0x00, 0x00, 0x1e, 0x00, 
	0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0x80, 0x1c, 0x00, 0xe0, 0x1c, 0x03, 0x00, 0x00, 0x0e, 0x00, 
	0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0x00, 0x1c, 0x00, 0x60, 0x0c, 0x03, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0x00, 0x1c, 0x00, 0x60, 0x0c, 0x01, 0x80, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x0e, 0x00, 0x70, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0x00, 0x0e, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x07, 0xff, 0xfe, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x0f, 0xfe, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

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

  if (life == 0) {
    ShowDeath();
    return;
  }
  //Single Digits
  else if (life < 10) {
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

void ShowDeath() {
  display.clearDisplay();
  display.drawBitmap(0, 0,  zeroImage, 128, 64, WHITE);
  //display.invertDisplay(1);
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
    DecrementLife(); 
    //IncrementLife(); 
  }
  else if (input == LOW) {
    Serial.println("-");
  }
}

void loop() {
  buttonStatus = digitalRead(buttonInc);

  LED(buttonStatus == HIGH);
  
  HandleUserInput(buttonStatus);

  UpdateDisplay();
}