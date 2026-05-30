#include "SelectionManager.h"
#include "StudentManager.h"
#include "TopicManager.h"

SelectionManager::SelectionManager(const string& file) : fileName(file) {}

// ---- 文件读写 ----

void SelectionManager::loadFromFile() {
    selections.clear();
    ifstream fin(fileName);
    if (!fin) {
        // 选题文件可以初始为空
        return;
    }

    string line;
    while (getline(fin, line)) {
        line = trimString(line);
        if (line.empty()) continue;
        vector<string> parts = splitString(line, '|');
        if (parts.size() >= 3) {
            Selection sel;
            sel.topicId = trimString(parts[0]);
            sel.studentId = trimString(parts[1]);
            sel.selectionDate = trimString(parts[2]);
            selections.add(sel);
        }
    }
    fin.close();
    cout << "选题数据加载完成，共 " << selections.size() << " 条记录。" << endl;
}

void SelectionManager::saveToFile() {
    ofstream fout(fileName);
    if (!fout) {
        cout << "错误：无法写入选题文件！" << endl;
        return;
    }
    for (int i = 0; i < selections.size(); i++) {
        Selection* sel = selections.get(i);
        if (sel) {
            fout << sel->topicId << "|" << sel->studentId << "|"
                 << sel->selectionDate << endl;
        }
    }
    fout.close();
}

// ---- 辅助查询 ----

bool SelectionManager::hasSelectionByStudent(const string& studentId) {
    for (int i = 0; i < selections.size(); i++) {
        Selection* sel = selections.get(i);
        if (sel && sel->studentId == studentId) return true;
    }
    return false;
}

bool SelectionManager::hasSelectionByTopic(const string& topicId) {
    for (int i = 0; i < selections.size(); i++) {
        Selection* sel = selections.get(i);
        if (sel && sel->topicId == topicId) return true;
    }
    return false;
}

void SelectionManager::removeSelectionsByStudent(const string& studentId) {
    // 从后往前删除，避免索引变化问题
    for (int i = selections.size() - 1; i >= 0; i--) {
        Selection* sel = selections.get(i);
        if (sel && sel->studentId == studentId) {
            selections.remove(i);
        }
    }
}

void SelectionManager::removeSelectionsByTopic(const string& topicId) {
    for (int i = selections.size() - 1; i >= 0; i--) {
        Selection* sel = selections.get(i);
        if (sel && sel->topicId == topicId) {
            selections.remove(i);
        }
    }
}

// ---- 浏览选题信息 ----

void SelectionManager::displayAll(StudentManager* stuMgr, TopicManager* topMgr) {
    cout << "\n===== 选题信息列表 =====" << endl;
    if (selections.isEmpty()) {
        cout << "暂无选题记录。" << endl;
        return;
    }

    cout << left << setw(10) << "题号"
         << setw(16) << "题目名称"
         << setw(10) << "学号"
         << setw(10) << "学生姓名"
         << setw(14) << "选题日期" << endl;
    cout << string(60, '-') << endl;

    for (int i = 0; i < selections.size(); i++) {
        Selection* sel = selections.get(i);
        if (!sel) continue;

        string topicTitle = "";
        string studentName = "";

        if (topMgr) {
            Topic* t = topMgr->findById(sel->topicId);
            if (t) topicTitle = t->title;
        }
        if (stuMgr) {
            Student* s = stuMgr->findById(sel->studentId);
            if (s) studentName = s->name;
        }

        cout << left << setw(10) << sel->topicId
             << setw(16) << topicTitle
             << setw(10) << sel->studentId
             << setw(10) << studentName
             << setw(14) << sel->selectionDate << endl;
    }
    cout << "共 " << selections.size() << " 条选题记录。" << endl;
}

// ---- 自动选题匹配 ----

void SelectionManager::autoMatch(StudentManager* stuMgr, TopicManager* topMgr) {
    cout << "\n===== 自动选题匹配 =====" << endl;
    cout << "匹配规则：按学生学号后两位对应题目编号（如学号后两位为03，则匹配T03）" << endl;

    cout << "请输入要进行选题匹配的班级：";
    string className;
    getline(cin, className);
    className = trimString(className);
    if (className.empty()) {
        cout << "班级不能为空！" << endl;
        return;
    }

    // 收集该班级的学生
    LinkedList<Student>& stuList = stuMgr->getList();
    vector<Student*> classStudents;
    for (int i = 0; i < stuList.size(); i++) {
        Student* s = stuList.get(i);
        if (s && s->className == className) {
            classStudents.push_back(s);
        }
    }

    if (classStudents.empty()) {
        cout << "班级 " << className << " 中没有学生。" << endl;
        return;
    }

    cout << "班级 " << className << " 共有 " << classStudents.size() << " 名学生。" << endl;
    cout << "开始自动匹配..." << endl;

    int matchedCount = 0;

    for (size_t idx = 0; idx < classStudents.size(); idx++) {
        Student* s = classStudents[idx];

        // 取学号后两位
        string stuId = s->id;
        string lastTwo = "";
        if (stuId.length() >= 2) {
            lastTwo = stuId.substr(stuId.length() - 2);
        } else {
            lastTwo = stuId;
        }

        // 构建目标题号（统一用两位数字）
        string targetId = "T" + lastTwo;

        // 查找对应题目
        Topic* matchedTopic = topMgr->findById(targetId);
        if (!matchedTopic) {
            cout << "学生 " << s->name << "(" << s->id
                 << ") —— 未找到对应题号 " << targetId << "，跳过。" << endl;
            continue;
        }

        // 先移除该学生之前的选题记录
        removeSelectionsByStudent(s->id);

        // 创建新选题记录
        Selection newSel;
        newSel.topicId = matchedTopic->id;
        newSel.studentId = s->id;
        newSel.selectionDate = getCurrentDate();
        selections.add(newSel);
        matchedCount++;

        cout << "学生 " << s->name << "(" << s->id
             << ") → 题目 " << matchedTopic->id << " " << matchedTopic->title << endl;
    }

    saveToFile();
    cout << "\n自动匹配完成！成功匹配 " << matchedCount << " 名学生。"
         << (classStudents.size() - matchedCount) << " 名学生未匹配。" << endl;
}

// ---- 手工选题匹配 ----

void SelectionManager::manualMatch(StudentManager* stuMgr, TopicManager* topMgr) {
    cout << "\n===== 手工选题匹配 =====" << endl;
    cout << "规则：每次输入学号，从可选题目中随机匹配，每个题目只能被一名学生选中。" << endl;
    cout << "可以连续进行手工选题，输入 0 退出。" << endl;

    LinkedList<Topic>& topList = topMgr->getList();

    while (true) {
        // 统计可选题目（未被选中的题目）
        vector<Topic*> availableTopics;
        for (int i = 0; i < topList.size(); i++) {
            Topic* t = topList.get(i);
            if (t && !hasSelectionByTopic(t->id)) {
                availableTopics.push_back(t);
            }
        }

        if (availableTopics.empty()) {
            cout << "所有题目都已被选中，无法继续手工选题。" << endl;
            break;
        }

        cout << "\n当前可选题目数：" << availableTopics.size() << endl;
        cout << "请输入学号（输入 0 退出）：";
        string studentId;
        getline(cin, studentId);
        studentId = trimString(studentId);

        if (studentId == "0") break;

        // 验证学号
        Student* s = stuMgr->findById(studentId);
        if (!s) {
            cout << "未找到该学号对应的学生！" << endl;
            continue;
        }

        // 检查该学生是否已有选题
        if (hasSelectionByStudent(studentId)) {
            cout << "该学生已有选题记录，是否覆盖？(y/n)：";
            string choice;
            getline(cin, choice);
            if (choice == "y" || choice == "Y") {
                removeSelectionsByStudent(studentId);
            } else {
                continue;
            }
        }

        // 随机抽取题目
        int randIdx = rand() % availableTopics.size();
        Topic* chosen = availableTopics[randIdx];

        Selection newSel;
        newSel.topicId = chosen->id;
        newSel.studentId = studentId;
        newSel.selectionDate = getCurrentDate();
        selections.add(newSel);

        cout << "学生 " << s->name << "(" << s->id << ") 成功匹配题目："
             << chosen->id << " " << chosen->title << endl;

        saveToFile();

        // 检查是否所有学生都已选题
        LinkedList<Student>& stuList = stuMgr->getList();
        bool allMatched = true;
        for (int i = 0; i < stuList.size(); i++) {
            Student* cur = stuList.get(i);
            if (cur && !hasSelectionByStudent(cur->id)) {
                allMatched = false;
                break;
            }
        }

        if (allMatched) {
            cout << "\n所有学生都已完成选题匹配！" << endl;
            break;
        }

        cout << "是否继续手工选题？(y/n)：";
        string cont;
        getline(cin, cont);
        if (cont != "y" && cont != "Y" && cont != "") break;
    }
}

// ---- 按题号查询选题情况 ----

void SelectionManager::queryByTopic(StudentManager* stuMgr, TopicManager* topMgr) {
    cout << "\n===== 按题号查询选题情况 =====" << endl;
    cout << "请输入题号：";
    string topicId;
    getline(cin, topicId);
    topicId = trimString(topicId);

    // 显示题目信息
    if (topMgr) {
        Topic* t = topMgr->findById(topicId);
        if (t) {
            cout << "题目：" << t->title << "（" << t->requirements << "）" << endl;
        } else {
            cout << "题号 " << topicId << " 不存在于题库中。" << endl;
        }
    }

    bool found = false;
    cout << left << setw(10) << "学号"
         << setw(10) << "姓名"
         << setw(14) << "班级"
         << setw(14) << "选题日期" << endl;
    cout << string(48, '-') << endl;

    for (int i = 0; i < selections.size(); i++) {
        Selection* sel = selections.get(i);
        if (!sel || sel->topicId != topicId) continue;

        string stuName = "", stuClass = "";
        if (stuMgr) {
            Student* s = stuMgr->findById(sel->studentId);
            if (s) {
                stuName = s->name;
                stuClass = s->className;
            }
        }

        cout << left << setw(10) << sel->studentId
             << setw(10) << stuName
             << setw(14) << stuClass
             << setw(14) << sel->selectionDate << endl;
        found = true;
    }

    if (!found) {
        cout << "暂无学生选择此题。" << endl;
    }
}

// ---- 按学号/姓名查询选题情况 ----

void SelectionManager::queryByStudent(StudentManager* stuMgr, TopicManager* topMgr) {
    cout << "\n===== 查询学生选题情况 =====" << endl;
    cout << "请选择查询方式：1-按学号  2-按姓名" << endl;
    cout << "请输入选项：";
    string opt;
    getline(cin, opt);
    opt = trimString(opt);

    string keyword;
    vector<Student*> matchedStudents;

    LinkedList<Student>& stuList = stuMgr->getList();

    if (opt == "1") {
        cout << "请输入学号：";
        getline(cin, keyword);
        keyword = trimString(keyword);
        Student* s = stuMgr->findById(keyword);
        if (s) matchedStudents.push_back(s);
    } else if (opt == "2") {
        cout << "请输入姓名：";
        getline(cin, keyword);
        keyword = trimString(keyword);
        for (int i = 0; i < stuList.size(); i++) {
            Student* s = stuList.get(i);
            if (s && containsString(s->name, keyword)) {
                matchedStudents.push_back(s);
            }
        }
    } else {
        cout << "无效选项！" << endl;
        return;
    }

    if (matchedStudents.empty()) {
        cout << "未找到匹配的学生。" << endl;
        return;
    }

    for (size_t idx = 0; idx < matchedStudents.size(); idx++) {
        Student* s = matchedStudents[idx];
        cout << "\n学生：" << s->name << "（" << s->id << "，班级：" << s->className << "）" << endl;

        bool hasSelection = false;
        for (int i = 0; i < selections.size(); i++) {
            Selection* sel = selections.get(i);
            if (!sel || sel->studentId != s->id) continue;

            string topicTitle = "";
            if (topMgr) {
                Topic* t = topMgr->findById(sel->topicId);
                if (t) topicTitle = t->title;
            }

            cout << "  题号：" << sel->topicId
                 << "，题目：" << topicTitle
                 << "，选题日期：" << sel->selectionDate << endl;
            hasSelection = true;
        }

        if (!hasSelection) {
            cout << "  该学生暂无选题记录。" << endl;
        }
    }
}
