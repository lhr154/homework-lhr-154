#include <WiFi.h>
#include <WebServer.h>
#include <Arduino.h>

const char* ap_ssid = "ESP32-LAB";
const char* ap_pass = "12345678";

const int TOUCH_PIN = 4;     // GPIO4 (ESP32-S3 直接用数字)

WebServer server(80);

// HTML 页面（带 AJAX 实时刷新）
String getHTML() {
    String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 实时传感器仪表盘</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            text-align: center;
            margin-top: 80px;
            background: linear-gradient(135deg, #1e3c72, #2a5298);
            color: white;
            min-height: 100vh;
        }
        .card {
            background: rgba(255,255,255,0.1);
            border-radius: 20px;
            padding: 40px;
            margin: 20px auto;
            width: 80%;
            max-width: 500px;
            backdrop-filter: blur(10px);
        }
        .value {
            font-size: 80px;
            font-weight: bold;
            margin: 20px;
            font-family: 'Courier New', monospace;
            background: rgba(0,0,0,0.5);
            padding: 20px;
            border-radius: 15px;
        }
        .label {
            font-size: 18px;
            opacity: 0.9;
        }
        .unit {
            font-size: 24px;
        }
        .bar {
            width: 80%;
            height: 30px;
            background: rgba(255,255,255,0.2);
            border-radius: 15px;
            margin: 20px auto;
            overflow: hidden;
        }
        .fill {
            height: 100%;
            background: #4CAF50;
            width: 0%;
            transition: width 0.1s;
            border-radius: 15px;
        }
        .instruction {
            font-size: 14px;
            margin-top: 30px;
            opacity: 0.7;
        }
    </style>
</head>
<body>
    <div class="card">
        <h1>📊 ESP32 实时仪表盘</h1>
        <p class="label">触摸传感器数值</p >
        <div class="value" id="sensorValue">---</div>
        <div class="bar">
            <div class="fill" id="fillBar"></div>
        </div>
        <p class="instruction">
            ✋ 用手指逐渐靠近 GPIO4 引脚<br>
            数值会 <span style="color:#ff6b6b">实时变化</span> ！<br>
            手指越近，数值越小 ⬇️
        </p >
        <p class="label" style="font-size:12px">更新频率: 100ms</p >
    </div>

    <script>
        function fetchSensor() {
            fetch('/data')
                .then(response => response.text())
                .then(data => {
                    const value = parseInt(data);
                    if (!isNaN(value)) {
                        document.getElementById('sensorValue').innerHTML = value;
                        // 映射到百分比 (触摸值范围 0-1000，触摸时降到 200 左右)
                        let percent = Math.min(100, Math.max(0, (1 - value / 1000) * 100));
                        document.getElementById('fillBar').style.width = percent + '%';
                        // 根据数值改变颜色
                        const fillBar = document.getElementById('fillBar');
                        if (value < 300) {
                            fillBar.style.background = '#ff6b6b';
                        } else if (value < 600) {
                            fillBar.style.background = '#ffd93d';
                        } else {
                            fillBar.style.background = '#4CAF50';
                        }
                    }
                });
        }
        setInterval(fetchSensor, 100);  // 每100毫秒刷新一次
        fetchSensor();
    </script>
</body>
</html>
)rawliteral";
    return html;
}

void handleRoot() {
    server.send(200, "text/html; charset=UTF-8", getHTML());
}

void handleData() {
    int touchValue = touchRead(TOUCH_PIN);
    server.send(200, "text/plain", String(touchValue));
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    // AP 模式
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ap_ssid, ap_pass);
    Serial.println("AP 热点已开启");
    Serial.print("访问地址: http://");
    Serial.println(WiFi.softAPIP());

    server.on("/", handleRoot);
    server.on("/data", handleData);
    server.begin();
    Serial.println("Web 服务器已启动");
    Serial.println("触摸引脚: GPIO4");
}

void loop() {
    server.handleClient();
}