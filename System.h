#ifndef SYSTEM_H
#define SYSTEM_H

#include "Common.h"
#include "StudentManager.h"
#include "TopicManager.h"
#include "SelectionManager.h"

class System {
private:
    StudentManager studentManager;
    TopicManager topicManager;
    SelectionManager selectionManager;

    // 菜单显示
    void showMainMenu();
    void showStudentMenu();
    void showTopicMenu();
    void showSelectionMenu();

    // 子菜单处理
    void handleStudentMenu();
    void handleTopicMenu();
    void handleSelectionMenu();

    // 暂停等待用户按键
    void pause();

public:
    System();
    void run();
};

#endif
