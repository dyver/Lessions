// Task #1

#include <memory>

template <typename T> class MyArray {
    public:
        explicit MyArray(size_t s = 0) {
            size = s;
            reserve = size * 2;
            data = reinterpret_cast<T*>(new char[sizeof(T) * reserve]);
            for (decltype(size) i = 0; i < size; ++i) {
                new(&data[i]) T{};
            }
        };
        MyArray(std::initializer_list<T> li) {
            size = li.size();
            reserve = size;
            data = reinterpret_cast<T*>(new char[sizeof(T) * reserve]);
            auto it = std::begin(li);
            for (decltype(size) i = 0; i < size; ++i) {
                new(&data[i]) T{*it};
                ++it;
            }
        }
        MyArray(const MyArray& other) {
            size = other.size;
            reserve = size;
            data = reinterpret_cast<T*>(new char[sizeof(T) * size]);
            for (decltype(size) i = 0; i < size; ++i) {
                new(&data[i]) T{other.data[i]};
            }
        }
        MyArray(MyArray&& other) {
            size = other.size;
            reserve = other.reserve;
            data = other.data;
            other.size = 0;
            other.reserve = 0;
            other.data = nullptr;
        }
        ~MyArray() {
            for (decltype(size) i = 0; i < size; ++i) {
                data[i].~T();
            }
            delete[] reinterpret_cast<char*>(data);
        }
        MyArray& operator=(const MyArray& other) {
            for (decltype(size) i = 0; i < size; ++i) {
                data[i].~T();
            }
            if (size < other.size) {
                delete[] reinterpret_cast<char*>(data);
                data = reinterpret_cast<T*>(new char[sizeof(T) * other.size]);
            }
            size = other.size;
            reserve = size;
            for (decltype(size) i = 0; i < size; ++i) {
                new(&data[i]) T{other.data[i]};
            }
            return *this;
        }
        MyArray& operator=(MyArray&& other) {
            for (decltype(size) i = 0; i < size; ++i) {
                data[i].~T();
            }
            delete[] reinterpret_cast<char*>(data);
            size = other.size;
            reserve = other.reserve;
            data = other.data;
            other.size = 0;
            other.reserve = 0;
            other.data = nullptr;
            return *this;
        }
        template <typename U> void push(U&& el) {
            if (size == reserve) {
                reserve = size ? size * 2 : 10;
                auto new_data = reinterpret_cast<T*>(new char[sizeof(T) * reserve]);
                // Не работает для std::string!!!
//                 memcpy(new_data, data, size * sizeof(T));
                for (decltype(size) i = 0; i < size; ++i) {
                    new(&new_data[i]) T{std::move(data[i])};
                }
                delete[] reinterpret_cast<char*>(data);
                data = nullptr;
                data = new_data;
            }
            new(&data[size]) T{std::forward<U>(el)};
            ++size;
        }
        T pop() {
            if (size == 0) {
                throw std::range_error("MyArray::pop(): size == 0");
            }
            --size;
            auto result{data[size]};
            data[size].~T();
            return result;
        }
        size_t length() { return size; }
    private:
        T* data;
        size_t size;
        size_t reserve;
};

// Task #2

template <typename T> class MyList {
    public:
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;
        using size_type = std::size_t;

        class Iterator;
        using iterator = Iterator;
        using const_iterator = const Iterator;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = const std::reverse_iterator<iterator>;

        MyList(size_type size, const T& v = T{}) {
            assign(size, v);
        }
        MyList(std::initializer_list<T> il) {
            clear();
            for (const auto& el : il) { push_back(el); }
        }
        MyList(const MyList& other) {
            clear();
            for (const auto& el : other) { push_back(el); }
        }
        MyList(MyList&& other) {
            first = other.first;
            last = other.last;
            length = other.length;
            other.first = new Node;
            other.last = new Node;
            other.first->next = last;
            other.last->previous = first;
            other.length = 0;
        }
        MyList& operator=(std::initializer_list<T> il) {
            assign(il);
            return *this;
        }
        MyList& operator=(const MyList& other) {
            clear();
            for (const auto& el : other) { push_back(el); }
            return *this;
        }
        MyList& operator=(MyList&& other) {
            clear();
            first->next = other.first->next;
            first->next->previous = first;
            last->previous = other.last->previous;
            last->previous->next = last;
            other.first->next = other.last;
            other.last->previous = other.first;
            return *this;
        }
        ~MyList() {
            clear();
            delete first;
            delete last;
        }
        size_type size() const {
            return length;
        }
        bool empty() const {
            return not length;
        }
        void assign(std::initializer_list<T> il) {
            clear();
            for (const auto& el : il) { push_back(el); }
        }
        void assign(size_type size, const T& v) {
            clear();
            for (size_type i = 0; i < size; ++i) { push_back(v); }
        }
        template <typename InIt> void assign(InIt first, InIt last) {
            clear();
            for (auto it = first; it != last; ++it) {
                push_back(*it);
            }
        }
        reference front() {
            if (empty()) throw std::range_error("MyList::front(): list is empty.");
            return first->next->data;
        }
        const_reference front() const {
            if (empty()) throw std::range_error("MyList::front(): list is empty.");
            return first->next->data;
        }
        void push_front(const_reference v) {
            first->data = v;
            first->previous = new Node;
            first->previous->next = first;
            first = first->previous;
            ++length;
        }
        void pop_front() {
            if (empty()) throw std::range_error("MyList::pop_front(): list is empty.");
            first = first->next;
            delete first->previous;
            first->previous = nullptr;
            --length;
        }
        reference back() {
            if (empty()) throw std::range_error("MyList::back(): list is empty.");
            return last->previous->data;
        }
        const_reference back() const {
            if (empty()) throw std::range_error("MyList::back(): list is empty.");
            return last->previous->data;
        }
        void push_back(const_reference v) {
            last->data = v;
            last->next = new Node;
            last->next->previous = last;
            last = last->next;
            ++length;
        }
        void pop_back() {
            if (empty()) throw std::range_error("MyList::pop_back(): list is empty.");
            last = last->previous;
            delete last->next;
            last->next = nullptr;
            --length;
        }
        void clear() {
            if (not (first and last)) {
                first = new Node;
                last = new Node;
                first->next = last;
                last->previous = first;
                return;
            }
            auto current = first->next->next;
            while (current) {
                delete current->previous;
                current = current->next;
            }
            first->next = last;
            last->previous = first;
            length = 0;
        }
    private:
        struct Node {
            Node* previous = nullptr;
            Node* next = nullptr;
            T data;
        };
    public:
        class Iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
            friend class MyList;
            public:
                Iterator() {}
                Iterator& operator++() { current = current->next; return *this; }
                Iterator& operator--() { current = current->previous; return *this; }
                T& operator*() { return current->data; }
                bool operator==(const Iterator& other) { return current == other.current; }
                bool operator!=(const Iterator& other) { return not (*this == other); }
            protected:
                Iterator(Node* holder) : current{holder} {}
                Node* current = nullptr;
        };
        iterator begin() { return iterator{first->next}; }
        iterator end() { return iterator{last}; }
        const_iterator begin() const { return iterator{first->next}; }
        const_iterator end() const { return iterator{last}; }
        const_iterator cbegin() const { return iterator{first->next}; }
        const_iterator cend() const { return iterator{last}; }

        reverse_iterator rbegin() {
            return std::make_reverse_iterator<iterator>(end());
        }
        reverse_iterator rend() {
            return std::make_reverse_iterator<iterator>(begin());
        }
        const_reverse_iterator rbegin() const {
            return std::make_reverse_iterator<iterator>(end());
        }
        const_reverse_iterator rend() const {
            return std::make_reverse_iterator<iterator>(begin());
        }
        const_reverse_iterator crbegin() const {
            return std::make_reverse_iterator<iterator>(end());
        }
        const_reverse_iterator crend() const {
            return std::make_reverse_iterator<iterator>(begin());
        }
    private:
        Node* first = nullptr;
        Node* last = nullptr;
        size_type length = 0;
};


// Variant #2

// Task #1

class Any {

    public:

        template <typename T> Any(const T& object)
            : holder{new Holder<T>{object}} {}
        Any(const Any& other)
            : holder{other.holder->clone()} {}
        Any(Any&& other)
            : holder{other.holder}
        {
            other.holder = nullptr;
        }
        ~Any() { delete holder; }
        template <typename T> Any& operator=(const T& object) {
            holder = new Holder<T>{object};
            return *this;
        }
        Any& operator=(const Any& other) {
            delete holder;
            holder = other.holder->clone();
            return *this;
        }
        Any& operator=(Any&& other) {
            delete holder;
            holder = other.holder;
            other.holder = nullptr;
            return *this;
        }
        template <typename U> operator U() const {
            auto unwrapped = dynamic_cast<Holder<U>*>(holder);
            if (not unwrapped) throw std::runtime_error("Bad any cast.");
            return unwrapped->object;
        }

    private:

        struct HolderBase {
            virtual ~HolderBase() {}
            virtual HolderBase* clone() = 0;
            virtual std::ostream& out(std::ostream& os) const = 0;
            virtual HolderBase* addition(HolderBase* rhs) const = 0;
        };

        template <typename T> struct Holder : public HolderBase {
            Holder(const T& object) : object{object} {}
            virtual HolderBase* clone() {
                return new Holder<T>(object);
            }
            virtual std::ostream& out(std::ostream& os) const override {
                return os << object;
            }
            virtual HolderBase* addition(HolderBase* rhs) const override {
                return new Holder<T>(object + static_cast<Holder<T>*>(rhs)->object);
            }
            T object;
        };

        Any(HolderBase* holder) : holder{holder} {}

        friend std::ostream& operator<<(std::ostream& os, const Any& lhs);
        friend Any operator+(const Any& lhs, const Any& rhs);

        void checkConsistency() const {
            if (not holder) throw std::runtime_error("Empty type");
        }
        static void checkTypesEquality(const Any& lhs, const Any& rhs) {
            if (typeid(*lhs.holder) != typeid(*rhs.holder)) {
                throw std::runtime_error("Types mismatch.");
            }
        }

        HolderBase* holder;
};

std::ostream& operator<<(std::ostream& os, const Any& lhs) {
    lhs.checkConsistency();
    return lhs.holder->out(os);
}

Any operator+(const Any& lhs, const Any& rhs) {
    lhs.checkConsistency();
    rhs.checkConsistency();
    Any::checkTypesEquality(lhs, rhs);
    return lhs.holder->addition(rhs.holder);
}
