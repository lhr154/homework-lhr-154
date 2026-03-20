// SOS 信号（非阻塞）
const int ledPin = 2;

// 信号时序定义（单位：毫秒）
const int shortOn = 200;
const int shortOff = 200;
const int longOn = 600;
const int longOff = 200;
const int wordGap = 1200;   // 每组 SOS 结束后的长停顿

// 状态机
enum SOSState {
  SOS_SHORT1_ON, SOS_SHORT1_OFF,
  SOS_SHORT2_ON, SOS_SHORT2_OFF,
  SOS_SHORT3_ON, SOS_SHORT3_OFF,
  SOS_LONG1_ON,  SOS_LONG1_OFF,
  SOS_LONG2_ON,  SOS_LONG2_OFF,
  SOS_LONG3_ON,  SOS_LONG3_OFF,
  SOS_SHORT4_ON, SOS_SHORT4_OFF,
  SOS_SHORT5_ON, SOS_SHORT5_OFF,
  SOS_SHORT6_ON, SOS_SHORT6_OFF,
  SOS_WORD_GAP
};

SOSState state = SOS_SHORT1_ON;
unsigned long previousMillis = 0;
unsigned long currentOnset = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {
  unsigned long now = millis();

  // 状态机驱动
  switch (state) {
    case SOS_SHORT1_ON:
      digitalWrite(ledPin, HIGH);
      previousMillis = now;
      state = SOS_SHORT1_OFF;
      break;
    case SOS_SHORT1_OFF:
      if (now - previousMillis >= shortOn) {
        digitalWrite(ledPin, LOW);
        previousMillis = now;
        state = SOS_SHORT2_ON;
      }
      break;

    case SOS_SHORT2_ON:
      if (now - previousMillis >= shortOff) {
        digitalWrite(ledPin, HIGH);
        previousMillis = now;
        state = SOS_SHORT2_OFF;
      }
      break;
    case SOS_SHORT2_OFF:
      if (now - previousMillis >= shortOn) {
        digitalWrite(ledPin, LOW);
        previousMillis = now;
        state = SOS_SHORT3_ON;
      }
      break;

    case SOS_SHORT3_ON:
      if (now - previousMillis >= shortOff) {
        digitalWrite(ledPin, HIGH);
        previousMillis = now;
        state = SOS_SHORT3_OFF;
      }
      break;
    case SOS_SHORT3_OFF:
      if (now - previousMillis >= shortOn) {
        digitalWrite(ledPin, LOW);
        previousMillis = now;
        state = SOS_LONG1_ON;
      }
      break;

    // 三次长闪（类似结构）
    case SOS_LONG1_ON:
      if (now - previousMillis >= shortOff) {
        digitalWrite(ledPin, HIGH);
        previousMillis = now;
        state = SOS_LONG1_OFF;
      }
      break;
    case SOS_LONG1_OFF:
      if (now - previousMillis >= longOn) {
        digitalWrite(ledPin, LOW);
        previousMillis = now;
        state = SOS_LONG2_ON;
      }
      break;

    case SOS_LONG2_ON:
      if (now - previousMillis >= longOff) {
        digitalWrite(ledPin, HIGH);
        previousMillis = now;
        state = SOS_LONG2_OFF;
      }
      break;
    case SOS_LONG2_OFF:
      if (now - previousMillis >= longOn) {
        digitalWrite(ledPin, LOW);
        previousMillis = now;
        state = SOS_LONG3_ON;
      }
      break;

    case SOS_LONG3_ON:
      if (now - previousMillis >= longOff) {
        digitalWrite(ledPin, HIGH);
        previousMillis = now;
        state = SOS_LONG3_OFF;
      }
      break;
    case SOS_LONG3_OFF:
      if (now - previousMillis >= longOn) {
        digitalWrite(ledPin, LOW);
        previousMillis = now;
        state = SOS_SHORT4_ON;
      }
      break;

    // 最后三次短闪
    case SOS_SHORT4_ON:
      if (now - previousMillis >= shortOff) {
        digitalWrite(ledPin, HIGH);
        previousMillis = now;
        state = SOS_SHORT4_OFF;
      }
      break;
    case SOS_SHORT4_OFF:
      if (now - previousMillis >= shortOn) {
        digitalWrite(ledPin, LOW);
        previousMillis = now;
        state = SOS_SHORT5_ON;
      }
      break;

    case SOS_SHORT5_ON:
      if (now - previousMillis >= shortOff) {
        digitalWrite(ledPin, HIGH);
        previousMillis = now;
        state = SOS_SHORT5_OFF;
      }
      break;
    case SOS_SHORT5_OFF:
      if (now - previousMillis >= shortOn) {
        digitalWrite(ledPin, LOW);
        previousMillis = now;
        state = SOS_SHORT6_ON;
      }
      break;

    case SOS_SHORT6_ON:
      if (now - previousMillis >= shortOff) {
        digitalWrite(ledPin, HIGH);
        previousMillis = now;
        state = SOS_SHORT6_OFF;
      }
      break;
    case SOS_SHORT6_OFF:
      if (now - previousMillis >= shortOn) {
        digitalWrite(ledPin, LOW);
        previousMillis = now;
        state = SOS_WORD_GAP;
      }
      break;

    case SOS_WORD_GAP:
      if (now - previousMillis >= wordGap) {
        // 重新开始一轮 SOS
        state = SOS_SHORT1_ON;
      }
      break;
  }
}