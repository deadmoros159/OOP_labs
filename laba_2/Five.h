#ifndef FIVE_H
#define FIVE_H

#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>

class Five {
private:
    std::vector<unsigned char> digits;
    bool isValidDigit(unsigned char digit) const;
    bool isValidChar(char c) const;

public:
    Five();
    Five(const size_t &n, unsigned char t = 0);
    Five(const std::initializer_list<unsigned char> &t);
    Five(const std::string &t);
    Five(const Five &other);
    Five(Five &&other) noexcept;
    virtual ~Five() noexcept;

    Five ReturnAdress();
    void PrintData();
    std::string toString() const;
    Five add(const Five &other) const;
    int GraterThan(const Five &other) const;
    int Equals(const Five &other) const;
    int LowerThan(const Five &other) const;
    Five Substract(const Five &other) const;
};

#endif