// 非线性呼吸灯（正弦映射）
const int ledPin = 2;          // LED 引脚
const int freq = 5000;         // PWM 频率 5kHz
const int resolution = 8;      // 8位分辨率 (0-255)
const float pi = 3.1415926;

void setup() {
  ledcAttach(ledPin, freq, resolution);
}

void loop() {
  // 半周期正弦波：从 0 -> 255 -> 0
  for (float phase = 0; phase <= pi; phase += 0.02) {
    int duty = (int)(255 * sin(phase));   // sin(0)=0, sin(π/2)=1, sin(π)=0
    ledcWrite(ledPin, duty);
    delay(8);                             // 控制呼吸速度
  }
}