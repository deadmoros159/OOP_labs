#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <algorithm>
#include "Five.h"
using namespace std;
using ll = long long;
// #define DEBUG 1


class Five
{
private:
    vector<unsigned char> digits;
    bool isValidDigit(unsigned char digit) const
    {
        return digit >= 0 && digit <= 4;
    }
    bool isValidChar(char c) const
    {
        return c >= '0' && c <= '4';
    }
public:
    //Конструкторы
    Five() {};
    Five(const size_t &n, unsigned char t = 0)
    {
        if (!isValidDigit(t)) throw invalid_argument("Digit must be between 0 and 4_1");
        for (size_t i = 0; i < n; i++)
        {
            digits.push_back(t);
        }
        reverse(digits.begin(), digits.end());
        #ifdef DEBUG
        for (size_t i = 0; i < n; i++)
        {
            cout << digits[i] << endl;
        }
        #endif
    };

    Five(const initializer_list<unsigned char> &t)
    {
        for (auto it = t.end() - 1; it >= t.begin(); it--)
        {
            if (!isValidDigit(*it)) throw invalid_argument("Digit must be between 0 and 4_2");
            digits.push_back(*it);
            #ifdef DEBUG
            cout << (int)*it << endl;
            #endif
        }
    }
    Five(const string &t)
    {
        for (char c : t)
        {
            if (!isValidChar(c)) throw invalid_argument("Digit must be between 0 and 4_3");
            #ifdef DEBUG
            cout << c << endl;
            #endif
            digits.push_back(c - '0');
        }
        reverse(digits.begin(), digits.end());
    };

    Five(const Five &other)
    {
        for (int i = 0; i < other.digits.size(); i++)
        {
            this->digits.push_back(other.digits[i]);
        }
        #ifdef DEBUG
        for (int i = 0; i < this->digits.size(); i++)
        {
            cout << (int)this->digits[i] << endl;
        }
        #endif
    };
    Five(Five &&other) noexcept
    {
        for (int i = 0; i < other.digits.size(); i++)
        {
            this->digits.push_back(other.digits[i]);
        }
    };

    virtual ~Five() noexcept
    {
        #ifdef DEBUG
        cout << "Вызван деструктор" << endl;
        #endif
    };


    Five ReturnAdress()
    {
        return Five(*this);
    }
    void PrintData()
    {
        if (!digits.size()) cout << "no data found \n";
        else
        {
            for (int i = 0; i < digits.size(); i++)
            {
                cout << (int)digits[i];
            }
            cout << '\n';
        }
    }
    //Функции
    string toString() const
    {
        if(this->digits.empty()) return "0";

        string result;
        for (int i = digits.size() - 1; i >= 0; --i) 
        {
            result += ('0' + digits[i]);
        }
        return result;
    }
    Five add(const Five &other) const
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
        reverse(result.begin(), result.end());
        newNumber.digits = result;
        return newNumber;
    }
    int GraterThan(const Five &other) const
    {
        if (this->digits.size() > other.digits.size())
        {
            return 1;
        }
        if (this->digits.size() < other.digits.size())
        {
            return 0;
        }

        int flag = 1;
        for (int i = this->digits.size() - 1; i != 0; i--)
        {
            if(this->digits[i] < other.digits[i])
            {
                flag = 0;
                break;
            }
        }
        return flag;
        
    }
    int Equals(const Five &other) const
    {
        if (this->digits.size() != other.digits.size()) return 0;
        int flag = 1;
        for (int i = this->digits.size() - 1; i != 0; i--)
        {
            if (this->digits[i] != other.digits[i]) flag = 0;
        }
        return flag;
    }
    int LowerThan(const Five &other) const
    {
        if (this->digits.size() < other.digits.size())
        {
            return 1;
        }
        if (this->digits.size() > other.digits.size())
        {
            return 0;
        }

        int flag = 1;
        for (int i = this->digits.size() - 1; i != 0; i--)
        {
            if(this->digits[i] > other.digits[i])
            {
                flag = 0;
                break;
            }
        }
        return flag;
    }
    Five Substract(const Five &other) const
    {
        if (LowerThan(other))
        {
            throw invalid_argument("Cant sub smaller from larger");
        }
        Five result;
        Five temp = move(other);
        #ifdef DEBUG
        cout << "Data from temp" << endl;
        temp.PrintData();
        #endif


        while (temp.digits.size() != this->digits.size()) temp.digits.push_back(0);
        #ifdef DEBUG
        cout << "temp data after 0" << endl;
        temp.PrintData();
        #endif
        
        int zapas = 5;
        bool vichet = false; //true
        // Первая итерация
        if (this->digits[0] < temp.digits[0])
        {
            vichet = true;
            result.digits.push_back(5 + this->digits[0] - temp.digits[0]);
        }
        else result.digits.push_back(this->digits[0] - temp.digits[0]);

        //Цикл for
        for (int i = 1; i < this->digits.size(); i++)
        {
            if (this->digits[i] < temp.digits[i])
            {
                if (vichet) result.digits.push_back((zapas + this->digits[i] - temp.digits[i]) - 1);
                else result.digits.push_back((zapas + this->digits[i] - temp.digits[i]));
                vichet = true;

            }
            else
            {
                if ((this->digits[i] - temp.digits[i] - 1) == 0 && i == this->digits.size())
                {
                    cout << "exit on" << i << endl;
                    continue;
                }
                else if (vichet) result.digits.push_back(this->digits[i] - temp.digits[i] - 1);
                else result.digits.push_back(this->digits[i] - temp.digits[i]);
                vichet = false;
            }
        }
        reverse(result.digits.begin(), result.digits.end());
        return result;

    }

};



int main()
{
    Five a(5, 2);
    cout << "Конструктор Five(const size_t &n, unsigned char t = 0):   ";
    a.PrintData();
    Five b{1, 2, 3};
    cout << "Конструктор Five(const initializer_list<unsigned char> &t):   ";
    b.PrintData();
    Five c("1234");
    cout << "Конструктор Five(const string &t):   ";
    c.PrintData();
    Five d_1{1, 2};
    cout << "Конструктор Five(const Five &other):   ";
    Five d_2 = d_1.ReturnAdress();
    d_2.PrintData();
    Five e = move(c);
    cout << "Конструктор Five(const Five &&other):   ";
    e.PrintData();
    c.PrintData();
    //Проверка функций
    Five sum_1{2, 3};
    Five sum_2{2, 2};
    Five sum = sum_1.add(sum_2);
    cout << "Try to func add " << sum_1.toString() << " + " << sum_2.toString() << endl;
    cout << "Result for add:   ";
    sum.PrintData();

    // Test < > =
    Five g_1("1234");
    Five g_2("123");
    if (g_2.LowerThan(g_1)) cout << "First number is lower than second" << endl;
    else if (g_2.Equals(g_1)) cout << "First and second are equals" << endl;
    else cout << "First number is higher than the second" << endl;
    // Test '-'
    cout << "Try to func substruct" << endl;
    Five sub_1("324");
    Five sub_2("124");
    Five sub = sub_1.Substract(sub_2);
    cout << "Result for substruct:   " << endl;
    sub.PrintData();
    return 0;
}

