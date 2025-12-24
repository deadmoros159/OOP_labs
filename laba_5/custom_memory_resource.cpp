#include "custom_memory_resource.h"
#include <algorithm>
#include <stdexcept>
#include <memory>

using namespace std;

FixedMemoryResource::FixedMemoryResource(size_t total_size)
    : memory_block(::operator new(total_size, align_val_t{alignof(max_align_t)})),
      block_size(total_size),
      used_size(0) {
    if (!memory_block) throw bad_alloc();
}

FixedMemoryResource::~FixedMemoryResource() {
    block_info.clear();
    ::operator delete(memory_block, align_val_t{alignof(max_align_t)});
}

void* FixedMemoryResource::do_allocate(size_t bytes, size_t alignment) {
    if (alignment > alignof(max_align_t)) {
        throw bad_alloc();
    }
    
    for (auto& info : block_info) {
        if (!info.allocated && info.size >= bytes) {
            info.allocated = true;
            used_size += bytes;
            return info.ptr;
        }
    }
    
    if (used_size + bytes > block_size) {
        throw bad_alloc();
    }
    
    void* ptr = static_cast<char*>(memory_block) + used_size;
    block_info.emplace_back(ptr, bytes, true);
    used_size += bytes;
    
    return ptr;
}

void FixedMemoryResource::do_deallocate(void* p, size_t bytes, size_t alignment) {
    if (!p) return;
    
    auto it = find_if(block_info.begin(), block_info.end(),
        [p](const BlockInfo& info) {
            return info.ptr == p && info.allocated;
        });
    
    if (it != block_info.end()) {
        it->allocated = false;
    }
}

bool FixedMemoryResource::do_is_equal(const pmr::memory_resource& other) const noexcept {
    return this == &other;
}