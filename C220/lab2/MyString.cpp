#include <cstring>

#include "MyString.h"

using namespace std;

MyString::MyString() {
    p = new char[1];
    p[0] = '\0';
}

MyString::MyString(const char* pch) {
    p = new char[strlen(pch) + 1];
    strcpy(p, pch);
}

MyString::MyString(const MyString& other) {
    p = new char[strlen(other.p) + 1];
    strcpy(p, other.p);
}

MyString::MyString(MyString&& other) {
    p = other.p;
    other.p = nullptr;
}

MyString::~MyString() {
    delete[] p;
    p = nullptr;
}

MyString& MyString::operator=(const char* str) {
    delete[] p;
    p = new char[strlen(str) + 1];
    strcpy(p, str);
    return *this;
}

MyString& MyString::operator=(const MyString& s) {
    if (this != &s) {
        delete[] p;
        p = new char[strlen(s.p) + 1];
        strcpy(p, s.p);
    }
    return *this;
}

MyString& MyString::operator=(MyString&& s) {
    delete[] p;
    p = s.p;
    s.p = nullptr;
    return *this;
}

MyString MyString::operator+(const MyString& s) {
    int len = strlen(p) + strlen(s.p) + 1;
    char* ptr = new char[len];
    strcpy(ptr, p);
    strcat(ptr, s.p);
    MyString temp(ptr);
    delete[] ptr;
    return(temp);
}

MyString MyString::operator+(const MyString& s) const {
    int len = strlen(p) +strlen(s.p) + 1;
    char* ptr = new char[len];
    strcpy(ptr, p);
    strcat(ptr, s.p);
    MyString temp(ptr);
    delete[] ptr;
    return(temp);
}

MyString& MyString::operator+=(const MyString& s) {
    MyString temp = *this;
    delete[] p;
    p = new char[strlen(temp.p) + strlen(s.p) + 1];
    strcpy(p, temp.p);
    strcat(p, s.p);
    return *this;
}

bool MyString::operator==(const char* other) {
    return(strcmp(this->p, other) == 0);
}

bool MyString::operator==(const MyString& other) {
    return(strcmp(this->p, other.p) == 0);
}

const char* MyString::GetString() {
    if (p) return p;
    return "";
}

void MyString::SetNewString(const char* pch) {
    int len1, len2;
    len1 = strlen(this->p) + 1;
    len2 = strlen(pch) + 1;
    if (len2 <= len1) {
        strcpy(p, pch);
    } else {
        delete[] p;
        p = new char[len2];
        strcpy(p, pch);
    }
 }

ostream& operator<<(ostream& os, const MyString& s) {
    os << s.p;
    return os;
}
