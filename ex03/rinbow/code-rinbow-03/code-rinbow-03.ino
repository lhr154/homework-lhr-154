// 三色LED渐变（红→绿→黄→红）
const int redPin = 2;          // 红色引脚
const int greenPin = 16;       // 绿色引脚
const int yellowPin = 4;       // 黄色引脚（替代原蓝色通道）

const int freq = 5000;
const int resolution = 8;      // 0-255

void setup() {
  ledcAttach(redPin, freq, resolution);
  ledcAttach(greenPin, freq, resolution);
  ledcAttach(yellowPin, freq, resolution);
}

void loop() {
  // 红色 -> 绿色
  for (int i = 0; i <= 255; i++) {
    ledcWrite(redPin, 255 - i);   // 红色逐渐减少
    ledcWrite(greenPin, i);       // 绿色逐渐增加
    ledcWrite(yellowPin, 0);
    delay(10);
  }

  // 绿色 -> 黄色
  for (int i = 0; i <= 255; i++) {
    ledcWrite(redPin, 0);
    ledcWrite(greenPin, 255 - i);
    ledcWrite(yellowPin, i);
    delay(10);
  }

  // 黄色 -> 红色
  for (int i = 0; i <= 255; i++) {
    ledcWrite(redPin, i);
    ledcWrite(greenPin, 0);
    ledcWrite(yellowPin, 255 - i);
    delay(10);
  }
}