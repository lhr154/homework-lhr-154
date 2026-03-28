// 触摸自锁开关（摸一下亮，再摸一下灭）
#define TOUCH_PIN 4
#define LED_PIN 2
#define THRESHOLD 800

bool ledState = false;           // LED状态（布尔型变量）
bool lastTouchState = false;     // 上一次触摸状态（用于边缘检测）
unsigned long lastTouchTime = 0; // 上次触发时间（用于防抖）
const unsigned long debounceDelay = 50;  // 防抖延迟

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
}

void loop() {
    // 读取当前触摸值
    int touchValue = touchRead(TOUCH_PIN);
    bool currentTouchState = (touchValue < THRESHOLD);
    
    // 边缘检测：检测到按下瞬间（上一次没按，这次按了）
    if (currentTouchState == true && lastTouchState == false) {
        // 防抖：确保不是误触
        if (millis() - lastTouchTime > debounceDelay) {
            // 翻转LED状态（按一下亮，再按一下灭）
            ledState = !ledState;
            digitalWrite(LED_PIN, ledState);
            
            // 记录触发时间
            lastTouchTime = millis();
            
            // 串口调试
            Serial.print("Touch! LED: ");
            Serial.println(ledState ? "ON" : "OFF");
        }
    }
    
    // 更新上一次状态
    lastTouchState = currentTouchState;
    
    delay(20);
}