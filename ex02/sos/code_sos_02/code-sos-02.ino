// SOS 信号灯 - 阻塞式版本（使用 delay）
// 三短、三长、三短，停顿后重复

const int ledPin = 2;   // ESP32 板载 LED 通常为 2，Arduino 可改为 13

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // === 第一组：S（短闪3次）===
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledPin, HIGH);   // 点亮
    delay(200);                    // 亮 200ms
    digitalWrite(ledPin, LOW);     // 熄灭
    delay(200);                    // 灭 200ms（点内间隔）
  }

  // 字母间隔（S 和 O 之间）
  delay(600);

  // === 第二组：O（长闪3次）===
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledPin, HIGH);   // 点亮
    delay(600);                    // 亮 600ms
    digitalWrite(ledPin, LOW);     // 熄灭
    delay(200);                    // 灭 200ms（划内间隔）
  }

  // 字母间隔（O 和 S 之间）
  delay(600);

  // === 第三组：S（短闪3次）===
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledPin, HIGH);   // 点亮
    delay(200);                    // 亮 200ms
    digitalWrite(ledPin, LOW);     // 熄灭
    delay(200);                    // 灭 200ms（点内间隔）
  }

  // 完整 SOS 播放完毕，长停顿
  delay(3000);
}