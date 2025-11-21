#ifndef FIXED_MAP_MEMORY_RESOURCE_HPP
#define FIXED_MAP_MEMORY_RESOURCE_HPP

#include <memory_resource>
#include <map>
#include <new>
#include <stdexcept>
#include <cstddef>

class FixedMapMemoryResource : public std::pmr::memory_resource {
public:
    FixedMapMemoryResource(std::size_t chunk_size, std::size_t chunk_count)
        : buffer_(nullptr),
          chunk_size_(chunk_size),
          chunk_count_(chunk_count),
          total_size_(chunk_size * chunk_count)
    {
        if (chunk_size_ == 0 || chunk_count_ == 0) {
            throw std::invalid_argument("chunk_size and chunk_count must be > 0");
        }
        buffer_ = ::operator new(total_size_);

        char* base = static_cast<char*>(buffer_);
        for (std::size_t i = 0; i < chunk_count_; ++i) {
            void* ptr = base + i * chunk_size_;
            chunks_.emplace(ptr, false);
        }
    }

    ~FixedMapMemoryResource() override {
        ::operator delete(buffer_);
        buffer_ = nullptr;
    }

private:
    void* do_allocate(std::size_t bytes, std::size_t alignment) override {
        if (bytes == 0) bytes = 1;
        if (bytes > chunk_size_ || alignment > alignof(std::max_align_t)) {
            throw std::bad_alloc();
        }

        for (auto& kv : chunks_) {
            if (!kv.second) {
                void* candidate = kv.first;
                std::uintptr_t addr = reinterpret_cast<std::uintptr_t>(candidate);
                if (addr % alignment == 0) {
                    kv.second = true;
                    allocated_info_[candidate] = bytes;
                    return candidate;
                }
            }
        }

        throw std::bad_alloc();
    }

    void do_deallocate(void* p, std::size_t, std::size_t) override {
        if (!p) return;
        auto it = chunks_.find(p);
        if (it == chunks_.end())
            throw std::invalid_argument("Pointer not allocated by this memory_resource");

        it->second = false;
        allocated_info_.erase(p);
    }

    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
        return this == &other;
    }

private:
    void* buffer_;
    std::size_t chunk_size_;
    std::size_t chunk_count_;
    std::size_t total_size_;

    std::map<void*, bool> chunks_;
    std::map<void*, std::size_t> allocated_info_;
};

#endif
