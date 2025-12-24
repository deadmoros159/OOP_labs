#include "custom_memory_resource.h"
#include <gtest/gtest.h>
#include <string>

// Тест 1: Базовая функциональность очереди с простыми типами
TEST(CustomQueueTest, BasicFunctionalityWithSimpleTypes) {
    FixedMemoryResource resource(512);
    CustomQueue<int> queue(&resource);
    
    EXPECT_TRUE(queue.empty());
    EXPECT_EQ(queue.size(), 0);
    
    queue.push(10);
    queue.push(20);
    queue.push(30);
    
    EXPECT_FALSE(queue.empty());
    EXPECT_EQ(queue.size(), 3);
    EXPECT_EQ(queue.front_element(), 10);
    
    queue.pop();
    EXPECT_EQ(queue.size(), 2);
    EXPECT_EQ(queue.front_element(), 20);
    
    queue.pop();
    queue.pop();
    EXPECT_TRUE(queue.empty());
}

// Тест 2: Работа со сложными типами данных
TEST(CustomQueueTest, ComplexTypesAndIterators) {
    struct ComplexType {
        int id;
        std::string name;
        double value;
        
        bool operator==(const ComplexType& other) const {
            return id == other.id && name == other.name && value == other.value;
        }
    };
    
    FixedMemoryResource resource(1024);
    CustomQueue<ComplexType> queue(&resource);
    
    queue.push(ComplexType{1, "Alice", 100.5});
    queue.push(ComplexType{2, "Bob", 200.75});
    queue.push(ComplexType{3, "Charlie", 300.25});
    
    EXPECT_EQ(queue.size(), 3);
    
    // Проверка итераторов
    auto it = queue.begin();
    EXPECT_EQ(it->id, 1);
    EXPECT_EQ(it->name, "Alice");
    EXPECT_EQ(it->value, 100.5);
    
    ++it;
    EXPECT_EQ(it->id, 2);
    
    // Range-based for loop
    int count = 0;
    for (const auto& item : queue) {
        EXPECT_TRUE(item.id >= 1 && item.id <= 3);
        count++;
    }
    EXPECT_EQ(count, 3);
}



// Тест 3: Копирование и перемещение
TEST(CustomQueueTest, CopyAndMoveSemantics) {
    FixedMemoryResource resource(512);
    CustomQueue<std::string> queue1(&resource);
    
    queue1.push("Hello");
    queue1.push("World");
    queue1.push("Test");
    
    // Тест копирования
    CustomQueue<std::string> queue2 = queue1;
    EXPECT_EQ(queue1.size(), 3);
    EXPECT_EQ(queue2.size(), 3);
    
    // Изменение копии не должно влиять на оригинал
    queue2.pop();
    queue2.push("Copied");
    EXPECT_EQ(queue1.size(), 3);
    EXPECT_EQ(queue2.size(), 3);
    EXPECT_EQ(queue1.front_element(), "Hello");
    EXPECT_EQ(queue2.front_element(), "World");
    
    // Тест перемещения
    CustomQueue<std::string> queue3(std::move(queue1));
    EXPECT_EQ(queue1.size(), 0);
    EXPECT_TRUE(queue1.empty());
    EXPECT_EQ(queue3.size(), 3);
    EXPECT_EQ(queue3.front_element(), "Hello");
}

// Тест 4: Очистка и деструкторы (требование из задания)
TEST(CustomQueueTest, CleanupOnDestruction) {
    size_t initial_blocks = 0;
    
    {
        FixedMemoryResource resource(256);
        CustomQueue<std::string> queue(&resource);
        
        queue.push("First");
        queue.push("Second");
        queue.push("Third");
        
        initial_blocks = resource.get_blocks();
        EXPECT_GT(initial_blocks, 0);
        
        // Явная очистка
        queue.clear();
        EXPECT_TRUE(queue.empty());
        EXPECT_EQ(queue.size(), 0);
        
        // Проверяем, что память все еще зарезервирована
        EXPECT_EQ(resource.get_blocks(), initial_blocks);
        
        // Деструктор queue вызовет do_deallocate для оставшихся блоков
    }
    
    // После выхода из scope resource уничтожается и очищает всю память
    // (проверяется отсутствие утечек через valgrind/sanitizers)
}

// Тест 5: Forward iterator требования
TEST(CustomQueueTest, ForwardIteratorRequirements) {
    FixedMemoryResource resource(256);
    CustomQueue<int> queue(&resource);
    
    for (int i = 1; i <= 5; ++i) {
        queue.push(i * 10);
    }
    
    // Проверка категории итератора
    using Iterator = CustomQueue<int>::iterator;
    
    // Должен быть forward iterator
    EXPECT_TRUE((std::is_same_v<
        std::iterator_traits<Iterator>::iterator_category,
        std::forward_iterator_tag>));
    
    // Проверка инкремента
    Iterator it = queue.begin();
    EXPECT_EQ(*it, 10);
    
    ++it; // Префиксный
    EXPECT_EQ(*it, 20);
    
    Iterator it2 = it++; // Постфиксный
    EXPECT_EQ(*it2, 20);
    EXPECT_EQ(*it, 30);
    
    // Проверка сравнения
    Iterator begin = queue.begin();
    Iterator end = queue.end();
    EXPECT_NE(begin, end);
    
    // Проверка прохода по всем элементам
    int sum = 0;
    for (auto it = queue.begin(); it != queue.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 150); // 10+20+30+40+50
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}