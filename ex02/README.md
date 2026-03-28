# 实验2：基础IO控制

## 硬件连接
| 元件 | 引脚 |
|------|------|
| LED | GPIO 2 |

## 代码文件

### 1. code_flicker_02（基础闪烁）
- LED 亮 1 秒，灭 1 秒，循环往复

### 2. code_double-flicker_02（交替闪烁）
- GPIO 2 和 GPIO 4 两个 LED 交替闪烁
- 一个亮时另一个灭

### 3. code_sos_02（SOS求救信号）
- 使用 `millis()` 实现非阻塞 SOS 信号
- 三短闪、三长闪、三短闪，每组结束后有较长停顿

### 4. code-extra.task02（millis 闪烁）
- 使用 `millis()` 函数实现 1Hz 稳定闪烁
- 非阻塞方式，不占用 CPU

## 实验现象

### code_flicker_02
- LED 亮 1 秒，灭 1 秒，循环

### code_double-flicker_02
- 红色 LED 亮时绿色 LED 灭
- 红色 LED 灭时绿色 LED 亮
- 循环交替

### code_sos_02
- 短闪 3 次（约 200ms/次）
- 长闪 3 次（约 600ms/次）
- 短闪 3 次
- 长停顿（约 1200ms）
- 循环播放 SOS 信号

### code-extra.task02
- LED 亮 500ms，灭 500ms，循环
- 使用 `millis()` 实现，无 `delay()`
