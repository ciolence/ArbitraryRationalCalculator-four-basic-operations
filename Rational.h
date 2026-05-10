#ifndef __RATIONAL_H
#define __RATIONAL_H

#include "BigInt.h"

class Rational {
private:
    BigInt numerator;    // 分子
    BigInt denominator;  // 分母

    // 约分
    void normalize() {
        if (denominator == BigInt(0)) {
            throw std::runtime_error("Rational: division by zero");
        }

        // 分子为零，分母为1
        if (numerator == BigInt(0)) {
            denominator = BigInt(1);
            return;
        }

        // 求分子分母最大公约数
        BigInt g = gcd(numerator.abs(), denominator.abs());
        numerator /= g;
        denominator /= g;

        // 确保分母为正
        if (denominator < BigInt(0)) {
            numerator = -numerator;
            denominator = -denominator;
        }
    }

    // 最大公约数 (欧几里得算法)
    static BigInt gcd(BigInt a, BigInt b) {
        // 确保a, b非负
        a = a.abs();
        b = b.abs();

        // 处理特殊情况
        if (a == BigInt(0)) return b;
        if (b == BigInt(0)) return a;

        // 辗转相除法
        while (b != BigInt(0)) {
            BigInt temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    // 最小公倍数
    static BigInt lcm(const BigInt& a, const BigInt& b) {
        if (a == BigInt(0) && b == BigInt(0)) {
            return BigInt(0);
        }
        return (a * b) / gcd(a, b);
    }

    // 去除字符串中多余的负号
    std::string extinguisher(const std::string& s) {
        if (s.size() == 0 || s[0] != '-') {
            return s;
        }
        std::string res = "-";
        for (int i = 1; i < s.size(); ++i) {
            if (s[i] != '-') {
                res += s[i];
            }
        }
        return res;
    }

public:
    // 构造函数
    Rational() : numerator(0), denominator(1) {}
    
    Rational(ll num) : numerator(num), denominator(1) {}
    
    Rational(const BigInt& num) : numerator(num), denominator(1) {}
    
    Rational(const BigInt& num, const BigInt& den) : numerator(num), denominator(den) {
        normalize();
    }

    Rational(const std::string& s) {
        // 去除空白字符
        std::string str = s;
        str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
        
        if (str.empty()) {
            numerator = BigInt(0);
            denominator = BigInt(1);
            return;
        }
        
        size_t slash_pos = str.find('/');
        if (slash_pos != std::string::npos) {
            // 分数形式: "a/b"
            std::string num_str = str.substr(0, slash_pos);
            std::string den_str = str.substr(slash_pos + 1);
            
            numerator = BigInt(num_str);
            denominator = BigInt(den_str);
            normalize();
        } else {
            // 检查是否为小数形式
            size_t dot_pos = str.find('.');
            if (dot_pos != std::string::npos) {
                // 处理小数形式
                std::string int_part = str.substr(0, dot_pos);
                std::string frac_part = str.substr(dot_pos + 1);
                
                // 处理科学计数法
                size_t e_pos = frac_part.find('e');
                if (e_pos == std::string::npos) {
                    e_pos = frac_part.find('E');
                }
                
                int exponent = 0;
                if (e_pos != std::string::npos) {
                    std::string exp_str = frac_part.substr(e_pos + 1);
                    frac_part = frac_part.substr(0, e_pos);
                    exponent = std::stoi(exp_str);
                }
                
                // 处理符号
                bool is_negative = false;
                if (!int_part.empty() && int_part[0] == '-') {
                    is_negative = true;
                    int_part = int_part.substr(1);
                } else if (!int_part.empty() && int_part[0] == '+') {
                    int_part = int_part.substr(1);
                }
                
                // 如果整数部分是空字符，视为0
                if (int_part.empty()) {
                    int_part = "0";
                }
                
                // 去除小数部分末尾的零
                while (!frac_part.empty() && frac_part.back() == '0') {
                    frac_part.pop_back();
                }
                
                // 构建分子：整数部分 + 小数部分
                std::string combined = int_part + frac_part;
                
                // 如果所有数字都是零
                bool all_zero = true;
                for (char c : combined) {
                    if (c != '0') {
                        all_zero = false;
                        break;
                    }
                }
                
                if (all_zero) {
                    numerator = BigInt(0);
                    denominator = BigInt(1);
                    return;
                }
                
                // 去除前导零
                size_t non_zero = combined.find_first_not_of('0');
                if (non_zero != std::string::npos) {
                    combined = combined.substr(non_zero);
                }
                
                // 设置分子
                numerator = BigInt(combined);
                if (is_negative) {
                    numerator = -numerator;
                }
                
                // 构造分母：10^(小数位数)
                denominator = BigInt(1);
                for (size_t i = 0; i < frac_part.length(); ++i) {
                    denominator = denominator * BigInt(10);
                }
                
                // 处理指数
                if (exponent != 0) {
                    if (exponent > 0) {
                        // 正指数：乘以10^exponent
                        for (int i = 0; i < exponent; ++i) {
                            numerator = numerator * BigInt(10);
                        }
                    } else {
                        // 负指数：除以10^exponent (即分母乘以10^exponent)
                        for (int i = 0; i < -exponent; ++i) {
                            denominator = denominator * BigInt(10);
                        }
                    }
                }
                
                normalize();
            } else {
                // 整数形式
                numerator = BigInt(str);
                denominator = BigInt(1);
            }
        }
    }

    // 拷贝构造函数
    Rational(const Rational& other) = default;
    
    // 赋值运算符
    Rational& operator=(const Rational& other) = default;
    
    // 获取分子分母
    const BigInt& getNumerator() const { return numerator; }
    const BigInt& getDenominator() const { return denominator; }
    
    // 设置分子分母
    void setNumerator(const BigInt& num) {
        numerator = num;
        normalize();
    }
    
    void setDenominator(const BigInt& den) {
        denominator = den;
        normalize();
    }
    
    // 一元运算符
    Rational operator+() const {
        return *this;
    }
    
    Rational operator-() const {
        Rational result = *this;
        result.numerator = -result.numerator;
        return result;
    }

    // 算术运算符
    Rational operator+(const Rational& other) const {
        Rational result;
        
        if (denominator == other.denominator) {
            result.numerator = numerator + other.numerator;
            result.denominator = denominator;
        } else {
            BigInt l = lcm(denominator, other.denominator);
            result.numerator = numerator * (l / denominator) + 
                              other.numerator * (l / other.denominator);
            result.denominator = l;
        }
        
        result.normalize();
        return result;
    }
    
    Rational operator-(const Rational& other) const {
        return *this + (-other);
    }
    
    Rational operator*(const Rational& other) const {
        Rational result;
        result.numerator = numerator * other.numerator;
        result.denominator = denominator * other.denominator;
        result.normalize();
        return result;
    }
    
    Rational operator/(const Rational& other) const {
        if (other.numerator == BigInt(0)) {
            throw std::runtime_error("Rational division by zero");
        }
        
        Rational result;
        result.numerator = numerator * other.denominator;
        result.denominator = denominator * other.numerator;
        result.normalize();
        return result;
    }
    
    // 复合赋值运算符
    Rational& operator+=(const Rational& other) {
        *this = *this + other;
        return *this;
    }
    
    Rational& operator-=(const Rational& other) {
        *this = *this - other;
        return *this;
    }
    
    Rational& operator*=(const Rational& other) {
        *this = *this * other;
        return *this;
    }
    
    Rational& operator/=(const Rational& other) {
        *this = *this / other;
        return *this;
    }
    
    // 比较运算符
    bool operator==(const Rational& other) const {
        return numerator == other.numerator && denominator == other.denominator;
    }
    
    bool operator!=(const Rational& other) const {
        return !(*this == other);
    }
    
    bool operator<(const Rational& other) const {
        BigInt lhs = numerator * other.denominator;
        BigInt rhs = other.numerator * denominator;
        return lhs < rhs;
    }
    
    bool operator<=(const Rational& other) const {
        BigInt lhs = numerator * other.denominator;
        BigInt rhs = other.numerator * denominator;
        return lhs <= rhs;
    }
    
    bool operator>(const Rational& other) const {
        return !(*this <= other);
    }
    
    bool operator>=(const Rational& other) const {
        return !(*this < other);
    }
    
    // 数学函数
    Rational abs() const {
        Rational result = *this;
        result.numerator = result.numerator.abs();
        result.normalize();
        return result;
    }

    // 转换为字符串
    std::string toString() const {
        if (denominator == BigInt(1)) {
            return numerator.toString();
        } else {
            return numerator.toString() + "/" + denominator.toString();
        }
    }

    // 转换为小数形式
    std::string toDecimalString(int precision = 20) const {
        if (denominator == BigInt(0)) {
            return "INF";
        }

        if (numerator == BigInt(0)) {
            return "0";
        }

        // 处理符号
        bool is_negative = (numerator < BigInt(0));
        BigInt abs_num = numerator.abs();
        BigInt abs_den = denominator.abs();

        // 整数部分
        BigInt int_part = abs_num / abs_den;
        BigInt remainder = abs_num % abs_den;

        std::string result = int_part.toString();

        // 如果余数不为零且有精度要求，计算小数部分
        if (remainder != BigInt(0) && precision > 0) {
            result += ".";

            for (ll i = 0; i < precision && remainder != BigInt(0); ++i) {
                remainder *= BigInt(10);
                BigInt digit = remainder / abs_den;
                result += digit.toString();
                remainder %= abs_den;
            }
        }

        // 处理符号
        if (is_negative) {
            result = "-" + result;
        }

        // 处理潜在问题 (可选)
        // result = extinguisher(result);
        
        return result;
    }

    // 是否为整数
    bool isInteger() const {
        return denominator == BigInt(1) || (numerator % denominator == BigInt(0));
    }
    
    // 获取整数部分
    BigInt getIntegerPart() const {
        return numerator / denominator;
    }
    
    // 获取小数部分
    Rational getFractionalPart() const {
        return *this - Rational(getIntegerPart());
    }
};

#endif