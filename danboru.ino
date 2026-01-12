#include <M5StickCPlus.h>
#include "BluetoothSerial.h"
#include "Battery.h"
#include <Wire.h>
#include <VL53L0X.h> // by Pololu http://librarymanager/All#vl53l0x-arduino https://github.com/pololu/vl53l0x-arduino

unsigned long previousMillis = 0;
const long interval = 60000;
const long duration = 600;
char a, b, c;
Battery battery = Battery();
BluetoothSerial bts;
VL53L0X sensor;

void hanoi(int n, char a, char b, char c) { // 消費電流値UPのための処理
  if(n > 0) {
    hanoi(n-1, a, c, b);
    hanoi(n-1, c, b, a);
  }
}

void setup() {
  Serial.begin(115200);
  
  M5.begin();
  M5.Axp.ScreenBreath(20); // 画面の明るさ(0-100)
  M5.Lcd.setRotation(3);   // 画面を横向きに(0-3)

  // 表示左上の位置x, yと表示サイズ(1-7)を設定
  battery.setPosAndSize(5, 5, 2);

  // deleteBattery()時の塗りつぶし色を設定
  battery.setDeleteBgColor(TFT_BLACK); 

  // 電池図形と%表示の色を設定
  battery.setTextColor(TFT_WHITE);

  // バッテリーを表示
  battery.showBattery();
  Serial.println("setup end!");

  bts.begin("M5Stick-C-Plus-danboru");//PC側で確認するときの名前
  Wire.begin(32, 33); // M5StickC
  sensor.setAddress(0x29);
  sensor.setTimeout(500);
  if (!sensor.init()) {
    //Serial.println("Failed to detect and initialize sensor!");
    bts.println(10000);
    while (1) {}
  }
  sensor.startContinuous();
}

void loop() {
  M5.update();
  M5.Axp.ScreenBreath(20);
  battery.batteryUpdate();
  delay(100);
  bts.print(sensor.readRangeContinuousMillimeters());
  if (sensor.timeoutOccurred()) {
    //Serial.print(" TIMEOUT");
    bts.print(1);
  }
  bts.println();

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    // 処理を開始
    unsigned long startMillis = millis();
    while (millis() - startMillis < duration) {
      // ここに0.6秒間の処理を書く
      M5.Axp.ScreenBreath(100);
      setCpuFrequencyMhz(240);
      hanoi(12, a, b, c);
    }
  }
}