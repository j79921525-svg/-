#include "StudentManager.h"
#include "SelectionManager.h"
#include <conio.h>

StudentManager::StudentManager(const string& file) : fileName(file) {}

// ---- 文件读写 ----

void StudentManager::loadFromFile() {
    students.clear();
    ifstream fin(fileName);
    if (!fin) {
        // 文件不存在时创建示例数据
        ofstream fout(fileName);
        fout << "2024001|张三|计算机2401" << endl;
        fout << "2024002|李四|计算机2401" << endl;
        fout << "2024003|王五|计算机2401" << endl;
        fout << "2024004|赵六|计算机2402" << endl;
        fout << "2024005|孙七|计算机2402" << endl;
        fout << "2024006|周八|计算机2402" << endl;
        fout << "2024007|吴九|软件2401" << endl;
        fout << "2024008|郑十|软件2401" << endl;
        fout << "2024009|刘一|软件2402" << endl;
        fout << "2024010|陈二|软件2402" << endl;
        fout.close();

        fin.open(fileName);
        if (!fin) {
            cout << "错误：无法创建学生数据文件！" << endl;
            return;
        }
    }

    string line;
    while (getline(fin, line)) {
        line = trimString(line);
        if (line.empty()) continue;
        vector<string> parts = splitString(line, '|');
        if (parts.size() >= 3) {
            Student s;
            s.id = trimString(parts[0]);
            s.name = trimString(parts[1]);
            s.className = trimString(parts[2]);
            students.add(s);
        }
    }
    fin.close();
    cout << "学生数据加载完成，共 " << students.size() << " 条记录。" << endl;
}

void StudentManager::saveToFile() {
    ofstream fout(fileName);
    if (!fout) {
        cout << "错误：无法写入学生数据文件！" << endl;
        return;
    }
    for (int i = 0; i < students.size(); i++) {
        Student* s = students.get(i);
        if (s) {
            fout << s->id << "|" << s->name << "|" << s->className << endl;
        }
    }
    fout.close();
}

// ---- 辅助函数 ----

bool StudentManager::isIdExist(const string& id) {
    for (int i = 0; i < students.size(); i++) {
        Student* s = students.get(i);
        if (s && s->id == id) return true;
    }
    return false;
}

Student* StudentManager::findById(const string& id) {
    for (int i = 0; i < students.size(); i++) {
        Student* s = students.get(i);
        if (s && s->id == id) return s;
    }
    return nullptr;
}

Student* StudentManager::findByName(const string& name) {
    for (int i = 0; i < students.size(); i++) {
        Student* s = students.get(i);
        if (s && s->name == name) return s;
    }
    return nullptr;
}

// ---- 增删改查 ----

void StudentManager::addStudent() {
    Student s;
    cout << "\n===== 添加学生 =====" << endl;

    cout << "请输入学号：";
    getline(cin, s.id);
    s.id = trimString(s.id);
    if (s.id.empty()) {
        cout << "学号不能为空！" << endl;
        return;
    }
    if (isIdExist(s.id)) {
        cout << "该学号已存在，无法重复添加！" << endl;
        return;
    }

    cout << "请输入姓名：";
    getline(cin, s.name);
    s.name = trimString(s.name);
    if (s.name.empty()) {
        cout << "姓名不能为空！" << endl;
        return;
    }

    cout << "请输入班级：";
    getline(cin, s.className);
    s.className = trimString(s.className);
    if (s.className.empty()) {
        cout << "班级不能为空！" << endl;
        return;
    }

    students.add(s);
    saveToFile();
    cout << "学生添加成功！" << endl;
}

void StudentManager::modifyStudent() {
    cout << "\n===== 修改学生信息 =====" << endl;
    cout << "请输入要修改的学生学号：";
    string id;
    getline(cin, id);
    id = trimString(id);

    Student* s = findById(id);
    if (!s) {
        cout << "未找到学号为 " << id << " 的学生！" << endl;
        return;
    }

    cout << "当前信息 —— 姓名：" << s->name << "，班级：" << s->className << endl;
    cout << "请输入新的姓名（直接回车保持不变）：";
    string input;
    getline(cin, input);
    input = trimString(input);
    if (!input.empty()) {
        s->name = input;
    }

    cout << "请输入新的班级（直接回车保持不变）：";
    getline(cin, input);
    input = trimString(input);
    if (!input.empty()) {
        s->className = input;
    }

    saveToFile();
    cout << "学生信息修改成功！" << endl;
}

void StudentManager::deleteStudent(SelectionManager* selMgr) {
    cout << "\n===== 删除学生 =====" << endl;
    cout << "请输入要删除的学生学号：";
    string id;
    getline(cin, id);
    id = trimString(id);

    Student* s = findById(id);
    if (!s) {
        cout << "未找到学号为 " << id << " 的学生！" << endl;
        return;
    }

    // 检查学生是否有选题记录
    if (selMgr && selMgr->hasSelectionByStudent(id)) {
        cout << "该学生存在选题记录，需要先取消选题关联才能删除。" << endl;
        cout << "是否取消该学生的所有选题记录？(y/n)：";
        string choice;
        getline(cin, choice);
        if (choice == "y" || choice == "Y") {
            selMgr->removeSelectionsByStudent(id);
            selMgr->saveToFile();
        } else {
            cout << "已取消删除操作。" << endl;
            return;
        }
    }

    for (int i = 0; i < students.size(); i++) {
        Student* cur = students.get(i);
        if (cur && cur->id == id) {
            students.remove(i);
            saveToFile();
            cout << "学生 " << s->name << " 删除成功！" << endl;
            return;
        }
    }
}

// ---- 浏览 ----

void StudentManager::displayAll() {
    cout << "\n===== 学生信息列表 =====" << endl;
    if (students.isEmpty()) {
        cout << "暂无学生信息。" << endl;
        return;
    }
    cout << left << setw(12) << "学号"
         << setw(10) << "姓名"
         << setw(16) << "班级" << endl;
    cout << string(38, '-') << endl;
    for (int i = 0; i < students.size(); i++) {
        Student* s = students.get(i);
        if (s) {
            cout << left << setw(12) << s->id
                 << setw(10) << s->name
                 << setw(16) << s->className << endl;
        }
    }
    cout << "共 " << students.size() << " 名学生。" << endl;
}

// ---- 精确查询 ----

void StudentManager::exactQuery() {
    cout << "\n===== 精确查询学生 =====" << endl;
    cout << "请选择查询字段：1-学号  2-姓名  3-班级" << endl;
    cout << "请输入选项：";
    string opt;
    getline(cin, opt);
    opt = trimString(opt);

    cout << "请输入查询值：";
    string keyword;
    getline(cin, keyword);
    keyword = trimString(keyword);

    bool found = false;
    cout << left << setw(12) << "学号"
         << setw(10) << "姓名"
         << setw(16) << "班级" << endl;
    cout << string(38, '-') << endl;

    for (int i = 0; i < students.size(); i++) {
        Student* s = students.get(i);
        if (!s) continue;
        bool match = false;
        if (opt == "1") match = (s->id == keyword);
        else if (opt == "2") match = (s->name == keyword);
        else if (opt == "3") match = (s->className == keyword);
        else {
            cout << "无效选项！" << endl;
            return;
        }
        if (match) {
            cout << left << setw(12) << s->id
                 << setw(10) << s->name
                 << setw(16) << s->className << endl;
            found = true;
        }
    }
    if (!found) cout << "未找到匹配的学生信息。" << endl;
}

// ---- 模糊查询 ----

void StudentManager::fuzzyQuery() {
    cout << "\n===== 模糊查询学生 =====" << endl;
    cout << "请选择查询字段：1-学号  2-姓名  3-班级" << endl;
    cout << "请输入选项：";
    string opt;
    getline(cin, opt);
    opt = trimString(opt);

    cout << "请输入关键词（支持部分匹配）：";
    string keyword;
    getline(cin, keyword);
    keyword = trimString(keyword);

    bool found = false;
    cout << left << setw(12) << "学号"
         << setw(10) << "姓名"
         << setw(16) << "班级" << endl;
    cout << string(38, '-') << endl;

    for (int i = 0; i < students.size(); i++) {
        Student* s = students.get(i);
        if (!s) continue;
        bool match = false;
        if (opt == "1") match = containsString(s->id, keyword);
        else if (opt == "2") match = containsString(s->name, keyword);
        else if (opt == "3") match = containsString(s->className, keyword);
        else {
            cout << "无效选项！" << endl;
            return;
        }
        if (match) {
            cout << left << setw(12) << s->id
                 << setw(10) << s->name
                 << setw(16) << s->className << endl;
            found = true;
        }
    }
    if (!found) cout << "未找到匹配的学生信息。" << endl;
}
