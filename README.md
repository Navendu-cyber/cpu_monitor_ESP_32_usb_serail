🚀 ESP32 PC System Monitor – USB Serial Dashboard

A real-time PC hardware monitoring dashboard built with an ESP32, a 1.8” TFT display, a servo motor, and a WS2812 LED strip.
All CPU, RAM, and temperature stats are streamed from the PC using a lightweight Python script over USB serial.

This project brings AIDA64-style dashboards to life with real moving hardware, smooth animations, and RGB response.

✨ Features
📊 Live PC Hardware Monitoring

CPU usage (0–100%)

RAM usage (%)

CPU temperature (°C)

Smooth animated transitions (lerp)

Flicker-free rendering using TFT_eSPI sprites

⚙️ Physical CPU Gauge (Servo Needle)

SG90/MG90 180° servo indicates CPU load

Smooth synchronized movement

Redline bounce effect above 70% usage

Startup sweep animation like a sports-car tachometer

🌈 Reactive WLED LED Strip

WS2812 LEDs visualize CPU load in real time:

CPU Load	LED Color
0–40%	🟢 Green
40–70%	🟡 Yellow
70–100%	🔴 Red

Powered by FastLED for clean performance.

📟 1.8” TFT Display Dashboard (Portrait Mode)

ST7735 128×160 TFT (portrait layout)

Clean CPU/RAM/TEMP bars

Hardware-accelerated via TFT_eSPI

Perfect readability and layout alignment

Zero flicker due to off-screen sprite rendering

🔌 USB Serial Communication

Lightweight Python script using psutil

Cross-platform (Linux & Windows)

Updates sent every 0.5 seconds

No WiFi delays — ultra responsive

🛠 Hardware Used

ESP32 DevKit v1

1.8” ST7735 TFT Display (128×160)

SG90/MG90 Servo (180°)

WS2812 LED Strip (10 LEDs)

Software dashboards are cool — but hardware dashboards feel alive.
This project blends:

Embedded systems

UI design

RGB lighting

Real-time PC monitoring

Motion via servo hardware

…into a compact, desk-friendly device that’s fun to build and even better to use.
Push-button (optional)

External 5V supply for servo + LEDs (recommended)

🔌 Pinout Connections

Below is the complete wiring diagram for the ESP32 + 1.8” ST7735 TFT + Servo + WS2812 LED Strip.

📟 1.8” TFT Display (ST7735 – 128×160)

Configured for TFT_eSPI using your custom User_Setup.

TFT Pin	ESP32 Pin	Description
VCC	3.3V	Power (⚠️ Do NOT use 5V)
GND	GND	Ground
SCK / SCL	GPIO 18	SPI Clock
SDA / MOSI	GPIO 23	SPI Data
DC / A0	GPIO 2	Data/Command
CS	GPIO 5	Chip Select
RST	GPIO 4	Reset Pin
LED	3.3V	Backlight (or through transistor for dimming)

TFT_eSPI User_Setup must match these pins.

⚙️ Servo Motor (SG90/MG90 — 180°)
Servo Pin	ESP32 Pin	Notes
Signal	GPIO 25	PWM control
VCC	5V	Use external 5V for best stability
GND	GND	MUST share ground with ESP32

⚠️ Important:
Do NOT power the servo from the ESP32 3.3V pin.
Use a 5V supply (1A recommended) and common ground.

🌈 WS2812 LED Strip (WLED / FastLED)
LED Pin	ESP32 Pin	Notes
Data In	GPIO 14	Safe for FastLED
5V	External 5V	Power source
GND	Common GND	Shared with ESP32 & servo

⚠️ Always connect the GND of LED, ESP32, and servo together.

🔘 Button (Mode Switch) – Optional
Button Pin	ESP32 Pin	Notes
One side	GPIO 13	With INPUT_PULLUP
Other side	GND	Pull-down on press

Button behavior:

Press → Change dashboard mode

🪢 Common Ground Wiring

To avoid flicker, servo jitter, or LED noise:

GND of ESP32, Servo, and LED strip must be connected together.
