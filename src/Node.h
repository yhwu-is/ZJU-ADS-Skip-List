#ifndef _NODE_H
#define _NODE_H

template<class T>
class SkipList;  // forward declaration

template<class T>
class Node
{
public:
    Node() {}
    Node(T d) : data(d) {}
    ~Node() { delete[] forward; }
    T getData() const { return data; }
private:
    T data;
    int nodeLevel;
    Node<T> **forward;
    friend class SkipList<T>;
};

#endif