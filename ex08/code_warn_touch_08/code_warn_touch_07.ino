#include <WiFi.h>
#include <WebServer.h>

const char* ap_ssid = "ESP32-LAB";
const char* ap_pass = "12345678";

const int LED_PIN = 2;       // D2 报警 LED
const int TOUCH_PIN = 4;     // GPIO4 (ESP32-S3 直接用数字)

WebServer server(80);

bool armed = false;
bool alarming = false;
unsigned long lastBlink = 0;
bool blinkState = false;
unsigned long lastTouchCheck = 0;

String getHTML() {
    String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 安防报警器</title>
    <style>
        body { font-family: Arial; text-align: center; margin-top: 50px; }
        button { padding: 12px 24px; font-size: 18px; margin: 10px; cursor: pointer; border: none; border-radius: 5px; }
        .arm { background-color: #4CAF50; color: white; }
        .disarm { background-color: #f44336; color: white; }
        .status { font-size: 24px; margin: 20px; padding: 10px; border-radius: 10px; }
        .armed { background-color: #ffeb3b; color: #333; }
        .disarmed { background-color: #ccc; color: #333; }
        .alarm { background-color: red; color: white; animation: blink 0.5s step-end infinite; }
        @keyframes blink { 50% { opacity: 0; } }
    </style>
</head>
<body>
    <h1>🚨 ESP32 安防报警器</h1>
    <div class="status" id="statusDisplay">状态：未知</div>
    <button class="arm" onclick="fetch('/arm')">🔒 布防</button>
    <button class="disarm" onclick="fetch('/disarm')">🔓 撤防</button>
    <p>布防后，触摸引脚 (GPIO4) 会触发报警！</p >
    <p>报警后 LED 会持续闪烁，只有点击"撤防"才能停止。</p >

    <script>
        function updateStatus() {
            fetch('/status')
                .then(res => res.text())
                .then(data => {
                    const statusDiv = document.getElementById('statusDisplay');
                    if (data === 'armed') {
                        statusDiv.innerHTML = '🔐 已布防 - 触摸即报警';
                        statusDiv.className = 'status armed';
                    } else if (data === 'alarm') {
                        statusDiv.innerHTML = '🚨 报警中！！！ 请点击撤防 🚨';
                        statusDiv.className = 'status alarm';
                    } else {
                        statusDiv.innerHTML = '🔓 已撤防 - 安全模式';
                        statusDiv.className = 'status disarmed';
                    }
                });
        }
        setInterval(updateStatus, 500);
        updateStatus();
    </script>
</body>
</html>
)rawliteral";
    return html;
}

void handleRoot() {
    server.send(200, "text/html; charset=UTF-8", getHTML());
}

void handleArm() {
    armed = true;
    alarming = false;
    digitalWrite(LED_PIN, LOW);
    Serial.println("系统已布防");
    server.send(200, "text/plain", "armed");
}

void handleDisarm() {
    armed = false;
    alarming = false;
    digitalWrite(LED_PIN, LOW);
    Serial.println("系统已撤防");
    server.send(200, "text/plain", "disarmed");
}

void handleStatus() {
    if (alarming) {
        server.send(200, "text/plain", "alarm");
    } else if (armed) {
        server.send(200, "text/plain", "armed");
    } else {
        server.send(200, "text/plain", "disarmed");
    }
}

void handleAlarmBlink() {
    if (alarming) {
        unsigned long now = millis();
        if (now - lastBlink > 100) {
            lastBlink = now;
            blinkState = !blinkState;
            digitalWrite(LED_PIN, blinkState);
        }
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    // AP 模式
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ap_ssid, ap_pass);
    Serial.println("AP 热点已开启");
    Serial.print("访问地址: http://");
    Serial.println(WiFi.softAPIP());

    server.on("/", handleRoot);
    server.on("/arm", handleArm);
    server.on("/disarm", handleDisarm);
    server.on("/status", handleStatus);
    server.begin();
    Serial.println("Web 服务器已启动");
}

void loop() {
    server.handleClient();
    handleAlarmBlink();
    
    // 轮询检测触摸（每 100ms，阈值 600）
    if (millis() - lastTouchCheck > 100) {
        lastTouchCheck = millis();
        int touchVal = touchRead(TOUCH_PIN);
        
        // 调试：每 2 秒打印一次数值（去掉注释可查看）
        // static unsigned long lastPrint = 0;
        // if (millis() - lastPrint > 2000) {
        //     lastPrint = millis();
        //     Serial.print("触摸值: ");
        //     Serial.println(touchVal);
        // }
        
        if (touchVal < 600 && armed && !alarming) {
            alarming = true;
            Serial.println("⚠️ 触发报警！⚠️");
        }
    }
}