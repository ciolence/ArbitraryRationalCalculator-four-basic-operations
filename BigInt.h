#ifndef BIGINT_H
#define BIGINT_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

#include "Storage.h"

typedef long long ll;
typedef unsigned long long ull;

class BigInt {
private:
    static const ull BASE = 1e9;     // 一位储存9个数字
    static const int BASE_DIGITS = 9;

    DigitNode* head;        // 链表存储（小端序，每个节点一个数字0-9）
    bool is_negative;       // 符号位，true表示负

    // 释放链表
    void clear_linked() {
        DigitNode* cur = head;
        while (cur) {
            DigitNode* next = cur->next;
            delete cur;
            cur = next;
        }
        head = nullptr;
    }

    // 去除高位多余的零（从链表尾部删除值为0的节点）
    void trim_linked_zeros() {
        if (!head) return;

        // 找最后一个非零节点
        DigitNode* last_nonzero = nullptr;
        DigitNode* cur = head;
        while (cur) {
            if (cur->digit != 0) {
                last_nonzero = cur;
            }
            cur = cur->next;
        }

        if (last_nonzero) {
            // 删除 last_nonzero 之后的节点
            DigitNode* to_del = last_nonzero->next;
            last_nonzero->next = nullptr;
            while (to_del) {
                DigitNode* next = to_del->next;
                delete to_del;
                to_del = next;
            }
        } else {
            // 全是零 → 保留一个0节点
            clear_linked();
            head = new DigitNode(0);
            is_negative = false;
        }

        if (head && !head->next && head->digit == 0) {
            is_negative = false;
        }
    }

    // 链表 → vector<ull>（BASE=1e9 表示）
    std::vector<ull> to_vec() const {
        std::vector<ull> result;
        if (!head) {
            result.push_back(0);
            return result;
        }

        // 统计节点数
        int count = 0;
        DigitNode* cur = head;
        while (cur) {
            ++count;
            cur = cur->next;
        }

        // 每9个数字一组，合并为一个ull
        int vec_size = (count + BASE_DIGITS - 1) / BASE_DIGITS;
        result.assign(vec_size, 0);

        cur = head;
        ull pow10[9] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};
        for (int i = 0; i < count && cur; ++i) {
            int idx = i / BASE_DIGITS;
            int pos = i % BASE_DIGITS;
            result[idx] += (ull)(cur->digit) * pow10[pos];
            cur = cur->next;
        }

        // 去除高位零（vector尾部）
        while (result.size() > 1 && result.back() == 0) {
            result.pop_back();
        }

        return result;
    }

    // vector<ull>（BASE=1e9）→ 链表
    void from_vec(const std::vector<ull>& vec) {
        clear_linked();
        if (vec.empty()) {
            head = new DigitNode(0);
            return;
        }

        DigitNode* tail = nullptr;
        for (ull elem : vec) {
            ull v = elem;
            for (int j = 0; j < BASE_DIGITS; ++j) {
                DigitNode* node = new DigitNode(v % 10);
                if (!head) {
                    head = tail = node;
                } else {
                    tail->next = node;
                    tail = node;
                }
                v /= 10;
            }
        }

        trim_linked_zeros();
    }

    // 比较绝对值大小（通过临时vector）：1表示this>other, 0表示相等, -1表示this<other
    int compare_abs(const BigInt& other) const {
        auto v1 = to_vec();
        auto v2 = other.to_vec();

        if (v1.size() != v2.size()) {
            return v1.size() > v2.size() ? 1 : -1;
        }
        for (int i = v1.size() - 1; i >= 0; --i) {
            if (v1[i] != v2[i]) {
                return v1[i] > v2[i] ? 1 : -1;
            }
        }
        return 0;
    }

    // 无符号加法（在vector层操作）
    static std::vector<ull> unsigned_add_vec(const std::vector<ull>& a, const std::vector<ull>& b) {
        std::vector<ull> result;
        int carry = 0;
        size_t max_size = std::max(a.size(), b.size());

        for (size_t i = 0; i < max_size || carry; ++i) {
            ull sum = carry;
            if (i < a.size()) sum += a[i];
            if (i < b.size()) sum += b[i];

            result.push_back(sum % BASE);
            carry = sum / BASE;
        }

        return result;
    }

    // 无符号减法（在vector层操作，假设a >= b）
    static std::vector<ull> unsigned_subtract_vec(const std::vector<ull>& a, const std::vector<ull>& b) {
        std::vector<ull> result;
        int borrow = 0;

        for (size_t i = 0; i < a.size(); ++i) {
            ll diff = a[i] - borrow;
            if (i < b.size()) {
                diff -= b[i];
            }

            if (diff < 0) {
                borrow = 1;
                diff += BASE;
            } else {
                borrow = 0;
            }

            result.push_back(diff);
        }

        // 去除前导零
        while (result.size() > 1 && result.back() == 0) {
            result.pop_back();
        }
        if (result.empty()) result.push_back(0);

        return result;
    }

public:
    // 默认构造函数
    BigInt() : head(nullptr), is_negative(false) {
        head = new DigitNode(0);
    }

    // 从整数构造
    BigInt(ll num) : head(nullptr), is_negative(false) {
        is_negative = (num < 0);
        num = std::abs(num);

        if (num == 0) {
            head = new DigitNode(0);
        } else {
            DigitNode* tail = nullptr;
            while (num > 0) {
                DigitNode* node = new DigitNode(num % 10);
                if (!head) {
                    head = tail = node;
                } else {
                    tail->next = node;
                    tail = node;
                }
                num /= 10;
            }
        }
    }

    // 从字符串构造
    BigInt(const std::string& s) : head(nullptr), is_negative(false) {
        if (s.empty()) {
            head = new DigitNode(0);
            return;
        }

        ll start = 0;

        if (s[0] == '-') {
            is_negative = true;
            start = 1;
        } else if (s[0] == '+') {
            is_negative = false;
            start = 1;
        } else {
            is_negative = false;
        }

        // 跳过前导零
        while (start < s.size() && s[start] == '0') {
            ++start;
        }

        if (start == s.size()) {
            head = new DigitNode(0);
            is_negative = false;
            return;
        }

        // 直接用链表存储数字（小端序）
        DigitNode* tail = nullptr;
        for (ll i = s.size() - 1; i >= start; --i) {
            if (std::isdigit(static_cast<unsigned char>(s[i]))) {
                DigitNode* node = new DigitNode(s[i] - '0');
                if (!head) {
                    head = tail = node;
                } else {
                    tail->next = node;
                    tail = node;
                }
            }
        }
    }

    // 拷贝构造函数
    BigInt(const BigInt& other) : head(nullptr), is_negative(other.is_negative) {
        if (other.head) {
            DigitNode* tail = nullptr;
            DigitNode* cur = other.head;
            while (cur) {
                DigitNode* node = new DigitNode(cur->digit);
                if (!head) {
                    head = tail = node;
                } else {
                    tail->next = node;
                    tail = node;
                }
                cur = cur->next;
            }
        }
    }

    // 析构函数
    ~BigInt() {
        clear_linked();
    }

    // 赋值运算符
    BigInt& operator=(const BigInt& other) {
        if (this == &other) return *this;

        clear_linked();
        is_negative = other.is_negative;

        if (other.head) {
            DigitNode* tail = nullptr;
            DigitNode* cur = other.head;
            while (cur) {
                DigitNode* node = new DigitNode(cur->digit);
                if (!head) {
                    head = tail = node;
                } else {
                    tail->next = node;
                    tail = node;
                }
                cur = cur->next;
            }
        }

        return *this;
    }

    // 一元运算符
    BigInt operator-() const {
        BigInt result = *this;
        if (result != BigInt(0)) {
            result.is_negative = !result.is_negative;
        }
        return result;
    }

    // 比较运算符
    bool operator==(const BigInt& other) const {
        if (is_negative != other.is_negative) return false;

        DigitNode* a = head;
        DigitNode* b = other.head;
        while (a && b) {
            if (a->digit != b->digit) return false;
            a = a->next;
            b = b->next;
        }
        return a == nullptr && b == nullptr;
    }

    bool operator!=(const BigInt& other) const {
        return !(*this == other);
    }

    bool operator<(const BigInt& other) const {
        if (is_negative != other.is_negative) {
            return is_negative;
        }

        if (is_negative) {
            return compare_abs(other) > 0;
        }
        return compare_abs(other) < 0;
    }

    bool operator<=(const BigInt& other) const {
        return *this < other || *this == other;
    }

    bool operator>(const BigInt& other) const {
        return !(*this <= other);
    }

    bool operator>=(const BigInt& other) const {
        return !(*this < other);
    }

    // 算术运算符
    BigInt operator+(const BigInt& other) const {
        auto v1 = to_vec();
        auto v2 = other.to_vec();

        if (is_negative == other.is_negative) {
            BigInt result;
            result.from_vec(unsigned_add_vec(v1, v2));
            result.is_negative = is_negative;
            return result;
        }

        int cmp = compare_abs(other);
        if (cmp == 0) return BigInt(0);

        if (cmp > 0) {
            BigInt result;
            result.from_vec(unsigned_subtract_vec(v1, v2));
            result.is_negative = is_negative;
            return result;
        }
        BigInt result;
        result.from_vec(unsigned_subtract_vec(v2, v1));
        result.is_negative = other.is_negative;
        return result;
    }

    BigInt operator-(const BigInt& other) const {
        return *this + (-other);
    }

    BigInt operator*(const BigInt& other) const {
        auto v1 = to_vec();
        auto v2 = other.to_vec();

        if ((v1.size() == 1 && v1[0] == 0) || (v2.size() == 1 && v2[0] == 0)) {
            return BigInt(0);
        }

        std::vector<ull> result_vec(v1.size() + v2.size(), 0);

        for (ull i = 0; i < v1.size(); ++i) {
            ull carry = 0;
            for (ull j = 0; j < v2.size() || carry; ++j) {
                ull product = result_vec[i + j] +
                                v1[i] *
                                (j < v2.size() ? v2[j] : 0) +
                                carry;
                result_vec[i + j] = product % BASE;
                carry = product / BASE;
            }
        }

        BigInt result;
        result.from_vec(result_vec);
        result.is_negative = (is_negative != other.is_negative);
        return result;
    }

    BigInt operator/(const BigInt& other) const {
        if (other == BigInt(0)) {
            throw std::runtime_error("BigInt: division by zero");
        }

        auto v_this = to_vec();

        BigInt abs_other = other.abs();
        BigInt current;
        std::vector<ull> v_result(v_this.size(), 0);

        for (ll i = v_this.size() - 1; i >= 0; --i) {
            current = current * BigInt(BASE) + BigInt((ll)v_this[i]);

            ll quotient = 0;
            ll left = 0, right = BASE - 1;
            while (left <= right) {
                ll mid = (left + right) / 2;
                if (abs_other * BigInt(mid) <= current) {
                    quotient = mid;
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }

            v_result[i] = quotient;
            current = current - abs_other * BigInt(quotient);
        }

        BigInt result;
        result.from_vec(v_result);
        result.is_negative = (is_negative != other.is_negative);
        return result;
    }

    BigInt operator%(const BigInt& other) const {
        return *this - (*this / other) * other;
    }

    // 复合赋值运算符
    BigInt& operator+=(const BigInt& other) {
        *this = *this + other;
        return *this;
    }

    BigInt& operator-=(const BigInt& other) {
        *this = *this - other;
        return *this;
    }

    BigInt& operator*=(const BigInt& other) {
        *this = *this * other;
        return *this;
    }

    BigInt& operator/=(const BigInt& other) {
        *this = *this / other;
        return *this;
    }

    BigInt& operator%=(const BigInt& other) {
        *this = *this % other;
        return *this;
    }

    // 转为字符串
    std::string toString() const {
        auto v = to_vec();
        std::string result;
        if (is_negative) {
            result += '-';
        }
        for (ll i = v.size() - 1; i >= 0; --i) {
            result += std::to_string(v[i]);
        }
        return result;
    }

    BigInt abs() const {
        return (is_negative ? -*this : *this);
    }
};

#endif
