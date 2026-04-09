#include <WiFi.h>
#include <WebServer.h>

const char* ap_ssid = "ESP32-LAB";
const char* ap_pass = "12345678";

const int LED_PIN = 2;       // D2 引脚（GPIO2）
const int PWM_FREQ = 5000;   // 频率 5kHz
const int PWM_RES = 8;       // 8位分辨率（0-255）

WebServer server(80);

// HTML 页面（包含滑动条）
String getHTML() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 无极调光器</title>
    <style>
        body { font-family: Arial; text-align: center; margin-top: 50px; }
        input { width: 80%; max-width: 400px; margin: 30px 0; }
        #value { font-size: 24px; font-weight: bold; color: #4CAF50; }
        .container { padding: 20px; }
    </style>
</head>
<body>
    <div class="container">
        <h1>🎛️ ESP32 无极调光器</h1>
        <p>拖动滑动条调节 LED 亮度</p >
        <input type="range" id="brightness" min="0" max="255" value="0">
        <p>当前亮度值：<span id="value">0</span></p >
        <p style="color: #888; font-size: 14px;">滑动条值 0=熄灭 | 255=最亮</p >
    </div>

    <script>
        const slider = document.getElementById('brightness');
        const valueSpan = document.getElementById('value');

        slider.addEventListener('input', function() {
            const val = slider.value;
            valueSpan.textContent = val;
            fetch('/set?value=' + val);
        });
    </script>
</body>
</html>
)rawliteral";
  return html;
}

void handleRoot() {
    server.send(200, "text/html; charset=UTF-8", getHTML());
}

void handleSet() {
    if (server.hasArg("value")) {
        int brightness = server.arg("value").toInt();
        brightness = constrain(brightness, 0, 255);
        ledcWrite(LED_PIN, brightness);
        Serial.print("设置亮度: ");
        Serial.println(brightness);
        server.send(200, "text/plain", "OK");
    } else {
        server.send(400, "text/plain", "Missing value");
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    // 初始化 PWM
    ledcAttach(LED_PIN, PWM_FREQ, PWM_RES);
    ledcWrite(LED_PIN, 0);  // 初始熄灭

    // 开启 AP 热点
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ap_ssid, ap_pass);
    Serial.println("AP 热点已开启");
    Serial.print("访问地址: http://");
    Serial.println(WiFi.softAPIP());

    server.on("/", handleRoot);
    server.on("/set", handleSet);
    server.begin();
    Serial.println("Web 服务器已启动");
}

void loop() {
    server.handleClient();
}