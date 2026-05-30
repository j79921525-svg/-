#ifndef STUDENTMANAGER_H
#define STUDENTMANAGER_H

#include "Common.h"

// 前向声明，避免循环依赖
class SelectionManager;

class StudentManager {
private:
    LinkedList<Student> students;
    const string fileName;

    bool isIdExist(const string& id);

public:
    StudentManager(const string& file = "students.txt");

    void loadFromFile();
    void saveToFile();

    void addStudent();
    void modifyStudent();
    void deleteStudent(SelectionManager* selMgr = nullptr);

    void displayAll();
    void exactQuery();
    void fuzzyQuery();

    Student* findById(const string& id);
    Student* findByName(const string& name);

    LinkedList<Student>& getList() { return students; }
};

#endif
