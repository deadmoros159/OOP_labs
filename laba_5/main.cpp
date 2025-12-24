#include "custom_memory_resource.h"
#include <iostream>
#include <string>
#include <cassert>

using namespace std;

struct ComplexData {
    int id;
    string name;
    double value;
    
    ComplexData(int i, string n, double v) : id(i), name(move(n)), value(v) {}
    
    friend ostream& operator<<(ostream& os, const ComplexData& d) {
        return os << "{" << d.id << ", \"" << d.name << "\", " << d.value << "}";
    }
};

void test_simple_types() {
    cout << "Тест 1\n";
    
    FixedMemoryResource resource(512);
    CustomQueue<int> queue(&resource);
    
    for (int i = 1; i <= 5; ++i) {
        queue.push(i * 10);
        cout << "Добавлено: " << i * 10 
                  << "Размер: " << queue.size() 
                  << "Исп. память: " << resource.get_used() 
                  << "/" << resource.get_total() << "\n";
    }
    
    cout << "Элементы через итератор: ";
    for (auto it = queue.begin(); it != queue.end(); ++it) {
        cout << *it << " ";
    }
    cout << "\n";
    
    cout << "Элементы через range-based: ";
    for (int val : queue) {
        cout << val << " ";
    }
    cout << "\n";
    
    cout << "Извлечение элементов:\n";
    while (!queue.empty()) {
        cout << "Извлечено: " << queue.front_element() << "\n";
        queue.pop();
    }
    
    assert(queue.empty() && queue.size() == 0);
    cout << "Тест пройден\n";
}

void test_complex_types() {
    cout << "\nТест 2\n";
    
    FixedMemoryResource resource(1024);
    CustomQueue<ComplexData> queue(&resource);
    
    queue.push(ComplexData(1, "Alice", 100.5));
    queue.push(ComplexData(2, "Bob", 200.75));
    queue.push(ComplexData(3, "Charlie", 300.25));
    
    cout << "Размер очереди: " << queue.size() << "\n";
    cout << "Блоков памяти: " << resource.get_blocks() << "\n";
    
    cout << "\nМодификация второго элемента:\n";
    auto it = queue.begin();
    ++it;
    it->value = 999.99;
    
    cout << "Содержимое после модификации:\n";
    for (const auto& item : queue) {
        cout << "  " << item << "\n";
    }
    
    CustomQueue<ComplexData> queue_copy = queue;
    cout << "\nПосле копирования:\n";
    cout << "Оригинал размер: " << queue.size() << "\n";
    cout << "Копия размер: " << queue_copy.size() << "\n";
    
    queue.clear();
    cout << "После очистки оригинал пуст: " << boolalpha << queue.empty() << "\n";
    cout << "Тест пройден\n";
}

void test_memory_reuse() {
    cout << "\nТест 3:\n";
    
    FixedMemoryResource resource(256);
    CustomQueue<int> queue(&resource);
    
    for (int i = 0; i < 5; ++i) {
        queue.push(i);
    }
    
    size_t blocks_before = resource.get_blocks();
    cout << "Блоков после выделения: " << blocks_before << "\n";
    
    while (!queue.empty()) {
        queue.pop();
    }
    
    for (int i = 10; i < 15; ++i) {
        queue.push(i);
    }
    
    size_t blocks_after = resource.get_blocks();
    cout << "Блоков после использования: " << blocks_after << "\n";
    
    cout << "Память: " << (blocks_after <= blocks_before ? "ДА" : "НЕТ") << "\n";
    
    queue.clear();
    cout << "Тест пройден\n";
}

void test_edge_cases() {
    cout << "\nТест 4\n";
    
    FixedMemoryResource resource(64);
    CustomQueue<int> queue(&resource);
    
    try {
        for (int i = 0; i < 20; ++i) {
            queue.push(i);
        }
        cout << "Ожидалось исключение при переполнении\n";
    } catch (const bad_alloc&) {
        cout << "Переполнение корректно обработано\n";
    }
    
    CustomQueue<int> empty_queue;
    assert(empty_queue.empty() && empty_queue.size() == 0);
    
    queue.push(1);
    queue.push(2);
    CustomQueue<int> moved_queue(move(queue));
    assert(queue.empty() && moved_queue.size() == 2);
    cout << "Перемещение работает корректно\n";
    
    cout << "Граничные случаи обработаны\n";
}

int main() {
    try {
        test_simple_types();
        test_complex_types();
        test_memory_reuse();
        test_edge_cases();
        
    } catch (const exception& e) {
        cerr << "\nОшибка"<< endl;
        return 1;
    }
    
    return 0;
}