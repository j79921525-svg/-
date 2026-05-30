#ifndef TOPICMANAGER_H
#define TOPICMANAGER_H

#include "Common.h"

class SelectionManager;

class TopicManager {
private:
    LinkedList<Topic> topics;
    const string fileName;

    bool isIdExist(const string& id);

public:
    TopicManager(const string& file = "topics.txt");

    void loadFromFile();
    void saveToFile();

    void addTopic();
    void modifyTopic();
    void deleteTopic(SelectionManager* selMgr = nullptr);

    void displayAll();
    void exactQuery();
    void fuzzyQuery();

    Topic* findById(const string& id);

    LinkedList<Topic>& getList() { return topics; }
};

#endif
