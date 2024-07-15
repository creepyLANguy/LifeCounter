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

int16_t life = 40;

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

  // initialize the OLED object
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.setTextColor(WHITE);
  display.setTextSize(textSize);
  UpdateDisplay();
}

void DecrementLife() {
  --life;
  if(life < 0) {
    life = 0;
  } 
}

void IncrementLife() {
  ++life;
}

void UpdateDisplay() {
  display.clearDisplay();

  if (life < 10) {
    cursorX = 48;
  }
  else {
    cursorX = 24;
  }
  display.setCursor(cursorX, cursorY);
  
  display.println(life);
  display.display();
}

void loop() {
  
  // DecrementLife(); 
  // UpdateDisplay();
  // delay(1000); 

  buttonStatus = digitalRead(buttonInc);

  // if (buttonStatus == HIGH) {
  //   digitalWrite(LED_BUILTIN, HIGH);
  // }
  // else {
  //   digitalWrite(LED_BUILTIN, LOW);
  // }

  if (buttonStatus == HIGH) {
    Serial.println("Button pressed!");
    DecrementLife(); 
    UpdateDisplay();
    delay(1000);
  }
}