#ifndef C23_LAB_1_H
#define C23_LAB_1_H

#include <mutex>

void Beep(int frequency, int length);
void Play(const char* note, int length);
void PlaySequence(const char* notes, int length);

template <typename InIt, typename OutIt> void transform_(
    InIt in_first, InIt in_last, OutIt out_first
) {
    std::transform(in_first, in_last, out_first, [] (decltype(*in_first) a) { return std::abs(a); });
}

template <typename T> class threadsafe_stack {
    public:
        threadsafe_stack() {}
        explicit threadsafe_stack(const threadsafe_stack& other) {
            std::lock_guard<std::mutex> other_locker(other.mutex);
            data = other.data;
        }
        auto operator=(const threadsafe_stack& other) {
            std::lock_guard<std::mutex> this_locker(mutex, std::adopt_lock);
            std::lock_guard<std::mutex> other_locker(other.mutex, std::adopt_lock);
            std::lock(mutex, other.mutex);
            data = other.data;
        }
        void push(T value) {
            std::lock_guard<std::mutex> locker(mutex);
            data.push_back(value);
        }
        void pop(T& value) {
            std::lock_guard<std::mutex> locker(mutex);
            if (data.empty()) throw std::runtime_error("Empty stack");
            value = data.back();
            data.pop_back();
        }
        std::shared_ptr<T> pop() {
            std::lock_guard<std::mutex> locker(mutex);
            if (data.empty()) throw std::runtime_error("Empty stack");
            const std::shared_ptr<T> res = std::make_shared<T>(data.back());
            data.pop_back();
            return res;
        }
        bool empty() const {
            std::lock_guard<std::mutex> locker(mutex);
            return data.empty();
        }
    private:
        std::vector<T> data;
        mutable std::mutex mutex;
};


#endif // C23_LAB_1
