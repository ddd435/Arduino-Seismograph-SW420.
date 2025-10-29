/*
  СЕЙСМОГРАФ НА ARDUINO
  Датчик: SW-420
  Дисплей: SSD1306
  Потенциометр: для настройки чувствительности
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Настройки дисплея
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Пины
const int vibrationSensor = 2;    // SW-420
const int potentiometer = A0;     // Потенциометр
const int ledPin = 13;            // Светодиод

// Переменные
int sensitivity = 0;
int vibrationValue = 0;
bool vibrationDetected = false;

void setup() {
  Serial.begin(9600);
  
  // Инициализация дисплея
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  // Настройка пинов
  pinMode(vibrationSensor, INPUT);
  pinMode(ledPin, OUTPUT);
  
  // Очистка дисплея
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  Serial.println("Сейсмограф запущен");
}

void loop() {
  // Чтение датчиков
  sensitivity = analogRead(potentiometer) / 10; // 0-102
  vibrationValue = digitalRead(vibrationSensor);
  
  // Обработка вибрации
  if (vibrationValue == HIGH) {
    vibrationDetected = true;
    digitalWrite(ledPin, HIGH);
  } else {
    vibrationDetected = false;
    digitalWrite(ledPin, LOW);
  }
  
  // Отображение на дисплее
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("== СЕЙСМОГРАФ ==");
  display.print("Чувств: ");
  display.println(sensitivity);
  display.print("Вибрация: ");
  display.println(vibrationDetected ? "ОБНАРУЖЕНА!" : "нет");
  display.print("Порог: ");
  display.println(map(sensitivity, 0, 102, 0, 100));
  display.display();
  
  // Вывод в Serial Monitor
  Serial.print("Чувствительность: ");
  Serial.print(sensitivity);
  Serial.print(" | Вибрация: ");
  Serial.println(vibrationDetected ? "ДА" : "нет");
  
  delay(500);
}
