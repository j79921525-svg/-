#include "TopicManager.h"
#include "SelectionManager.h"

TopicManager::TopicManager(const string& file) : fileName(file) {}

// ---- 文件读写 ----

void TopicManager::loadFromFile() {
    topics.clear();
    ifstream fin(fileName);
    if (!fin) {
        ofstream fout(fileName);
        fout << "T01|学生成绩管理系统|开发一个完整的成绩录入、查询、统计系统|2026-07-15" << endl;
        fout << "T02|图书借阅管理系统|实现图书的借阅、归还、查询功能|2026-07-20" << endl;
        fout << "T03|校园导航系统|基于图结构实现校园最短路径导航|2026-07-10" << endl;
        fout << "T04|宿舍管理系统|管理学生宿舍分配、调换、查询|2026-07-25" << endl;
        fout << "T05|课程选课系统|实现学生选课、退课和课表查询|2026-07-18" << endl;
        fout << "T06|在线考试系统|实现题库管理、自动组卷和在线答题|2026-08-01" << endl;
        fout << "T07|运动会管理系统|管理运动会项目报名、成绩记录和排名|2026-07-12" << endl;
        fout << "T08|社团活动管理|管理社团信息、活动发布和成员招募|2026-07-22" << endl;
        fout << "T09|校园二手交易平台|实现二手物品发布、搜索和交易管理|2026-08-05" << endl;
        fout << "T10|学生考勤系统|实现学生出勤记录、统计和预警功能|2026-07-28" << endl;
        fout.close();

        fin.open(fileName);
        if (!fin) {
            cout << "错误：无法创建题库文件！" << endl;
            return;
        }
    }

    string line;
    while (getline(fin, line)) {
        line = trimString(line);
        if (line.empty()) continue;
        vector<string> parts = splitString(line, '|');
        if (parts.size() >= 4) {
            Topic t;
            t.id = trimString(parts[0]);
            t.title = trimString(parts[1]);
            t.requirements = trimString(parts[2]);
            t.deadline = trimString(parts[3]);
            topics.add(t);
        }
    }
    fin.close();
    cout << "题库数据加载完成，共 " << topics.size() << " 条记录。" << endl;
}

void TopicManager::saveToFile() {
    ofstream fout(fileName);
    if (!fout) {
        cout << "错误：无法写入题库文件！" << endl;
        return;
    }
    for (int i = 0; i < topics.size(); i++) {
        Topic* t = topics.get(i);
        if (t) {
            fout << t->id << "|" << t->title << "|"
                 << t->requirements << "|" << t->deadline << endl;
        }
    }
    fout.close();
}

// ---- 辅助 ----

bool TopicManager::isIdExist(const string& id) {
    for (int i = 0; i < topics.size(); i++) {
        Topic* t = topics.get(i);
        if (t && t->id == id) return true;
    }
    return false;
}

Topic* TopicManager::findById(const string& id) {
    for (int i = 0; i < topics.size(); i++) {
        Topic* t = topics.get(i);
        if (t && t->id == id) return t;
    }
    return nullptr;
}

// ---- 增删改查 ----

void TopicManager::addTopic() {
    Topic t;
    cout << "\n===== 添加题目 =====" << endl;

    cout << "请输入题目编号：";
    getline(cin, t.id);
    t.id = trimString(t.id);
    if (t.id.empty()) {
        cout << "编号不能为空！" << endl;
        return;
    }
    if (isIdExist(t.id)) {
        cout << "该编号已存在！" << endl;
        return;
    }

    cout << "请输入题目名称：";
    getline(cin, t.title);
    t.title = trimString(t.title);
    if (t.title.empty()) {
        cout << "题目名称不能为空！" << endl;
        return;
    }

    cout << "请输入任务要求：";
    getline(cin, t.requirements);
    t.requirements = trimString(t.requirements);

    cout << "请输入完成时间（如 2026-07-15）：";
    getline(cin, t.deadline);
    t.deadline = trimString(t.deadline);

    topics.add(t);
    saveToFile();
    cout << "题目添加成功！" << endl;
}

void TopicManager::modifyTopic() {
    cout << "\n===== 修改题目信息 =====" << endl;
    cout << "请输入要修改的题目编号：";
    string id;
    getline(cin, id);
    id = trimString(id);

    Topic* t = findById(id);
    if (!t) {
        cout << "未找到编号为 " << id << " 的题目！" << endl;
        return;
    }

    cout << "当前信息 —— 题目：" << t->title
         << "，要求：" << t->requirements
         << "，截止：" << t->deadline << endl;

    string input;
    cout << "请输入新的题目名称（直接回车保持不变）：";
    getline(cin, input);
    input = trimString(input);
    if (!input.empty()) t->title = input;

    cout << "请输入新的任务要求（直接回车保持不变）：";
    getline(cin, input);
    input = trimString(input);
    if (!input.empty()) t->requirements = input;

    cout << "请输入新的完成时间（直接回车保持不变）：";
    getline(cin, input);
    input = trimString(input);
    if (!input.empty()) t->deadline = input;

    saveToFile();
    cout << "题目信息修改成功！" << endl;
}

void TopicManager::deleteTopic(SelectionManager* selMgr) {
    cout << "\n===== 删除题目 =====" << endl;
    cout << "请输入要删除的题目编号：";
    string id;
    getline(cin, id);
    id = trimString(id);

    Topic* t = findById(id);
    if (!t) {
        cout << "未找到编号为 " << id << " 的题目！" << endl;
        return;
    }

    if (selMgr && selMgr->hasSelectionByTopic(id)) {
        cout << "该题目已被学生选取，需要先取消选题关联才能删除。" << endl;
        cout << "是否取消该题目的所有选题记录？(y/n)：";
        string choice;
        getline(cin, choice);
        if (choice == "y" || choice == "Y") {
            selMgr->removeSelectionsByTopic(id);
            selMgr->saveToFile();
        } else {
            cout << "已取消删除操作。" << endl;
            return;
        }
    }

    for (int i = 0; i < topics.size(); i++) {
        Topic* cur = topics.get(i);
        if (cur && cur->id == id) {
            topics.remove(i);
            saveToFile();
            cout << "题目 " << t->title << " 删除成功！" << endl;
            return;
        }
    }
}

// ---- 浏览 ----

void TopicManager::displayAll() {
    cout << "\n===== 题库信息列表 =====" << endl;
    if (topics.isEmpty()) {
        cout << "暂无题目信息。" << endl;
        return;
    }
    cout << left << setw(8) << "编号"
         << setw(22) << "题目"
         << setw(30) << "任务要求"
         << setw(14) << "完成时间" << endl;
    cout << string(74, '-') << endl;
    for (int i = 0; i < topics.size(); i++) {
        Topic* t = topics.get(i);
        if (t) {
            cout << left << setw(8) << t->id
                 << setw(22) << t->title
                 << setw(30) << t->requirements
                 << setw(14) << t->deadline << endl;
        }
    }
    cout << "共 " << topics.size() << " 道题目。" << endl;
}

// ---- 精确查询 ----

void TopicManager::exactQuery() {
    cout << "\n===== 精确查询题目 =====" << endl;
    cout << "请选择查询字段：1-编号  2-题目  3-完成时间" << endl;
    cout << "请输入选项：";
    string opt;
    getline(cin, opt);
    opt = trimString(opt);

    cout << "请输入查询值：";
    string keyword;
    getline(cin, keyword);
    keyword = trimString(keyword);

    bool found = false;
    cout << left << setw(8) << "编号"
         << setw(22) << "题目"
         << setw(30) << "任务要求"
         << setw(14) << "完成时间" << endl;
    cout << string(74, '-') << endl;

    for (int i = 0; i < topics.size(); i++) {
        Topic* t = topics.get(i);
        if (!t) continue;
        bool match = false;
        if (opt == "1") match = (t->id == keyword);
        else if (opt == "2") match = (t->title == keyword);
        else if (opt == "3") match = (t->deadline == keyword);
        else {
            cout << "无效选项！" << endl;
            return;
        }
        if (match) {
            cout << left << setw(8) << t->id
                 << setw(22) << t->title
                 << setw(30) << t->requirements
                 << setw(14) << t->deadline << endl;
            found = true;
        }
    }
    if (!found) cout << "未找到匹配的题目信息。" << endl;
}

// ---- 模糊查询 ----

void TopicManager::fuzzyQuery() {
    cout << "\n===== 模糊查询题目 =====" << endl;
    cout << "请选择查询字段：1-编号  2-题目  3-任务要求  4-完成时间" << endl;
    cout << "请输入选项：";
    string opt;
    getline(cin, opt);
    opt = trimString(opt);

    cout << "请输入关键词（支持部分匹配）：";
    string keyword;
    getline(cin, keyword);
    keyword = trimString(keyword);

    bool found = false;
    cout << left << setw(8) << "编号"
         << setw(22) << "题目"
         << setw(30) << "任务要求"
         << setw(14) << "完成时间" << endl;
    cout << string(74, '-') << endl;

    for (int i = 0; i < topics.size(); i++) {
        Topic* t = topics.get(i);
        if (!t) continue;
        bool match = false;
        if (opt == "1") match = containsString(t->id, keyword);
        else if (opt == "2") match = containsString(t->title, keyword);
        else if (opt == "3") match = containsString(t->requirements, keyword);
        else if (opt == "4") match = containsString(t->deadline, keyword);
        else {
            cout << "无效选项！" << endl;
            return;
        }
        if (match) {
            cout << left << setw(8) << t->id
                 << setw(22) << t->title
                 << setw(30) << t->requirements
                 << setw(14) << t->deadline << endl;
            found = true;
        }
    }
    if (!found) cout << "未找到匹配的题目信息。" << endl;
}
