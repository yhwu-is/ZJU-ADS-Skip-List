#ifndef _SKIPLIST_H
#define _SKIPLIST_H

#include <cassert>
#include <ctime>
#include "Node.h"
#include "Rand.h"

using namespace std;

#define DEBUG
#define MAX_LEVEL 16

template<class K, class V>
class SkipList {
public:
    SkipList(K FootKey);  // initialize a skip list
    ~SkipList();            // remove a skip list
    Node<K, V> *search(K key) const;  // search operation for skip list
    bool insert(K key, V value);      // insert operation for skip list
    bool remove(K key, V &value);     // delete operation for skip list
    int size() { return nodeCount; }
    int getLevel() { return level; }
private:
    void createNode(int level, Node<K, V> *&node);                  // create a new node only with level
    void createNode(int level, Node<K, V> *&node, K key, V value);  // create a new node with level, key and value
    int getRandomLevel();
    void dumpAllNodes();
    void dumpNodeDetail(Node<K, V> *node, int nodeLevel);
private:
    int level;
    Node<K, V> *head;
    Node<K, V> *foot;
    unsigned long long nodeCount;
    Rand rand;
};

#endif