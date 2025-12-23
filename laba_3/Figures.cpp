using namespace std;
#include "Figures.h"


ostream& operator<<(ostream& os, const Figure& fig) {
    fig.print(os);
    return os;
}

istream& operator>>(istream& is, Figure& fig) {
    fig.read(is);
    return is;
}

double operator+(const Figure& fig) {
    return fig.area();
}

Octagon::Octagon() : side(0) { calculateVertices(); }
Octagon::Octagon(double s) : side(s) { calculateVertices(); }
Octagon::Octagon(const Octagon& other) : side(other.side) { calculateVertices(); }
Octagon::Octagon(Octagon&& other) noexcept : side(other.side), vertices(move(other.vertices)) {
    other.side = 0;
}
Octagon& Octagon::operator=(const Octagon& other) {
    if (this != &other) {
        side = other.side;
        calculateVertices();
    }
    return *this;
}
Octagon& Octagon::operator=(Octagon&& other) noexcept {
    if (this != &other) {
        side = other.side;
        vertices = move(other.vertices);
        other.side = 0;
    }
    return *this;
}
bool Octagon::operator==(const Octagon& other) const {
    return abs(side - other.side) < 1e-9;
}
void Octagon::calculateVertices() {
    vertices.clear();
    const double PI = 3.14159265358979323846;
    double R = side / (2 * sin(PI / 8));
    for (int i = 0; i < 8; ++i) {
        double angle = i * PI / 4;
        vertices.emplace_back(R * cos(angle), R * sin(angle));
    }
}
double Octagon::area() const {
    return 2 * side * side * (1 + sqrt(2));
}
pair<double, double> Octagon::center() const {
    return {0.0, 0.0};
}
void Octagon::print(ostream& os) const {
    os << "Octagon with side = " << side << ", vertices: ";
    for (const auto& v : vertices) {
        os << "(" << v.first << ", " << v.second << ") ";
    }
}
void Octagon::read(istream& is) {
    is >> side;
    if (side <= 0) throw invalid_argument("Octagon side must be positive");
    calculateVertices();
}
unique_ptr<Figure> Octagon::clone() const {
    return make_unique<Octagon>(*this);
}

Triangle::Triangle() : side(0) { calculateVertices(); }
Triangle::Triangle(double s) : side(s) { calculateVertices(); }
Triangle::Triangle(const Triangle& other) : side(other.side) { calculateVertices(); }
Triangle::Triangle(Triangle&& other) noexcept : side(other.side), vertices(move(other.vertices)) {
    other.side = 0;
}
Triangle& Triangle::operator=(const Triangle& other) {
    if (this != &other) {
        side = other.side;
        calculateVertices();
    }
    return *this;
}
Triangle& Triangle::operator=(Triangle&& other) noexcept {
    if (this != &other) {
        side = other.side;
        vertices = move(other.vertices);
        other.side = 0;
    }
    return *this;
}
bool Triangle::operator==(const Triangle& other) const {
    return abs(side - other.side) < 1e-9;
}
void Triangle::calculateVertices() {
    vertices.clear();
    double R = side / sqrt(3);
    for (int i = 0; i < 3; ++i) {
        double angle = 2 * M_PI * i / 3;
        vertices.emplace_back(R * cos(angle), R * sin(angle));
    }
}
double Triangle::area() const {
    return (sqrt(3) / 4) * side * side;
}
pair<double, double> Triangle::center() const {
    return {0.0, 0.0};
}
void Triangle::print(ostream& os) const {
    os << "Triangle with side = " << side << ", vertices: ";
    for (const auto& v : vertices) {
        os << "(" << v.first << ", " << v.second << ") ";
    }
}
void Triangle::read(istream& is) {
    is >> side;
    if (side <= 0) throw invalid_argument("Triangle side must be positive");
    calculateVertices();
}
unique_ptr<Figure> Triangle::clone() const {
    return make_unique<Triangle>(*this);
}

Square::Square() : side(0) { calculateVertices(); }
Square::Square(double s) : side(s) { calculateVertices(); }
Square::Square(const Square& other) : side(other.side) { calculateVertices(); }
Square::Square(Square&& other) noexcept : side(other.side), vertices(move(other.vertices)) {
    other.side = 0;
}
Square& Square::operator=(const Square& other) {
    if (this != &other) {
        side = other.side;
        calculateVertices();
    }
    return *this;
}
Square& Square::operator=(Square&& other) noexcept {
    if (this != &other) {
        side = other.side;
        vertices = move(other.vertices);
        other.side = 0;
    }
    return *this;
}
bool Square::operator==(const Square& other) const {
    return abs(side - other.side) < 1e-9;
}
void Square::calculateVertices() {
    vertices.clear();
    double R = side / sqrt(2);
    for (int i = 0; i < 4; ++i) {
        double angle = M_PI_4 + i * M_PI_2;
        vertices.emplace_back(R * cos(angle), R * sin(angle));
    }
}
double Square::area() const {
    return side * side;
}
pair<double, double> Square::center() const {
    return {0.0, 0.0};
}
void Square::print(ostream& os) const {
    os << "Square with side = " << side << ", vertices: ";
    for (const auto& v : vertices) {
        os << "(" << v.first << ", " << v.second << ") ";
    }
}
void Square::read(istream& is) {
    is >> side;
    if (side <= 0) throw invalid_argument("Square side must be positive");
    calculateVertices();
}
unique_ptr<Figure> Square::clone() const {
    return make_unique<Square>(*this);
}

Array::Array() : capacity(10), size(0) {
    figures = make_unique<unique_ptr<Figure>[]>(capacity);
}
Array::~Array() = default;
void Array::resize(size_t new_capacity) {
    auto new_figures = make_unique<unique_ptr<Figure>[]>(new_capacity);
    for (size_t i = 0; i < size; ++i) {
        new_figures[i] = move(figures[i]);
    }
    figures = move(new_figures);
    capacity = new_capacity;
}
void Array::add(unique_ptr<Figure> fig) {
    if (size >= capacity) {
        resize(capacity * 2);
    }
    figures[size++] = move(fig);
}
void Array::remove(size_t index) {
    if (index >= size) return;
    for (size_t i = index; i < size - 1; ++i) {
        figures[i] = move(figures[i + 1]);
    }
    figures[size - 1].reset();
    --size;
}
double Array::totalArea() const {
    double total = 0;
    for (size_t i = 0; i < size; ++i) {
        total += figures[i]->area();
    }
    return total;
}
void Array::printAll() const {
    for (size_t i = 0; i < size; ++i) {
        cout << *figures[i] << endl;
        auto c = figures[i]->center();
        cout << "Center: (" << c.first << ", " << c.second << ")" << endl;
        cout << "Area: " << figures[i]->area() << endl;
        cout << "\n";
    }
}
size_t Array::getSize() const {
    return size;
}
Figure* Array::operator[](size_t index) const {
    if (index >= size) throw out_of_range("Index out of range");
    return figures[index].get();
}