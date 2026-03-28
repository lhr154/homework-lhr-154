// 定义LED引脚
const int redPin = 2;      // 红色LED
const int greenPin = 5;    // 绿色LED

// 设置PWM属性
const int freq = 5000;
const int resolution = 8;

// 警灯闪烁参数
int brightness = 0;
int direction = 1;
unsigned long lastTime = 0;
int delayTime = 5;

void setup() {
    ledcAttach(redPin, freq, resolution);
    ledcAttach(greenPin, freq, resolution);
    
    ledcWrite(redPin, 0);
    ledcWrite(greenPin, 0);
}

void loop() {
    unsigned long currentMillis = millis();
    
    if (currentMillis - lastTime >= delayTime) {
        lastTime = currentMillis;
        
        brightness = brightness + direction;
        
        if (brightness >= 255) {
            brightness = 255;
            direction = -1;
        } else if (brightness <= 0) {
            brightness = 0;
            direction = 1;
        }
        
        ledcWrite(redPin, brightness);
        ledcWrite(greenPin, 255 - brightness);
    }
}