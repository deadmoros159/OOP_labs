#ifndef FIGURES_H
#define FIGURES_H

#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <stdexcept>


class Figure {
public:
    virtual ~Figure() = default;
    virtual double area() const = 0;
    virtual std::pair<double, double> center() const = 0;
    virtual void print(std::ostream& os) const = 0;
    virtual void read(std::istream& is) = 0;
    virtual std::unique_ptr<Figure> clone() const = 0;
    virtual double getSide() const = 0;
};

std::ostream& operator<<(std::ostream& os, const Figure& fig);
std::istream& operator>>(std::istream& is, Figure& fig);
double operator+(const Figure& fig);

class Octagon : public Figure {
private:
    double side;
    std::vector<std::pair<double, double>> vertices;
    void calculateVertices();
    
public:
    Octagon();
    Octagon(double s);
    Octagon(const Octagon& other);
    Octagon(Octagon&& other) noexcept;
    Octagon& operator=(const Octagon& other);
    Octagon& operator=(Octagon&& other) noexcept;
    bool operator==(const Octagon& other) const;
    
    double area() const override;
    std::pair<double, double> center() const override;
    void print(std::ostream& os) const override;
    void read(std::istream& is) override;
    std::unique_ptr<Figure> clone() const override;
    double getSide() const override { return side; }
};

class Triangle : public Figure {
private:
    double side;
    std::vector<std::pair<double, double>> vertices;
    void calculateVertices();
    
public:
    Triangle();
    Triangle(double s);
    Triangle(const Triangle& other);
    Triangle(Triangle&& other) noexcept;
    Triangle& operator=(const Triangle& other);
    Triangle& operator=(Triangle&& other) noexcept;
    bool operator==(const Triangle& other) const;
    
    double area() const override;
    std::pair<double, double> center() const override;
    void print(std::ostream& os) const override;
    void read(std::istream& is) override;
    std::unique_ptr<Figure> clone() const override;
    double getSide() const override { return side; }
};

class Square : public Figure {
private:
    double side;
    std::vector<std::pair<double, double>> vertices;
    void calculateVertices();
    
public:
    Square();
    Square(double s);
    Square(const Square& other);
    Square(Square&& other) noexcept;
    Square& operator=(const Square& other);
    Square& operator=(Square&& other) noexcept;
    bool operator==(const Square& other) const;
    
    double area() const override;
    std::pair<double, double> center() const override;
    void print(std::ostream& os) const override;
    void read(std::istream& is) override;
    std::unique_ptr<Figure> clone() const override;
    double getSide() const override { return side; }
};

class Array {
private:
    std::unique_ptr<std::unique_ptr<Figure>[]> figures;
    size_t capacity;
    size_t size;
    
    void resize(size_t new_capacity);
    
public:
    Array();
    ~Array();
    void add(std::unique_ptr<Figure> fig);
    void remove(size_t index);
    double totalArea() const;
    void printAll() const;
    size_t getSize() const;
    Figure* operator[](size_t index) const;
};

#endif 