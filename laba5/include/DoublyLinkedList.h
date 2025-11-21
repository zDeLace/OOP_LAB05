#ifndef DOUBLY_LINKED_LIST_HPP
#define DOUBLY_LINKED_LIST_HPP

#include <memory_resource>
#include <iterator>
#include <utility>
#include <cstddef>

template <typename T>
class DoublyLinkedList {
private:
    struct Node {
        T value;
        Node* prev;
        Node* next;

        template<typename... Args>
        Node(Args&&... args)
            : value(std::forward<Args>(args)...), prev(nullptr), next(nullptr) {}
    };

public:
    struct iterator {
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Node* node;
        iterator(Node* n = nullptr) : node(n) {}

        reference operator*() const { return node->value; }
        pointer operator->() const { return &node->value; }

        iterator& operator++() {
            if (node) node = node->next;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp(*this);
            ++(*this);
            return tmp;
        }

        bool operator==(const iterator& other) const { return node == other.node; }
        bool operator!=(const iterator& other) const { return node != other.node; }
    };

    using allocator_type = std::pmr::polymorphic_allocator<Node>;

    explicit DoublyLinkedList(std::pmr::memory_resource* mr = std::pmr::get_default_resource())
        : alloc_(mr), head_(nullptr), tail_(nullptr), size_(0) {}

    ~DoublyLinkedList() { clear(); }

    DoublyLinkedList(const DoublyLinkedList&) = delete;
    DoublyLinkedList& operator=(const DoublyLinkedList&) = delete;

    template<typename U>
    void push_back(U&& value) {
        Node* n = allocate_node(std::forward<U>(value));
        if (!tail_) head_ = tail_ = n;
        else {
            tail_->next = n;
            n->prev = tail_;
            tail_ = n;
        }
        size_++;
    }

    template<typename U>
    void push_front(U&& value) {
        Node* n = allocate_node(std::forward<U>(value));
        if (!head_) head_ = tail_ = n;
        else {
            n->next = head_;
            head_->prev = n;
            head_ = n;
        }
        size_++;
    }

    void pop_front() {
        if (!head_) return;
        Node* old = head_;
        head_ = head_->next;
        if (head_) head_->prev = nullptr;
        else tail_ = nullptr;
        destroy_node(old);
        size_--;
    }

    void pop_back() {
        if (!tail_) return;
        Node* old = tail_;
        tail_ = tail_->prev;
        if (tail_) tail_->next = nullptr;
        else head_ = nullptr;
        destroy_node(old);
        size_--;
    }

    void clear() {
        Node* cur = head_;
        while (cur) {
            Node* next = cur->next;
            destroy_node(cur);
            cur = next;
        }
        head_ = tail_ = nullptr;
        size_ = 0;
    }

    iterator begin() { return iterator(head_); }
    iterator end()   { return iterator(nullptr); }

    bool empty() const { return size_ == 0; }
    std::size_t size() const { return size_; }

private:
    template<typename U>
    Node* allocate_node(U&& value) {
        Node* p = alloc_.allocate(1);
        std::allocator_traits<allocator_type>::construct(alloc_, p, std::forward<U>(value));
        return p;
    }

    void destroy_node(Node* p) {
        std::allocator_traits<allocator_type>::destroy(alloc_, p);
        alloc_.deallocate(p, 1);
    }

    allocator_type alloc_;
    Node* head_;
    Node* tail_;
    std::size_t size_;
};

#endif
