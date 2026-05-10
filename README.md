[English](README.md) | [简体中文](README.zh-CN.md)

---

<h1 align="center">Arbitrary Precision Rational Arithmetic Calculator</h1>

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-17-blue.svg" alt="C++17">
  <img src="https://img.shields.io/badge/Qt-6-green.svg" alt="Qt6">
  <img src="https://img.shields.io/badge/CMake-3.16+-red.svg" alt="CMake">
</p>

## 📖 Introduction

A GUI calculator built with **C++17** and **Qt6** that supports **arbitrary-precision** rational number arithmetic. It utilizes a custom `BigInt` class (linked-list-based arbitrary precision integer) and a `Rational` class built on top of it for exact fraction arithmetic with no precision loss.

### ✨ Key Features

- **Arbitrary-precision integer**: Linked list (`DigitNode`) storage, theoretically limited only by memory
- **Exact rational arithmetic**: Always stored and computed as fractions, automatically reduced
- **Multiple input formats**: Integers, decimals, fractions (`a/b`), and scientific notation
- **Dual display**: Results shown as both fractions and decimals
- **Modern Qt GUI**: Fusion style, clean layout, friendly interaction

---

## 📁 Project Structure

```
├── main.cpp                # Entry point
├── CalculatorWidget.h      # Calculator UI header
├── CalculatorWidget.cpp    # Calculator UI implementation
├── Storage.h               # Linked list node + BigInteger/BigRational (reference)
├── BigInt.h                # Arbitrary-precision integer (header-only)
├── Rational.h              # Rational number class (header-only)
├── CMakeLists.txt          # CMake build configuration
└── PLAN.md                 # Development plan
```

---

## 🔧 Build & Run

### Dependencies

- CMake ≥ 3.16
- C++17 compatible compiler (GCC, Clang, MSVC)
- Qt 6 (Core + Widgets modules)

### Build Steps

```bash
cd ArbitraryRationalCalculator
mkdir build && cd build
cmake ..
cmake --build .
./ArbitraryRationalCalculator
```

> **Windows Users**: Make sure Qt6 is in your system `PATH`, or use Visual Studio's CMake integration.
>
> **macOS Users**: If Qt6 is installed via Homebrew, use `cmake .. -DCMAKE_PREFIX_PATH=$(brew --prefix qt6)`.

---

## ⌨️ Input Formats

| Format | Example | Description |
|--------|---------|-------------|
| **Integer** | `12345678901234567890` | Arbitrary length integer |
| **Decimal** | `3.14159265358979323846` | Arbitrary precision decimal, auto-converted to fraction |
| **Fraction** | `1/2` | Numerator/Denominator, auto-reduced |
| **Scientific** | `1.23e-5` | Supports positive and negative exponents |

**Notes**: Denominator must not be zero; divisor must not be zero for division; whitespace is automatically ignored.

---

## ⚙️ Technical Implementation

### BigInt (Arbitrary-Precision Integer)

The `BigInt` class uses a **little-endian linked list** to store each digit (0–9). Each `DigitNode` holds one digit and a pointer to the next higher digit.

- **Base unit**: `BASE = 1e9` (stores 9 decimal digits per element in `to_vec()`)
- **Core algorithms**:
  - Addition / Subtraction: column addition/subtraction on vector representation
  - Multiplication: double loop column multiplication
  - Division: binary search trial quotient method
  - `normalize()`: auto-reduction using Euclidean algorithm (GCD)
- **Operator overloads**: Full set of comparison, arithmetic, and compound assignment operators

### Rational

The `Rational` class builds upon `BigInt` to represent rational numbers exactly as numerator/denominator fractions. Automatically reduced on construction, results always simplified, supports decimal conversion via `toDecimalString(int precision)`.

### GUI (Qt Widgets)

Two `QLineEdit` widgets for operands, `QComboBox` for operator selection, results displayed as both fraction and decimal, custom stylesheet.

---

## 🎯 Use Cases

- Applications requiring exact large integer and fraction arithmetic
- Educational demonstrations of big number algorithms, fraction arithmetic, and Qt GUI programming
- Any calculation exceeding the limits of native C++ integer types (e.g., `int64_t`)
