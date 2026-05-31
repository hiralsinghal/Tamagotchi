#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, & Wire, OLED_RESET);

#define BTN1 2
#define BTN2 3
#define BTN3 4
#define BTN4 5

#define BTN_LEFT D0
#define BTN_MIDDLE_LEFT D1
#define BTN_MIDDLE_RIGHT D2
#define BTN_RIGHT D3

#define SDA_PIN 6
#define SCL_PIN 7

#define BUZZER 10

#define BUZZER_PIN D10

struct Pet {
  int hunger;
  int happiness;
  int energy;
  unsigned long age;
};

Pet pet;

enum Screen {
  SCREEN_MAIN,
  SCREEN_FEED,
  SCREEN_PLAY,
  SCREEN_SLEEP
};

Screen currentScreen = SCREEN_MAIN;

void handleScreenLogic() {
  switch(currentScreen) {

    case SCREEN_FEED:
      pet.hunger += 10;
      if(pet.hunger > 100) pet.hunger = 100;
      currentScreen = SCREEN_MAIN;
      break;

    case SCREEN_PLAY:
      pet.happiness += 10;
      pet.energy -= 5;
      if (pet.happiness > 100) pet.happiness = 100;
      if (pet.energy < 0) pet.energy = 0;
      currentScreen = SCREEN_MAIN;
      break;
    
    case SCREEN_SLEEP:
      pet.energy += 15;
      if (pet.energy > 100) pet.energy = 100;
      currentScreen = SCREEN_MAIN;
      break;

    case SCREEN_MAIN:
      break;
  }
}

unsigned long lastUpdate = 0;

void updatePet() {
  if (millis() - lastUpdate > 5000) {
    pet.hunger--;
    pet.happiness--;
    pet.energy--;

    if (pet.hunger < 0) pet.hunger = 0;
    if (pet.happiness < 0) pet.happiness =0;
    if (pet.energy < 0) pet.energy = 0;

    pet.age += 5;
    lastUpdate = millis();
  }
}

unsigned long lastButtonPress = 0;

void checkButtons() {
  if (millis() - lastButtonPress < 200) return;

  if (digitalRead(BTN_LEFT) == LOW) {
    currentScreen = SCREEN_FEED;
    tone(BUZZER_PIN, 1000, 50);
    lastButtonPress = millis();
  }

  else if (digitalRead(BTN_RIGHT) == LOW) {
    currentScreen = SCREEN_SLEEP;
    tone(BUZZER_PIN, 800, 50);
    lastButtonPress = millis();
  }
}

// Happy Face 
const unsigned char PROGMEM petHappy[] = {
  0b00000000, 0b00000000,
  0b00001111, 0b11111000,
  0b00001100, 0b00011000,
  0b00001000, 0b00001000,
  0b00001011, 0b01101000,
  0b00001000, 0b00001000,
  0b00001010, 0b00101000,
  0b00001011, 0b11101000,
  0b00001000, 0b00001000,
  0b00001111, 0b11111000,
  0b00000010, 0b00100000,
  0b00001110, 0b00111000,
  0b00001000, 0b00001000,
  0b00001000, 0b00001000,
  0b00001000, 0b00001000,
  0b00001000, 0b00001000
};

// Sad Face
const unsigned char PROGMEM petSad[] = {
  0b00000000, 0b00000000,
  0b00001111, 0b11111000,
  0b00001100, 0b00011000,
  0b00001000, 0b00001000,
  0b00001011, 0b01101000,
  0b00001000, 0b00001000,
  0b00001011, 0b11101000,
  0b00001010, 0b00101000,
  0b00001000, 0b00001000,
  0b00001111, 0b11111000,
  0b00000010, 0b00100000,
  0b00001110, 0b00111000,
  0b00001000, 0b00001000,
  0b00001000, 0b00001000,
  0b00001000, 0b00001000,
  0b00001000, 0b00001000
};

// Usage:
// display.drawBitmap(x, y, sprite, 16, 16, SSD1306_WHITE);

// Neutral Face
const unsigned char PROGMEM petNeutral[] = {
  0b00000000, 0b00000000,
  0b00001111, 0b11111000,
  0b00001100, 0b00011000,
  0b00001000, 0b00001000,
  0b00001011, 0b01101000,
  0b00001000, 0b00001000,
  0b00001000, 0b00001000,
  0b00001011, 0b11101000,
  0b00001000, 0b00001000,
  0b00001111, 0b11111000,
  0b00000010, 0b00100000,
  0b00001110, 0b00111000,
  0b00001000, 0b00001000,
  0b00001000, 0b00001000,
  0b00001000, 0b00001000,
  0b00001000, 0b00001000
};

// Sleeping Face
const unsigned char PROGMEM petSleep[] = {
  0b00000000, 0b00000000,
  0b00001111, 0b11111000,
  0b00001100, 0b00011000,
  0b00001000, 0b00001000,
  0b00001010, 0b00101000,
  0b00001000, 0b00001000,
  0b00001000, 0b00001000,
  0b00001011, 0b11101000,
  0b00001000, 0b00001000,
  0b00001111, 0b11111000,
  0b00000010, 0b00100000,
  0b00001110, 0b00111000,
  0b00001000, 0b00001000,
  0b00001000, 0b00001000,
  0b00001000, 0b00001000,
  0b00001000, 0b00001000
};

// Usage:
// display.drawBitmap(x, y, sprite, 16, 16, SSD1306_WHITE);

void render() {
  display.clearDisplay();

  const unsigned char* sprite;
  if (pet.hunger < 30 || pet.happiness < 30 || pet.energy < 30) {
    sprite = petSad;
  } else if (pet.hunger > 50 && pet.happiness > 50 && pet.energy > 50) {
    sprite = petHappy;
  } else {
    sprite = petNeutral;
  }

  display.drawBitmap(56, 2, sprite, 16, 16, SSD1306_WHITE);

  display.setTextSize(1);

  display.setCursor(0, 24);
  display.print("HUN ");
  drawBar(24, 24, pet.hunger);

  display.setCursor(0, 34);
  display.print("HAP ");
  drawBar(24, 34, pet.happiness);

  display.setCursor(0, 44);
  display.print("ENG ");
  drawBar(24, 44, pet.energy);

  display.setCursor(0, 56);
  display.println("[Feed] [Play] [Sleep]");

  display.display();
}

void drawBar(int x, int y, int value) {
  int barWidth = 100;
  int barHeight = 6;
  int fillWidth = map(value, 0, 100, 0, barWidth);

  display.drawRect(x, y, barWidth, barHeight, SSD1306_WHITE);
  display.fillRect(x, y, fillWidth, barHeight, SSD1306_WHITE);
}

void setup() {
  pinModer(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_MIDDLE_LEFT, INPUT_PULLUP);
  pinMode(BTN_MIDDLE_RIGHT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Tamagotchi Init...");
  display.display();
  delay(1000);

  pet.hunger = 80;
  pet.happiness = 80;
  pet.energy = 80;
  pet.age = 0;
}

void loop() {
 checkButtons();
 updatePet();
 handleScreenLogic();
 render();
 delay(100);

}
