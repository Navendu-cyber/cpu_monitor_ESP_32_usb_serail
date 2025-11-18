/******************************************************
   ESP32 PORTRAIT PC MONITOR (CPU / RAM / TEMP)
   + SERVO CPU NEEDLE (GPIO 25)
   + WLED STRIP CPU REACTION (GPIO 14)
   + 1.8" TFT ST7735 (128×160 Portrait)
******************************************************/

#include <TFT_eSPI.h>
#include <SPI.h>
#include <ESP32Servo.h>
#include <FastLED.h>

// ------------------ LED STRIP ------------------
#define LED_PIN 14
#define NUM_LEDS 10
CRGB leds[NUM_LEDS];

// ------------------ SERVO ------------------
#define SERVO_PIN 25
Servo cpuServo;

// ------------------ DISPLAY ------------------
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);

// Portrait size
const int W = 128;
const int H = 160;

// ------------------ VALUES ------------------
float cpuReal = 0;
float ramReal = 0;
float tempReal = 0;

float cpuDisp = 0;
float ramDisp = 0;
float tempDisp = 0;

// ------------------ SMOOTH LERP ------------------
float smooth(float a, float b, float t) {
  return a + (b - a) * t;
}

// ------------------ TEMP COLOR ------------------
uint16_t tempColor(float t) {
  if (t < 50) return TFT_CYAN;
  if (t < 60) return TFT_YELLOW;
  return TFT_RED;
}

// ------------------ DRAW BAR ------------------
void drawBar(int y, float value, const char *label, uint16_t color) {
  int barX = 22;
  int barW = 90;
  int barH = 18;

  spr.setTextColor(TFT_WHITE, TFT_BLACK);
  spr.drawString(label, 2, y + 2);

  spr.drawRect(barX, y, barW, barH, TFT_DARKGREY);

  int fill = map(value, 0, 100, 0, barW);
  spr.fillRect(barX + 1, y + 1, fill - 2, barH - 2, color);

  spr.drawString(String((int)value) + "%", barX + barW + 5, y + 2);
}

// ------------------ SERVO UPDATE ------------------
void updateServo(float cpu) {
  int angle = map((int)cpu, 0, 100, 0, 180);

  // redline bounce effect
  if (cpu > 70) angle += random(0, 3);

  cpuServo.write(constrain(angle, 0, 180));
}

// ------------------ WLED UPDATE ------------------
void updateWLED(float cpu) {
  CRGB color;

  if (cpu < 40) color = CRGB::Green;
  else if (cpu < 70) color = CRGB::Yellow;
  else color = CRGB::Red;

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
  }

  FastLED.show();
}

// ------------------ SETUP ------------------
void setup() {
  Serial.begin(115200);

  // Display
  tft.init();
  tft.setRotation(0);      // PORTRAIT
  spr.createSprite(W, H);
  spr.setSwapBytes(true);

  // Servo
  cpuServo.attach(SERVO_PIN, 500, 2400);

  // WS2812 LEDs
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(100);

  // Startup servo sweep (like car cluster)
  for (int i = 0; i <= 180; i += 4) {
    cpuServo.write(i);
    delay(5);
  }
  for (int i = 180; i >= 0; i -= 5) {
    cpuServo.write(i);
    delay(5);
  }
}

// ------------------ LOOP ------------------
void loop() {

  // -------- Read from PC --------
  if (Serial.available()) {
    String msg = Serial.readStringUntil('\n');

    int c1 = msg.indexOf(',');
    int c2 = msg.lastIndexOf(',');

    cpuReal  = msg.substring(0, c1).toFloat();
    ramReal  = msg.substring(c1 + 1, c2).toFloat();
    tempReal = msg.substring(c2 + 1).toFloat();
  }

  // -------- Smooth Animation --------
  cpuDisp  = smooth(cpuDisp, cpuReal, 0.12);
  ramDisp  = smooth(ramDisp, ramReal, 0.12);
  tempDisp = smooth(tempDisp, tempReal, 0.12);

  // -------- Update Servo --------
  updateServo(cpuDisp);

  // -------- Update LEDs --------
  updateWLED(cpuDisp);

  // -------- Draw UI --------
  spr.fillSprite(TFT_BLACK);

  drawBar(20, cpuDisp, "CPU", TFT_GREEN);
  drawBar(65, ramDisp, "RAM", TFT_BLUE);
  drawBar(110, tempDisp, "TEMP", tempColor(tempDisp));

  spr.pushSprite(0, 0);

  delay(25);
}
