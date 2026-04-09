#include <WiFi.h>
#include <WebServer.h>

const char* ap_ssid = "ESP32-LAB";
const char* ap_pass = "12345678";

const int LED_PIN = 2;       // D2 引脚 (GPIO2)
const int TOUCH_PIN = 4;     // GPIO4 (D4) 触摸引脚
const int PWM_FREQ = 5000;   // PWM 频率 5kHz
const int PWM_RES = 8;       // 8位分辨率 (0-255)

WebServer server(80);

int speedLevel = 1;          // 当前档位: 1-慢, 2-中, 3-快
bool touchTriggered = false; // 触摸触发标志
unsigned long lastTouchTime = 0;

// HTML 页面
String getHTML() {
    String levelText;
    if (speedLevel == 1) levelText = "🐢 慢速";
    else if (speedLevel == 2) levelText = "⚡ 中速";
    else levelText = "🐇 快速";
    
    String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 触摸调速呼吸灯</title>
    <style>
        body { font-family: Arial; text-align: center; margin-top: 50px; background: #1a1a2e; color: white; }
        .card { background: rgba(255,255,255,0.1); padding: 30px; border-radius: 20px; margin: 20px auto; width: 80%; max-width: 500px; }
        .level { font-size: 32px; margin: 20px; padding: 15px; border-radius: 10px; }
        .level-1 { background-color: #4CAF50; }
        .level-2 { background-color: #FF9800; }
        .level-3 { background-color: #f44336; }
        button { padding: 12px 24px; font-size: 18px; margin: 10px; cursor: pointer; border: none; border-radius: 5px; }
        .refresh { background-color: #2196F3; color: white; }
    </style>
</head>
<body>
    <div class="card">
        <h1>🎛️ 触摸调速呼吸灯</h1>
        <p>当前档位：</p >
        <div class="level level-)rawliteral" + String(speedLevel) + R"rawliteral(" id="levelDisplay">)rawliteral" + levelText + R"rawliteral(</div>
        <p>👇 触摸 GPIO4 引脚切换速度</p >
        <button class="refresh" onclick="location.reload()">🔄 刷新状态</button>
        <p style="font-size: 14px; opacity: 0.7;">档位1: 慢速 | 档位2: 中速 | 档位3: 快速</p >
    </div>
    <script>
        function fetchStatus() {
            fetch('/status')
                .then(res => res.text())
                .then(data => {
                    const levelDiv = document.getElementById('levelDisplay');
                    if (data === '1') {
                        levelDiv.innerHTML = '🐢 慢速';
                        levelDiv.className = 'level level-1';
                    } else if (data === '2') {
                        levelDiv.innerHTML = '⚡ 中速';
                        levelDiv.className = 'level level-2';
                    } else if (data === '3') {
                        levelDiv.innerHTML = '🐇 快速';
                        levelDiv.className = 'level level-3';
                    }
                });
        }
        setInterval(fetchStatus, 500);
        fetchStatus();
    </script>
</body>
</html>
)rawliteral";
    return html;
}

void handleRoot() {
    server.send(200, "text/html; charset=UTF-8", getHTML());
}

void handleStatus() {
    server.send(200, "text/plain", String(speedLevel));
}

// 呼吸灯效果（非阻塞）
void breathEffect() {
    static int brightness = 0;
    static int fadeAmount = 5;
    static unsigned long lastUpdate = 0;
    
    // 根据档位设置速度（delay 时间）
    int speedDelay;
    if (speedLevel == 1) speedDelay = 20;      // 慢速
    else if (speedLevel == 2) speedDelay = 10; // 中速
    else speedDelay = 5;                       // 快速
    
    unsigned long now = millis();
    if (now - lastUpdate >= speedDelay) {
        lastUpdate = now;
        
        brightness = brightness + fadeAmount;
        if (brightness <= 0 || brightness >= 255) {
            fadeAmount = -fadeAmount;
            brightness = constrain(brightness, 0, 255);
        }
        ledcWrite(LED_PIN, brightness);
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    // 初始化 PWM
    ledcAttach(LED_PIN, PWM_FREQ, PWM_RES);
    ledcWrite(LED_PIN, 0);
    
    // AP 模式
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ap_ssid, ap_pass);
    Serial.println("AP 热点已开启");
    Serial.print("访问地址: http://");
    Serial.println(WiFi.softAPIP());
    
    server.on("/", handleRoot);
    server.on("/status", handleStatus);
    server.begin();
    Serial.println("Web 服务器已启动");
    Serial.println("触摸引脚: GPIO4 | LED引脚: GPIO2");
}

void loop() {
    server.handleClient();
    
    // 轮询检测触摸（防抖）
    int touchVal = touchRead(TOUCH_PIN);
    static bool lastTouchState = false;
    bool isTouched = (touchVal < 600);  // ESP32-S3 阈值
    
    if (isTouched && !lastTouchState) {
        // 触摸触发，切换档位
        speedLevel++;
        if (speedLevel > 3) speedLevel = 1;
        Serial.print("档位切换为: ");
        Serial.println(speedLevel);
        delay(300);  // 防抖
    }
    lastTouchState = isTouched;
    
    // 执行呼吸灯效果
    breathEffect();
}