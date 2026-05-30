#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <vector>
using namespace std;

// ==================== 数据类定义 ====================

// 学生信息
struct Student {
    string id;         // 学号
    string name;       // 姓名
    string className;  // 班级
};

// 题目信息
struct Topic {
    string id;            // 编号
    string title;         // 题目
    string requirements;  // 任务要求
    string deadline;      // 完成时间
};

// 选题记录
struct Selection {
    string topicId;       // 题号
    string studentId;     // 学号
    string selectionDate; // 选题日期
};

// ==================== 自定义链表模板 ====================

template<typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& d) : data(d), next(nullptr) {}
    };
    Node* head;
    int count;

public:
    LinkedList() : head(nullptr), count(0) {}

    ~LinkedList() { clear(); }

    // 添加节点到链表末尾
    void add(const T& data) {
        Node* newNode = new Node(data);
        if (head == nullptr) {
            head = newNode;
        } else {
            Node* cur = head;
            while (cur->next != nullptr) {
                cur = cur->next;
            }
            cur->next = newNode;
        }
        count++;
    }

    // 获取指定索引的节点数据指针（用于读取和修改）
    T* get(int index) {
        if (index < 0 || index >= count) return nullptr;
        Node* cur = head;
        for (int i = 0; i < index; i++) {
            cur = cur->next;
        }
        return &(cur->data);
    }

    // 删除指定索引的节点
    bool remove(int index) {
        if (index < 0 || index >= count) return false;
        Node* toDelete = nullptr;
        if (index == 0) {
            toDelete = head;
            head = head->next;
        } else {
            Node* prev = head;
            for (int i = 0; i < index - 1; i++) {
                prev = prev->next;
            }
            toDelete = prev->next;
            prev->next = toDelete->next;
        }
        delete toDelete;
        count--;
        return true;
    }

    // 删除匹配条件的第一个节点，返回被删除数据的副本
    bool removeByMatch(bool (*predicate)(const T&)) {
        if (head == nullptr) return false;
        if (predicate(head->data)) {
            Node* toDelete = head;
            head = head->next;
            delete toDelete;
            count--;
            return true;
        }
        Node* prev = head;
        Node* cur = head->next;
        while (cur != nullptr) {
            if (predicate(cur->data)) {
                prev->next = cur->next;
                delete cur;
                count--;
                return true;
            }
            prev = cur;
            cur = cur->next;
        }
        return false;
    }

    // 查找第一个匹配条件的节点数据指针
    T* findByMatch(bool (*predicate)(const T&)) {
        Node* cur = head;
        while (cur != nullptr) {
            if (predicate(cur->data)) {
                return &(cur->data);
            }
            cur = cur->next;
        }
        return nullptr;
    }

    // 获取链表大小
    int size() const { return count; }

    // 清空链表
    void clear() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        count = 0;
    }

    // 判断是否为空
    bool isEmpty() const { return count == 0; }
};

// ==================== 工具函数 ====================

// 获取当前日期字符串
inline string getCurrentDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buf[20];
    snprintf(buf, sizeof(buf), "%04d-%02d-%02d",
             1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);
    return string(buf);
}

// 字符串分割
inline vector<string> splitString(const string& str, char delimiter) {
    vector<string> result;
    stringstream ss(str);
    string item;
    while (getline(ss, item, delimiter)) {
        result.push_back(item);
    }
    return result;
}

// 去除字符串首尾空格
inline string trimString(const string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

// 判断str是否包含sub（模糊匹配用）
inline bool containsString(const string& str, const string& sub) {
    if (sub.empty()) return true;
    return str.find(sub) != string::npos;
}

#endif
