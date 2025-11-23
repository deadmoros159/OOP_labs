#include "Five.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;


bool Five::isValidDigit(unsigned char digit) const
{
    return digit >= 0 && digit <= 4;
}

bool Five::isValidChar(char c) const
{
    return c >= '0' && c <= '4';
}

// Конструкторы
Five::Five() {};

Five::Five(const size_t &n, unsigned char t)
{
    if (!isValidDigit(t)) throw invalid_argument("Digit must be between 0 and 4_1");
    for (size_t i = 0; i < n; i++)
    {
        digits.push_back(t);
    }
    reverse(digits.begin(), digits.end());
}

Five::Five(const initializer_list<unsigned char> &t)
{
    for (auto it = t.end() - 1; it >= t.begin(); it--)
    {
        if (!isValidDigit(*it)) throw invalid_argument("Digit must be between 0 and 4_2");
        digits.push_back(*it);
    }
}

Five::Five(const string &t)
{
    for (char c : t)
    {
        if (!isValidChar(c)) throw invalid_argument("Digit must be between 0 and 4_3");
        digits.push_back(c - '0');
    }
    reverse(digits.begin(), digits.end());
}

Five::Five(const Five &other)
{
    for (size_t i = 0; i < other.digits.size(); i++)
    {
        this->digits.push_back(other.digits[i]);
    }
}

Five::Five(Five &&other) noexcept
{
    this->digits = std::move(other.digits);
}

Five::~Five() noexcept {}

// Функции 
Five Five::ReturnAdress()
{
    return Five(*this);
}

void Five::PrintData()
{
    if (!digits.size()) cout << "no data found \n";
    else
    {
        for (int i = digits.size() - 1; i >= 0; i--)
        {
            cout << (int)digits[i];
        }
        cout << '\n';
    }
}

string Five::toString() const
{
    if(this->digits.size() == 0) return "0";

    string result;
    for (int i = digits.size() - 1; i >= 0; --i) 
    {
        result += ('0' + digits[i]);
    }
    return result;
}

Five Five::add(const Five &other) const
{
    vector<unsigned char> result;
    size_t maxSize = max(this->digits.size(), other.digits.size());
    unsigned char carry = 0;
    for (size_t i = 0; i < maxSize || carry; i++)
    {
        unsigned char sum = carry;
    
        if (i < this->digits.size()) 
            sum += this->digits[i];
        if (i < other.digits.size()) 
            sum += other.digits[i];
            
        carry = sum / 5;
        result.push_back(sum % 5);
    }
    Five newNumber;
    newNumber.digits = result;
    return newNumber;
}

int Five::GraterThan(const Five &other) const
{
    if (this->digits.size() > other.digits.size())
    {
        return 1;
    }
    if (this->digits.size() < other.digits.size())
    {
        return 0;
    }

    for (int i = this->digits.size() - 1; i >= 0; i--)
    {
        if (this->digits[i] > other.digits[i])
        {
            return 1;
        }
        else if (this->digits[i] < other.digits[i])
        {
            return 0;
        }
    }
    return 0;
}

int Five::Equals(const Five &other) const
{
    if (this->digits.size() != other.digits.size()) return 0;
    
    for (size_t i = 0; i < this->digits.size(); i++)
    {
        if (this->digits[i] != other.digits[i]) 
            return 0;
    }
    return 1;
}

int Five::LowerThan(const Five &other) const
{
    if (this->digits.size() < other.digits.size())
    {
        return 1;
    }
    if (this->digits.size() > other.digits.size())
    {
        return 0;
    }

    for (int i = this->digits.size() - 1; i >= 0; i--)
    {
        if (this->digits[i] < other.digits[i])
        {
            return 1;
        }
        else if (this->digits[i] > other.digits[i])
        {
            return 0;
        }
    }
    return 0;
}

Five Five::Substract(const Five &other) const
{
    if (LowerThan(other))
    {
        throw invalid_argument("Cant sub smaller from larger");
    }
    
    if (Equals(other))
    {
        return Five();
    }

    Five result;
    vector<unsigned char> temp_1 = this->digits;
    vector<unsigned char> temp_2 = other.digits;

    while (temp_2.size() < temp_1.size()) 
    {
        temp_2.push_back(0);
    }
    
    int vichet = 0;
    for (size_t i = 0; i < temp_1.size(); i++)
    {
        int diff = temp_1[i] - vichet - temp_2[i];
        
        if (diff < 0)
        {
            diff += 5;
            vichet = 1;
        }
        else
        {
            vichet = 0;
        }
        
        result.digits.push_back(diff);
    }

    while (result.digits.size() > 1 && result.digits.back() == 0)
    {
        result.digits.pop_back();
    }
    
    return result;
}