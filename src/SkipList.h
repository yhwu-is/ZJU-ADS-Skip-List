#ifndef _SKIPLIST_H
#define _SKIPLIST_H

#include <iostream>
#include <random>
#include <cmath>
#include <cassert>
#include <ctime>
#include "Node.h"

using namespace std;

// #define DEBUG
#define MAX_LEVEL 16

template<class T>
class SkipList
{
public:
    SkipList(T footData);    // initialize a skip list
    ~SkipList();             // remove a skip list
    Node<T> *search(T data) const;  // search operation for skip list
    bool insert(T data);      // insert operation for skip list
    bool remove(T data);      // delete operation for skip list
    int size() { return nodeCount; }
    int getLevel() { return level; }
private:
    void createNode(int level, Node<T> *&node);          // create a new node only with level
    void createNode(int level, Node<T> *&node, T data);  // create a new node with level, key and value
    int getRandomLevel();
    void dumpAllNodes();
    void dumpNodeDetail(Node<T> *node, int nodeLevel);
private:
    int level;      // the max level of current nodes(1 ~ MAX_LEVEL)
    Node<T> *head;  // head node of the skip list
    Node<T> *foot;  // tail node of the skip list
    unsigned long long nodeCount;
};

template<class T>
SkipList<T>::SkipList(T footData)
{
    createNode(0, foot);  // set up the first node
    foot->data = footData;
    this->level = 0;
    createNode(MAX_LEVEL, head);  // set up the head node
    for (int i = 0; i < MAX_LEVEL; i++)
        head->forward[i] = foot;
    nodeCount = 0;
}

template<class T>
SkipList<T>::~SkipList()
{
    Node<T> *p = head;
    Node<T> *q;
    while (p != NULL)
    {
        q = p->forward[0];
        delete p;
        p = q;
    }
    delete p;
}

template<class T>
void SkipList<T>::createNode(int level, Node<T> *&node)
{
    node = new Node<T>(0);
    assert(node != NULL);                      // judge if new successfully
    node->forward = new Node<T> *[level + 1];  // need to initialize the forward node array
    node->nodeLevel = level;
};

template<class T>
void SkipList<T>::createNode(int level, Node<T> *&node, T data)
{
    node = new Node<T>(data);                      // construct the new node with key and value
    assert(node != NULL);                          // judge if new successfully
    if (level > 0)
        node->forward = new Node<T> *[level + 1];  // need to initialize the forward node array
    node->nodeLevel = level;
};

template<class T>
Node<T> *SkipList<T>::search(const T data) const {
    Node<T> *node = head;                      // start search from the head node
    for (int i = level; i >= 0; i--)           // search from top down
        while (node->forward[i]->data < data)  // if the forward number is still less than the key
            node = node->forward[i];           // keep the level and go forward
    node = node->forward[0];
    if (node->data == data)                    // it means the key is found
        return node;                           // return the correspond node
    else                                       // it means the key is not found
        return nullptr;                        // return null pointer
};

template<class T>
bool SkipList<T>::insert(T data) {
    Node<T> *update[MAX_LEVEL];
    Node<T> *node = head;

    for (int i = level; i >= 0; i--)
    {   // find the position to be inserted
        while (node->forward[i]->data < data)
            node = node->forward[i];
        update[i] = node;     // reserve the node that might have pointers to be update
    }
    node = node->forward[0];
    if (node->data == data)
        return false;         // return false if the key is already exist
    
    int nodeLevel = getRandomLevel();  // get random new level of the new node
    if (nodeLevel > level)
    {   // it means we need to update the level of the skip list
        nodeLevel = ++level;       // the level increase only by 1 even though the random number larger than (level + 1)
        update[nodeLevel] = head;  // and we need to update head node in this level
    }

    Node<T> *newNode;
    createNode(nodeLevel, newNode, data);  // create the new node
    for (int i = nodeLevel; i >= 0; i--)
    {   // update the forward pointer of nodes in update[]
        node = update[i];
        newNode->forward[i] = node->forward[i];
        node->forward[i] = newNode;
    }
    nodeCount++;

#ifdef DEBUG
    dumpAllNodes();
#endif

    return true;
};

template<class T>
void SkipList<T>::dumpAllNodes()
{
    Node<T> *tmp = head;
    while (tmp->forward[0] != foot)
    {
        tmp = tmp->forward[0];
        dumpNodeDetail(tmp, tmp->nodeLevel);
        cout << "----------------------------" << endl;
    }
    cout << endl;
}

template<class T>
void SkipList<T>::dumpNodeDetail(Node<T> *node, int nodeLevel)
{
    if (node == nullptr)
        return;
    cout << "node->data:" << node->data << endl;
    for (int i = 0; i <= nodeLevel; ++i)
        cout << "forward[" << i << "]:" << "data:" << node->forward[i]->data << endl;
}

template<class T>
bool SkipList<T>::remove(T data)
{   // the search part is the same as insert
    Node<T> *update[MAX_LEVEL];
    Node<T> *node = head;
    for (int i = level; i >= 0; i--)
    {
        while (node->forward[i]->data < data)
            node = node->forward[i];
        update[i] = node;
    }
    node = node->forward[0];
    if (node->data != data)
        return false;

    data = node->data;
    for (int i = 0; i <= level; i++)
    {   // reverse the update process of insert
        if (update[i]->forward[i] != node)         // it means this value don't need to update
            break;
        update[i]->forward[i] = node->forward[i];
    }
    delete node;  // delete the node
    while (level > 0 && head->forward[level] == foot)
        level--;  // update level if needed
    nodeCount--;

#ifdef DEBUG
    dumpAllNodes();
#endif

    return true;
};

template<class T>
int SkipList<T>::getRandomLevel()
{   // get random level that satisfy geometry distribution(with p = 1/2)
    default_random_engine generator;
	geometric_distribution<int> distribution(0.5);
	int ret = distribution(generator) + 1;  // get number that obeys geometry distribution
    if (ret > MAX_LEVEL)
        ret = MAX_LEVEL;                    // restrict to MAX_LEVEL
    return ret;
}

#endif