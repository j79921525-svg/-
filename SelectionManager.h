#ifndef SELECTIONMANAGER_H
#define SELECTIONMANAGER_H

#include "Common.h"

class StudentManager;
class TopicManager;

class SelectionManager {
private:
    LinkedList<Selection> selections;
    const string fileName;

public:
    SelectionManager(const string& file = "selections.txt");

    void loadFromFile();
    void saveToFile();

    // 浏览所有选题记录
    void displayAll(StudentManager* stuMgr, TopicManager* topMgr);

    // 自动选题（为指定班级的学生匹配题目）
    void autoMatch(StudentManager* stuMgr, TopicManager* topMgr);

    // 手工选题（输入学号，随机匹配题目）
    void manualMatch(StudentManager* stuMgr, TopicManager* topMgr);

    // 按题号查询选题情况
    void queryByTopic(StudentManager* stuMgr, TopicManager* topMgr);

    // 按学号/姓名查询该学生的选题情况
    void queryByStudent(StudentManager* stuMgr, TopicManager* topMgr);

    // 判断某学生是否已有选题记录
    bool hasSelectionByStudent(const string& studentId);

    // 判断某题目是否已被选取
    bool hasSelectionByTopic(const string& topicId);

    // 删除某学生的所有选题记录
    void removeSelectionsByStudent(const string& studentId);

    // 删除某题目的所有选题记录
    void removeSelectionsByTopic(const string& topicId);

    // 获取链表引用
    LinkedList<Selection>& getList() { return selections; }
};

#endif
