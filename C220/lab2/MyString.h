#include <iostream>

class MyString {
    public:
        friend std::ostream& operator<<(std::ostream& os, const MyString& s);
        MyString();
        explicit MyString(const char* p);
        MyString(const MyString& other);
        MyString(MyString&& other);
        ~MyString();
        MyString& operator=(const char* str);
        MyString& operator=(const MyString& s);
        MyString& operator=(MyString&& s);
        MyString operator+(const MyString& s);
        MyString operator+(const MyString& s) const;
        MyString& operator+=(const MyString& s);
        bool operator==(const char* other);
        bool operator==(const MyString& other);
        const char* GetString();
        void SetNewString(const char* p);
    private:
        char* p;
};

std::ostream& operator<<(std::ostream& os, const MyString& s);
