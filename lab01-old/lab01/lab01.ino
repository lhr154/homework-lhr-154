// 定义两个LED的引脚
const int ledPin1 = 2;   // 第一个LED（原引脚）
const int ledPin2 = 4;   // 新增的第二个LED

// 设置PWM属性
const int freq = 5000;          // 频率 5000Hz
const int resolution = 8;       // 分辨率 8位 (0-255)

void setup() {
  Serial.begin(115200);

  // 分别将两个引脚绑定到PWM（会自动分配通道）
  ledcAttach(ledPin1, freq, resolution);
  ledcAttach(ledPin2, freq, resolution);
}

void loop() {
  // 第一个LED逐渐变亮，第二个逐渐变暗（亮度互补）
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
    ledcWrite(ledPin1, dutyCycle);          // 引脚2从0→255
    ledcWrite(ledPin2, 255 - dutyCycle);    // 引脚4从255→0
    delay(10);                               // 控制呼吸速度
  }

  // 第一个LED逐渐变暗，第二个逐渐变亮
  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
    ledcWrite(ledPin1, dutyCycle);          // 引脚2从255→0
    ledcWrite(ledPin2, 255 - dutyCycle);    // 引脚4从0→255
    delay(10);
  }
  
  Serial.println("Breathing cycle completed");
}