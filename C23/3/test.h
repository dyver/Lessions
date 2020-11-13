#ifndef C23_LAB_3_H
#define C23_LAB_3_H

#include <mutex>
#include <queue>
#include <memory>
#include <numeric>
#include <initializer_list>

// Task #2

template <typename T> class LockFreeStack {
    private:
        struct Node {
            Node* next;
            std::shared_ptr<T> data;
            Node(const T& d) : data(std::make_shared<T>(d)) {}
        };
        std::atomic<Node*> head;
    public:
        void push(const T& data) {
            Node* newNode = new Node(data);
            newNode->next = head.load();
            while (not head.compare_exchange_weak(newNode->next, newNode));
        }
        std::shared_ptr<T> pop() {
            Node* oldHead = head.load();
            while (oldHead && not head.compare_exchange_weak(oldHead, oldHead->next)) {}
            return oldHead ? oldHead->data : std::shared_ptr<T>();
        }
};


#endif // C23_LAB_3_H
