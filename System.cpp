#include "System.h"

System::System() {
    cout << "========================================" << endl;
    cout << "      选题管理系统 v1.0" << endl;
    cout << "========================================" << endl;
    cout << "\n正在初始化系统..." << endl;

    // 初始化随机数种子
    srand((unsigned int)time(0));

    // 加载数据
    studentManager.loadFromFile();
    topicManager.loadFromFile();
    selectionManager.loadFromFile();

    cout << "\n系统初始化完成！" << endl;
}

void System::pause() {
    cout << "\n按回车键继续...";
    string dummy;
    getline(cin, dummy);
}

void System::showMainMenu() {
    cout << "\n========================================" << endl;
    cout << "           主 菜 单" << endl;
    cout << "========================================" << endl;
    cout << "  1. 学生信息管理" << endl;
    cout << "  2. 题库信息管理" << endl;
    cout << "  3. 选题信息管理" << endl;
    cout << "  0. 退出系统" << endl;
    cout << "========================================" << endl;
    cout << "请输入选项：";
}

void System::showStudentMenu() {
    cout << "\n===== 学生信息管理 =====" << endl;
    cout << "  1. 浏览学生信息" << endl;
    cout << "  2. 添加学生" << endl;
    cout << "  3. 修改学生信息" << endl;
    cout << "  4. 删除学生" << endl;
    cout << "  5. 精确查询学生" << endl;
    cout << "  6. 模糊查询学生" << endl;
    cout << "  0. 返回上级菜单" << endl;
    cout << "请输入选项：";
}

void System::showTopicMenu() {
    cout << "\n===== 题库信息管理 =====" << endl;
    cout << "  1. 浏览题库信息" << endl;
    cout << "  2. 添加题目" << endl;
    cout << "  3. 修改题目信息" << endl;
    cout << "  4. 删除题目" << endl;
    cout << "  5. 精确查询题目" << endl;
    cout << "  6. 模糊查询题目" << endl;
    cout << "  0. 返回上级菜单" << endl;
    cout << "请输入选项：";
}

void System::showSelectionMenu() {
    cout << "\n===== 选题信息管理 =====" << endl;
    cout << "  1. 浏览选题信息" << endl;
    cout << "  2. 自动选题匹配（按班级）" << endl;
    cout << "  3. 手工选题匹配" << endl;
    cout << "  4. 按题号查询选题情况" << endl;
    cout << "  5. 按学号/姓名查询选题情况" << endl;
    cout << "  0. 返回上级菜单" << endl;
    cout << "请输入选项：";
}

void System::handleStudentMenu() {
    string choice;
    while (true) {
        showStudentMenu();
        getline(cin, choice);
        choice = trimString(choice);

        if (choice == "0") break;
        else if (choice == "1") { studentManager.displayAll(); pause(); }
        else if (choice == "2") { studentManager.addStudent(); pause(); }
        else if (choice == "3") { studentManager.modifyStudent(); pause(); }
        else if (choice == "4") { studentManager.deleteStudent(&selectionManager); pause(); }
        else if (choice == "5") { studentManager.exactQuery(); pause(); }
        else if (choice == "6") { studentManager.fuzzyQuery(); pause(); }
        else { cout << "无效选项，请重新输入！" << endl; }
    }
}

void System::handleTopicMenu() {
    string choice;
    while (true) {
        showTopicMenu();
        getline(cin, choice);
        choice = trimString(choice);

        if (choice == "0") break;
        else if (choice == "1") { topicManager.displayAll(); pause(); }
        else if (choice == "2") { topicManager.addTopic(); pause(); }
        else if (choice == "3") { topicManager.modifyTopic(); pause(); }
        else if (choice == "4") { topicManager.deleteTopic(&selectionManager); pause(); }
        else if (choice == "5") { topicManager.exactQuery(); pause(); }
        else if (choice == "6") { topicManager.fuzzyQuery(); pause(); }
        else { cout << "无效选项，请重新输入！" << endl; }
    }
}

void System::handleSelectionMenu() {
    string choice;
    while (true) {
        showSelectionMenu();
        getline(cin, choice);
        choice = trimString(choice);

        if (choice == "0") break;
        else if (choice == "1") {
            selectionManager.displayAll(&studentManager, &topicManager);
            pause();
        }
        else if (choice == "2") {
            selectionManager.autoMatch(&studentManager, &topicManager);
            pause();
        }
        else if (choice == "3") {
            selectionManager.manualMatch(&studentManager, &topicManager);
            pause();
        }
        else if (choice == "4") {
            selectionManager.queryByTopic(&studentManager, &topicManager);
            pause();
        }
        else if (choice == "5") {
            selectionManager.queryByStudent(&studentManager, &topicManager);
            pause();
        }
        else { cout << "无效选项，请重新输入！" << endl; }
    }
}

void System::run() {
    string choice;
    while (true) {
        showMainMenu();
        getline(cin, choice);
        choice = trimString(choice);

        if (choice == "0") {
            cout << "\n正在保存数据..." << endl;
            studentManager.saveToFile();
            topicManager.saveToFile();
            selectionManager.saveToFile();
            cout << "感谢使用选题管理系统，再见！" << endl;
            break;
        }
        else if (choice == "1") {
            handleStudentMenu();
        }
        else if (choice == "2") {
            handleTopicMenu();
        }
        else if (choice == "3") {
            handleSelectionMenu();
        }
        else {
            cout << "无效选项，请重新输入！" << endl;
        }
    }
}
