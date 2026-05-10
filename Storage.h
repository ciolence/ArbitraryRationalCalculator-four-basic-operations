#ifndef STORAGE_H
#define STORAGE_H

#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include <stdexcept>

// 链表节点结构
struct DigitNode {
    int digit;          // 存储0-9的数字
    DigitNode* next;    // 指向下一个节点（更高位）
    
    DigitNode(int d = 0, DigitNode* n = nullptr) : digit(d), next(n) {}
};

class BigInteger {
private:
    DigitNode* head;    // 链表头，指向最低有效位
    bool isNegative;    // 符号标志，true表示负数
    
    // 清理前导零
    void trimLeadingZeros() {
        if (!head) return;
        
        // 找到倒数第二个节点
        DigitNode* prev = nullptr;
        DigitNode* current = head;
        
        while (current && current->next) {
            prev = current;
            current = current->next;
        }
        
        // 从链表尾部开始删除多余的零
        while (prev && prev->next && prev->next->digit == 0) {
            delete prev->next;
            prev->next = nullptr;
            current = prev;
            
            // 向上回溯
            if (prev == head) break;
            
            // 找到新的最后一个节点
            DigitNode* temp = head;
            while (temp && temp->next != prev) {
                temp = temp->next;
            }
            prev = temp;
        }
        
        // 处理只有一个节点且为0的情况
        if (head && !head->next && head->digit == 0) {
            isNegative = false;  // 0没有负号
        }
    }
    
    // 清除所有节点
    void clear() {
        DigitNode* current = head;
        while (current) {
            DigitNode* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
    }
    
    // 反转链表，用于从字符串构造
    void reverseList() {
        DigitNode* prev = nullptr;
        DigitNode* current = head;
        DigitNode* next = nullptr;
        
        while (current) {
            next = current->next;
            current->next = prev;
            prev = current;
            current = next;
        }
        
        head = prev;
    }
    
public:
    // 默认构造函数，创建值为0的大整数
    BigInteger() : head(nullptr), isNegative(false) {
        head = new DigitNode(0);
    }
    
    // 从字符串构造
    BigInteger(const std::string& str) : head(nullptr), isNegative(false) {
        fromString(str);
    }
    
    // 拷贝构造函数
    BigInteger(const BigInteger& other) : head(nullptr), isNegative(other.isNegative) {
        if (!other.head) {
            head = nullptr;
            return;
        }
        
        DigitNode* otherCurrent = other.head;
        DigitNode* tail = nullptr;
        
        while (otherCurrent) {
            DigitNode* newNode = new DigitNode(otherCurrent->digit);
            
            if (!head) {
                head = tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
            
            otherCurrent = otherCurrent->next;
        }
    }
    
    // 析构函数
    ~BigInteger() {
        clear();
    }
    
    // 从字符串解析并存储
    void fromString(const std::string& str) {
        clear();
        
        if (str.empty()) {
            head = new DigitNode(0);
            isNegative = false;
            return;
        }
        
        // 处理符号和提取有效数字
        isNegative = false;
        std::string digits;
        
        for (char c : str) {
            if (c == '-' || c == '+') {
                // 符号只处理第一次出现
                if (digits.empty() && !isNegative) {
                    isNegative = (c == '-');
                }
            } else if (std::isdigit(static_cast<unsigned char>(c))) {
                digits.push_back(c);
            }
            // 其他字符（空格、字母、标点等）都被忽略
        }
        
        // 如果没有数字，则设置为0
        if (digits.empty()) {
            head = new DigitNode(0);
            isNegative = false;
            return;
        }
        
        // 移除前导零（在转换为链表之前）
        size_t startPos = 0;
        while (startPos < digits.size() - 1 && digits[startPos] == '0') {
            startPos++;
        }
        digits = digits.substr(startPos);
        
        // 如果所有数字都是0，确保有一个0
        if (digits.empty()) {
            head = new DigitNode(0);
            isNegative = false;
            return;
        }
        
        // 将数字字符串转换为链表（目前是大端序：最高位在字符串开头）
        for (char c : digits) {
            int digit = c - '0';
            DigitNode* newNode = new DigitNode(digit, head);
            head = newNode;
        }
        
        // 反转链表，使其变为小端序（最低有效位在头部）
        reverseList();
        
        // 清理前导零
        trimLeadingZeros();
        
        // 如果是0，确保符号为正
        if (head && !head->next && head->digit == 0) {
            isNegative = false;
        }
    }
    
    // 转换为字符串
    std::string toString() const {
        if (!head) {
            return "0";
        }
        
        std::string result;
        
        // 由于是小端序，需要从最低位开始收集数字
        // 先收集到向量中，然后反转
        std::string digits;
        DigitNode* current = head;
        
        while (current) {
            digits.push_back(static_cast<char>('0' + current->digit));
            current = current->next;
        }
        
        // 反转字符串，得到正确顺序
        std::reverse(digits.begin(), digits.end());
        
        // 添加符号
        if (isNegative) {
            result.push_back('-');
        }
        
        result += digits;
        
        return result;
    }
    
    // 赋值运算符
    BigInteger& operator=(const BigInteger& other) {
        if (this == &other) {
            return *this;
        }
        
        clear();
        isNegative = other.isNegative;
        
        if (other.head) {
            DigitNode* otherCurrent = other.head;
            DigitNode* tail = nullptr;
            
            while (otherCurrent) {
                DigitNode* newNode = new DigitNode(otherCurrent->digit);
                
                if (!head) {
                    head = tail = newNode;
                } else {
                    tail->next = newNode;
                    tail = newNode;
                }
                
                otherCurrent = otherCurrent->next;
            }
        }
        
        return *this;
    }
    
    // 从字符串赋值
    BigInteger& operator=(const std::string& str) {
        fromString(str);
        return *this;
    }
    
    // 打印函数（用于调试）
    void print() const {
        std::cout << "BigInteger: " << toString() << std::endl;
        std::cout << "Internal (little-endian): ";
        
        DigitNode* current = head;
        while (current) {
            std::cout << current->digit;
            if (current->next) {
                std::cout << " -> ";
            }
            current = current->next;
        }
        std::cout << std::endl;
    }
};


class BigRational {
private:
    BigInteger numerator;   // 分子
    BigInteger denominator; // 分母 (保证不为零)

    // 辅助函数：检查字符串是否为纯数字（用于分母检查）
    bool isAllDigits(const std::string& s) const {
        if (s.empty()) return false;
        for (char c : s) {
            if (!std::isdigit(static_cast<unsigned char>(c))) return false;
        }
        return true;
    }

    // 辅助函数：解析科学计数法（如 "1.23e-5"）
    void parseScientific(const std::string& str) {
        // 1. 分割尾数和指数
        size_t ePos = str.find_first_of("eE");
        if (ePos == std::string::npos || ePos == str.length() - 1) {
            throw std::invalid_argument("科学计数法格式错误：缺少指数部分");
        }

        std::string mantissaStr = str.substr(0, ePos);
        std::string exponentStr = str.substr(ePos + 1);

        // 2. 验证指数必须为整数（允许带符号）
        if (!isValidInteger(exponentStr)) {
            throw std::invalid_argument("科学计数法指数部分必须为整数");
        }

        // 3. 将尾数解析为分数（递归调用，走小数/整数分支）
        //    此时 mantissa 的分子分母已通过纯字符串方式确定
        BigRational mantissa(mantissaStr);

        // 4. 纯字符串操作：根据指数 n 调整分子分母
        int n = std::stoi(exponentStr); // 仅用于确定拼接次数，不用于计算

        if (n > 0) {
            // 正指数：相当于分子乘以 10^n -> 在分子字符串后拼接 n 个 '0'
            std::string newNumStr = mantissa.getNumerator() + std::string(n, '0');
            numerator = BigInteger(newNumStr);
            denominator = mantissa.denominator; // 分母不变
        }
        else if (n < 0) {
            // 负指数：相当于分母乘以 10^|n| -> 在分母字符串后拼接 |n| 个 '0'
            std::string newDenStr = mantissa.getDenominator() + std::string(-n, '0');
            numerator = mantissa.numerator;
            denominator = BigInteger(newDenStr);
        }
        else {
            // 指数为 0：直接等于尾数
            numerator = mantissa.numerator;
            denominator = mantissa.denominator;
        }
    }

    // 辅助函数：检查是否为有效整数（允许符号）
    bool isValidInteger(const std::string& s) {
        if (s.empty()) return false;
        size_t start = 0;
        if (s[0] == '+' || s[0] == '-') {
            if (s.size() == 1) return false; // 只有符号不是数字
            start = 1;
        }
        return std::all_of(s.begin() + start, s.end(), ::isdigit);
    }

    // 辅助函数：解析小数（如 "123.456"）
    void parseDecimal(const std::string& str) {
        size_t dotPos = str.find('.');
        if (dotPos == std::string::npos) {
            // 情况1：整数（无小数点）
            numerator = BigInteger(str);       // 分子 = 原字符串
            denominator = BigInteger("1");     // 分母 = 1
            return;
        }

        // 情况2：小数
        std::string intPartStr = str.substr(0, dotPos);
        std::string fracPartStr = str.substr(dotPos + 1);

        // --- 核心修改：纯字符串拼接，零运算 ---
        // 分子字符串 = 整数部分 + 小数部分（直接拼接成一个整数字符串）
        std::string numStr = intPartStr + fracPartStr;
        
        // 分母字符串 = "1" + (fracPartStr.size() 个 '0')
        std::string denStr = "1" + std::string(fracPartStr.size(), '0');

        // --- 仅进行存储赋值 ---
        numerator = BigInteger(numStr);
        denominator = BigInteger(denStr);

        // 处理特殊情况：如果整数部分为空（如 ".123"），则分子就是小数部分
        if (intPartStr.empty() || (intPartStr == "-") || (intPartStr == "+")) {
            // 注意：符号已包含在 intPartStr 中，numStr 已经正确包含了符号
            // 此处无需额外操作，因为拼接后符号自然在头部
        }
    }

public:
    // 默认构造函数：0/1
    BigRational() : numerator("0"), denominator("1") {}

    // 从分子分母构造
    BigRational(const BigInteger& num, const BigInteger& den) {
        if (den.toString() == "0") {
            throw std::invalid_argument("分母不能为零");
        }
        numerator = num;
        denominator = den;
    }

    // 从字符串构造（核心逻辑）
    BigRational(const std::string& str) {
        if (str.empty()) {
            numerator = BigInteger("0");
            denominator = BigInteger("1");
            return;
        }

        // 1. 检查是否为分数形式 (a/b)
        size_t slashPos = str.find('/');
        if (slashPos != std::string::npos) {
            std::string numStr = str.substr(0, slashPos);
            std::string denStr = str.substr(slashPos + 1);

            // 基本验证
            if (numStr.empty() || denStr.empty()) {
                throw std::invalid_argument("分数格式错误：分子或分母为空");
            }

            numerator = BigInteger(numStr);
            denominator = BigInteger(denStr);

            if (denominator.toString() == "0") {
                throw std::invalid_argument("分母不能为零");
            }
            return;
        }

        // 2. 检查是否为科学计数法 (含有e/E)
        if (str.find_first_of("eE") != std::string::npos) {
            parseScientific(str);
            return;
        }

        // 3. 处理小数或整数
        parseDecimal(str);
    }

    // 拷贝构造函数
    BigRational(const BigRational& other) = default;

    // 赋值运算符
    BigRational& operator=(const BigRational& other) = default;

    // 获取分子字符串
    std::string getNumerator() const {
        return numerator.toString();
    }

    // 获取分母字符串
    std::string getDenominator() const {
        return denominator.toString();
    }

    // 转换为分数形式字符串（用于调试）
    std::string toString() const {
        return numerator.toString() + "/" + denominator.toString();
    }
};


#endif