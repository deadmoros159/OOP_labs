#ifndef CUSTOM_MEMORY_RESOURCE_H
#define CUSTOM_MEMORY_RESOURCE_H

#include <memory_resource>
#include <vector>
#include <cstddef>
#include <stdexcept>

class FixedMemoryResource : public std::pmr::memory_resource {
private:
    struct BlockInfo {
        void* ptr;
        size_t size;
        bool allocated;
        
        BlockInfo(void* p = nullptr, size_t s = 0, bool a = true)
            : ptr(p), size(s), allocated(a) {}
    };
    
    void* memory_block;
    size_t block_size;
    size_t used_size;
    std::vector<BlockInfo> block_info;
    
protected:
    virtual void* do_allocate(size_t bytes, size_t alignment) override;
    virtual void do_deallocate(void* p, size_t bytes, size_t alignment) override;
    virtual bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;
    
public:
    FixedMemoryResource(size_t total_size);
    ~FixedMemoryResource();
    
    FixedMemoryResource(const FixedMemoryResource&) = delete;
    FixedMemoryResource& operator=(const FixedMemoryResource&) = delete;
    
    size_t get_used() const { return used_size; }
    size_t get_total() const { return block_size; }
    size_t get_blocks() const { return block_info.size(); }
};

template<typename T>
class CustomQueue {
private:
    struct Node {
        T data;
        Node* next;
        
        template<typename... Args>
        Node(Args&&... args) : data(std::forward<Args>(args)...), next(nullptr) {}
    };
    
    Node* front;
    Node* back;
    size_t size_;
    std::pmr::polymorphic_allocator<Node> alloc;
    
public:
    class Iterator {
    private:
        Node* current;
        
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        
        Iterator(Node* node = nullptr) : current(node) {}
        
        Iterator& operator++() { if (current) current = current->next; return *this; }
        Iterator operator++(int) { Iterator temp = *this; ++(*this); return temp; }
        T& operator*() const { return current->data; }
        T* operator->() const { return &current->data; }
        bool operator==(const Iterator& other) const { return current == other.current; }
        bool operator!=(const Iterator& other) const { return current != other.current; }
    };
    
    using iterator = Iterator;
    using const_iterator = Iterator;
    
    explicit CustomQueue(std::pmr::memory_resource* mr = std::pmr::get_default_resource())
        : front(nullptr), back(nullptr), size_(0), alloc(mr) {}
    
    ~CustomQueue() { clear(); }
    
    CustomQueue(const CustomQueue& other) 
        : front(nullptr), back(nullptr), size_(0), alloc(other.alloc) {
        for (auto it = other.begin(); it != other.end(); ++it) {
            push(*it);
        }
    }
    
    CustomQueue(CustomQueue&& other) noexcept
        : front(other.front), back(other.back), size_(other.size_), alloc(std::move(other.alloc)) {
        other.front = other.back = nullptr;
        other.size_ = 0;
    }
    
    CustomQueue& operator=(const CustomQueue& other) {
        if (this != &other) {
            clear();
            alloc = other.alloc;
            for (auto it = other.begin(); it != other.end(); ++it) {
                push(*it);
            }
        }
        return *this;
    }
    
    template<typename U>
    void push(U&& value) {
        Node* new_node = alloc.allocate(1);
        try {
            alloc.construct(new_node, std::forward<U>(value));
        } catch (...) {
            alloc.deallocate(new_node, 1);
            throw;
        }
        
        if (back) back->next = new_node;
        else front = new_node;
        back = new_node;
        ++size_;
    }
    
    void pop() {
        if (front) {
            Node* old = front;
            front = front->next;
            if (!front) back = nullptr;
            alloc.destroy(old);
            alloc.deallocate(old, 1);
            --size_;
        }
    }
    
    T& front_element() {
        if (!front) throw std::runtime_error("Queue is empty");
        return front->data;
    }
    
    const T& front_element() const {
        if (!front) throw std::runtime_error("Queue is empty");
        return front->data;
    }
    
    bool empty() const { return size_ == 0; }
    size_t size() const { return size_; }
    
    void clear() {
        while (!empty()) pop();
    }
    
    iterator begin() { return iterator(front); }
    iterator end() { return iterator(nullptr); }
    const_iterator begin() const { return const_iterator(front); }
    const_iterator end() const { return const_iterator(nullptr); }
};

#endif