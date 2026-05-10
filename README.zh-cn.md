[English](README.md) | [简体中文](README.zh-CN.md)

---

<h1 align="center">任意大有理数四则运算计算器</h1>

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-17-blue.svg" alt="C++17">
  <img src="https://img.shields.io/badge/Qt-6-green.svg" alt="Qt6">
  <img src="https://img.shields.io/badge/CMake-3.16+-red.svg" alt="CMake">
</p>

## 📖 简介

这是一个基于 **C++17** 和 **Qt6** 开发的图形界面计算器，支持**任意大**的有理数（分数）四则运算。核心采用自定义的 `BigInt` 任意精度整数类（以链表存储），并在其之上构建了有理数 `Rational` 类，实现分数的精确运算，无精度损失。

### ✨ 核心特性

- **任意精度整数**：基于链表（`DigitNode`）存储，理论上仅受内存限制
- **精确有理数运算**：始终以分数形式存储和计算，自动约分
- **多格式输入**：支持整数、小数、分数（`a/b`）、科学计数法
- **分数 + 小数双显示**：结果同时以分数形式和小数形式展示
- **现代 Qt GUI**：Fusion 风格，布局清晰，交互友好

---

## 📁 项目结构

```
├── main.cpp                # 程序入口
├── CalculatorWidget.h      # 计算器 UI 头文件
├── CalculatorWidget.cpp    # 计算器 UI 实现
├── Storage.h               # 链表节点 + 基础大数类（用于参考）
├── BigInt.h                # 任意精度整数类（头文件实现）
├── Rational.h              # 有理数类（头文件实现）
├── CMakeLists.txt          # CMake 构建配置
└── PLAN.md                 # 开发计划
```

---

## 🔧 构建与运行

### 依赖

- CMake ≥ 3.16
- C++17 兼容的编译器（GCC, Clang, MSVC）
- Qt 6（Core + Widgets 模块）

### 构建步骤

```bash
cd ArbitraryRationalCalculator
mkdir build && cd build
cmake ..
cmake --build .
./ArbitraryRationalCalculator
```

> **Windows 用户**：确保 Qt6 的安装路径已添加到系统环境变量 `PATH` 中，或使用 Visual Studio 的 CMake 集成进行构建。
>
> **macOS 用户**：若通过 Homebrew 安装 Qt6，可使用 `cmake .. -DCMAKE_PREFIX_PATH=$(brew --prefix qt6)`。

---

## ⌨️ 输入格式

| 格式 | 示例 | 说明 |
|------|------|------|
| **整数** | `12345678901234567890` | 任意长度整数 |
| **小数** | `3.14159265358979323846` | 任意精度小数，自动转为分数 |
| **分数** | `1/2` | 分子/分母，自动约分 |
| **科学计数法** | `1.23e-5` | 支持正负指数 |

**注意**：分母不能为零；除法除数不能为零；空白字符会被自动忽略。

---

## ⚙️ 技术实现

### BigInt（任意精度整数）

`BigInt` 使用**小端序链表**存储每一位数字（0–9），每个节点 `DigitNode` 包含一个数字和指向更高位的指针。

- **基础单位**：`BASE = 1e9`（`to_vec()` 层面每元素存 9 位十进制数字）
- **核心算法**：
  - 加 / 减：基于 vector 的竖式计算
  - 乘法：双重循环模拟竖式乘法
  - 除法：二分搜索试商法
  - `normalize()`：辗转相除法求 GCD 自动约分
- **运算符重载**：全套比较、算术、复合赋值运算符

### Rational（有理数）

`Rational` 在 `BigInt` 基础上以分子/分母分数形式精确表示有理数，构造时自动约分，四则运算结果自动化为最简分数，支持小数转换（`toDecimalString(int precision)`）。

### 图形界面（Qt Widgets）

两个 `QLineEdit` 接收操作数，`QComboBox` 选择运算符，结果显示分数和小数两种形式，自定义样式表美化。

---

## 🎯 适用范围

- 需要精确的大整数与分数运算的场景
- 教学演示：大数算法、分数运算、Qt GUI 编程
- 任何需要突破 C++ 原生整数类型（如 `int64_t`）限制的计算
