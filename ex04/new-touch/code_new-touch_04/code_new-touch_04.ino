// 定义LED引脚
const int ledPin = 2;

// 设置PWM属性
const int freq = 5000;          // 频率 5000Hz
const int resolution = 8;       // 分辨率 8位 (0-255)

// 触摸引脚定义
#define TOUCH_PIN 4
#define THRESHOLD 800

// 速度档位变量
int speedLevel = 1;              // 1档、2档、3档
int delayTime = 10;              // 对应档位的延时时间

// 呼吸灯控制变量
int dutyCycle = 0;               // 当前占空比
int direction = 1;               // 1: 变亮, -1: 变暗
unsigned long lastBreathTime = 0;

// 触摸控制变量
bool lastTouchState = false;
unsigned long lastTouchTime = 0;
const unsigned long debounceDelay = 50;

void setup() {
    Serial.begin(115200);
    
    // 绑定PWM引脚
    ledcAttach(ledPin, freq, resolution);
    
    // 初始档位对应延时
    updateDelayTime();
}

void loop() {
    unsigned long currentMillis = millis();
    
    // ----- 触摸检测部分（档位切换）-----
    int touchValue = touchRead(TOUCH_PIN);
    bool currentTouchState = (touchValue < THRESHOLD);
    
    // 边缘检测：检测到按下瞬间
    if (currentTouchState == true && lastTouchState == false) {
        // 防抖
        if (currentMillis - lastTouchTime > debounceDelay) {
            // 每次触摸切换档位：1->2->3->1
            speedLevel = speedLevel + 1;
            if (speedLevel > 3) {
                speedLevel = 1;
            }
            updateDelayTime();  // 更新延时时间
            
            // 串口输出当前档位
            Serial.print("Speed Level: ");
            Serial.println(speedLevel);
            
            // 记录触发时间
            lastTouchTime = currentMillis;
        }
    }
    lastTouchState = currentTouchState;
    
    // ----- 呼吸灯部分（非阻塞）-----
    if (currentMillis - lastBreathTime >= delayTime) {
        lastBreathTime = currentMillis;
        
        // 改变占空比
        dutyCycle = dutyCycle + direction;
        
        // 边界判断，到达最大或最小值时反向
        if (dutyCycle >= 255) {
            dutyCycle = 255;
            direction = -1;
        } else if (dutyCycle <= 0) {
            dutyCycle = 0;
            direction = 1;
        }
        
        // 输出PWM
        ledcWrite(ledPin, dutyCycle);
    }
}

// 根据档位更新延时时间
void updateDelayTime() {
    if (speedLevel == 1) {
        delayTime = 10;      // 1档：慢速呼吸
    } else if (speedLevel == 2) {
        delayTime = 5;       // 2档：中速呼吸
    } else if (speedLevel == 3) {
        delayTime = 2;       // 3档：快速呼吸
    }
}